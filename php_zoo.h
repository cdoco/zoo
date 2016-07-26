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

#ifndef PHP_ZOO_H
#define PHP_ZOO_H

extern zend_module_entry zoo_module_entry;

#define phpext_zoo_ptr &zoo_module_entry

#define PHP_ZOO_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_ZOO_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_ZOO_API __attribute__ ((visibility("default")))
#else
#	define PHP_ZOO_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

typedef struct _zoo_struct {
    char *database_type;
    char *database_name;
    char *server;
    char *username;
    char *password;
    char *port;
    char *charset;
    zval *option;
} zoo_orm_option;

#define ZSTR_INIT(str)                      (zend_string_init(str, sizeof(str)-1, 0))
#define ZOO_PROPERTY_NAME_PDO	            "pdo"
#define zoo_hash_find(ht, str, v)           ((v = zend_hash_find(ht, ZSTR_INIT(str))) != NULL)

#define MAKE_STD_ZVAL(p)             zval _stack_zval_##p; p = &(_stack_zval_##p)

PHP_MINIT_FUNCTION(zoo);
PHP_MSHUTDOWN_FUNCTION(zoo);
PHP_RINIT_FUNCTION(zoo);
PHP_RSHUTDOWN_FUNCTION(zoo);
PHP_MINFO_FUNCTION(zoo);

PHP_METHOD(Zoo, __construct);
PHP_METHOD(Zoo, query);
PHP_METHOD(Zoo, select);

ZEND_BEGIN_ARG_INFO_EX(zoo_construct_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, construct)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(zoo_query_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, query)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(zoo_select_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, select)
ZEND_END_ARG_INFO()

#ifdef ZTS
#define ZOO_G(v) TSRMG(zoo_globals_id, zend_zoo_globals *, v)
#else
#define ZOO_G(v) (zoo_globals.v)
#endif

#endif	/* PHP_ZOO_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
