#!/bin/sh
# Script to run tests
#
# Version: 20201121

if test -f ${PWD}/libfsfat/.libs/libfsfat.1.dylib && test -f ./pyfsfat/.libs/pyfsfat.so;
then
	install_name_tool -change /usr/local/lib/libfsfat.1.dylib ${PWD}/libfsfat/.libs/libfsfat.1.dylib ./pyfsfat/.libs/pyfsfat.so;
fi

make check CHECK_WITH_STDERR=1;
RESULT=$?;

if test ${RESULT} -ne 0 && test -f tests/test-suite.log;
then
	cat tests/test-suite.log;
fi
exit ${RESULT};

