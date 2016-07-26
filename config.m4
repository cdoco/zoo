dnl $Id$
dnl config.m4 for extension zoo

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(zoo, for zoo support,
dnl Make sure that the comment is aligned:
dnl [  --with-zoo             Include zoo support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(zoo, whether to enable zoo support,
Make sure that the comment is aligned:
[  --enable-zoo           Enable zoo support])

if test "$PHP_ZOO" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-zoo -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/zoo.h"  # you most likely want to change this
  dnl if test -r $PHP_ZOO/$SEARCH_FOR; then # path given as parameter
  dnl   ZOO_DIR=$PHP_ZOO
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for zoo files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       ZOO_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$ZOO_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the zoo distribution])
  dnl fi

  dnl # --with-zoo -> add include path
  dnl PHP_ADD_INCLUDE($ZOO_DIR/include)

  dnl # --with-zoo -> check for lib and symbol presence
  dnl LIBNAME=zoo # you may want to change this
  dnl LIBSYMBOL=zoo # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $ZOO_DIR/lib, ZOO_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_ZOOLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong zoo lib version or lib not found])
  dnl ],[
  dnl   -L$ZOO_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(ZOO_SHARED_LIBADD)

  PHP_NEW_EXTENSION(zoo, zoo.c, $ext_shared)
fi
