
#include "system-includes.h"

#include "tws_database_io.h"
#include "tws_comm_thread.h"
#include "support-code.h"


// where to start numbering for the database 'names':
#define HAM_DBNAME_OFFSET					2




int ib_open_databases(my_tws_io_info *info)
{
	struct my_databases_info *dbi = &info->dbi;
	int i;
	int err = 0;
	const char *db_filename = info->tws_cfg->database_path;
	ham_parameter_t create_params[] = 
	{
		{ HAM_PARAM_MAX_ENV_DATABASES, 300 },
		{ HAM_PARAM_PAGESIZE, 65536 },
		{ HAM_PARAM_CACHESIZE, 128E6 },
		{0, 0}
	};

	(void)ib_close_databases(info);

	ham_env_new(&dbi->env);

	return 0;

	err = ham_env_open_ex(dbi->env, db_filename, 0, NULL);
	if (err == HAM_FILE_NOT_FOUND)
	{
		err = ham_env_create_ex(dbi->env, db_filename, 0, 0644, create_params);
	}
	if (err)
		return err;

	for (i = 0; i < ARRAY_SIZE(dbi->db); i++)
	{
		ham_new(&dbi->db[i]);

		err = ham_env_open_db(dbi->env, dbi->db[i], i + HAM_DBNAME_OFFSET, 0, NULL);
		if (err == HAM_DATABASE_NOT_FOUND)
		{
			err = ham_env_create_db(dbi->env, dbi->db[i], i + HAM_DBNAME_OFFSET, 0, NULL);
		}
		if (err)
			return err;
	}

	return 0;
}

int ib_close_databases(my_tws_io_info *info)
{
	struct my_databases_info *dbi = &info->dbi;
	int err = 0;

	if (dbi->env)
	{
		int i;

		err = ham_env_close(dbi->env, 0);

		for (i = 0; i < ARRAY_SIZE(dbi->db); i++)
		{
			if (dbi->db[i])
			{
				ham_delete(dbi->db[i]);
				dbi->db[i] = NULL;
			}
		}

		ham_env_delete(dbi->env);
		dbi->env = NULL;
	}

	return err;
}

const char *ib_strerror(int errcode)
{
	return ham_strerror(errcode);
}





int ib_cache_ticker_info(const tr_contract_details_t *cd)
{
	ham_key_t key;

	return 0;
}

int ib_get_ticker_info(tr_contract_details_t *cd)
{
	return 0;
}





int ib_store_scanner_parameters_xml(my_tws_io_info *info, const char *xml)
{
	// TODO: store this in the DB_MISC_BLOBS database table

	const char *db_filename = info->tws_cfg->database_path;
	char fname[PATH_MAXSIZE];
	char *dst;

	dst = concat_path(fname, PATH_MAXSIZE, db_filename, "/../scanner-parameters.xml", NULL);
	if (dst)
	{
		writefile(dst, xml, strlen(xml), 0);
	}

	return 0;
}
