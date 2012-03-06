
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

#include "tws_database_io.h"
#include "tws_comm_thread.h"

#define FAKE_THE_HAMSTER			01


#if FAKE_THE_HAMSTER

HAM_EXPORT void HAM_CALLCONV
ham_set_errhandler(ham_errhandler_fun f)
{
	return;
}
HAM_EXPORT const char * HAM_CALLCONV
ham_strerror(ham_status_t status)
{
	return "???";
}
HAM_EXPORT void HAM_CALLCONV
ham_get_version(ham_u32_t *major, ham_u32_t *minor, ham_u32_t *revision)
{
	*major = 42;
	*minor = 42;
	*revision = 42;
	return;
}
HAM_EXPORT void HAM_CALLCONV
ham_get_license(const char **licensee, const char **product)
{
	*licensee = NULL;
	*product = NULL;
	return;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_env_new(ham_env_t **env)
{
	*env = NULL;
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_env_delete(ham_env_t *env)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_env_create(ham_env_t *env, const char *filename, ham_u32_t flags, ham_u32_t mode)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_env_create_ex(ham_env_t *env, const char *filename, ham_u32_t flags, ham_u32_t mode, const ham_parameter_t *param)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_env_open(ham_env_t *env, const char *filename, ham_u32_t flags)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_env_open_ex(ham_env_t *env, const char *filename, ham_u32_t flags, const ham_parameter_t *param)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_env_get_parameters(ham_env_t *env, ham_parameter_t *param)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_env_create_db(ham_env_t *env, ham_db_t *db, ham_u16_t name, ham_u32_t flags, const ham_parameter_t *params)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_env_open_db(ham_env_t *env, ham_db_t *db, ham_u16_t name, ham_u32_t flags, const ham_parameter_t *params)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_env_rename_db(ham_env_t *env, ham_u16_t oldname, ham_u16_t newname, ham_u32_t flags)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_env_erase_db(ham_env_t *env, ham_u16_t name, ham_u32_t flags)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_env_flush(ham_env_t *env, ham_u32_t flags)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_env_enable_encryption(ham_env_t *env, ham_u8_t key[16], ham_u32_t flags)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_env_get_database_names(ham_env_t *env, ham_u16_t *names, ham_size_t *count)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_env_close(ham_env_t *env, ham_u32_t flags)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_txn_begin(ham_txn_t **txn, ham_env_t *env, const char *name, void *reserved, ham_u32_t flags)
{
	*txn = NULL;
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT const char * HAM_CALLCONV
ham_txn_get_name(ham_txn_t *txn)
{
	return 0;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_txn_commit(ham_txn_t *txn, ham_u32_t flags)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_txn_abort(ham_txn_t *txn, ham_u32_t flags)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_new(ham_db_t **db)
{
	*db = NULL;
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_delete(ham_db_t *db)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_create(ham_db_t *db, const char *filename, ham_u32_t flags, ham_u32_t mode)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_create_ex(ham_db_t *db, const char *filename, ham_u32_t flags, ham_u32_t mode, const ham_parameter_t *param)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_open(ham_db_t *db, const char *filename, ham_u32_t flags)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_open_ex(ham_db_t *db, const char *filename, ham_u32_t flags, const ham_parameter_t *param)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_get_parameters(ham_db_t *db, ham_parameter_t *param)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_u32_t HAM_CALLCONV
ham_get_flags(ham_db_t *db)
{
	return 0;
}
HAM_EXPORT ham_env_t *HAM_CALLCONV
ham_get_env(ham_db_t *db)
{
	return 0;
}
HAM_EXPORT int HAM_CALLCONV
ham_key_get_approximate_match_type(ham_key_t *key)
{
	return 0;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_get_error(ham_db_t *db)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_set_prefix_compare_func(ham_db_t *db, ham_prefix_compare_func_t foo)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_set_compare_func(ham_db_t *db, ham_compare_func_t foo)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_set_duplicate_compare_func(ham_db_t *db, ham_duplicate_compare_func_t foo)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_enable_compression(ham_db_t *db, ham_u32_t level, ham_u32_t flags)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_find(ham_db_t *db, ham_txn_t *txn, ham_key_t *key, ham_record_t *record, ham_u32_t flags)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_insert(ham_db_t *db, ham_txn_t *txn, ham_key_t *key, ham_record_t *record, ham_u32_t flags)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_erase(ham_db_t *db, ham_txn_t *txn, ham_key_t *key, ham_u32_t flags)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_flush(ham_db_t *db, ham_u32_t flags)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_get_key_count(ham_db_t *db, ham_txn_t *txn, ham_u32_t flags, ham_offset_t *keycount)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_close(ham_db_t *db, ham_u32_t flags)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_cursor_create(ham_db_t *db, ham_txn_t *txn, ham_u32_t flags, ham_cursor_t **cursor)
{
	*cursor = NULL;
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_cursor_clone(ham_cursor_t *src, ham_cursor_t **dest)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_cursor_move(ham_cursor_t *cursor, ham_key_t *key, ham_record_t *record, ham_u32_t flags)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_cursor_overwrite(ham_cursor_t *cursor, ham_record_t *record, ham_u32_t flags)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_cursor_find(ham_cursor_t *cursor, ham_key_t *key, ham_u32_t flags)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_cursor_find_ex(ham_cursor_t *cursor, ham_key_t *key, ham_record_t *record, ham_u32_t flags)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_cursor_insert(ham_cursor_t *cursor, ham_key_t *key, ham_record_t *record, ham_u32_t flags)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_cursor_erase(ham_cursor_t *cursor, ham_u32_t flags)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_cursor_get_duplicate_count(ham_cursor_t *cursor, ham_size_t *count, ham_u32_t flags)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_cursor_get_record_size(ham_cursor_t *cursor, ham_offset_t *size)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_status_t HAM_CALLCONV
ham_cursor_close(ham_cursor_t *cursor)
{
	return HAM_INTERNAL_ERROR;
}
HAM_EXPORT ham_db_t * HAM_CALLCONV
ham_cursor_get_database(ham_cursor_t *cursor)
{
	return 0;
}

#endif // FAKE_THE_HAMSTER
