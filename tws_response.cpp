
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


#include "tws_response.h"

#include "tws_request.h"
#include "tier2_message_processor.h"
#include "interthread_comm.h"
#include "app_manager.h"
#include "tws_instance.h"

#include <mongoose/mongoose_ex.h>






int ib_msg_resp_update_mkt_depth::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_update_mkt_depth");

	return 0;
}

int ib_msg_resp_market_data_type::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_market_data_type");

	return 0;
}

int ib_msg_resp_open_order::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_open_order");

	return 0;
}

int ib_msg_resp_order_status::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_order_status");

	return 0;
}

int ib_msg_resp_scanner_data::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_scanner_data");

	return 0;
}

int ib_msg_resp_historical_data::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_historical_data");

	return 0;
}

int ib_msg_resp_update_mkt_depth_l2::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_update_mkt_depth_l2");

	return 0;
}

int ib_msg_resp_delta_neutral_validation::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_delta_neutral_validation");

	return 0;
}

int ib_msg_resp_bond_contract_details::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_bond_contract_details");

	return 0;
}

int ib_msg_resp_scanner_data_end::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_scanner_data_end");

	return 0;
}

int ib_msg_resp_scanner_data_start::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_scanner_data_start");

	return 0;
}

int ib_msg_resp_tick_size::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_tick_size");

	return 0;
}

int ib_msg_resp_tick_snapshot_end::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_tick_snapshot_end");

	return 0;
}

int ib_msg_resp_update_account_value::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_update_account_value");

	return 0;
}

int ib_msg_resp_receive_fa::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_receive_fa");

	return 0;
}

int ib_msg_resp_open_order_end::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_open_order_end");

	return 0;
}

int ib_msg_resp_contract_details::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_contract_details");

	return 0;
}

int ib_msg_resp_exec_details::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_exec_details");

	return 0;
}

int ib_msg_resp_managed_accounts::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_managed_accounts");

	return 0;
}

int ib_msg_resp_tick_efp::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_tick_efp");

	return 0;
}

int ib_msg_resp_update_account_time::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_update_account_time");

	return 0;
}

int ib_msg_resp_tick_option_computation::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_tick_option_computation");

	return 0;
}

int ib_msg_resp_tick_price::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_tick_price");

	return 0;
}

int ib_msg_resp_error::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_error");

	return 0;
}

int ib_msg_resp_acct_download_end::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_acct_download_end");

	return 0;
}

int ib_msg_resp_commission_report::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_commission_report");

	return 0;
}

int ib_msg_resp_fundamental_data::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_fundamental_data");

	return 0;
}

int ib_msg_resp_realtime_bar::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_realtime_bar");

	return 0;
}

int ib_msg_resp_historical_data_end::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_historical_data_end");

	return 0;
}

int ib_msg_resp_next_valid_id::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_next_valid_id");

	return 0;
}

int ib_msg_resp_contract_details_end::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_contract_details_end");

	return 0;
}

int ib_msg_resp_update_news_bulletin::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_update_news_bulletin");

	return 0;
}

int ib_msg_resp_exec_details_end::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_exec_details_end");

	return 0;
}

int ib_msg_resp_tick_string::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_tick_string");

	return 0;
}

int ib_msg_resp_scanner_parameters::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_scanner_parameters");

	return 0;
}

int ib_msg_resp_current_time::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_current_time");

	return 0;
}

int ib_msg_resp_tick_generic::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_tick_generic");

	return 0;
}

int ib_msg_resp_update_portfolio::process_response(class tier2_message *resp_msg)
{
	assert(resp_msg == NULL);
	app_manager *mgr = get_requester()->get_app_manager();
	ib_tws_manager *ibm = mgr->get_ib_tws_manager();
	struct mg_connection *conn = ibm->get_connection();

	mg_cry(conn, "process response message for %s?", "ib_msg_resp_update_portfolio");

	return 0;
}


