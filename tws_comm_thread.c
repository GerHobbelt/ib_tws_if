/*
 * Mongoose-based server app which interfaces with a TWS station and displays data in HTML form,
 * suitable for loading by, for example, 64-bit Excel 2010, using web queries.
 */

/*
TODO / roadmap:

- prioritizing outgoing requests, e.g. ORDERs have priority over historical data requests, and requests for recent historical data have priority over requests for older data.

- use a priority queue for the above plus a 'idle time' delay to prevent hammering the TWS machines with historical data requests: only fire those when the interface has been 'quiet' for X seconds

- store/cache historical data; use 'smart' code to request consecutive and _large_ chunks of historical data to be cached: one request, served many times (from local cache)

- async TWS TX/RX: push requests asap, using a 'telnet' TCP setting (you don't want orders to wait for a TCP buffer fill timeout!): single thread/connection connected to TWS,
  all requests are posted in a 'response queue' (so we know which responses are for whom) upon transmission --> true full duplex communication instead of the standard TWS sample
  which uses the TCP connection as a half-duplex connect (as it waits for the response to the request before firing another).
*/

#if 0
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <pthread.h>

#include "mongoose.h"
#else
// no need to edit the mongoose source file a lot when we do it this way. Meanwhile, we can use its socket functions, etc.
#include "mongoose/mongoose.c"
#endif

#include <float.h>
#include <math.h>

#include "tws_c_api/twsapi.h"

#include "tws_comm_thread.h"




#pragma warning(disable: 4100)  // shut up MSVC about 'unreferenced formal parameter



#define TWS_CONNECT_RETRY_DELAY     10 // unit: seconds


//------------------------------------------------------------------------------------------------



#define MAX_USER_LEN  20
#define MAX_MESSAGE_LEN  100
#define MAX_MESSAGES 5
#define MAX_SESSIONS 2
#define SESSION_TTL 120

static const char *authorize_url = "/authorize";
static const char *login_url = "/login.html";
static const char *ajax_reply_start =
  "HTTP/1.1 200 OK\r\n"
  "Cache: no-cache\r\n"
  "Content-Type: application/x-javascript\r\n"
  "\r\n";

// Describes single message sent to a chat. If user is empty (0 length),
// the message is then originated from the server itself.
struct message {
  long id;                     // Message ID
  char user[MAX_USER_LEN];     // User that have sent the message
  char text[MAX_MESSAGE_LEN];  // Message text
  time_t timestamp;            // Message timestamp, UTC
};

// Describes web session.
struct session {
  char session_id[33];      // Session ID, must be unique
  char random[20];          // Random data used for extra user validation
  char user[MAX_USER_LEN];  // Authenticated user
  time_t expire;            // Expiration timestamp, UTC
};

static struct message messages[MAX_MESSAGES];  // Ringbuffer for messages
static struct session sessions[MAX_SESSIONS];  // Current sessions
static long last_message_id;

// Protects messages, sessions, last_message_id
static pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

// Get session object for the connection. Caller must hold the lock.
static struct session *get_session(const struct mg_connection *conn) {
  int i;
  char session_id[33];
  time_t now = time(NULL);
  mg_get_cookie(conn, "session", session_id, sizeof(session_id));
  for (i = 0; i < MAX_SESSIONS; i++) {
    if (sessions[i].expire != 0 &&
        sessions[i].expire > now &&
        strcmp(sessions[i].session_id, session_id) == 0) {
      break;
    }
  }
  return i == MAX_SESSIONS ? NULL : &sessions[i];
}

static void get_qsvar(const struct mg_request_info *request_info,
                      const char *name, char *dst, size_t dst_len) {
  const char *qs = request_info->query_string;
  mg_get_var(qs, strlen(qs == NULL ? "" : qs), name, dst, dst_len);
}

// Get a get of messages with IDs greater than last_id and transform them
// into a JSON string. Return that string to the caller. The string is
// dynamically allocated, caller must free it. If there are no messages,
// NULL is returned.
static char *messages_to_json(long last_id) {
  const struct message *message;
  int max_msgs, len;
  char buf[sizeof(messages)];  // Large enough to hold all messages

  // Read-lock the ringbuffer. Loop over all messages, making a JSON string.
  pthread_rwlock_rdlock(&rwlock);
  len = 0;
  max_msgs = sizeof(messages) / sizeof(messages[0]);
  // If client is too far behind, return all messages.
  if (last_message_id - last_id > max_msgs) {
    last_id = last_message_id - max_msgs;
  }
  for (; last_id < last_message_id; last_id++) {
    message = &messages[last_id % max_msgs];
    if (message->timestamp == 0) {
      break;
    }
    // buf is allocated on stack and hopefully is large enough to hold all
    // messages (it may be too small if the ringbuffer is full and all
    // messages are large. in this case asserts will trigger).
    len += snprintf(buf + len, sizeof(buf) - len,
        "{user: '%s', text: '%s', timestamp: %lu, id: %lu},",
        message->user, message->text, message->timestamp, message->id);
    assert(len > 0);
    assert((size_t) len < sizeof(buf));
  }
  pthread_rwlock_unlock(&rwlock);

  return len == 0 ? NULL : strdup(buf);
}

// If "callback" param is present in query string, this is JSONP call.
// Return 1 in this case, or 0 if "callback" is not specified.
// Wrap an output in Javascript function call.
static int handle_jsonp(struct mg_connection *conn,
                        const struct mg_request_info *request_info) {
  char cb[64];

  get_qsvar(request_info, "callback", cb, sizeof(cb));
  if (cb[0] != '\0') {
    mg_printf(conn, "%s(", cb);
  }

  return cb[0] == '\0' ? 0 : 1;
}

// A handler for the /ajax/get_messages endpoint.
// Return a list of messages with ID greater than requested.
static void ajax_get_messages(struct mg_connection *conn,
                              const struct mg_request_info *request_info) {
  char last_id[32], *json;
  int is_jsonp;

  mg_printf(conn, "%s", ajax_reply_start);
  is_jsonp = handle_jsonp(conn, request_info);

  get_qsvar(request_info, "last_id", last_id, sizeof(last_id));
  if ((json = messages_to_json(strtoul(last_id, NULL, 10))) != NULL) {
    mg_printf(conn, "[%s]", json);
    free(json);
  }

  if (is_jsonp) {
    mg_printf(conn, "%s", ")");
  }
}

// Allocate new message. Caller must hold the lock.
static struct message *new_message(void) {
  static int size = sizeof(messages) / sizeof(messages[0]);
  struct message *message = &messages[last_message_id % size];
  message->id = last_message_id++;
  message->timestamp = time(0);
  return message;
}

static void my_strlcpy(char *dst, const char *src, size_t len) {
  strncpy(dst, src, len);
  dst[len - 1] = '\0';
}

// A handler for the /ajax/send_message endpoint.
static void ajax_send_message(struct mg_connection *conn,
                              const struct mg_request_info *request_info) {
  struct message *message;
  struct session *session;
  char text[sizeof(message->text) - 1];
  int is_jsonp;

  mg_printf(conn, "%s", ajax_reply_start);
  is_jsonp = handle_jsonp(conn, request_info);

  get_qsvar(request_info, "text", text, sizeof(text));
  if (text[0] != '\0') {
    // We have a message to store. Write-lock the ringbuffer,
    // grab the next message and copy data into it.
    pthread_rwlock_wrlock(&rwlock);
    message = new_message();
    // TODO(lsm): JSON-encode all text strings
    session = get_session(conn);
    assert(session != NULL);
    my_strlcpy(message->text, text, sizeof(text));
    my_strlcpy(message->user, session->user, sizeof(message->user));
    pthread_rwlock_unlock(&rwlock);
  }

  mg_printf(conn, "%s", text[0] == '\0' ? "false" : "true");

  if (is_jsonp) {
    mg_printf(conn, "%s", ")");
  }
}

// Redirect user to the login form. In the cookie, store the original URL
// we came from, so that after the authorization we could redirect back.
static void redirect_to_login(struct mg_connection *conn,
                              const struct mg_request_info *request_info) {
  mg_printf(conn, "HTTP/1.1 302 Found\r\n"
      "Set-Cookie: original_url=%s\r\n"
      "Location: %s\r\n\r\n",
      request_info->uri, login_url);
}

// Return 1 if username/password is allowed, 0 otherwise.
static int do_check_password(const char *user, const char *password) {
  // In production environment we should ask an authentication system
  // to authenticate the user.
  // Here however we do trivial check that user and password are not empty
  return (user[0] && password[0]);
}

// Allocate new session object
static struct session *new_session(void) {
  int i;
  time_t now = time(NULL);
  pthread_rwlock_wrlock(&rwlock);
  for (i = 0; i < MAX_SESSIONS; i++) {
    if (sessions[i].expire == 0 || sessions[i].expire < now) {
      sessions[i].expire = time(0) + SESSION_TTL;
      break;
    }
  }
  pthread_rwlock_unlock(&rwlock);
  return i == MAX_SESSIONS ? NULL : &sessions[i];
}

// Generate session ID. buf must be 33 bytes in size.
// Note that it is easy to steal session cookies by sniffing traffic.
// This is why all communication must be SSL-ed.
static void generate_session_id(char *buf, const char *random,
                                const char *user) {
  mg_md5(buf, random, user, NULL);
}

static void send_server_message(const char *fmt, ...) {
  va_list ap;
  struct message *message;

  pthread_rwlock_wrlock(&rwlock);
  message = new_message();
  message->user[0] = '\0';  // Empty user indicates server message
  va_start(ap, fmt);
  vsnprintf(message->text, sizeof(message->text), fmt, ap);
  va_end(ap);

  pthread_rwlock_unlock(&rwlock);
}

// A handler for the /authorize endpoint.
// Login page form sends user name and password to this endpoint.
static void do_authorize(struct mg_connection *conn,
                      const struct mg_request_info *request_info) {
  char user[MAX_USER_LEN], password[MAX_USER_LEN];
  struct session *session;

  // Fetch user name and password.
  get_qsvar(request_info, "user", user, sizeof(user));
  get_qsvar(request_info, "password", password, sizeof(password));

  if (do_check_password(user, password) && (session = new_session()) != NULL) {
    // Authentication success:
    //   1. create new session
    //   2. set session ID token in the cookie
    //   3. remove original_url from the cookie - not needed anymore
    //   4. redirect client back to the original URL
    //
    // The most secure way is to stay HTTPS all the time. However, just to
    // show the technique, we redirect to HTTP after the successful
    // authentication. The danger of doing this is that session cookie can
    // be stolen and an attacker may impersonate the user.
    // Secure application must use HTTPS all the time.
    my_strlcpy(session->user, user, sizeof(session->user));
    snprintf(session->random, sizeof(session->random), "%d", rand());
    generate_session_id(session->session_id, session->random, session->user);
    send_server_message("<%s> joined", session->user);
    mg_printf(conn, "HTTP/1.1 302 Found\r\n"
        "Set-Cookie: session=%s; max-age=3600; http-only\r\n"  // Session ID
        "Set-Cookie: user=%s\r\n"  // Set user, needed by Javascript code
        "Set-Cookie: original_url=/; max-age=0\r\n"  // Delete original_url
        "Location: /\r\n\r\n",
        session->session_id, session->user);
  } else {
    // Authentication failure, redirect to login.
    redirect_to_login(conn, request_info);
  }
}

// Return 1 if request is authorized, 0 otherwise.
static int is_authorized(const struct mg_connection *conn,
                         const struct mg_request_info *request_info) {
  struct session *session;
  char valid_id[33];
  int authorized = 0;

  // Always authorize accesses to login page and to authorize URI
  if (!strcmp(request_info->uri, login_url) ||
      !strcmp(request_info->uri, authorize_url)) {
    return 1;
  }

  pthread_rwlock_rdlock(&rwlock);
  if ((session = get_session(conn)) != NULL) {
    generate_session_id(valid_id, session->random, session->user);
    if (strcmp(valid_id, session->session_id) == 0) {
      session->expire = time(0) + SESSION_TTL;
      authorized = 1;
    }
  }
  pthread_rwlock_unlock(&rwlock);

  return authorized;
}

static void redirect_to_ssl(struct mg_connection *conn,
                            const struct mg_request_info *request_info) {
  const char *p, *host = mg_get_header(conn, "Host");
  if (host != NULL && (p = strchr(host, ':')) != NULL) {
    mg_printf(conn, "HTTP/1.1 302 Found\r\n"
              "Location: https://%.*s:8082/%s:8082\r\n\r\n",
              p - host, host, request_info->uri);
  } else {
    mg_printf(conn, "%s", "HTTP/1.1 500 Error\r\n\r\nHost: header is not set");
  }
}


//------------------------------------------------------------------------------------------------











// http://www.unixguide.net/network/socketfaq/2.11.shtml
// http://www.techrepublic.com/article/tcpip-options-for-high-performance-data-transmission/1050878
static int set_nodelay_mode(struct socket sock, int on) {
#if !defined(SOL_TCP) || (defined(_WIN32) && !defined(__SYMBIAN32__))
  DWORD v_on = !!on;
  return setsockopt(sock.sock, IPPROTO_TCP, TCP_NODELAY, (void *)&v_on, sizeof(v_on));
#else
  int v_on = !!on;
  return setsockopt(sock.sock, SOL_TCP, TCP_NODELAY, &v_on, sizeof (v_on));
#endif
}


/* internal communication stuff between mongoose threads and the tws back-end thread goes here: */
struct tws_thread_exch
{
  pthread_mutex_t tws_exch_mutex;     // mutex related to the TX & RX conditions in here
  pthread_cond_t tws_tx_signal;       // signalled when a TX request is pending (should be processed by the tws 'back-end' thread.
  pthread_cond_t tws_rx_signal;       // signalled when a RX response is pending (should be processed by the related TX-invoking 'front-end' mongoose thread.

  int command;
  int response;
  time_t current_time;
};


void init_tws_thread_exch(struct tws_thread_exch **ptr)
{
  *ptr = (struct tws_thread_exch *)calloc(1, sizeof(*ptr));

  pthread_mutex_init(&ptr[0]->tws_exch_mutex, NULL);
  pthread_cond_init(&ptr[0]->tws_rx_signal, NULL);
  pthread_cond_init(&ptr[0]->tws_tx_signal, NULL);
}

void destroy_tws_thread_exch(struct tws_thread_exch **ptr)
{
  if (*ptr)
  {
    pthread_cond_destroy(&ptr[0]->tws_rx_signal);
    pthread_cond_destroy(&ptr[0]->tws_tx_signal);
    pthread_mutex_destroy(&ptr[0]->tws_exch_mutex);

    free(*ptr);
    *ptr = NULL;
  }
}




struct my_tws_io_info
{
  struct mg_connection *conn;
	struct tws_conn_cfg *tws_cfg;
  void *tws_handle;

  /* tracking some TWS values here as well: */
  long next_order_id;
};

int tws_transmit_func(void *arg, const void *buf, unsigned int buflen)
{
  struct my_tws_io_info *info = (struct my_tws_io_info *)arg;

  return mg_write(info->conn, buf, buflen);
}
int tws_receive_func(void *arg, void *buf, unsigned int max_bufsize)
{
  struct my_tws_io_info *info = (struct my_tws_io_info *)arg;
  struct tws_thread_exch *exch = info->tws_cfg->exch;

  // check whether there's anything available:
  fd_set read_set;
  struct timeval tv;
  int max_fd;

  tv.tv_sec = info->tws_cfg->backend_poll_period / 1000;
  tv.tv_usec = (info->tws_cfg->backend_poll_period % 1000) * 1000;

  while (info->conn->ctx->stop_flag == 0) 
  {
    struct timeval tv2 = tv;

    FD_ZERO(&read_set);
    max_fd = -1;

    // Add listening sockets to the read set
    add_to_set(info->conn->client.sock, &read_set, &max_fd);

    if (select(max_fd + 1, &read_set, NULL, NULL, &tv2) < 0) 
    {
      break;
    } 
    else 
    {
      if (FD_ISSET(info->conn->client.sock, &read_set)) 
      {
        /*
        Mongoose mg_read() does NOT fetch any pending data from the TCP/IP stack when the 'content length' isn't set yet.

        We, however, desire to load an unknown and arbitrary amount of data here to fill a buffer and our protocol doesn't
        have somthing like a 'content length' to guide us along, so we'll have to use another method to make sure
        the read operation actually delivers DATA!
        */
        // conn->content_len = MAX_INT;
        break;
      }

      /*
      When there's no pending incoming data from TWS itself, we'll be running around in this loop while waiting for
      more data to arrive. Meanwhile, we can process incomign requests from the front-end now:
      */
      pthread_mutex_lock(&exch->tws_exch_mutex);
      if (0 != pthread_cond_signal(&exch->tws_tx_signal))
      {
        pthread_mutex_unlock(&exch->tws_exch_mutex);
      }
      else
      {
        int cmd;

        // process incoming TX command/request from one of the front-end threads:
        if (exch->command != 0)
        {
          mg_cry(info->conn, "backend recv waiter: command count = %d", exch->command);
        }

        // copy command to local storage???
        cmd = (exch->command == 1);

        pthread_mutex_unlock(&exch->tws_exch_mutex);

        // fire command at TWS itself:
        if (cmd)
        {
          tws_req_current_time(info->tws_handle);

          pthread_mutex_lock(&exch->tws_exch_mutex);
          exch->command--;
          pthread_mutex_unlock(&exch->tws_exch_mutex);
        }
      }
    }
  }

  return pull(NULL, info->conn->client.sock, info->conn->ssl, buf, max_bufsize);
  // return mg_read(info->conn, buf, max_bufsize);
}
/* 'flush()' marks the end of the outgoing message: it should be transmitted ASAP */
int tws_flush_func(void *arg)
{
  struct my_tws_io_info *info = (struct my_tws_io_info *)arg;

  return 0;
}
/* close callback is invoked on error or when tws_disconnect is invoked */
int tws_close_func(void *arg)
{
  struct my_tws_io_info *info = (struct my_tws_io_info *)arg;

  close_connection(info->conn);

  return 0;
}


static const char *tws_errcode2str(int errcode)
{
  const struct twsclient_errmsg *einfo = tws_strerror(errcode);

  return einfo->err_msg;
}



/* find top percentage gainers (US stocks) with price > 5 and volume > 2M */
static void scan_market(void *ti)
{
    /* illustrate new feature as of version 23 */
    tr_scanner_subscription_t s;

    tws_req_scanner_parameters(ti);

    memset(&s, 0, sizeof s);
    s.scan_number_of_rows = -1;
    s.scan_instrument = "STK";
    s.scan_location_code = "STK.AEB,STK.SBF";
    s.scan_code = "MOST_ACTIVE"; // "TOP_PERC_GAIN"; /* see xml returned by tws_req_scanner_parameters for more choices */

    s.scan_above_price = 0.05;
    s.scan_below_price = s.scan_coupon_rate_above = s.scan_coupon_rate_below = DBL_MAX;
    s.scan_above_volume = 1000;
    s.scan_market_cap_above = s.scan_market_cap_below = DBL_MAX;
    s.scan_moody_rating_above = s.scan_moody_rating_below = s.scan_sp_rating_above = s.scan_sp_rating_below = s.scan_maturity_date_above = s.scan_maturity_date_below = s.scan_exclude_convertible = "";
    s.scan_scanner_setting_pairs = "";
    s.scan_stock_type_filter = "";
    tws_req_scanner_subscription(ti, 1, &s);
}



/*
Connect to the TWS machine; when this fails or when the connection breaks, we retry after a while.

Send all the queued requests to the TWS node and process the responses; signal the origin for each
request when their response has arrived.
*/
void tws_worker_thread(struct mg_context *ctx)
{
  int tws_app_is_down = 0;

  // retry connecting to TWS as long as the server itself hasn't been stopped!
  while (ctx->stop_flag == 0) 
  {
    struct mg_connection fake_conn = {0};
    struct mg_connection *conn;
	  struct tws_conn_cfg *tws_cfg = (struct tws_conn_cfg *)ctx->user_functions.user_data;
    struct my_tws_io_info info = 
    { 
      NULL,
      tws_cfg,
      NULL
    };
    int err;

    fake_conn.ctx = ctx;
    conn = mg_connect(&fake_conn, tws_cfg->ip_address, tws_cfg->port, 0);
    if (conn != NULL)
    {
      tws_app_is_down = 0;
      conn->ctx = ctx;
      conn->birth_time = time(NULL);

      // Disable Nagle - act a la telnet:
      set_nodelay_mode(conn->client, 1);
      
      info.conn = conn;
      info.tws_handle = tws_create(&info, tws_transmit_func, tws_receive_func, tws_flush_func, tws_close_func);
      err = tws_connect(info.tws_handle, tws_cfg->our_id_code);
      if (err) 
      {
        mg_cry(conn, "tws connect returned %s\n", tws_errcode2str(err)); 
      }
      else
      {
        // request the valid set of scanner parameters first: this will trigger the requesting of several market scans from the msg receive handler:
        tws_req_scanner_parameters(info.tws_handle);

        //scan_market(info.tws_handle);

        while (ctx->stop_flag == 0 && tws_connected(info.tws_handle)) 
        {
          if (0 != tws_event_process(info.tws_handle))
          {
            break;
          }
        }
      }

      tws_disconnect(info.tws_handle);
      tws_destroy(info.tws_handle);
      free(conn);
    }
    else 
    {
      if (!tws_app_is_down)
      {
        mg_cry(fc(ctx), "cannot establish a connection with the TWS application. Retrying in %d seconds...", TWS_CONNECT_RETRY_DELAY);
        tws_app_is_down++;
      }
    }

    // wait N seconds before retrying to connect to TWS:
    if (ctx->stop_flag == 0) 
    {
      sleep(TWS_CONNECT_RETRY_DELAY);
    }
  }

  // Signal master that we're done with connection and exiting
  (void) pthread_mutex_lock(&ctx->mutex);
  ctx->num_threads--;
  (void) pthread_cond_signal(&ctx->cond);
  assert(ctx->num_threads >= 0);
  (void) pthread_mutex_unlock(&ctx->mutex);

  DEBUG_TRACE(("exiting"));
}


















const char *mg_get_option_ex(const struct mg_context *ctx, const char *name, const char *default_value)
{
	const char *v = mg_get_option(ctx, name);

	if (!v)
		return default_value;
	return v;
}

int mg_get_option_int(const struct mg_context *ctx, const char *name, int default_value)
{
	const char *v = mg_get_option(ctx, name);

	if (!v)
		return default_value;
	return atol(v);
}


void *event_handler(enum mg_event event_id, struct mg_connection *conn, const struct mg_request_info *request_info) 
{
	void *processed = "yes";
	struct mg_context *ctx = conn->ctx;
	struct tws_conn_cfg *tws_cfg = (struct tws_conn_cfg *)ctx->user_functions.user_data;
  struct tws_thread_exch *exch = tws_cfg->exch;

	switch (event_id) 
	{
	case MG_NEW_REQUEST:
#if 0
		if (!request_info->is_ssl) 
		{
      redirect_to_ssl(conn, request_info);
      processed = NULL;
		} 
		else if (!is_authorized(conn, request_info)) 
		{
      redirect_to_login(conn, request_info);
      processed = NULL;
		} 
		else if (strcmp(request_info->uri, authorize_url) == 0) 
		{
			do_authorize(conn, request_info);
		} 
		else if (strcmp(request_info->uri, "/ajax/get_messages") == 0) 
		{
			ajax_get_messages(conn, request_info);
		} 
		else if (strcmp(request_info->uri, "/ajax/send_message") == 0) 
		{
			ajax_send_message(conn, request_info);
		} 
    else
#endif
    
    if (strncmp(request_info->uri, "/tws/", 5) == 0)
    {
      struct timespec poll_time;
      poll_time.tv_sec = tws_cfg->backend_poll_period / 1000;
      poll_time.tv_nsec = (tws_cfg->backend_poll_period % 1000) * 1000000;

      // raw TWS backend requests: decode the request and pass the request to the backend in a serialized fashion; block & wait for the response...
      pthread_mutex_lock(&exch->tws_exch_mutex);

      // block & wait until we can go and submit the request:
      while (ctx->stop_flag == 0 && ETIMEOUT == pthread_cond_timedwait(&exch->tws_tx_signal, &exch->tws_exch_mutex, &poll_time))
        ;

      // send the request
      mg_cry(conn, "frontend request handler for url '%s': command count = %d", request_info->uri, exch->command);

      exch->command++;

      // block & wait until we can go and fetch the response:
      while (ctx->stop_flag == 0 && ETIMEOUT == pthread_cond_timedwait(&exch->tws_rx_signal, &exch->tws_exch_mutex, &poll_time))
        ;

      // receive the response
      mg_cry(conn, "frontend request handler for url '%s': response count = %d", request_info->uri, exch->response);

      mg_printf(conn, "<h1>TWS says the time is: %s</h1>\n", ctime(&exch->current_time));

      pthread_mutex_unlock(&exch->tws_exch_mutex);
    }
    else
		{
			// No suitable handler found, mark as not processed. Mongoose will
			// try to serve the request.
			processed = NULL;
		}
		break;

  case MG_EXIT0:
    // threads have already shut down; discard our custom mutexes, etc.:
    destroy_tws_thread_exch(&tws_cfg->exch);
    break;

	case MG_INIT0:
    // set up the 'front-end to back-end communication serialization' mutexes:
    init_tws_thread_exch(&tws_cfg->exch);

    // kickstart the TWS backend thread now:
    if (start_thread(ctx, (mg_thread_func_t) tws_worker_thread, ctx) != 0) {
      mg_cry(fc(ctx), "Cannot start TWS connection thread: %d", mg_strerror(ERRNO));
			processed = NULL;
    } else {
      // count this thread too so the master_thread will wait for this one to end as well when we stop.
      ctx->num_threads++;
    }
		break;

  case MG_EVENT_LOG:
    // dump log to stderr as well:
    fprintf(stderr, "%s\n", request_info->log_message);
    // and let the default file logging do its own magic as well:
		processed = NULL;
    break;

	default:
		processed = NULL;
		break;
	}

	return processed;
}




// --- user callback handlers for mongoose --

int option_decode(struct mg_context *ctx, const char *name, const char *value)
{
	struct tws_conn_cfg *tws_cfg = (struct tws_conn_cfg *)ctx->user_functions.user_data;

  if (0 == strcmp("tws_ip_address", name))
  {
    tws_cfg->ip_address = mg_strdup(value);
    return 1;
  }
  else if (0 == strcmp("tws_ip_port", name))
  {
    int portno = atoi(value);
    if (portno > 0) 
    {
      tws_cfg->port = portno;
      return 1;
    }
  } 
  else if (0 == strcmp("tws_connect_id", name))
  {
    int tws_id = atoi(value);
    if (tws_id > 0)
    {
      tws_cfg->our_id_code = tws_id;
      return 1;
    }
  }
  else if (0 == strcmp("tws_poll_delay", name))
  {
    long poll_period = atol(value);
    if (poll_period > 0)
    {
      tws_cfg->backend_poll_period = poll_period;
      return 1;
    }
  }
  return 0;
}


int option_fill(struct mg_context *ctx)
{
  // don't do anything...
  return 1;
}

const char * option_get(const struct mg_context *ctx, const char *name)
{
  // we don't use this one, so keep it a dummy until we do...
  return NULL;
}






















// TWS API callbacks:





void event_tick_price(void *opaque, int ticker_id, long field, double price, int can_auto_execute)
{
    printf("tick_price: opaque=%p, ticker_id=%d, type=%ld, price=%.2lf, can_auto=%d\n",
           opaque, ticker_id, field, price, can_auto_execute);
}

void event_tick_size(void *opaque, int ticker_id, long type, int size)
{
    printf("tick_size: opaque=%p, ticker_id=%d, type=%ld, size=%d\n",
           opaque, ticker_id, type, size);
}

void event_tick_option_computation(void *opaque, int ticker_id, int type, double implied_vol, double delta, double opt_price, double pv_dividend, double gamma, double vega, double theta, double und_price)
{
    printf("tick option computation: opaque=%p, ticker_id=%d, type=%d, implied_vol=%f, delta=%f, opt_price=%f, pv_dividend=%f, gamma=%f, vega=%f, theta=%f, und_price=%f\n",
           opaque, ticker_id, type, implied_vol, delta, opt_price, pv_dividend, gamma, vega, theta, und_price);
}

void event_tick_generic(void *opaque, int ticker_id, int type, double value)
{
    printf("tick_generic: opaque=%p, ticker_id=%d, type=%d, ...\n", opaque, ticker_id, type);
}

void event_tick_string(void *opaque, int ticker_id, int type, const char value[])
{
    printf("tick_string: opaque=%p, ticker_id=%d, type=%d, ...\n", opaque, ticker_id, type);
}

void event_tick_efp(void *opaque, int ticker_id, int tick_type, double basis_points, const char formatted_basis_points[], double implied_futures_price, int hold_days, const char future_expiry[], double dividend_impact, double dividends_to_expiry)
{
    printf("tick_efp: opaque=%p, ticker_id=%d, type=%d, ...\n", opaque, ticker_id, tick_type);
}

void event_order_status(void *opaque, long order_id, const char status[],
                        int filled, int remaining, double avg_fill_price, int perm_id,
                        int parent_id, double last_fill_price, int client_id, const char why_held[])
{
    printf("order_status: opaque=%p, order_id=%ld, filled=%d remaining %d, avg_fill_price=%lf, last_fill_price=%lf, why_held=%s\n", opaque, order_id, filled, remaining, avg_fill_price, last_fill_price, why_held);
}

void event_open_order(void *opaque, long order_id, const tr_contract_t *contract, const tr_order_t *order, const tr_order_status_t *ostatus)
{
    /* commission values might be DBL_MAX */
    if(fabs(ostatus->ost_commission - DBL_MAX) < DBL_EPSILON)
        printf("open_order: commission not reported\n");
    else
        printf("open_order: commission for %ld was %.4lf\n", order_id, ostatus->ost_commission);

    printf("open_order: opaque=%p, order_id=%ld, sym=%s\n", opaque, order_id, contract->c_symbol);
}

void event_connection_closed(void *opaque)
{
    printf("connection_closed: opaque=%p\n", opaque);
}

void event_update_account_value(void *opaque, const char key[], const char val[],
                                const char currency[], const char account_name[])
{
    printf("update_account_value: %p, key=%s val=%s, currency=%s, name=%s\n",
           opaque, key, val, currency, account_name);
}

void event_update_portfolio(void *opaque, const tr_contract_t *contract, int position,
                            double mkt_price, double mkt_value, double average_cost,
                            double unrealized_pnl, double realized_pnl, const char account_name[])
{
    printf("update_portfolio: %p, sym=%s, position=%d, mkt_price=%.4lf, mkt_value=%.4lf, avg_cost=%.4lf, unrealized_pnl=%.4lf, realized pnl=%.4lf name=%s\n",
           opaque, contract->c_symbol, position, mkt_price, mkt_value, average_cost, unrealized_pnl, realized_pnl, account_name);
}

void event_update_account_time(void *opaque, const char time_stamp[])
{
    printf("update_account_time: opaque=%p, ...\n", opaque);
}

void event_next_valid_id(void *opaque, long order_id)
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

    /* invoked once at connection establishment
     * the scope of this variable is not program wide, instance wide,
     * or even system wide. It has the same scope as the TWS account itself.
     * It appears to be persistent across TWS restarts.
     * Well behaved human and automatic TWS clients shall increment
     * this order_id atomically and cooperatively
     */
    info->next_order_id = order_id;

    printf("next_valid_id for order placement %ld\n", order_id);
}

void event_contract_details(void *opaque, int req_id, const tr_contract_details_t *contract_details)
{
    printf("contract_details: opaque=%p, ...\n", opaque);
}

void event_bond_contract_details(void *opaque, int req_id, const tr_contract_details_t *contract_details)
{
    printf("bond_contract_details: opaque=%p, ...\n", opaque);
}

void event_exec_details(void *opaque, long order_id, const tr_contract_t *contract, const tr_execution_t *execution)
{
    printf("exec_details: opaque=%p, ...\n", opaque);
}

void event_error(void *opaque, int id, int error_code, const char error_string[])
{
    printf("error: opaque=%p, error_code=%d, msg=%s\n", opaque, error_code, error_string);
}

void event_update_mkt_depth(void *opaque, int ticker_id, int position, int operation, int side, double price, int size)
{
    printf("update_mkt_depth: opaque=%p, ticker_id=%d, ...\n", opaque, ticker_id);
}

void event_update_mkt_depth_l2(void *opaque, int ticker_id, int position, char *market_maker, int operation, int side, double price, int size)
{
    printf("update_mkt_depth_l2: opaque=%p, ticker_id=%d, ...\n", opaque, ticker_id);
}

void event_update_news_bulletin(void *opaque, int msgid, int msg_type, const char news_msg[], const char origin_exch[])
{
    printf("update_news_bulletin: opaque=%p, ...\n", opaque);
}

void event_managed_accounts(void *opaque, const char accounts_list[])
{
    printf("managed_accounts: opaque=%p, ...\n", opaque);
}

void event_receive_fa(void *opaque, long fa_data_type, const char cxml[])
{
    printf("receive_fa: opaque=%p, fa_data_type=%ld, xml='%s'\n", opaque, fa_data_type, cxml);
}

void event_historical_data(void *opaque, int reqid, const char date[], double open, double high, double low, double close, int volume, int bar_count, double wap, int has_gaps)
{
    printf("historical: opaque=%p, reqid=%d, date=%s, %.3lf, %.3lf, %.3lf, %.3lf, %d, wap=%.3lf, has_gaps=%d\n", opaque, reqid, date, open, high, low, close, volume, wap, has_gaps);
}

void event_scanner_parameters(void *opaque, const char xml[])
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;

#if defined(_DEBUG)
    mg_cry(info->conn, "INFO: dumping TWS scanner parameters to log file:");
    mg_cry_raw(info->conn, xml);
#endif

    printf("scanner_parameters: opaque=%p, xml=%s\n", opaque, xml);
}

void event_scanner_data(void *opaque, int ticker_id, int rank, tr_contract_details_t *cd, const char distance[], const char benchmark[], const char projection[], const char legs_str[])
{
    printf("scanner_data: opaque=%p, ticker_id=%d, rank=%d, distance=%s, benchmark=%s, projection=%s\n",
           opaque, ticker_id, rank, distance, benchmark, projection);
    printf("scanner_data details: sym=%s, sectype=%s, expiry=%s, strike=%.3lf, right=%s, exch=%s, currency=%s, local_sym=%s, market_name=%s, trading_class=%s\n", cd->d_summary.s_symbol, cd->d_summary.s_sectype, cd->d_summary.s_expiry, cd->d_summary.s_strike, cd->d_summary.s_right, cd->d_summary.s_exchange, cd->d_summary.s_currency, cd->d_summary.s_local_symbol, cd->d_market_name, cd->d_trading_class);

    /* it seems that NYSE traded stocks have different market_name and trading_class from NASDAQ */
}

void event_scanner_data_end(void *opaque, int ticker_id)
{
    printf("scanner_data_end: opaque=%p, ticker_id=%d\n", opaque, ticker_id);
}

void event_current_time(void *opaque, long time)
{
    struct my_tws_io_info *info = (struct my_tws_io_info *)opaque;
    struct tws_thread_exch *exch = info->tws_cfg->exch;

    printf("current_time: opaque=%p, time=%ld, ...\n", opaque, time);

    /*
    Pass response on to front-end now:
    */
    pthread_mutex_lock(&exch->tws_exch_mutex);
    if (0 != pthread_cond_signal(&exch->tws_rx_signal))
    {
      pthread_mutex_unlock(&exch->tws_exch_mutex);
    }
    else
    {
      // process incoming TX command/request from one of the front-end threads:
      mg_cry(info->conn, "backend CURRENT TIME message handler: response count = %d", exch->response);

      exch->response++;
      exch->current_time = time;

      pthread_mutex_unlock(&exch->tws_exch_mutex);
    }
}

void event_realtime_bar(void *opaque, int reqid, long time, double open, double high, double low, double close, long volume, double wap, int count)
{
    printf("realtime_bar: %p reqid=%d time=%ld, ohlc=%.4lf/%.4lf/%.4lf/%.4lf, vol=%ld, wap=%.4lf, count=%d\n", opaque, reqid, time, open, high, low, close, volume, wap, count);
}

void event_fundamental_data(void *opaque, int reqid, const char data[])
{
    printf("fundamental_data: opaque=%p, reqid=%d, ...\n", opaque, reqid);
}

void event_contract_details_end(void *opaque, int reqid)
{
    printf("contract_details_end: opaque=%p, ...\n", opaque);
}

void event_open_order_end(void *opaque)
{
    printf("open_order_end: opaque=%p\n", opaque);
}

void event_delta_neutral_validation(void *opaque, int reqid, under_comp_t *und)
{
    printf("delta_neutral_validation: opaque=%p, reqid=%d, ...\n", opaque, reqid);
}

void event_acct_download_end(void *opaque, char acct_name[])
{
    printf("acct_download_end: opaque=%p, account name='%s'\n", opaque, acct_name);
}

void event_exec_details_end(void *opaque, int reqid)
{
    printf("exec_details_end: opaque=%p, reqid=%d\n", opaque, reqid);
}

void event_tick_snapshot_end(void *opaque, int reqid)
{
    printf("tick_snapshot_end: opaque=%p, reqid=%d\n", opaque, reqid);
}
