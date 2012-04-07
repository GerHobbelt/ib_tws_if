
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

#include "tws_stocklist_loading.h"

#include "app_manager.h"
#include "tws_database_io.h"
#include "tws_request.h"


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







static xmlNodePtr xmlGetParent(xmlNodePtr node)
{
    if (node && node->parent)
    {
        return node->parent;
    }
    return NULL;
}

static xmlNodePtr xmlFindChildWithName(xmlNodePtr parent, const xmlChar *tag_name)
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


long tws_mkNextOrderId(app_manager *mgr)
{
    return mgr->next_order_id++;
}
void tws_setNextOrderId(app_manager *mgr, long order_id)
{
    mgr->next_order_id = order_id;
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
static void xmlXPathContainsAnyOfFunction(xmlXPathParserContextPtr ctxt, int nargs)
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








void request_range_of_interesting_market_scans(app_manager *mgr, xmlNodePtr location, xmlXPathObjectPtr usable_scanners)
{
    xmlNodeSetPtr nodeset;
    int i;

    nodeset = usable_scanners->nodesetval;

    if (nodeset)
    {
        for (i = 0; i < nodeset->nodeNr; i++)
        {
            xmlNodePtr node = nodeset->nodeTab[i];
            ib_req_scanner_subscription *s = new ib_req_scanner_subscription(mgr);
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

            //s->reqdata.scan_number_of_rows = 128;

            child = xmlFindChildWithName(node, (const xmlChar *)"scanCode");
            if (child)
            {
                val = xmlXPathCastNodeToString(child);
                s->set_scan_code((const char *)val);
				xmlFree(val);
            }
            // do NOT take the <instruments> from the <ScanType> node as it's be too wide a range and cause an error report to be generated by TWS:
            child = xmlFindChildWithName(location, (const xmlChar *)"instruments");
            if (child)
            {
                val = xmlXPathCastNodeToString(child);
                s->set_instrument((const char *)val);
				xmlFree(val);
                //tws_strcpy(s.scan_instrument, "STK");
            }
            child = xmlFindChildWithName(location, (const xmlChar *)"locationCode");
            if (child)
            {
                val = xmlXPathCastNodeToString(child); // e.g. "STK.AEB,STK.SBF"
                s->set_location_code((const char *)val);
				xmlFree(val);
            }

            s->set_above_price(0.05);
            s->set_above_volume(1000);

            s->set_ticker_id(tws_mkNextOrderId(mgr));
            push_tws_req_scanner_subscription(mgr, s);
        }
    }
}







/*
As TWS limits the number of active scanner requests to 10, we might delegate the processing of this message to a separate thread,
but that would mean we'd need inter-thread messaging back & forth, at least for the outgoing request messages (to TWS) and
the scanner reports returning from TWS.

The alternative is to process this message and queue all the scanner subscription requests generated from it in a separate
queue which is slowly depleted as scanner reports come in and the code in that handler decides to UNsubscribe a given
scanner report.

By using the queue the incoming TWS message traffic processing is kept as simple as possible; the scanner subscribe msg queue
keeps this part essentially a single-thread process.
*/
void process_event_scanner_parameters(void *opaque, const char xml[])
{
	app_manager *mgr = (app_manager *)opaque;
	int rv;
	xmlTextReaderPtr reader;
	xmlNodePtr tree;
	xmlDocPtr doc;
	const char *URL = "tws://dummy";

	mg_log(mgr->get_tws_ib_connection(), "info", "INFO: dumping TWS scanner parameters to log file:");
	mg_log(mgr->get_tws_ib_connection(), "info", "XML: %s", xml);

	mg_log(mgr->get_tws_ib_connection(), "info", "scanner_parameters: opaque=%p, xml:(len=%d)", opaque, (int)strlen(xml));

	mgr->get_db_manager()->ib_store_scanner_parameters_xml(xml);

	doc = xmlReadDoc((const xmlChar *)xml, URL, "UTF-8", XML_PARSE_NOENT | XML_PARSE_NONET | XML_PARSE_NOCDATA);

	if (doc)
	{
		xmlXPathObjectPtr usable_locations;
		xmlXPathContextPtr ctxt = NULL;

		ctxt = xmlXPathNewContext(doc);
		if (ctxt)
		{
			FILE *fp;

			ctxt->node = xmlDocGetRootElement(doc);

			xmlXPathRegisterFunc(ctxt, (const xmlChar *)"contains-any-of", xmlXPathContainsAnyOfFunction);

			usable_locations = xmlXPathEvalExpression((const xmlChar *)"//LocationTree//Location[not(contains(access, 'restricted')) and not(LocationTree)]", ctxt);

#ifdef LIBXML_DEBUG_ENABLED
			mg_write2log(mgr->get_tws_ib_connection(), NULL, time(NULL), "info", "INFO: dumping XPath result to log file:");
			fp = mg_fopen(mg_get_default_logfile_path(mgr->get_tws_ib_connection()), "a+");

			if (fp != NULL)
			{
				mg_flockfile(fp);

				xmlXPathDebugDumpObject(fp, usable_locations, 0);

				fflush(fp);
				mg_funlockfile(fp);
				if (fp != stderr)
				{
					fclose(fp);
				}
			}
#endif

			if (usable_locations == NULL)
			{
				mg_cry(mgr->get_tws_ib_connection(), "No usable entries found in the TWS scanner report. IB_TWS_SRV will NOT be collecting any stock data!");
			}
			else if (usable_locations->type != XPATH_NODESET)
			{
				mg_cry(mgr->get_tws_ib_connection(), "Unexpected XPath result type %d while searching the TWS scanner report. IB_TWS_SRV will NOT be collecting any stock data!", (int)usable_locations->type);
			}
			else if (usable_locations->nodesetval != NULL)
			{
				xmlNodeSetPtr nodeset = usable_locations->nodesetval;
				xmlXPathObjectPtr usable_scanners;
				int i;

				for (i = 0; i < nodeset->nodeNr; i++)
				{
					xmlNodePtr location = nodeset->nodeTab[i];
					xmlNodePtr child = xmlFirstElementChild(location);

					while (child && xmlStrcmp(child->name, (const xmlChar *)"instruments"))
					{
						child = xmlNextElementSibling(child);
					}

					if (child)
					{
						xmlXPathObjectPtr instruments = xmlXPathNewNodeSetList(xmlXPathNodeSetMerge(NULL, xmlXPathNodeSetCreate(child)));
						//xmlXPathObjectPtr instruments = xmlXPathNewNodeSet(child);   //<-- b0rks in RegisterVariable below as that one expects to free the specified nodeset using xmlXPathFreeObject()!

						xmlXPathRegisterVariable(ctxt, (const xmlChar *)"loc_instruments", instruments);

						usable_scanners = xmlXPathEvalExpression((const xmlChar *)"//ScanTypeList/ScanType[contains-any-of(instruments, $loc_instruments) and not(starts-with(access, 'restricted')) and not(contains(access, 'disabled'))]", ctxt);

						// fire a couple of useful market scanner report requests for each node
						request_range_of_interesting_market_scans(mgr, location, usable_scanners);

						//xmlXPathFreeObject(instruments);
						//xmlXPathFreeNodeSet();
						//xmlXPathFreeNodeSetList(instruments);
					}
				}
			}

			xmlXPathFreeObject(usable_locations);
			xmlXPathFreeContext(ctxt);
		}
	}

	xmlFreeDoc(doc);

#if 0
	// process the XML response using libxml:
	xmlResetLastError();
	reader = xmlReaderForDoc((const xmlChar *)xml, URL, "UTF-8", XML_PARSE_NOENT | XML_PARSE_NONET | XML_PARSE_NOCDATA);

	rv = xmlTextReaderRead(reader);

	// signal the xmlreader to 'preserve' all XML nodes in there; we'll free them when their time is due...
	doc = xmlTextReaderCurrentDoc(reader);

	tree = xmlTextReaderExpand(reader);

	doc = xmlTextReaderCurrentDoc(reader);

	xmlFreeTextReader(reader);

	xmlFreeDoc(doc);
#endif
}



