/*
 * Library extents_footer type test program
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

#include "../libfsfat/libfsfat_extents_footer.h"

#if defined( __GNUC__ ) && !defined( LIBFSFAT_DLL_IMPORT )

/* Tests the libfsfat_extents_footer_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsfat_test_extents_footer_initialize(
     void )
{
	libcerror_error_t *error                  = NULL;
	libfsfat_extents_footer_t *extents_footer = NULL;
	int result                                = 0;

#if defined( HAVE_FSFAT_TEST_MEMORY )
	int number_of_malloc_fail_tests           = 1;
	int number_of_memset_fail_tests           = 1;
	int test_number                           = 0;
#endif

	/* Test regular cases
	 */
	result = libfsfat_extents_footer_initialize(
	          &extents_footer,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSFAT_TEST_ASSERT_IS_NOT_NULL(
	 "extents_footer",
	 extents_footer );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsfat_extents_footer_free(
	          &extents_footer,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "extents_footer",
	 extents_footer );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsfat_extents_footer_initialize(
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

	extents_footer = (libfsfat_extents_footer_t *) 0x12345678UL;

	result = libfsfat_extents_footer_initialize(
	          &extents_footer,
	          &error );

	extents_footer = NULL;

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSFAT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_FSFAT_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libfsfat_extents_footer_initialize with malloc failing
		 */
		fsfat_test_malloc_attempts_before_fail = test_number;

		result = libfsfat_extents_footer_initialize(
		          &extents_footer,
		          &error );

		if( fsfat_test_malloc_attempts_before_fail != -1 )
		{
			fsfat_test_malloc_attempts_before_fail = -1;

			if( extents_footer != NULL )
			{
				libfsfat_extents_footer_free(
				 &extents_footer,
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
			 "extents_footer",
			 extents_footer );

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
		/* Test libfsfat_extents_footer_initialize with memset failing
		 */
		fsfat_test_memset_attempts_before_fail = test_number;

		result = libfsfat_extents_footer_initialize(
		          &extents_footer,
		          &error );

		if( fsfat_test_memset_attempts_before_fail != -1 )
		{
			fsfat_test_memset_attempts_before_fail = -1;

			if( extents_footer != NULL )
			{
				libfsfat_extents_footer_free(
				 &extents_footer,
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
			 "extents_footer",
			 extents_footer );

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
	if( extents_footer != NULL )
	{
		libfsfat_extents_footer_free(
		 &extents_footer,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsfat_extents_footer_free function
 * Returns 1 if successful or 0 if not
 */
int fsfat_test_extents_footer_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsfat_extents_footer_free(
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

#if defined( __GNUC__ ) && !defined( LIBFSFAT_DLL_IMPORT )

	FSFAT_TEST_RUN(
	 "libfsfat_extents_footer_initialize",
	 fsfat_test_extents_footer_initialize );

	FSFAT_TEST_RUN(
	 "libfsfat_extents_footer_free",
	 fsfat_test_extents_footer_free );

	/* TODO: add tests for libfsfat_extents_footer_read_data */

#endif /* defined( __GNUC__ ) && !defined( LIBFSFAT_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

