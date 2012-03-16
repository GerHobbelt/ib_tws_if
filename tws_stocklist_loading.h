
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

#ifndef TWS_STOCKLIST_LOAD_THREAD_HEADER_INCLUDED
#define TWS_STOCKLIST_LOAD_THREAD_HEADER_INCLUDED

#include "tws_backend.h"
#include "tws_comm_thread.h"


//#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
//#include <libxml/parserInternals.h>
#include <libxml/HTMLparser.h>
#include <libxml/HTMLtree.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/debugXML.h>
#include <libxml/xmlerror.h>
#ifdef LIBXML_XINCLUDE_ENABLED
#include <libxml/xinclude.h>
#endif
#ifdef LIBXML_CATALOG_ENABLED
#include <libxml/catalog.h>
#endif
#include <libxml/globals.h>
#include <libxml/xmlreader.h>
#ifdef LIBXML_SCHEMATRON_ENABLED
#include <libxml/schematron.h>
#endif
#ifdef LIBXML_SCHEMAS_ENABLED
#include <libxml/relaxng.h>
#include <libxml/xmlschemas.h>
#endif
#ifdef LIBXML_PATTERN_ENABLED
#include <libxml/pattern.h>
#endif
#ifdef LIBXML_C14N_ENABLED
#include <libxml/c14n.h>
#endif
#ifdef LIBXML_OUTPUT_ENABLED
#include <libxml/xmlsave.h>
#endif




xmlNodePtr xmlGetParent(xmlNodePtr node);

void xmlXPathContainsAnyOfFunction(xmlXPathParserContextPtr ctxt, int nargs);


long tws_mkNextOrderId(my_tws_io_info *info);
void tws_setNextOrderId(my_tws_io_info *info, long order_id);


void push_tws_req_scanner_subscription(my_tws_io_info *info, scanner_subscription_request *reqdata);
size_t pop_tws_req_scanner_subscription(my_tws_io_info *info);
void cancel_tws_scanner_subscription(my_tws_io_info *info, int ticker_id);
int is_active_tws_scanner_subscription(my_tws_io_info *info, int ticker_id);
scanner_subscription_request *get_active_tws_scanner_subscription(my_tws_io_info *info, int ticker_id);


void request_range_of_interesting_market_scans(my_tws_io_info *info, xmlNodePtr location, xmlXPathObjectPtr usable_scanners);
void request_contract_details_from_tws(my_tws_io_info *info, tr_contract_details_t *cd);



#endif // TWS_STOCKLIST_LOAD_THREAD_HEADER_INCLUDED
