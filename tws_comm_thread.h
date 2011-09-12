
// Copyright (c) 2011 Ger Hobbelt
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef TWS_COMM_THREAD_HEADER_INCLUDED
#define TWS_COMM_THREAD_HEADER_INCLUDED

#include "tws_backend.h"

#include <tws_c_api/twsapi.h>



#ifdef __cplusplus
extern "C" {
#endif // __cplusplus



/*
===============================================================================

backend ('middle tier' bla bla bla) request / response queue structures and functions:

===============================================================================
*/

// forward reference:
typedef struct tier2_queue_item tier2_queue_item_t;

/*
callback which is invoked when the command has been received and has been parsed,
i.e. has been popped from the queue and the backend doesn't need the *command* data
any longer; it WILL continue to access to the response data/hooks though!

Use this one to clean up allocation of dynamic resources done in the front-end.
*/
typedef int tier2_command_done_handler(tier2_queue_item_t *cmd);
/*
And this one can be invoked by the front-end to have the back-end clean up its own resources
related to the reponse, as the front-end will invoke this one when it's completed processing
and accessing the response data contained in the tier2_queue_item.
*/
typedef int tier2_response_done_handler(tier2_queue_item_t **cmd);

enum tier2_command_code
{
    TIER2_NO_COMMAND = 0,
    TIER2_FETCH_SERVER_TIMESTAMP,
};

enum tier2_response_code
{
    TIER2_ABORTED = -1,
    TIER2_NO_RESPONSE_YET = 0,
    TIER2_RESPONSE_READY = 1,
};


struct tier2_queue_item
{
    enum tier2_command_code command_code;       // what's the front-end requesting we do?
    union
    {
        void *propagate_ptr;
        // ...
    } command_params;

    tier2_command_done_handler *cleanup_after_command; // invoked when front-end MAY clean the command-side related resources it allocated for us
    tier2_response_done_handler *cleanup_after_response; // invoked when the front-end has completed processing the reponse and the backend can release any allocated resources which are related to this request.

    volatile union
    {
        enum tier2_response_code code;
        int value;
    } response;      // 0: pending, 1: success/done; < 0: error; > 1: warning/notification
    union
    {
        void *propagate_ptr;
        // ...
    } response_params;
};

// front-end invoked queue calls:
int tier2_push_request(tier2_queue_item_t *cmd);
int tier2_abort_request(tier2_queue_item_t *cmd);

// back-end invoked queue calls:
tier2_queue_item_t *tier2_pop_request(void);

// utility calls:
unsigned int tier2_queue_depth(void);
tier2_queue_item_t *tier2_peek_request(int optional_index);






/* internal communication stuff between mongoose threads and the tws back-end thread goes here: */
struct tws_thread_exch
{
    pthread_mutex_t tws_exch_mutex;     // mutex related to the TX & RX conditions in here
    pthread_cond_t tws_tx_signal;       // signalled when a TX request is pending (should be processed by the tws 'back-end' thread.
    pthread_cond_t tws_rx_signal;       // signalled when a RX response is pending (should be processed by the related TX-invoking 'front-end' mongoose thread.

    int command;
    int response;
    time_t current_time;

    tier2_queue_item_t **work_queue;
    size_t work_queue_size;           // allocated size
    size_t work_queue_fill;           // number of requests pending in the queue
    size_t work_queue_poppos;         // the position of the queue 'head' for popping
    size_t work_queue_diane;          // the position of the queue 'head' for pushing
};





void init_tws_thread_exch(struct tws_thread_exch **ptr);
void destroy_tws_thread_exch(struct tws_thread_exch **ptr);





struct scanner_subscription_request_t;

/*
struct passed around as user parameter for all TWS API callbacks.
*/
struct my_tws_io_info
{
    struct mg_connection *conn;
    struct mg_context *ctx;
    struct tws_conn_cfg *tws_cfg;
    void *tws_handle;

    /* tracking some TWS values here as well: */
    int next_order_id;

    /* -- and the working men -- */

    /* scanner subscription request active set and queue: */
    size_t active_scanner_subscription_count;
    struct scanner_subscription_request_t *active_scanner_subscriptions[10];

    size_t queued_scanner_subscription_count;
    size_t queued_scanner_subscription_allocsize;
    struct scanner_subscription_request_t **scanner_subscription_queue;

    /* keeping track of the current TWS response message's number of items in the TWS response message: */
    size_t row_count;
};



/* --- scanner subscription request queue --- */


typedef struct scanner_subscription_request_t
{
    int ticker_id;
    tr_scanner_subscription_t reqdata;
} scanner_subscription_request_t;







void tws_worker_thread(struct mg_context *ctx);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // TWS_COMM_THREAD_HEADER_INCLUDED
