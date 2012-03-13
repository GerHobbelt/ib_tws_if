
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

#include "tws_stocklist_loading_thread.h"

#include <ham/hamsterdb.h>




xmlNodePtr xmlGetParent(xmlNodePtr node)
{
    if (node && node->parent)
    {
        return node->parent;
    }
    return NULL;
}

xmlNodePtr xmlFindChildWithName(xmlNodePtr parent, const xmlChar *tag_name)
{
    xmlNodePtr child;

    for (child = xmlFirstElementChild(parent); child; child = xmlNextElementSibling(child))
    {
        if (0 == xmlStrcmp(child->name, tag_name))
        {
            return child;
        }
    }
    return NULL;
}


long tws_mkNextOrderId(struct my_tws_io_info *info)
{
    return info->next_order_id++;
}
void tws_setNextOrderId(struct my_tws_io_info *info, long order_id)
{
    info->next_order_id = order_id;
}



/**
 * xmlXPathContainsAnyOfFunction:
 * @ctxt:  the XPath Parser context
 * @nargs:  the number of arguments
 *
 * Implement the contains-any-of() XPath function
 *    boolean contains-any-of(item, nodeset)
 * The contains function returns true if the first argument string
 * contains any of the second argument node value strings, and otherwise returns false.
 */
void xmlXPathContainsAnyOfFunction(xmlXPathParserContextPtr ctxt, int nargs)
{
    xmlXPathObjectPtr item, set;
    int rv = 0;
    xmlNodeSetPtr nodeset;
    int i;

    CHECK_ARITY(2);
    //CAST_TO_STRING;
    CHECK_TYPE(XPATH_NODESET);
    set = valuePop(ctxt);
    CAST_TO_STRING;
    item = valuePop(ctxt);

    if ((set == NULL) || (set->type != XPATH_NODESET))
    {
        //xmlXPathReleaseObject(ctxt->context, hay);
        //xmlXPathReleaseObject(ctxt->context, needle);
        XP_ERROR(XPATH_INVALID_TYPE);
    }

    nodeset = set->nodesetval;

    if (nodeset)
    {
        for (i = 0; i < nodeset->nodeNr; i++)
        {
            xmlNodePtr node = nodeset->nodeTab[i];
            xmlChar *val = xmlXPathCastNodeToString(node);
            xmlChar *s;
            xmlChar *p;

            for (s = p = val; *p; p++)
            {
                if (*p == ',')
                {
                    *p = 0;
                    rv |= !!xmlStrstr(item->stringval, s);
                    s = p + 1;
                }
            }
            if (*s)
            {
                rv |= !!xmlStrstr(item->stringval, s);
            }

            xmlFree(val);
        }
    }
    valuePush(ctxt, xmlXPathNewBoolean(rv));
    //xmlXPathReleaseObject(ctxt->context, hay);
    //xmlXPathReleaseObject(ctxt->context, needle);
}








void request_range_of_interesting_market_scans(struct my_tws_io_info *info, xmlNodePtr location, xmlXPathObjectPtr usable_scanners)
{
    xmlNodeSetPtr nodeset;
    int i;

    nodeset = usable_scanners->nodesetval;

    if (nodeset)
    {
        for (i = 0; i < nodeset->nodeNr; i++)
        {
            xmlNodePtr node = nodeset->nodeTab[i];
            scanner_subscription_request_t *s = calloc(1, sizeof(*s));
            xmlNodePtr child;
            xmlChar *val;

            /*
            Example of node:

                <ScanType>
                    <displayName>Top % Gainers</displayName>
                    <scanCode>TOP_PERC_GAIN</scanCode>
                    <instruments>IND.US,STK,FUT.US,BOND,STOCK.NA,FUT.NA,STOCK.EU,FUT.EU,IND.EU,STOCK.HK,FUT.HK,IND.HK</instruments>
                    <absoluteColumns>false</absoluteColumns>
                    <Columns varName="columns">
                        <ColumnSetRef>
                            <colId>0</colId>
                            <name>PctPerf</name>
                            <display>false</display>
                            <minTwsBuild>9100</minTwsBuild>
                        </ColumnSetRef>
                        <Column>
                            <colId>48</colId>
                            <name>Time in Scan</name>
                            <display>true</display>
                        </Column>
                    </Columns>
                    <MobileColumns varName="mobileColumns">
                        <MobileColumn>
                            <colId>7</colId>
                            <name>Last</name>
                        </MobileColumn>
                        <MobileColumn>
                            <colId>16</colId>
                            <name>Change (%)</name>
                        </MobileColumn>
                    </MobileColumns>
                    <supportsSorting>true</supportsSorting>
                    <respSizeLimit>2147483647</respSizeLimit>
                    <snapshotSizeLimit>2147483647</snapshotSizeLimit>
                    <delayedAvail>yes</delayedAvail>
                    <feature>PCT</feature>
                    <searchName>% Gainers (High-Low)</searchName>
                    <searchDefault>false</searchDefault>
                    <access>allowed</access>
                </ScanType>
            */

			tws_init_scanner_subscription(info->tws_handle, &s->reqdata);

            //s->reqdata.scan_number_of_rows = 128;

            child = xmlFindChildWithName(node, (const xmlChar *)"scanCode");
            if (child)
            {
                val = xmlXPathCastNodeToString(child);
                tws_strcpy(s->reqdata.scan_code, (const char *)val);
            }
            // do NOT take the <instruments> from the <ScanType> node as it's be too wide a range and cause an error report to be generated by TWS:
            child = xmlFindChildWithName(location, (const xmlChar *)"instruments");
            if (child)
            {
                val = xmlXPathCastNodeToString(child);
                tws_strcpy(s->reqdata.scan_instrument, (const char *)val);
                //tws_strcpy(s.scan_instrument, "STK");
            }
            child = xmlFindChildWithName(location, (const xmlChar *)"locationCode");
            if (child)
            {
                val = xmlXPathCastNodeToString(child); // e.g. "STK.AEB,STK.SBF"
                tws_strcpy(s->reqdata.scan_location_code, (const char *)val);
            }

            s->reqdata.scan_above_price = 0.05;
            s->reqdata.scan_above_volume = 1000;

            s->ticker_id = tws_mkNextOrderId(info);
            push_tws_req_scanner_subscription(info, s);
        }
    }
}




/*
when there's a free slot in TWS, send the scanner subscription request to TWS, otherwise push it on a stack.
*/
void push_tws_req_scanner_subscription(struct my_tws_io_info *info, scanner_subscription_request_t *reqdata)
{
    if (info->active_scanner_subscription_count < ARRAY_SIZE(info->active_scanner_subscriptions))
    {
        // there's room for one more active subscription: transmit it!
        info->active_scanner_subscriptions[info->active_scanner_subscription_count++] = reqdata;

        tws_req_scanner_subscription(info->tws_handle, reqdata->ticker_id, &reqdata->reqdata);
    }
    else
    {
        // queue the request until TWS has a slot available for it!
        if (!info->scanner_subscription_queue)
        {
            info->queued_scanner_subscription_allocsize = 8;
            info->scanner_subscription_queue = calloc(info->queued_scanner_subscription_allocsize, sizeof(info->scanner_subscription_queue[0]));
            if (!info->scanner_subscription_queue)
                return;
            assert(info->queued_scanner_subscription_count == 0);
        }
        else if (info->queued_scanner_subscription_count == info->queued_scanner_subscription_allocsize)
        {
            size_t oldsize = info->queued_scanner_subscription_allocsize;
            info->queued_scanner_subscription_allocsize += 16;
            info->scanner_subscription_queue = realloc(info->scanner_subscription_queue, info->queued_scanner_subscription_allocsize * sizeof(info->scanner_subscription_queue[0]));
            if (!info->scanner_subscription_queue)
                return;
            memset(&info->scanner_subscription_queue[oldsize], 0, (info->queued_scanner_subscription_allocsize - oldsize) * sizeof(info->scanner_subscription_queue[0]));
        }
        assert(info->queued_scanner_subscription_count < info->queued_scanner_subscription_allocsize);

        // push in FIFO: push at the bottom, pop off the top.
        if (info->queued_scanner_subscription_count > 0)
        {
            memmove(&info->scanner_subscription_queue[1], &info->scanner_subscription_queue[0], info->queued_scanner_subscription_count * sizeof(info->scanner_subscription_queue[0]));
        }
        info->scanner_subscription_queue[0] = reqdata;
        info->queued_scanner_subscription_count++;
    }
}


/*
when there's a free slot in TWS, pop a scanner subscription request off the stack and send it to TWS

Return the number of queued requests still waiting in the queue after this call.
*/
size_t pop_tws_req_scanner_subscription(struct my_tws_io_info *info)
{
    if (info->active_scanner_subscription_count < ARRAY_SIZE(info->active_scanner_subscriptions)
        && info->queued_scanner_subscription_count > 0)
    {
        // there's room for one more active subscription: pop it and transmit it!
        scanner_subscription_request_t *reqdata = info->scanner_subscription_queue[--info->queued_scanner_subscription_count];
        info->active_scanner_subscriptions[info->active_scanner_subscription_count++] = reqdata;

        tws_req_scanner_subscription(info->tws_handle, reqdata->ticker_id, &reqdata->reqdata);
    }
    return info->queued_scanner_subscription_count;
}


/*
send a CANCEL REQUEST to TWS for the given scanner subscription

When the subscription request is canceled, we also check whether there's another subscription
request pending in the queue and submit that one immediately afterwards.
*/
void cancel_tws_scanner_subscription(struct my_tws_io_info *info, int ticker_id)
{
    size_t i;

    for (i = 0; i < info->active_scanner_subscription_count; i++)
    {
        scanner_subscription_request_t *item = info->active_scanner_subscriptions[i];

        if (item->ticker_id == ticker_id)
        {
            tws_cancel_scanner_subscription(info->tws_handle, ticker_id);

            xmlFree(item->reqdata.scan_code);
            xmlFree(item->reqdata.scan_instrument);
            xmlFree(item->reqdata.scan_location_code);
            free(item);

            if (i != info->active_scanner_subscription_count)
            {
                memmove(&info->active_scanner_subscriptions[i], &info->active_scanner_subscriptions[i + 1], (info->active_scanner_subscription_count - i - 1) * sizeof(info->active_scanner_subscriptions[0]));
            }
            info->active_scanner_subscription_count--;

            pop_tws_req_scanner_subscription(info);
            return;
        }
    }

    // when the given ticker_id is not in the active request set, we do NOT go and look in the queued set!
}


/*
return !0 when the given ticker_id represents an active subscription.
*/
int is_active_tws_scanner_subscription(struct my_tws_io_info *info, int ticker_id)
{
    size_t i;

    for (i = 0; i < info->active_scanner_subscription_count; i++)
    {
        scanner_subscription_request_t *item = info->active_scanner_subscriptions[i];

        if (item->ticker_id == ticker_id)
        {
            return !0;
        }
    }

    return 0;
}








/*
optionally request the full contract details so that we'll receive the extended info for this stock.

'optional' because we only do this when we don't have the desired info yet in our own caches.
*/
void request_contract_details_from_tws(struct my_tws_io_info *info, tr_contract_details_t *cd)
{
	if (!ib_get_ticker_info(cd))
	{
		tr_contract_t contract;
		int reqid = tws_mkNextOrderId(info);

	    contract = cd->d_summary;

	    tws_req_contract_details(info->tws_handle, reqid, &contract);
	}
}

