
#include "system-includes.h"

#include "tws_database_io.h"
#include "support-code.h"
#include <mongoose/mongoose.h>    // mg_strdup()

#include "app_manager.h"



// where to start numbering for the database 'names':
#define HAM_DBNAME_OFFSET                   2




int db_manager::open_databases(void)
{
    int i;
    int err = 0;
    const char *db_filename = m_cfg.m_database_path;
    ham_parameter_t create_params[] =
    {
        { HAM_PARAM_MAX_ENV_DATABASES, 300 },
        { HAM_PARAM_PAGESIZE, 65536 },
        { HAM_PARAM_CACHESIZE, 128E6 },
        {0, 0}
    };

    (void)close_databases();

    ham_env_new(&m_env);

    return 0;

    err = ham_env_open_ex(m_env, db_filename, 0, NULL);
    if (err == HAM_FILE_NOT_FOUND)
    {
        err = ham_env_create_ex(m_env, db_filename, 0, 0644, create_params);
    }
    if (err)
        return err;

    for (i = 0; i < ARRAY_SIZE(m_db); i++)
    {
        ham_new(&m_db[i]);

        err = ham_env_open_db(m_env, m_db[i], i + HAM_DBNAME_OFFSET, 0, NULL);
        if (err == HAM_DATABASE_NOT_FOUND)
        {
            err = ham_env_create_db(m_env, m_db[i], i + HAM_DBNAME_OFFSET, 0, NULL);
        }
        if (err)
            return err;
    }

    return 0;
}

int db_manager::close_databases(void)
{
    int err = 0;

    if (m_env)
    {
        int i;

        err = ham_env_close(m_env, 0);

        for (i = 0; i < ARRAY_SIZE(m_db); i++)
        {
            if (m_db[i])
            {
                ham_delete(m_db[i]);
                m_db[i] = NULL;
            }
        }

        ham_env_delete(m_env);
        m_env = NULL;
    }

    return err;
}

const char *db_manager::strerror(int errcode)
{
    return ham_strerror(errcode);
}


void db_manager::set_database_path(const char *path)
{
    m_cfg.m_database_path = mg_strdup(path);
}



int db_manager::ib_cache_ticker_info(const ib_contract_details &cd)
{
    ham_key_t key;

    return 0;
}

int db_manager::ib_get_ticker_info(ib_contract_details &cd)
{
    return 0;
}





int db_manager::ib_store_scanner_parameters_xml(const char *xml)
{
    ib_tws_manager *ibm = m_app_mgr->get_ib_tws_manager();

    // don't store the XML file when we're faking...
    if (!ibm->is_faking_the_ib_tws_connection())
    {
        // TODO: store this in the DB_MISC_BLOBS database table

        const char *db_filename = m_cfg.m_database_path;
        char fname[PATH_MAXSIZE];
        char *dst;

        dst = concat_path(fname, PATH_MAXSIZE, db_filename, "/../scanner-parameters.xml", NULL);
        if (dst)
        {
            writefile(dst, xml, strlen(xml), 0);
        }
    }

    return 0;
}







db_manager::db_manager(app_manager *mgr) :
    m_app_mgr(mgr),
    m_env(NULL)
{
    memset(m_db, 0, sizeof(m_db));
}

db_manager::~db_manager(void)
{
}

