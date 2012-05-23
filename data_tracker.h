
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

#ifndef DATA_TRACKER_CPP_HEADER_INCLUDED
#define DATA_TRACKER_CPP_HEADER_INCLUDED

/*
The 'data_tracker_manager' represents that single (back-end) thread which continuously tracks and processes stock data.
*/

#include "tws_config_struct.h"
#include "tws_request.h"
#include "tws_response.h"
#include "tier2_message_queue.h"
#include "unique_type_id.h"
#include "tws_backend_io.h"


// forward reference:
class app_manager;
namespace tws
{
	typedef struct tws_instance tws_instance_t;
}
class ib_backend_io_logger;
class ib_tws_manager;






class data_tracker_manager : public tier2_message_processor
{
protected:
	data_tracker_manager(app_manager *mgr);
public:
	virtual ~data_tracker_manager();
	static data_tracker_manager *get_instance(app_manager *mgr, bool instantiate_singleton = false);

protected:
	typedef std::vector<ib_contract_details *> cd_store_t;
	cd_store_t m_cds;

public:
	int register_contract_info(const ib_contract_details *cd);

protected:
	int scan_queue_and_process(tier2_message *resp_msg);
	int cancel_all_matching_requests(tws_request_message *cancel_req_msg);
};





#endif // DATA_TRACKER_CPP_HEADER_INCLUDED
