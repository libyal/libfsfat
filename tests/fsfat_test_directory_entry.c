/*
 * Library directory_entry type test program
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

#include "../libfsfat/libfsfat_directory_entry.h"

uint8_t fsfat_test_directory_entry_data1[ 16 ] = {
	0x0c, 0x00, 0x00, 0x00, 0x10, 0x00, 0x05, 0x01, 0x66, 0x69, 0x6c, 0x65, 0x31, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBFSFAT_DLL_IMPORT )

/* Tests the libfsfat_directory_entry_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsfat_test_directory_entry_initialize(
     void )
{
	libcerror_error_t *error                    = NULL;
	libfsfat_directory_entry_t *directory_entry = NULL;
	int result                                  = 0;

#if defined( HAVE_FSFAT_TEST_MEMORY )
	int number_of_malloc_fail_tests             = 1;
	int number_of_memset_fail_tests             = 1;
	int test_number                             = 0;
#endif

	/* Test regular cases
	 */
	result = libfsfat_directory_entry_initialize(
	          &directory_entry,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSFAT_TEST_ASSERT_IS_NOT_NULL(
	 "directory_entry",
	 directory_entry );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsfat_directory_entry_free(
	          &directory_entry,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "directory_entry",
	 directory_entry );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsfat_directory_entry_initialize(
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

	directory_entry = (libfsfat_directory_entry_t *) 0x12345678UL;

	result = libfsfat_directory_entry_initialize(
	          &directory_entry,
	          &error );

	directory_entry = NULL;

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
		/* Test libfsfat_directory_entry_initialize with malloc failing
		 */
		fsfat_test_malloc_attempts_before_fail = test_number;

		result = libfsfat_directory_entry_initialize(
		          &directory_entry,
		          &error );

		if( fsfat_test_malloc_attempts_before_fail != -1 )
		{
			fsfat_test_malloc_attempts_before_fail = -1;

			if( directory_entry != NULL )
			{
				libfsfat_directory_entry_free(
				 &directory_entry,
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
			 "directory_entry",
			 directory_entry );

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
		/* Test libfsfat_directory_entry_initialize with memset failing
		 */
		fsfat_test_memset_attempts_before_fail = test_number;

		result = libfsfat_directory_entry_initialize(
		          &directory_entry,
		          &error );

		if( fsfat_test_memset_attempts_before_fail != -1 )
		{
			fsfat_test_memset_attempts_before_fail = -1;

			if( directory_entry != NULL )
			{
				libfsfat_directory_entry_free(
				 &directory_entry,
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
			 "directory_entry",
			 directory_entry );

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
	if( directory_entry != NULL )
	{
		libfsfat_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsfat_directory_entry_free function
 * Returns 1 if successful or 0 if not
 */
int fsfat_test_directory_entry_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsfat_directory_entry_free(
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

/* Tests the libfsfat_directory_entry_clone function
 * Returns 1 if successful or 0 if not
 */
int fsfat_test_directory_entry_clone(
     void )
{
	libcerror_error_t *error                                = NULL;
	libfsfat_directory_entry_t *destination_directory_entry = NULL;
	libfsfat_directory_entry_t *source_directory_entry      = NULL;
	int result                                              = 0;

#if defined( HAVE_FSFAT_TEST_MEMORY )
	int number_of_malloc_fail_tests                         = 2;
	int test_number                                         = 0;

#if defined( OPTIMIZATION_DISABLED )
	int number_of_memcpy_fail_tests                         = 2;
#endif
#endif /* defined( HAVE_FSFAT_TEST_MEMORY ) */

	/* Initialize test
	 */
	result = libfsfat_directory_entry_initialize(
	          &source_directory_entry,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSFAT_TEST_ASSERT_IS_NOT_NULL(
	 "source_directory_entry",
	 source_directory_entry );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsfat_directory_entry_clone(
	          &destination_directory_entry,
	          source_directory_entry,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSFAT_TEST_ASSERT_IS_NOT_NULL(
	 "destination_directory_entry",
	 destination_directory_entry );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsfat_directory_entry_free(
	          &destination_directory_entry,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "destination_directory_entry",
	 destination_directory_entry );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsfat_directory_entry_clone(
	          &destination_directory_entry,
	          NULL,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "destination_directory_entry",
	 destination_directory_entry );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsfat_directory_entry_clone(
	          NULL,
	          source_directory_entry,
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

	destination_directory_entry = (libfsfat_directory_entry_t *) 0x12345678UL;

	result = libfsfat_directory_entry_clone(
	          &destination_directory_entry,
	          source_directory_entry,
	          &error );

	destination_directory_entry = NULL;

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
		/* Test libfsfat_directory_entry_clone with malloc failing
		 */
		fsfat_test_malloc_attempts_before_fail = test_number;

		result = libfsfat_directory_entry_clone(
		          &destination_directory_entry,
		          source_directory_entry,
		          &error );

		if( fsfat_test_malloc_attempts_before_fail != -1 )
		{
			fsfat_test_malloc_attempts_before_fail = -1;

			if( destination_directory_entry != NULL )
			{
				libfsfat_directory_entry_free(
				 &destination_directory_entry,
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
			 "destination_directory_entry",
			 destination_directory_entry );

			FSFAT_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#if defined( OPTIMIZATION_DISABLED )

	for( test_number = 0;
	     test_number < number_of_memcpy_fail_tests;
	     test_number++ )
	{
		/* Test libfsfat_directory_entry_clone with memcpy failing
		 */
		fsfat_test_memcpy_attempts_before_fail = test_number;

		result = libfsfat_directory_entry_clone(
		          &destination_directory_entry,
		          source_directory_entry,
		          &error );

		if( fsfat_test_memcpy_attempts_before_fail != -1 )
		{
			fsfat_test_memcpy_attempts_before_fail = -1;

			if( destination_directory_entry != NULL )
			{
				libfsfat_directory_entry_free(
				 &destination_directory_entry,
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
			 "destination_directory_entry",
			 destination_directory_entry );

			FSFAT_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( OPTIMIZATION_DISABLED ) */
#endif /* defined( HAVE_FSFAT_TEST_MEMORY ) */

	/* Clean up
	 */
	result = libfsfat_directory_entry_free(
	          &source_directory_entry,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "source_directory_entry",
	 source_directory_entry );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( destination_directory_entry != NULL )
	{
		libfsfat_directory_entry_free(
		 &destination_directory_entry,
		 NULL );
	}
	if( source_directory_entry != NULL )
	{
		libfsfat_directory_entry_free(
		 &source_directory_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsfat_directory_entry_read_data function
 * Returns 1 if successful or 0 if not
 */
int fsfat_test_directory_entry_read_data(
     void )
{
	libcerror_error_t *error                    = NULL;
	libfsfat_directory_entry_t *directory_entry = NULL;
	int result                                  = 0;

	/* Initialize test
	 */
	result = libfsfat_directory_entry_initialize(
	          &directory_entry,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSFAT_TEST_ASSERT_IS_NOT_NULL(
	 "directory_entry",
	 directory_entry );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsfat_directory_entry_read_data(
	          directory_entry,
	          fsfat_test_directory_entry_data1,
	          16,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsfat_directory_entry_read_data(
	          NULL,
	          fsfat_test_directory_entry_data1,
	          16,
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

	result = libfsfat_directory_entry_read_data(
	          directory_entry,
	          NULL,
	          16,
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

	result = libfsfat_directory_entry_read_data(
	          directory_entry,
	          fsfat_test_directory_entry_data1,
	          (size_t) SSIZE_MAX + 1,
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

	result = libfsfat_directory_entry_read_data(
	          directory_entry,
	          fsfat_test_directory_entry_data1,
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

	/* Clean up
	 */
	result = libfsfat_directory_entry_free(
	          &directory_entry,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "directory_entry",
	 directory_entry );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( directory_entry != NULL )
	{
		libfsfat_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsfat_directory_entry_get_inode_number function
 * Returns 1 if successful or 0 if not
 */
int fsfat_test_directory_entry_get_inode_number(
     libfsfat_directory_entry_t *directory_entry )
{
	libcerror_error_t *error = NULL;
	uint32_t inode_number    = 0;
	int inode_number_is_set  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsfat_directory_entry_get_inode_number(
	          directory_entry,
	          &inode_number,
	          &error );

	FSFAT_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	inode_number_is_set = result;

	/* Test error cases
	 */
	result = libfsfat_directory_entry_get_inode_number(
	          NULL,
	          &inode_number,
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

	if( inode_number_is_set != 0 )
	{
		result = libfsfat_directory_entry_get_inode_number(
		          directory_entry,
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
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsfat_directory_entry_get_utf8_name_size function
 * Returns 1 if successful or 0 if not
 */
int fsfat_test_directory_entry_get_utf8_name_size(
     libfsfat_directory_entry_t *directory_entry )
{
	libcerror_error_t *error = NULL;
	size_t utf8_name_size    = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsfat_directory_entry_get_utf8_name_size(
	          directory_entry,
	          &utf8_name_size,
	          &error );

	FSFAT_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsfat_directory_entry_get_utf8_name_size(
	          NULL,
	          &utf8_name_size,
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

	result = libfsfat_directory_entry_get_utf8_name_size(
		  directory_entry,
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

/* Tests the libfsfat_directory_entry_get_utf8_name function
 * Returns 1 if successful or 0 if not
 */
int fsfat_test_directory_entry_get_utf8_name(
     libfsfat_directory_entry_t *directory_entry )
{
	uint8_t utf8_name[ 256 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsfat_directory_entry_get_utf8_name(
	          directory_entry,
	          utf8_name,
	          256,
	          &error );

	FSFAT_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsfat_directory_entry_get_utf8_name(
	          NULL,
	          utf8_name,
	          256,
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

	result = libfsfat_directory_entry_get_utf8_name(
	          directory_entry,
	          NULL,
	          256,
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

	result = libfsfat_directory_entry_get_utf8_name(
	          directory_entry,
	          utf8_name,
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

	result = libfsfat_directory_entry_get_utf8_name(
	          directory_entry,
	          utf8_name,
	          (size_t) SSIZE_MAX + 1,
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

/* Tests the libfsfat_directory_entry_get_utf16_name_size function
 * Returns 1 if successful or 0 if not
 */
int fsfat_test_directory_entry_get_utf16_name_size(
     libfsfat_directory_entry_t *directory_entry )
{
	libcerror_error_t *error = NULL;
	size_t utf16_name_size   = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsfat_directory_entry_get_utf16_name_size(
	          directory_entry,
	          &utf16_name_size,
	          &error );

	FSFAT_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsfat_directory_entry_get_utf16_name_size(
	          NULL,
	          &utf16_name_size,
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

	result = libfsfat_directory_entry_get_utf16_name_size(
		  directory_entry,
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

/* Tests the libfsfat_directory_entry_get_utf16_name function
 * Returns 1 if successful or 0 if not
 */
int fsfat_test_directory_entry_get_utf16_name(
     libfsfat_directory_entry_t *directory_entry )
{
	uint16_t utf16_name[ 256 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsfat_directory_entry_get_utf16_name(
	          directory_entry,
	          utf16_name,
	          256,
	          &error );

	FSFAT_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsfat_directory_entry_get_utf16_name(
	          NULL,
	          utf16_name,
	          256,
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

	result = libfsfat_directory_entry_get_utf16_name(
	          directory_entry,
	          NULL,
	          256,
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

	result = libfsfat_directory_entry_get_utf16_name(
	          directory_entry,
	          utf16_name,
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

	result = libfsfat_directory_entry_get_utf16_name(
	          directory_entry,
	          utf16_name,
	          (size_t) SSIZE_MAX + 1,
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
#if defined( __GNUC__ ) && !defined( LIBFSFAT_DLL_IMPORT )
	libcerror_error_t *error                    = NULL;
	libfsfat_directory_entry_t *directory_entry = NULL;
	int result                                  = 0;
#endif

	FSFAT_TEST_UNREFERENCED_PARAMETER( argc )
	FSFAT_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBFSFAT_DLL_IMPORT )

	FSFAT_TEST_RUN(
	 "libfsfat_directory_entry_initialize",
	 fsfat_test_directory_entry_initialize );

	FSFAT_TEST_RUN(
	 "libfsfat_directory_entry_free",
	 fsfat_test_directory_entry_free );

	FSFAT_TEST_RUN(
	 "libfsfat_directory_entry_clone",
	 fsfat_test_directory_entry_clone );

	FSFAT_TEST_RUN(
	 "libfsfat_directory_entry_read_data",
	 fsfat_test_directory_entry_read_data );

	/* TODO: add tests for libfsfat_directory_entry_compare_with_utf8_string */

	/* TODO: add tests for libfsfat_directory_entry_compare_with_utf16_string */

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize directory_entry for tests
	 */
	result = libfsfat_directory_entry_initialize(
	          &directory_entry,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSFAT_TEST_ASSERT_IS_NOT_NULL(
	 "directory_entry",
	 directory_entry );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsfat_directory_entry_read_data(
	          directory_entry,
	          fsfat_test_directory_entry_data1,
	          16,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Run tests
	 */
	FSFAT_TEST_RUN_WITH_ARGS(
	 "libfsfat_directory_entry_get_inode_number",
	 fsfat_test_directory_entry_get_inode_number,
	 directory_entry );

	FSFAT_TEST_RUN_WITH_ARGS(
	 "libfsfat_directory_entry_get_utf8_name_size",
	 fsfat_test_directory_entry_get_utf8_name_size,
	 directory_entry );

	FSFAT_TEST_RUN_WITH_ARGS(
	 "libfsfat_directory_entry_get_utf8_name",
	 fsfat_test_directory_entry_get_utf8_name,
	 directory_entry );

	FSFAT_TEST_RUN_WITH_ARGS(
	 "libfsfat_directory_entry_get_utf16_name_size",
	 fsfat_test_directory_entry_get_utf16_name_size,
	 directory_entry );

	FSFAT_TEST_RUN_WITH_ARGS(
	 "libfsfat_directory_entry_get_utf16_name",
	 fsfat_test_directory_entry_get_utf16_name,
	 directory_entry );

	/* Clean up
	 */
	result = libfsfat_directory_entry_free(
	          &directory_entry,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "directory_entry",
	 directory_entry );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

#endif /* defined( __GNUC__ ) && !defined( LIBFSFAT_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBFSFAT_DLL_IMPORT )

on_error:
#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( directory_entry != NULL )
	{
		libfsfat_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFSFAT_DLL_IMPORT ) */
}

