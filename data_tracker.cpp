
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

/*
 * Mongoose-based server app which interfaces with a TWS station and displays data in HTML form,
 * suitable for loading by, for example, 64-bit Excel 2010, using web queries.
 */

#include <tws_c_api/twsapi.h>

#include "app_manager.h"
#include "tws_database_io.h"
#include "tws_backend_io.h"
#include "data_tracker.h"

#include <mongoose/mongoose_ex.h>

// kill mongoose/OTL collisions due to mongoose #defines:
#undef close

#include <otl/otlv4_h/otlv4.h>






#define DATA_TRACKER_INIT_RETRY_DELAY     10000 // unit: milliseconds



data_tracker_manager::data_tracker_manager(app_manager *mgr)
	: tier2_message_processor(new requester_id(NULL, NULL, 0), mgr)
	, m_global_ctx(NULL)
{
}

data_tracker_manager::~data_tracker_manager()
{
}




int data_tracker_manager::process_one_event(void)
{
	return 0;
}




/*
Track any 'registered' stocks: 

- use this thread to run any calculations based on the incoming data / user activity.

- issue historical data requests when the data is lacking

- run what-if scenarios, etc.etc.
*/
void data_tracker_thread(struct mg_context *ctx)
{
	app_manager *mgr = (app_manager *)mg_get_user_data(ctx)->user_data;
	data_tracker_manager *clm = mgr->get_data_tracker_manager();
	db_manager *dbm = mgr->get_db_manager();

	clm->set_context(ctx);
	mgr->register_backend_thread(ctx, app_manager::DATA_TRACKER_THREAD, clm);
	
    // retry connecting to the database as long as the server itself hasn't been stopped!
    while (mg_get_stop_flag(ctx) == 0)
    {
        int err;
		int abortus_provocatus = 0;

		err = dbm->open_databases();
		if (err)
		{
			mg_cry4ctx(ctx, "FATAL ERROR: Cannot access the database: %d (%s)\n", err, dbm->strerror(err));
			abortus_provocatus = 1;
			goto fail_dramatically;
		}

		assert(mgr->get_requester(ctx) == clm);

        while (mg_get_stop_flag(ctx) == 0)
        {
			// push messages across the thread barrier + destroy messages marked for destruction
			if (0 != clm->pulse_marked_messages())
			{
				break;
			}

            // process another message
            if (0 != clm->process_one_event())
            {
                break;
            }
		}

fail_dramatically:
		err = dbm->close_databases();

		if (abortus_provocatus)
		{
			mg_signal_stop(ctx);
			break;
		}

        // wait N seconds before retrying to connect to TWS:
        if (mg_get_stop_flag(ctx) == 0)
        {
            mg_sleep(DATA_TRACKER_INIT_RETRY_DELAY);
        }
    }

	mgr->unregister_backend_thread(ctx, app_manager::DATA_TRACKER_THREAD);

    mg_signal_mgr_this_thread_is_done(ctx);

    DEBUG_TRACE(("exiting"));
}







static data_tracker_manager *singleton = NULL;


data_tracker_manager *data_tracker_manager::get_instance(app_manager *mgr, bool instantiate_singleton)
{
	if (!singleton && instantiate_singleton)
	{
		data_tracker_manager *obj = new data_tracker_manager(mgr);

		set_instance(obj);
		return obj;
	}
	return singleton;
}

void data_tracker_manager::set_instance(data_tracker_manager *instance)
{
	if (singleton)
	{
		delete singleton;
	}
	singleton = instance;
}



