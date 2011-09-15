
#ifndef IB_DATABASE_IO_H
#define IB_DATABASE_IO_H

#include <ham/hamsterdb.h>
#include <tws_c_api/twsapi.h>


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


struct my_tws_io_info;


// identifiers for each of the databases we keep around:
typedef enum
{
	// stores the contract ID --> contract info map:
	DB_STOCK_DETAILS = 1, 
	// the index which maps ticker strings to contract ID:
	DB_INDEX_TICKER_2_STOCK_ID,

	// and 1 higher than the highest index we'll ever address:
	DB_MAX
} ib_db_indentifier_t;


struct my_databases_info
{
	ham_env_t *env;
	ham_db_t *db[DB_MAX];
};


int ib_open_databases(struct my_tws_io_info *info);
int ib_close_databases(struct my_tws_io_info *info);
const char *ib_strerror(int errcode);


int ib_cache_ticker_info(const tr_contract_details_t *cd);
int ib_get_ticker_info(tr_contract_details_t *cd);



#ifdef __cplusplus
}
#endif // __cplusplus

#endif
