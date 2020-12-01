#!/bin/sh
# Script to run tests
#
# Version: 20201121

if test -f ${PWD}/libqcow/.libs/libqcow.1.dylib && test -f ./pyqcow/.libs/pyqcow.so;
then
	install_name_tool -change /usr/local/lib/libqcow.1.dylib ${PWD}/libqcow/.libs/libqcow.1.dylib ./pyqcow/.libs/pyqcow.so;
fi

make check CHECK_WITH_STDERR=1;
RESULT=$?;

if test ${RESULT} -ne 0 && test -f tests/test-suite.log;
then
	cat tests/test-suite.log;
fi
exit ${RESULT};

