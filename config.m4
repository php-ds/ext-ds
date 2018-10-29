PHP_ARG_ENABLE(decimal, whether to enable decimal support,
[  --enable-decimal           Enable decimal support])

if test "$PHP_DECIMAL" != "no"; then

    AC_MSG_CHECKING([for libmpdec files in default path])
    for i in /usr/$PHP_LIBDIR $(locate -e /usr/**/libmpdec.so); do
      if test -r $i; then
        LIBMPDEC_DIR=$(dirname $i)
        AC_MSG_RESULT(found in $LIBMPDEC_DIR)
        break
      fi
    done

    if test -z "$LIBMPDEC_DIR"; then
      AC_MSG_RESULT([Could not find libmpdec])
      AC_MSG_ERROR([Please reinstall the libmpdec distribution from http://www.bytereef.org/mpdecimal/])
    fi

    PHP_CHECK_LIBRARY(mpdec, mpd_version, [
      PHP_ADD_LIBRARY_WITH_PATH(mpdec, $LIBMPDEC_DIR/$PHP_LIBDIR, DECIMAL_SHARED_LIBADD)
      AC_DEFINE(HAVE_LIBMPDEC, 1, [ ])
    ],[
      AC_MSG_ERROR([Please check your version of libmpdec (~2.4.0)])
    ],[
      -L$LIBMPDEC_DIR/$PHP_LIBDIR -lm
    ])

    PHP_NEW_EXTENSION(decimal, php_decimal.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)

    PHP_ADD_EXTENSION_DEP(decimal, standard)
    PHP_ADD_EXTENSION_DEP(decimal, spl)
    PHP_ADD_EXTENSION_DEP(decimal, json)
    PHP_SUBST(DECIMAL_SHARED_LIBADD)
fi
