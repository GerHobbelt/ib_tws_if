
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

#include "app_manager.h"

#include <tws_c_api/twsapi-debug.h>
#include <mongoose/mongoose_ex.h>





/*
replace TWSAPI debug printf call.
*/
void tws_debug_printf(void *opaque, const char *fmt, ...)
{
	app_manager *mgr = (app_manager *)opaque;
	ib_tws_manager *ibm = (mgr ? mgr->get_ib_tws_manager() : NULL);
	va_list ap;

	va_start(ap, fmt);
	mg_vlog((ibm ? ibm->get_connection() : NULL), "debug", fmt, ap);
	va_end(ap);
}
































































































































/*
de grote opvangbak van ellende
*/
#include "tws_request.h"
#include "tws_response.h"
#include "app_manager.h"
#include "tws_database_io.h"
#include "tws_instance.h"



tier2_message_requester * app_manager::get_requester(mg_connection *)
{
	return 0;
}

bool tws_reqresp_message::equal(tier2_message const &)const 
{
	return 0;
}

bool tws_reqresp_message::equal(tws_reqresp_message const &)const 
{
	return 0;
}

bool ib_req_current_time::equal(tier2_message const &)const 
{
	return 0;
}

int ib_req_current_time::load(void)
{
	return 0;
}

int ib_req_current_time::store(void)
{
	return 0;
}

int ib_req_current_time::process_response(tier2_message &)
{
	return 0;
}

int ib_req_current_time::cancel_request(void)
{
	return 0;
}

int ib_req_current_time::transmit(void)
{
	return 0;
}

bool tier2_message::equal(tier2_message const &)const 
{
	return 0;
}

int tier2_message::load(void)
{
	return 0;
}

int tier2_message::store(void)
{
	return 0;
}

int tier2_message::cancel_request(void)
{
	return 0;
}

int tier2_message::transmit_and_wait_for_response(app_manager *)
{
	return 0;
}

int tier2_message::process_response(tier2_message &)
{
	return 0;
}

int tier2_message::transmit(void)
{
	return 0;
}

void tier2_message::release_unique_msgID(void)
{
	return;
}

int tier2_message::obtain_next_unique_msgID(void)
{
	return 0;
}

bool ib_resp_acct_download_end::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_acct_download_end::load(void)
{
	return 0;
}

int ib_resp_acct_download_end::store(void)
{
	return 0;
}

bool ib_resp_tick_generic::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_tick_generic::load(void)
{
	return 0;
}

int ib_resp_tick_generic::store(void)
{
	return 0;
}

bool ib_resp_current_time::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_current_time::load(void)
{
	return 0;
}

int ib_resp_current_time::store(void)
{
	return 0;
}

bool ib_resp_next_valid_id::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_next_valid_id::load(void)
{
	return 0;
}

int ib_resp_next_valid_id::store(void)
{
	return 0;
}

bool ib_resp_contract_details_end::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_contract_details_end::load(void)
{
	return 0;
}

int ib_resp_contract_details_end::store(void)
{
	return 0;
}

bool ib_resp_update_account_time::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_update_account_time::load(void)
{
	return 0;
}

int ib_resp_update_account_time::store(void)
{
	return 0;
}

bool ib_resp_tick_snapshot_end::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_tick_snapshot_end::load(void)
{
	return 0;
}

int ib_resp_tick_snapshot_end::store(void)
{
	return 0;
}

bool ib_resp_open_order_end::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_open_order_end::load(void)
{
	return 0;
}

int ib_resp_open_order_end::store(void)
{
	return 0;
}

bool ib_resp_market_data_type::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_market_data_type::load(void)
{
	return 0;
}

int ib_resp_market_data_type::store(void)
{
	return 0;
}

bool ib_resp_receive_fa::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_receive_fa::load(void)
{
	return 0;
}

int ib_resp_receive_fa::store(void)
{
	return 0;
}

bool ib_resp_tick_size::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_tick_size::load(void)
{
	return 0;
}

int ib_resp_tick_size::store(void)
{
	return 0;
}

bool ib_resp_exec_details::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_exec_details::load(void)
{
	return 0;
}

int ib_resp_exec_details::store(void)
{
	return 0;
}

bool ib_resp_historical_data::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_historical_data::load(void)
{
	return 0;
}

int ib_resp_historical_data::store(void)
{
	return 0;
}

void app_manager::process_response_message(tier2_message *)
{
	return;
}

bool ib_resp_exec_details_end::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_exec_details_end::load(void)
{
	return 0;
}

int ib_resp_exec_details_end::store(void)
{
	return 0;
}

bool ib_resp_update_mkt_depth_l2::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_update_mkt_depth_l2::load(void)
{
	return 0;
}

int ib_resp_update_mkt_depth_l2::store(void)
{
	return 0;
}

bool ib_resp_order_status::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_order_status::load(void)
{
	return 0;
}

int ib_resp_order_status::store(void)
{
	return 0;
}

bool ib_resp_error::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_error::load(void)
{
	return 0;
}

int ib_resp_error::store(void)
{
	return 0;
}

bool ib_resp_contract_details::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_contract_details::load(void)
{
	return 0;
}

int ib_resp_contract_details::store(void)
{
	return 0;
}

bool ib_resp_commission_report::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_commission_report::load(void)
{
	return 0;
}

int ib_resp_commission_report::store(void)
{
	return 0;
}

bool ib_resp_scanner_data_start::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_scanner_data_start::load(void)
{
	return 0;
}

int ib_resp_scanner_data_start::store(void)
{
	return 0;
}

bool ib_resp_bond_contract_details::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_bond_contract_details::load(void)
{
	return 0;
}

int ib_resp_bond_contract_details::store(void)
{
	return 0;
}

bool ib_resp_tick_option_computation::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_tick_option_computation::load(void)
{
	return 0;
}

int ib_resp_tick_option_computation::store(void)
{
	return 0;
}

bool ib_resp_tick_price::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_tick_price::load(void)
{
	return 0;
}

int ib_resp_tick_price::store(void)
{
	return 0;
}

bool ib_resp_scanner_data_end::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_scanner_data_end::load(void)
{
	return 0;
}

int ib_resp_scanner_data_end::store(void)
{
	return 0;
}

bool ib_resp_fundamental_data::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_fundamental_data::load(void)
{
	return 0;
}

int ib_resp_fundamental_data::store(void)
{
	return 0;
}

bool ib_resp_delta_neutral_validation::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_delta_neutral_validation::load(void)
{
	return 0;
}

int ib_resp_delta_neutral_validation::store(void)
{
	return 0;
}

bool ib_resp_update_mkt_depth::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_update_mkt_depth::load(void)
{
	return 0;
}

int ib_resp_update_mkt_depth::store(void)
{
	return 0;
}

bool ib_resp_update_news_bulletin::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_update_news_bulletin::load(void)
{
	return 0;
}

int ib_resp_update_news_bulletin::store(void)
{
	return 0;
}

bool ib_resp_update_account_value::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_update_account_value::load(void)
{
	return 0;
}

int ib_resp_update_account_value::store(void)
{
	return 0;
}

bool ib_resp_update_portfolio::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_update_portfolio::load(void)
{
	return 0;
}

int ib_resp_update_portfolio::store(void)
{
	return 0;
}

bool ib_resp_tick_efp::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_tick_efp::load(void)
{
	return 0;
}

int ib_resp_tick_efp::store(void)
{
	return 0;
}

bool ib_resp_historical_data_end::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_historical_data_end::load(void)
{
	return 0;
}

int ib_resp_historical_data_end::store(void)
{
	return 0;
}

bool ib_resp_scanner_data::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_scanner_data::load(void)
{
	return 0;
}

int ib_resp_scanner_data::store(void)
{
	return 0;
}

bool ib_resp_realtime_bar::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_realtime_bar::load(void)
{
	return 0;
}

int ib_resp_realtime_bar::store(void)
{
	return 0;
}

bool ib_resp_scanner_parameters::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_scanner_parameters::load(void)
{
	return 0;
}

int ib_resp_scanner_parameters::store(void)
{
	return 0;
}

bool ib_resp_tick_string::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_tick_string::load(void)
{
	return 0;
}

int ib_resp_tick_string::store(void)
{
	return 0;
}

bool ib_resp_managed_accounts::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_managed_accounts::load(void)
{
	return 0;
}

int ib_resp_managed_accounts::store(void)
{
	return 0;
}

bool ib_resp_open_order::equal(tier2_message const &)const 
{
	return 0;
}

int ib_resp_open_order::load(void)
{
	return 0;
}

int ib_resp_open_order::store(void)
{
	return 0;
}

tier2_message_receiver * app_manager::get_receiver(mg_connection *)
{
	return 0;
}

tier2_message_requester * app_manager::get_requester(mg_context *,int)
{
	return 0;
}

bool ib_req_scanner_parameters::equal(tier2_message const &)const 
{
	return 0;
}

int ib_req_scanner_parameters::load(void)
{
	return 0;
}

int ib_req_scanner_parameters::store(void)
{
	return 0;
}

int ib_req_scanner_parameters::process_response(tier2_message &)
{
	return 0;
}

int ib_req_scanner_parameters::cancel_request(void)
{
	return 0;
}

int ib_req_scanner_parameters::transmit(void)
{
	return 0;
}

tier2_message_receiver *ib_tws_manager::get_receiver(void)
{
	return 0;
}





