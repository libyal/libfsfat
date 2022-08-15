/*
 * Library directory type test program
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
#include "fsfat_test_macros.h"
#include "fsfat_test_memory.h"
#include "fsfat_test_unused.h"

#include "../libfsfat/libfsfat_directory.h"

/* Define to make fsfat_test_directory generate verbose output
#define FSFAT_TEST_DIRECTORY_VERBOSE
 */

uint8_t fsfat_test_directory_data1[ 416 ] = {
	0x54, 0x45, 0x53, 0x54, 0x56, 0x4f, 0x4c, 0x55, 0x4d, 0x45, 0x20, 0x08, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x67, 0xa1, 0xc9, 0x52, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x41, 0x65, 0x00, 0x6d, 0x00, 0x70, 0x00, 0x74, 0x00, 0x79, 0x00, 0x0f, 0x00, 0xc9, 0x66, 0x00,
	0x69, 0x00, 0x6c, 0x00, 0x65, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0x45, 0x4d, 0x50, 0x54, 0x59, 0x46, 0x7e, 0x31, 0x20, 0x20, 0x20, 0x20, 0x00, 0x86, 0x67, 0xa1,
	0xc9, 0x52, 0xc9, 0x52, 0x00, 0x00, 0x68, 0xa1, 0xc9, 0x52, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x54, 0x45, 0x53, 0x54, 0x44, 0x49, 0x52, 0x31, 0x20, 0x20, 0x20, 0x10, 0x08, 0x86, 0x67, 0xa1,
	0xc9, 0x52, 0xc9, 0x52, 0x00, 0x00, 0x68, 0xa1, 0xc9, 0x52, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x44, 0x6f, 0x00, 0x6e, 0x00, 0x67, 0x00, 0x00, 0x00, 0xff, 0xff, 0x0f, 0x00, 0x93, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0x03, 0x6d, 0x00, 0x65, 0x00, 0x2c, 0x00, 0x20, 0x00, 0x73, 0x00, 0x0f, 0x00, 0x93, 0x6f, 0x00,
	0x20, 0x00, 0x76, 0x00, 0x65, 0x00, 0x72, 0x00, 0x79, 0x00, 0x00, 0x00, 0x20, 0x00, 0x6c, 0x00,
	0x02, 0x20, 0x00, 0x6c, 0x00, 0x6f, 0x00, 0x6e, 0x00, 0x67, 0x00, 0x0f, 0x00, 0x93, 0x20, 0x00,
	0x66, 0x00, 0x69, 0x00, 0x6c, 0x00, 0x65, 0x00, 0x20, 0x00, 0x00, 0x00, 0x6e, 0x00, 0x61, 0x00,
	0x01, 0x4d, 0x00, 0x79, 0x00, 0x20, 0x00, 0x6c, 0x00, 0x6f, 0x00, 0x0f, 0x00, 0x93, 0x6e, 0x00,
	0x67, 0x00, 0x2c, 0x00, 0x20, 0x00, 0x76, 0x00, 0x65, 0x00, 0x00, 0x00, 0x72, 0x00, 0x79, 0x00,
	0x4d, 0x59, 0x4c, 0x4f, 0x4e, 0x47, 0x7e, 0x31, 0x20, 0x20, 0x20, 0x20, 0x00, 0x8e, 0x67, 0xa1,
	0xc9, 0x52, 0xc9, 0x52, 0x00, 0x00, 0x68, 0xa1, 0xc9, 0x52, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x42, 0x20, 0x00, 0x49, 0x00, 0x6e, 0x00, 0x66, 0x00, 0x6f, 0x00, 0x0f, 0x00, 0x72, 0x72, 0x00,
	0x6d, 0x00, 0x61, 0x00, 0x74, 0x00, 0x69, 0x00, 0x6f, 0x00, 0x00, 0x00, 0x6e, 0x00, 0x00, 0x00,
	0x01, 0x53, 0x00, 0x79, 0x00, 0x73, 0x00, 0x74, 0x00, 0x65, 0x00, 0x0f, 0x00, 0x72, 0x6d, 0x00,
	0x20, 0x00, 0x56, 0x00, 0x6f, 0x00, 0x6c, 0x00, 0x75, 0x00, 0x00, 0x00, 0x6d, 0x00, 0x65, 0x00,
	0x53, 0x59, 0x53, 0x54, 0x45, 0x4d, 0x7e, 0x31, 0x20, 0x20, 0x20, 0x16, 0x00, 0xab, 0x67, 0xa1,
	0xc9, 0x52, 0xc9, 0x52, 0x00, 0x00, 0x68, 0xa1, 0xc9, 0x52, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBFSFAT_DLL_IMPORT )

/* Tests the libfsfat_directory_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsfat_test_directory_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libfsfat_directory_t *directory = NULL;
	int result                      = 0;

#if defined( HAVE_FSFAT_TEST_MEMORY )
	int number_of_malloc_fail_tests = 3;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libfsfat_directory_initialize(
	          &directory,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSFAT_TEST_ASSERT_IS_NOT_NULL(
	 "directory",
	 directory );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsfat_directory_free(
	          &directory,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "directory",
	 directory );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsfat_directory_initialize(
	          NULL,
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

	directory = (libfsfat_directory_t *) 0x12345678UL;

	result = libfsfat_directory_initialize(
	          &directory,
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

	directory = NULL;

#if defined( HAVE_FSFAT_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libfsfat_directory_initialize with malloc failing
		 */
		fsfat_test_malloc_attempts_before_fail = test_number;

		result = libfsfat_directory_initialize(
		          &directory,
		          &error );

		if( fsfat_test_malloc_attempts_before_fail != -1 )
		{
			fsfat_test_malloc_attempts_before_fail = -1;

			if( directory != NULL )
			{
				libfsfat_directory_free(
				 &directory,
				 NULL );
			}
		}
		else
		{
			FSFAT_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			FSFAT_TEST_ASSERT_IS_NULL(
			 "directory",
			 directory );

			FSFAT_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
	for( test_number = 0;
	     test_number < number_of_memset_fail_tests;
	     test_number++ )
	{
		/* Test libfsfat_directory_initialize with memset failing
		 */
		fsfat_test_memset_attempts_before_fail = test_number;

		result = libfsfat_directory_initialize(
		          &directory,
		          &error );

		if( fsfat_test_memset_attempts_before_fail != -1 )
		{
			fsfat_test_memset_attempts_before_fail = -1;

			if( directory != NULL )
			{
				libfsfat_directory_free(
				 &directory,
				 NULL );
			}
		}
		else
		{
			FSFAT_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			FSFAT_TEST_ASSERT_IS_NULL(
			 "directory",
			 directory );

			FSFAT_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_FSFAT_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( directory != NULL )
	{
		libfsfat_directory_free(
		 &directory,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsfat_directory_free function
 * Returns 1 if successful or 0 if not
 */
int fsfat_test_directory_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsfat_directory_free(
	          NULL,
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

#if defined( HAVE_DEBUG_OUTPUT ) && defined( FSFAT_TEST_DIRECTORY_VERBOSE )
	libfsfat_notify_set_verbose(
	 1 );
	libfsfat_notify_set_stream(
	 stderr,
	 NULL );
#endif

#if defined( __GNUC__ ) && !defined( LIBFSFAT_DLL_IMPORT )

	FSFAT_TEST_RUN(
	 "libfsfat_directory_initialize",
	 fsfat_test_directory_initialize );

	FSFAT_TEST_RUN(
	 "libfsfat_directory_free",
	 fsfat_test_directory_free );

#endif /* defined( __GNUC__ ) && !defined( LIBFSFAT_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBFSFAT_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFSFAT_DLL_IMPORT ) */
}

