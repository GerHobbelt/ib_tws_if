
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

#include "unique_type_id.h"


#include "tier2_message.h"
#include "tws_request.h"
#include "tws_response.h"





//UNIQUE_TYPE_ID_CLASSIMPL(tier2_message, tier2_msg_typeid_mgr);
bool tier2_message::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t tier2_message::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t tier2_message::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(cancel_message, tier2_msg_typeid_mgr);
bool cancel_message::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t cancel_message::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t cancel_message::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(schedule_message, tier2_msg_typeid_mgr);
bool schedule_message::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t schedule_message::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t schedule_message::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(tws_reqresp_message, tier2_msg_typeid_mgr);
bool tws_reqresp_message::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t tws_reqresp_message::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t tws_reqresp_message::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(tws_request_message, tier2_msg_typeid_mgr);
bool tws_request_message::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t tws_request_message::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t tws_request_message::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(tws_response_message, tier2_msg_typeid_mgr);
bool tws_response_message::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t tws_response_message::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t tws_response_message::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();


//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_update_mkt_depth, tier2_msg_typeid_mgr);
bool ib_msg_resp_update_mkt_depth::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_update_mkt_depth::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_update_mkt_depth::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_market_data_type, tier2_msg_typeid_mgr);
bool ib_msg_resp_market_data_type::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_market_data_type::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_market_data_type::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_open_order, tier2_msg_typeid_mgr);
bool ib_msg_resp_open_order::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_open_order::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_open_order::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_order_status, tier2_msg_typeid_mgr);
bool ib_msg_resp_order_status::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_order_status::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_order_status::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_scanner_data, tier2_msg_typeid_mgr);
bool ib_msg_resp_scanner_data::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_scanner_data::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_scanner_data::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_historical_data, tier2_msg_typeid_mgr);
bool ib_msg_resp_historical_data::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_historical_data::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_historical_data::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_update_mkt_depth_l2, tier2_msg_typeid_mgr);
bool ib_msg_resp_update_mkt_depth_l2::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_update_mkt_depth_l2::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_update_mkt_depth_l2::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_delta_neutral_validation, tier2_msg_typeid_mgr);
bool ib_msg_resp_delta_neutral_validation::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_delta_neutral_validation::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_delta_neutral_validation::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_bond_contract_details, tier2_msg_typeid_mgr);
bool ib_msg_resp_bond_contract_details::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_bond_contract_details::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_bond_contract_details::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_scanner_data_end, tier2_msg_typeid_mgr);
bool ib_msg_resp_scanner_data_end::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_scanner_data_end::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_scanner_data_end::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_scanner_data_start, tier2_msg_typeid_mgr);
bool ib_msg_resp_scanner_data_start::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_scanner_data_start::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_scanner_data_start::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_tick_size, tier2_msg_typeid_mgr);
bool ib_msg_resp_tick_size::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_tick_size::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_tick_size::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_tick_snapshot_end, tier2_msg_typeid_mgr);
bool ib_msg_resp_tick_snapshot_end::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_tick_snapshot_end::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_tick_snapshot_end::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_update_account_value, tier2_msg_typeid_mgr);
bool ib_msg_resp_update_account_value::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_update_account_value::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_update_account_value::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_receive_fa, tier2_msg_typeid_mgr);
bool ib_msg_resp_receive_fa::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_receive_fa::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_receive_fa::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_open_order_end, tier2_msg_typeid_mgr);
bool ib_msg_resp_open_order_end::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_open_order_end::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_open_order_end::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_contract_details, tier2_msg_typeid_mgr);
bool ib_msg_resp_contract_details::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_contract_details::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_contract_details::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_exec_details, tier2_msg_typeid_mgr);
bool ib_msg_resp_exec_details::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_exec_details::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_exec_details::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_managed_accounts, tier2_msg_typeid_mgr);
bool ib_msg_resp_managed_accounts::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_managed_accounts::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_managed_accounts::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_tick_efp, tier2_msg_typeid_mgr);
bool ib_msg_resp_tick_efp::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_tick_efp::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_tick_efp::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_update_account_time, tier2_msg_typeid_mgr);
bool ib_msg_resp_update_account_time::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_update_account_time::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_update_account_time::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_tick_option_computation, tier2_msg_typeid_mgr);
bool ib_msg_resp_tick_option_computation::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_tick_option_computation::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_tick_option_computation::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_tick_price, tier2_msg_typeid_mgr);
bool ib_msg_resp_tick_price::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_tick_price::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_tick_price::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_error, tier2_msg_typeid_mgr);
bool ib_msg_resp_error::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_error::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_error::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_acct_download_end, tier2_msg_typeid_mgr);
bool ib_msg_resp_acct_download_end::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_acct_download_end::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_acct_download_end::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_commission_report, tier2_msg_typeid_mgr);
bool ib_msg_resp_commission_report::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_commission_report::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_commission_report::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_fundamental_data, tier2_msg_typeid_mgr);
bool ib_msg_resp_fundamental_data::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_fundamental_data::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_fundamental_data::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_realtime_bar, tier2_msg_typeid_mgr);
bool ib_msg_resp_realtime_bar::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_realtime_bar::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_realtime_bar::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_historical_data_end, tier2_msg_typeid_mgr);
bool ib_msg_resp_historical_data_end::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_historical_data_end::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_historical_data_end::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_next_valid_id, tier2_msg_typeid_mgr);
bool ib_msg_resp_next_valid_id::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_next_valid_id::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_next_valid_id::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_contract_details_end, tier2_msg_typeid_mgr);
bool ib_msg_resp_contract_details_end::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_contract_details_end::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_contract_details_end::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_update_news_bulletin, tier2_msg_typeid_mgr);
bool ib_msg_resp_update_news_bulletin::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_update_news_bulletin::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_update_news_bulletin::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_exec_details_end, tier2_msg_typeid_mgr);
bool ib_msg_resp_exec_details_end::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_exec_details_end::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_exec_details_end::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_tick_string, tier2_msg_typeid_mgr);
bool ib_msg_resp_tick_string::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_tick_string::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_tick_string::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_scanner_parameters, tier2_msg_typeid_mgr);
bool ib_msg_resp_scanner_parameters::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_scanner_parameters::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_scanner_parameters::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_current_time, tier2_msg_typeid_mgr);
bool ib_msg_resp_current_time::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_current_time::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_current_time::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_tick_generic, tier2_msg_typeid_mgr);
bool ib_msg_resp_tick_generic::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_tick_generic::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_tick_generic::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_resp_update_portfolio, tier2_msg_typeid_mgr);
bool ib_msg_resp_update_portfolio::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_resp_update_portfolio::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_resp_update_portfolio::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();


//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_scanner_parameters, tier2_msg_typeid_mgr);
bool ib_msg_req_scanner_parameters::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_req_scanner_parameters::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_req_scanner_parameters::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_scanner_subscription, tier2_msg_typeid_mgr);
bool ib_msg_req_scanner_subscription::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_req_scanner_subscription::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_req_scanner_subscription::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_cancel_scanner_subscription, tier2_msg_typeid_mgr);
bool ib_msg_cancel_scanner_subscription::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_cancel_scanner_subscription::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_cancel_scanner_subscription::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_mkt_data, tier2_msg_typeid_mgr);
bool ib_msg_req_mkt_data::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_req_mkt_data::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_req_mkt_data::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_historical_data, tier2_msg_typeid_mgr);
bool ib_msg_req_historical_data::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_req_historical_data::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_req_historical_data::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_cancel_historical_data, tier2_msg_typeid_mgr);
bool ib_msg_cancel_historical_data::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_cancel_historical_data::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_cancel_historical_data::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_cancel_mkt_data, tier2_msg_typeid_mgr);
bool ib_msg_cancel_mkt_data::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_cancel_mkt_data::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_cancel_mkt_data::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_exercise_options, tier2_msg_typeid_mgr);
bool ib_msg_exercise_options::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_exercise_options::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_exercise_options::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_place_order, tier2_msg_typeid_mgr);
bool ib_msg_place_order::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_place_order::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_place_order::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_cancel_order, tier2_msg_typeid_mgr);
bool ib_msg_cancel_order::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_cancel_order::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_cancel_order::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_open_orders, tier2_msg_typeid_mgr);
bool ib_msg_req_open_orders::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_req_open_orders::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_req_open_orders::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_account_updates, tier2_msg_typeid_mgr);
bool ib_msg_req_account_updates::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_req_account_updates::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_req_account_updates::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_executions, tier2_msg_typeid_mgr);
bool ib_msg_req_executions::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_req_executions::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_req_executions::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_ids, tier2_msg_typeid_mgr);
bool ib_msg_req_ids::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_req_ids::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_req_ids::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_contract_details, tier2_msg_typeid_mgr);
bool ib_msg_req_contract_details::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_req_contract_details::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_req_contract_details::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_mkt_depth, tier2_msg_typeid_mgr);
bool ib_msg_req_mkt_depth::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_req_mkt_depth::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_req_mkt_depth::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_cancel_mkt_depth, tier2_msg_typeid_mgr);
bool ib_msg_cancel_mkt_depth::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_cancel_mkt_depth::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_cancel_mkt_depth::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_news_bulletins, tier2_msg_typeid_mgr);
bool ib_msg_req_news_bulletins::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_req_news_bulletins::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_req_news_bulletins::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_cancel_news_bulletins, tier2_msg_typeid_mgr);
bool ib_msg_cancel_news_bulletins::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_cancel_news_bulletins::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_cancel_news_bulletins::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_set_server_log_level, tier2_msg_typeid_mgr);
bool ib_msg_set_server_log_level::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_set_server_log_level::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_set_server_log_level::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_auto_open_orders, tier2_msg_typeid_mgr);
bool ib_msg_req_auto_open_orders::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_req_auto_open_orders::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_req_auto_open_orders::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_all_open_orders, tier2_msg_typeid_mgr);
bool ib_msg_req_all_open_orders::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_req_all_open_orders::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_req_all_open_orders::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_managed_accts, tier2_msg_typeid_mgr);
bool ib_msg_req_managed_accts::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_req_managed_accts::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_req_managed_accts::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_request_fa, tier2_msg_typeid_mgr);
bool ib_msg_request_fa::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_request_fa::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_request_fa::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_replace_fa, tier2_msg_typeid_mgr);
bool ib_msg_replace_fa::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_replace_fa::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_replace_fa::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_current_time, tier2_msg_typeid_mgr);
bool ib_msg_req_current_time::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_req_current_time::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_req_current_time::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_fundamental_data, tier2_msg_typeid_mgr);
bool ib_msg_req_fundamental_data::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_req_fundamental_data::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_req_fundamental_data::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_cancel_fundamental_data, tier2_msg_typeid_mgr);
bool ib_msg_cancel_fundamental_data::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_cancel_fundamental_data::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_cancel_fundamental_data::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_calculate_implied_volatility, tier2_msg_typeid_mgr);
bool ib_msg_calculate_implied_volatility::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_calculate_implied_volatility::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_calculate_implied_volatility::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_cancel_calculate_implied_volatility, tier2_msg_typeid_mgr);
bool ib_msg_cancel_calculate_implied_volatility::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_cancel_calculate_implied_volatility::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_cancel_calculate_implied_volatility::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_calculate_option_price, tier2_msg_typeid_mgr);
bool ib_msg_calculate_option_price::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_calculate_option_price::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_calculate_option_price::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_cancel_calculate_option_price, tier2_msg_typeid_mgr);
bool ib_msg_cancel_calculate_option_price::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_cancel_calculate_option_price::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_cancel_calculate_option_price::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_global_cancel, tier2_msg_typeid_mgr);
bool ib_msg_req_global_cancel::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_req_global_cancel::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_req_global_cancel::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();

//UNIQUE_TYPE_ID_CLASSIMPL(ib_msg_req_market_data_type, tier2_msg_typeid_mgr);
bool ib_msg_req_market_data_type::type_matches(const tier2_message *msg) const				
{																
	return msg->get_type_id() == get_type_id();					
}																
unique_id_t ib_msg_req_market_data_type::get_type_id(void) const							
{																
	if (!m_type_id)												
	{															
		m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();					
	}															
	return m_type_id;											
}																
/* 																
	uninitialized; 												
	will be set up as soon as anyone invokes get_type_id() 		
 */																
unique_id_t ib_msg_req_market_data_type::m_type_id = tier2_msg_typeid_mgr.obtain_unique_id();







