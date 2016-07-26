/*
  +----------------------------------------------------------------------+
  | Zoo                                                                  |
  +----------------------------------------------------------------------+
  | Copyright (c) 2016-2016 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: ZiHang Gao <ocdoco@gmail.com>                                |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_zoo.h"

#include "ext/pdo/php_pdo_driver.h"
#include "zend_smart_str.h"

static int le_zoo;

zend_class_entry *zoo_ce;

static void zoo_string_zval(zval *ret, const char *str) {
    
    if (str) {
        ZVAL_STRING(ret, (char*)str);
    } else {
        ZVAL_NULL(ret);
    }
}

static int zoo_connect(zoo_orm_option *orm_option, zval *this_ptr TSRMLS_CC) {
    zval pdo, retval, fnval, params[4];
    smart_str source = {0};
    int i;
    
    if (strcmp(orm_option->database_type, "mysql") == 0) {
        
        smart_str_appends(&source, "mysql:host=");
        smart_str_appends(&source, orm_option->server);
        smart_str_appends(&source, ";port=");
        smart_str_appends(&source, orm_option->port);
        smart_str_appends(&source, ";dbname=");
        smart_str_appends(&source, orm_option->database_name);
        
    } else {
        php_error(E_ERROR, "zoo not supported type");
    }
    
    object_init_ex(&pdo, php_pdo_get_dbh_ce());

    zoo_string_zval(&fnval, "__construct");
    zoo_string_zval(&params[0], ZSTR_VAL(source.s));
    zoo_string_zval(&params[1], orm_option->username);
    zoo_string_zval(&params[2], orm_option->password);
    
    ZVAL_COPY(&params[3], orm_option->option);
    
    call_user_function(NULL, &pdo, &fnval, &retval, 4, params);
    
    zend_update_property(zoo_ce, this_ptr, ZEND_STRL(ZOO_PROPERTY_NAME_PDO), &pdo);
    
    for (i = 0; i < 4; i++) {
        zval_ptr_dtor(&params[i]);
    }
    
    smart_str_0(&source);
    
    return 1;
}

PHP_METHOD(Zoo, query) {
    zval *query, *pdo, retval, params[1], fnval;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &query) == FAILURE) {
        return;
    }

    ZVAL_COPY(&params[0], query);
    
    pdo = zend_read_property(zoo_ce, getThis(), ZEND_STRL(ZOO_PROPERTY_NAME_PDO), 1, NULL);

    zoo_string_zval(&fnval, "query");

    call_user_function(NULL, pdo, &fnval, &retval, 1, params);

    zval_ptr_dtor(&fnval);

    RETURN_ZVAL(&retval, 1, 0);
}

PHP_METHOD(Zoo, select) {
    zval *table, *field, fields, retval, params[1], query, fetchall, data;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "z|a", &table, &field) == FAILURE) {
        return;
    }
    
    smart_str statements = {0};
    
    smart_str_appends(&statements, "select ");
    
    if (ZEND_NUM_ARGS() == 2) {
        php_implode(ZSTR_INIT(","), field, &fields);
        smart_str_appendl(&statements, Z_STRVAL_P(&fields), Z_STRLEN_P(&fields));
        smart_str_appends(&statements, " ");
    } else {
        smart_str_appends(&statements, "* ");
    }

    smart_str_appends(&statements, "from ");
    smart_str_appendl(&statements, Z_STRVAL_P(table), Z_STRLEN_P(table));
    printf("%s", ZSTR_VAL(statements.s));
    printf("%s", "\n");
    
    zoo_string_zval(&params[0], ZSTR_VAL(statements.s));
    zoo_string_zval(&query, "query");
    zoo_string_zval(&fetchall, "fetchAll");
    
    call_user_function(NULL, getThis(), &query, &retval, 1, params);
    call_user_function(NULL, &retval, &fetchall, &data, 0, NULL);
    
    smart_str_0(&statements);
    
    RETURN_ZVAL(&data, 1, 0);
}

PHP_METHOD(Zoo, __construct) {
    zval *option, *tmp;
    HashTable *option_ht;
    zoo_orm_option *orm_option;
    
    orm_option = (zoo_orm_option *)emalloc(sizeof(zoo_orm_option));
	bzero(orm_option, sizeof(zoo_orm_option));

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "a", &option) == FAILURE) {
        return;
    }

    option_ht = Z_ARRVAL_P(option);
    
    if (zoo_hash_find(option_ht, "database_type", tmp)) {
        orm_option->database_type = Z_STRVAL_P(tmp);
    } else {
        orm_option->database_type = "";
    }
    
    if (zoo_hash_find(option_ht, "database_name", tmp)) {
        orm_option->database_name = Z_STRVAL_P(tmp);
    } else {
        orm_option->database_name = "";
    }
    
    if (zoo_hash_find(option_ht, "server", tmp)) {
        orm_option->server = Z_STRVAL_P(tmp);
    } else {
        orm_option->server = "127.0.0.1";
    }
    
    if (zoo_hash_find(option_ht, "username", tmp)) {
        orm_option->username = Z_STRVAL_P(tmp);
    } else {
        orm_option->username = "";
    }
    
    if (zoo_hash_find(option_ht, "password", tmp)) {
        orm_option->password = Z_STRVAL_P(tmp);
    } else {
        orm_option->password = "";
    }
    
    if (zoo_hash_find(option_ht, "port", tmp)) {
        orm_option->port = Z_STRVAL_P(tmp);
    } else {
        orm_option->port = "3306";
    }
    
    if (zoo_hash_find(option_ht, "charset", tmp)) {
        orm_option->charset = Z_STRVAL_P(tmp);
    } else {
        orm_option->charset = "utf8";
    }
    
    if (zoo_hash_find(option_ht, "option", tmp)) {
        orm_option->option = tmp;
    } else {
        MAKE_STD_ZVAL(tmp);
        ZVAL_NULL(tmp);
        orm_option->option = tmp;
    }
    
    zoo_connect(orm_option, getThis() TSRMLS_CC);
}

const zend_function_entry zoo_methods[] = {
    PHP_ME(Zoo, __construct, zoo_construct_arginfo, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
    PHP_ME(Zoo, query, zoo_query_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(Zoo, select, zoo_select_arginfo, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL} /* Must be the last line in fetion_echo_functions[] */
};

zend_module_entry zoo_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"zoo",
	zoo_methods,
	PHP_MINIT(zoo),
	PHP_MSHUTDOWN(zoo),
	PHP_RINIT(zoo),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(zoo),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(zoo),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_ZOO_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_ZOO
ZEND_GET_MODULE(zoo)
#endif

PHP_MINIT_FUNCTION(zoo)
{
    zend_class_entry zoo;

    INIT_CLASS_ENTRY(zoo, "Zoo", zoo_methods);
    zoo_ce = zend_register_internal_class_ex(&zoo, NULL);

    zend_declare_property_null(zoo_ce, ZEND_STRL(ZOO_PROPERTY_NAME_PDO), ZEND_ACC_PUBLIC TSRMLS_CC);
	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(zoo)
{
	return SUCCESS;
}

PHP_RINIT_FUNCTION(zoo)
{
	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(zoo)
{
	return SUCCESS;
}

PHP_MINFO_FUNCTION(zoo)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "Zoo Support", "enabled");
	php_info_print_table_end();
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
