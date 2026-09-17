#!/bin/sh
# Script to run tests
#
# Version: 20260714

if [ -f "${PWD}/libqcow/.libs/libqcow.1.dylib" ] && [ -f ./pyqcow/.libs/pyqcow.so ]
then
    install_name_tool -change /usr/local/lib/libqcow.1.dylib "${PWD}/libqcow/.libs/libqcow.1.dylib" ./pyqcow/.libs/pyqcow.so
fi

make check-build > /dev/null

# shellcheck disable=SC2068
make check $@
RESULT=$?

if [ ${RESULT} -ne 0 ]
then
    find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

