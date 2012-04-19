
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

#include "tws_instance.h"

#include "app_manager.h"
#include "tier2_message_processor.h"

#include <mongoose/mongoose_ex.h>




ib_tws_manager::ib_tws_manager(app_manager *mgr) :
	app_mgr(mgr),
	tws_conn(NULL), tws_ctx(NULL), tws_handle(NULL), 
	next_order_id(0),
	req_scanner_parameters_active_set(),
	req_scanner_subscription_active_set(10 /* limit imposed by TWS/IB */),
	req_mkt_data_active_set(),
	req_historical_data_active_set(),
	exercise_options_active_set(),
	place_order_active_set(),
	req_open_orders_active_set(),
	req_account_updates_active_set(),
	req_executions_active_set(),
	req_ids_active_set(),
	req_contract_details_active_set(),
	req_mkt_depth_active_set(),
	req_news_bulletins_active_set(),
	set_server_log_level_active_set(),
	req_auto_open_orders_active_set(),
	req_all_open_orders_active_set(),
	req_managed_accts_active_set(),
	request_fa_active_set(),
	replace_fa_active_set(),
	req_current_time_active_set(),
	req_fundamental_data_active_set(),
	calculate_implied_volatility_active_set(),
	calculate_option_price_active_set(),
	req_market_data_type_active_set(),
	request_realtime_bars_active_set()
{
}

ib_tws_manager::~ib_tws_manager()
{
}




tier2_message_processor *ib_tws_manager::get_receiver(void)
{
	app_manager *mgr = get_app_manager();
	mg_context *ctx = get_context();
	mg_connection *conn = get_connection();

	tier2_message_processor *rv = mgr->get_receiver(ctx, app_manager::IB_TWS_API_CONNECTION_THREAD);
	assert(rv);
	return rv;
}
