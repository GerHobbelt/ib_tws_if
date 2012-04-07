
#ifndef IB_DATABASE_IO_H
#define IB_DATABASE_IO_H


#include <ham/hamsterdb.h>

#include "tws_data_structures.h"




class app_manager;



// identifiers for each of the databases we keep around:
typedef enum
{
	// stores the contract ID --> contract info map:
	DB_STOCK_DETAILS = 1, 
	// the index which maps ticker strings to contract ID:
	DB_INDEX_TICKER_2_STOCK_ID,
	// stores misc. blobs of data
	DB_MISC_BLOBS,

	// and 1 higher than the highest index we'll ever address:
	DB_MAX
} ib_db_indentifier_t;



struct database_cfg 
{
	const char *database_path;
};


class db_manager
{
protected:
	struct database_cfg cfg;
	app_manager *mgr;

	ham_env_t *env;
	ham_db_t *db[DB_MAX];

public:
	db_manager(app_manager *info);
	virtual ~db_manager();

public:
	const struct database_cfg &get_db_cfg(void);
	void set_database_path(const char *path);

	int ib_open_databases(void);
	int ib_close_databases(void);
	const char *ib_strerror(int errcode);

public:
	int ib_cache_ticker_info(const ib_contract_details &cd);
	int ib_get_ticker_info(ib_contract_details &cd);

	int ib_store_scanner_parameters_xml(const char *xml);
};


#endif
