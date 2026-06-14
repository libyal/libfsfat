#!/bin/sh
# Script to run tests
#
# Version: 20260609

if test -f ${PWD}/libfsfat/.libs/libfsfat.1.dylib && test -f ./pyfsfat/.libs/pyfsfat.so
then
	install_name_tool -change /usr/local/lib/libfsfat.1.dylib ${PWD}/libfsfat/.libs/libfsfat.1.dylib ./pyfsfat/.libs/pyfsfat.so
fi

make check-build > /dev/null

make check $@
RESULT=$?

if test ${RESULT} -ne 0
then
	find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

