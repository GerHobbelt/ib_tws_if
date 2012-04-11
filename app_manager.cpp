
#include <tws_c_api/twsapi.h>


#include "app_manager.h"
#include "tws_database_io.h"
#include "tws_instance.h"





struct tws_conn_cfg &app_manager::get_tws_ib_connection_config(void)
{
	ib_tws_manager *ibm = get_ib_tws_manager();

	return ibm->get_config();
}

const struct database_cfg &app_manager::get_db_config(void)
{
	db_manager *dbm = get_db_manager();

	return dbm->get_config();
}

db_manager *app_manager::get_db_manager(void)
{
	// TODO: create dbi when it's not alive yet
	assert(dbi);
	return dbi;
}

class ib_tws_manager *app_manager::get_ib_tws_manager(void)
{
	// TODO: create ib_tws when it's not alive yet
	assert(ib_tws);
	return ib_tws;
}



