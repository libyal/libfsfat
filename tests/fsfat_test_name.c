/*
 * Library name functions test program
 *
 * Copyright (C) 2021-2022, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "fsfat_test_libcerror.h"
#include "fsfat_test_libfsfat.h"
#include "fsfat_test_libuna.h"
#include "fsfat_test_macros.h"
#include "fsfat_test_unused.h"

#include "../libfsfat/libfsfat_name.h"

#if defined( __GNUC__ ) && !defined( LIBFSFAT_DLL_IMPORT )

/* Tests the libfsfat_name_compare_with_utf8_string function
 * Returns 1 if successful or 0 if not
 */
int fsfat_test_name_compare_with_utf8_string(
     void )
{
        uint8_t utf16_stream_equal[ 12 ]     = { 'e', 0, 'q', 0, 'u', 0, 'a', 0, 'l', 0, 0 };
        uint8_t utf16_stream_great[ 12 ]     = { 'g', 0, 'r', 0, 'e', 0, 'a', 0, 't', 0, 0, 0 };
        uint8_t utf16_stream_less[ 10 ]      = { 'l', 0, 'e', 0, 's', 0, 's', 0, 0, 0 };
        uint8_t utf16_stream_lesser[ 14 ]    = { 'l', 0, 'e', 0, 's', 0, 's', 0, 'e', 0, 'r', 0, 0, 0 };
        uint8_t utf16_stream_more[ 10 ]      = { 'm', 0, 'o', 0, 'r', 0, 'e', 0, 0, 0 };
        uint8_t utf8_string_equal[ 6 ]       = { 'e', 'q', 'u', 'a', 'l', 0 };
        uint8_t utf8_string_equal_upper[ 6 ] = { 'E', 'Q', 'U', 'A', 'L', 0 };
        uint8_t utf8_string_greater[ 7 ]     = { 'g', 'r', 'e', 'a', 't', 'e', 'r' };
        uint8_t utf8_string_less[ 4 ]        = { 'l', 'e', 's', 's' };
        uint8_t utf8_string_more[ 4 ]        = { 'm', 'o', 'r', 'e' };
	libcerror_error_t *error             = NULL;
	int result                           = 0;

	/* Test regular cases
	 */
	result = libfsfat_name_compare_with_utf8_string(
	          utf16_stream_equal,
	          12,
	          1,
	          utf8_string_equal,
	          5,
	          0,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsfat_name_compare_with_utf8_string(
	          utf16_stream_equal,
	          12,
	          1,
	          utf8_string_equal,
	          6,
	          0,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsfat_name_compare_with_utf8_string(
	          utf16_stream_equal,
	          12,
	          1,
	          utf8_string_equal_upper,
	          5,
	          1,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsfat_name_compare_with_utf8_string(
	          utf16_stream_great,
	          12,
	          1,
	          utf8_string_greater,
	          7,
	          0,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_GREATER );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsfat_name_compare_with_utf8_string(
	          utf16_stream_less,
	          10,
	          1,
	          utf8_string_more,
	          4,
	          0,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_GREATER );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsfat_name_compare_with_utf8_string(
	          utf16_stream_more,
	          10,
	          1,
	          utf8_string_less,
	          4,
	          0,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_LESS );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsfat_name_compare_with_utf8_string(
	          utf16_stream_lesser,
	          14,
	          1,
	          utf8_string_less,
	          4,
	          0,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_LESS );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsfat_name_compare_with_utf8_string(
	          NULL,
	          12,
	          1,
	          utf8_string_equal,
	          5,
	          0,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSFAT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsfat_name_compare_with_utf8_string(
	          utf16_stream_equal,
	          (size_t) SSIZE_MAX + 1,
	          1,
	          utf8_string_equal,
	          5,
	          0,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSFAT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsfat_name_compare_with_utf8_string(
	          utf16_stream_equal,
	          12,
	          1,
	          NULL,
	          5,
	          0,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSFAT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsfat_name_compare_with_utf8_string(
	          utf16_stream_equal,
	          12,
	          1,
	          utf8_string_equal,
	          (size_t) SSIZE_MAX + 1,
	          0,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSFAT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsfat_name_compare_with_utf16_string function
 * Returns 1 if successful or 0 if not
 */
int fsfat_test_name_compare_with_utf16_string(
     void )
{
        uint8_t utf16_stream_equal[ 12 ]       = { 'e', 0, 'q', 0, 'u', 0, 'a', 0, 'l', 0, 0 };
        uint8_t utf16_stream_great[ 12 ]       = { 'g', 0, 'r', 0, 'e', 0, 'a', 0, 't', 0, 0, 0 };
        uint8_t utf16_stream_less[ 10 ]        = { 'l', 0, 'e', 0, 's', 0, 's', 0, 0, 0 };
        uint8_t utf16_stream_lesser[ 14 ]      = { 'l', 0, 'e', 0, 's', 0, 's', 0, 'e', 0, 'r', 0, 0, 0 };
        uint8_t utf16_stream_more[ 10 ]        = { 'm', 0, 'o', 0, 'r', 0, 'e', 0, 0, 0 };
        uint16_t utf16_string_equal[ 6 ]       = { 'e', 'q', 'u', 'a', 'l', 0 };
        uint16_t utf16_string_equal_upper[ 6 ] = { 'E', 'Q', 'U', 'A', 'L', 0 };
        uint16_t utf16_string_greater[ 7 ]     = { 'g', 'r', 'e', 'a', 't', 'e', 'r' };
        uint16_t utf16_string_less[ 4 ]        = { 'l', 'e', 's', 's' };
        uint16_t utf16_string_more[ 4 ]        = { 'm', 'o', 'r', 'e' };
	libcerror_error_t *error               = NULL;
	int result                             = 0;

	/* Test regular cases
	 */
	result = libfsfat_name_compare_with_utf16_string(
	          utf16_stream_equal,
	          12,
	          1,
	          utf16_string_equal,
	          5,
	          0,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsfat_name_compare_with_utf16_string(
	          utf16_stream_equal,
	          12,
	          1,
	          utf16_string_equal,
	          6,
	          0,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsfat_name_compare_with_utf16_string(
	          utf16_stream_equal,
	          12,
	          1,
	          utf16_string_equal_upper,
	          5,
	          1,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsfat_name_compare_with_utf16_string(
	          utf16_stream_great,
	          12,
	          1,
	          utf16_string_greater,
	          7,
	          0,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_GREATER );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsfat_name_compare_with_utf16_string(
	          utf16_stream_less,
	          10,
	          1,
	          utf16_string_more,
	          4,
	          0,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_GREATER );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsfat_name_compare_with_utf16_string(
	          utf16_stream_more,
	          10,
	          1,
	          utf16_string_less,
	          4,
	          0,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_LESS );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsfat_name_compare_with_utf16_string(
	          utf16_stream_lesser,
	          14,
	          1,
	          utf16_string_less,
	          4,
	          0,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_LESS );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsfat_name_compare_with_utf16_string(
	          NULL,
	          12,
	          1,
	          utf16_string_equal,
	          5,
	          0,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSFAT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsfat_name_compare_with_utf16_string(
	          utf16_stream_equal,
	          (size_t) SSIZE_MAX + 1,
	          1,
	          utf16_string_equal,
	          5,
	          0,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSFAT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsfat_name_compare_with_utf16_string(
	          utf16_stream_equal,
	          12,
	          1,
	          NULL,
	          5,
	          0,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSFAT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsfat_name_compare_with_utf16_string(
	          utf16_stream_equal,
	          12,
	          1,
	          utf16_string_equal,
	          (size_t) SSIZE_MAX + 1,
	          0,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSFAT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) && !defined( LIBFSFAT_DLL_IMPORT ) */

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc FSFAT_TEST_ATTRIBUTE_UNUSED,
     wchar_t * const argv[] FSFAT_TEST_ATTRIBUTE_UNUSED )
#else
int main(
     int argc FSFAT_TEST_ATTRIBUTE_UNUSED,
     char * const argv[] FSFAT_TEST_ATTRIBUTE_UNUSED )
#endif
{
	FSFAT_TEST_UNREFERENCED_PARAMETER( argc )
	FSFAT_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBFSFAT_DLL_IMPORT )

	FSFAT_TEST_RUN(
	 "libfsfat_name_compare_with_utf8_string",
	 fsfat_test_name_compare_with_utf8_string );

	FSFAT_TEST_RUN(
	 "libfsfat_name_compare_with_utf16_string",
	 fsfat_test_name_compare_with_utf16_string );

#endif /* defined( __GNUC__ ) && !defined( LIBFSFAT_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBFSFAT_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFSFAT_DLL_IMPORT ) */
}

