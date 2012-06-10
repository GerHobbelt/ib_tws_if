#! /bin/bash
#
# pull old TWS C API code from SF; use that as a base for the new work
#

mkdir ../tws_c_api
pushd ../tws_c_api
git cvsimport -k -a -v -d :pserver:anonymous@tws-c-api.cvs.sourceforge.net:/cvsroot/tws-c-api tws_client_api
popd

