/*
 * Library volume type test program
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
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "fsfat_test_functions.h"
#include "fsfat_test_getopt.h"
#include "fsfat_test_libbfio.h"
#include "fsfat_test_libcerror.h"
#include "fsfat_test_libfsfat.h"
#include "fsfat_test_macros.h"
#include "fsfat_test_memory.h"
#include "fsfat_test_unused.h"

#include "../libfsfat/libfsfat_volume.h"

#if !defined( LIBFSFAT_HAVE_BFIO )

LIBFSFAT_EXTERN \
int libfsfat_check_volume_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

LIBFSFAT_EXTERN \
int libfsfat_volume_open_file_io_handle(
     libfsfat_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libfsfat_error_t **error );

#endif /* !defined( LIBFSFAT_HAVE_BFIO ) */

#if defined( HAVE_WIDE_SYSTEM_CHARACTER ) && SIZEOF_WCHAR_T != 2 && SIZEOF_WCHAR_T != 4
#error Unsupported size of wchar_t
#endif

/* Define to make fsfat_test_volume generate verbose output
#define FSFAT_TEST_VOLUME_VERBOSE
 */

/* Creates and opens a source volume
 * Returns 1 if successful or -1 on error
 */
int fsfat_test_volume_open_source(
     libfsfat_volume_t **volume,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "fsfat_test_volume_open_source";
	int result            = 0;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( libfsfat_volume_initialize(
	     volume,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize volume.",
		 function );

		goto on_error;
	}
	result = libfsfat_volume_open_file_io_handle(
	          *volume,
	          file_io_handle,
	          LIBFSFAT_OPEN_READ,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open volume.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *volume != NULL )
	{
		libfsfat_volume_free(
		 volume,
		 NULL );
	}
	return( -1 );
}

/* Closes and frees a source volume
 * Returns 1 if successful or -1 on error
 */
int fsfat_test_volume_close_source(
     libfsfat_volume_t **volume,
     libcerror_error_t **error )
{
	static char *function = "fsfat_test_volume_close_source";
	int result            = 0;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	if( libfsfat_volume_close(
	     *volume,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close volume.",
		 function );

		result = -1;
	}
	if( libfsfat_volume_free(
	     volume,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free volume.",
		 function );

		result = -1;
	}
	return( result );
}

/* Tests the libfsfat_volume_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsfat_test_volume_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libfsfat_volume_t *volume       = NULL;
	int result                      = 0;

#if defined( HAVE_FSFAT_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libfsfat_volume_initialize(
	          &volume,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSFAT_TEST_ASSERT_IS_NOT_NULL(
	 "volume",
	 volume );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsfat_volume_free(
	          &volume,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "volume",
	 volume );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsfat_volume_initialize(
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

	volume = (libfsfat_volume_t *) 0x12345678UL;

	result = libfsfat_volume_initialize(
	          &volume,
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

	volume = NULL;

#if defined( HAVE_FSFAT_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libfsfat_volume_initialize with malloc failing
		 */
		fsfat_test_malloc_attempts_before_fail = test_number;

		result = libfsfat_volume_initialize(
		          &volume,
		          &error );

		if( fsfat_test_malloc_attempts_before_fail != -1 )
		{
			fsfat_test_malloc_attempts_before_fail = -1;

			if( volume != NULL )
			{
				libfsfat_volume_free(
				 &volume,
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
			 "volume",
			 volume );

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
		/* Test libfsfat_volume_initialize with memset failing
		 */
		fsfat_test_memset_attempts_before_fail = test_number;

		result = libfsfat_volume_initialize(
		          &volume,
		          &error );

		if( fsfat_test_memset_attempts_before_fail != -1 )
		{
			fsfat_test_memset_attempts_before_fail = -1;

			if( volume != NULL )
			{
				libfsfat_volume_free(
				 &volume,
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
			 "volume",
			 volume );

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
	if( volume != NULL )
	{
		libfsfat_volume_free(
		 &volume,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsfat_volume_free function
 * Returns 1 if successful or 0 if not
 */
int fsfat_test_volume_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsfat_volume_free(
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

/* Tests the libfsfat_volume_open function
 * Returns 1 if successful or 0 if not
 */
int fsfat_test_volume_open(
     const system_character_t *source )
{
	char narrow_source[ 256 ];

	libcerror_error_t *error  = NULL;
	libfsfat_volume_t *volume = NULL;
	int result                = 0;

	/* Initialize test
	 */
	result = fsfat_test_get_narrow_source(
	          source,
	          narrow_source,
	          256,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsfat_volume_initialize(
	          &volume,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSFAT_TEST_ASSERT_IS_NOT_NULL(
	 "volume",
	 volume );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open
	 */
	result = libfsfat_volume_open(
	          volume,
	          narrow_source,
	          LIBFSFAT_OPEN_READ,
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
	result = libfsfat_volume_open(
	          NULL,
	          narrow_source,
	          LIBFSFAT_OPEN_READ,
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

	result = libfsfat_volume_open(
	          volume,
	          NULL,
	          LIBFSFAT_OPEN_READ,
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

	result = libfsfat_volume_open(
	          volume,
	          narrow_source,
	          -1,
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

	/* Test open when already opened
	 */
	result = libfsfat_volume_open(
	          volume,
	          narrow_source,
	          LIBFSFAT_OPEN_READ,
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
	result = libfsfat_volume_free(
	          &volume,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "volume",
	 volume );

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
	if( volume != NULL )
	{
		libfsfat_volume_free(
		 &volume,
		 NULL );
	}
	return( 0 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Tests the libfsfat_volume_open_wide function
 * Returns 1 if successful or 0 if not
 */
int fsfat_test_volume_open_wide(
     const system_character_t *source )
{
	wchar_t wide_source[ 256 ];

	libcerror_error_t *error  = NULL;
	libfsfat_volume_t *volume = NULL;
	int result                = 0;

	/* Initialize test
	 */
	result = fsfat_test_get_wide_source(
	          source,
	          wide_source,
	          256,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsfat_volume_initialize(
	          &volume,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSFAT_TEST_ASSERT_IS_NOT_NULL(
	 "volume",
	 volume );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open
	 */
	result = libfsfat_volume_open_wide(
	          volume,
	          wide_source,
	          LIBFSFAT_OPEN_READ,
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
	result = libfsfat_volume_open_wide(
	          NULL,
	          wide_source,
	          LIBFSFAT_OPEN_READ,
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

	result = libfsfat_volume_open_wide(
	          volume,
	          NULL,
	          LIBFSFAT_OPEN_READ,
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

	result = libfsfat_volume_open_wide(
	          volume,
	          wide_source,
	          -1,
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

	/* Test open when already opened
	 */
	result = libfsfat_volume_open_wide(
	          volume,
	          wide_source,
	          LIBFSFAT_OPEN_READ,
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
	result = libfsfat_volume_free(
	          &volume,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "volume",
	 volume );

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
	if( volume != NULL )
	{
		libfsfat_volume_free(
		 &volume,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Tests the libfsfat_volume_open_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int fsfat_test_volume_open_file_io_handle(
     const system_character_t *source )
{
	libbfio_handle_t *file_io_handle = NULL;
	libcerror_error_t *error         = NULL;
	libfsfat_volume_t *volume        = NULL;
	size_t string_length             = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libbfio_file_initialize(
	          &file_io_handle,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        FSFAT_TEST_ASSERT_IS_NOT_NULL(
         "file_io_handle",
         file_io_handle );

        FSFAT_TEST_ASSERT_IS_NULL(
         "error",
         error );

	string_length = system_string_length(
	                 source );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libbfio_file_set_name_wide(
	          file_io_handle,
	          source,
	          string_length,
	          &error );
#else
	result = libbfio_file_set_name(
	          file_io_handle,
	          source,
	          string_length,
	          &error );
#endif
	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        FSFAT_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libfsfat_volume_initialize(
	          &volume,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSFAT_TEST_ASSERT_IS_NOT_NULL(
	 "volume",
	 volume );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open
	 */
	result = libfsfat_volume_open_file_io_handle(
	          volume,
	          file_io_handle,
	          LIBFSFAT_OPEN_READ,
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
	result = libfsfat_volume_open_file_io_handle(
	          NULL,
	          file_io_handle,
	          LIBFSFAT_OPEN_READ,
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

	result = libfsfat_volume_open_file_io_handle(
	          volume,
	          NULL,
	          LIBFSFAT_OPEN_READ,
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

	result = libfsfat_volume_open_file_io_handle(
	          volume,
	          file_io_handle,
	          -1,
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

	/* Test open when already opened
	 */
	result = libfsfat_volume_open_file_io_handle(
	          volume,
	          file_io_handle,
	          LIBFSFAT_OPEN_READ,
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
	result = libfsfat_volume_free(
	          &volume,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "volume",
	 volume );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbfio_handle_free(
	          &file_io_handle,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSFAT_TEST_ASSERT_IS_NULL(
         "file_io_handle",
         file_io_handle );

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
	if( volume != NULL )
	{
		libfsfat_volume_free(
		 &volume,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsfat_volume_close function
 * Returns 1 if successful or 0 if not
 */
int fsfat_test_volume_close(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsfat_volume_close(
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

/* Tests the libfsfat_volume_open and libfsfat_volume_close functions
 * Returns 1 if successful or 0 if not
 */
int fsfat_test_volume_open_close(
     const system_character_t *source )
{
	libcerror_error_t *error  = NULL;
	libfsfat_volume_t *volume = NULL;
	int result                = 0;

	/* Initialize test
	 */
	result = libfsfat_volume_initialize(
	          &volume,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSFAT_TEST_ASSERT_IS_NOT_NULL(
	 "volume",
	 volume );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open and close
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfsfat_volume_open_wide(
	          volume,
	          source,
	          LIBFSFAT_OPEN_READ,
	          &error );
#else
	result = libfsfat_volume_open(
	          volume,
	          source,
	          LIBFSFAT_OPEN_READ,
	          &error );
#endif

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsfat_volume_close(
	          volume,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open and close a second time to validate clean up on close
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfsfat_volume_open_wide(
	          volume,
	          source,
	          LIBFSFAT_OPEN_READ,
	          &error );
#else
	result = libfsfat_volume_open(
	          volume,
	          source,
	          LIBFSFAT_OPEN_READ,
	          &error );
#endif

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsfat_volume_close(
	          volume,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libfsfat_volume_free(
	          &volume,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "volume",
	 volume );

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
	if( volume != NULL )
	{
		libfsfat_volume_free(
		 &volume,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsfat_volume_signal_abort function
 * Returns 1 if successful or 0 if not
 */
int fsfat_test_volume_signal_abort(
     libfsfat_volume_t *volume )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsfat_volume_signal_abort(
	          volume,
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
	result = libfsfat_volume_signal_abort(
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

/* Tests the libfsfat_volume_get_utf8_label_size function
 * Returns 1 if successful or 0 if not
 */
int fsfat_test_volume_get_utf8_label_size(
     libfsfat_volume_t *volume )
{
	libcerror_error_t *error = NULL;
	size_t utf8_label_size   = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsfat_volume_get_utf8_label_size(
	          volume,
	          &utf8_label_size,
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
	result = libfsfat_volume_get_utf8_label_size(
	          NULL,
	          &utf8_label_size,
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

	result = libfsfat_volume_get_utf8_label_size(
	          volume,
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

/* Tests the libfsfat_volume_get_utf8_label function
 * Returns 1 if successful or 0 if not
 */
int fsfat_test_volume_get_utf8_label(
     libfsfat_volume_t *volume )
{
	uint8_t utf8_label[ 512 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsfat_volume_get_utf8_label(
	          volume,
	          utf8_label,
	          512,
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
	result = libfsfat_volume_get_utf8_label(
	          NULL,
	          utf8_label,
	          512,
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

	result = libfsfat_volume_get_utf8_label(
	          volume,
	          NULL,
	          512,
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

	result = libfsfat_volume_get_utf8_label(
	          volume,
	          utf8_label,
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

	result = libfsfat_volume_get_utf8_label(
	          volume,
	          utf8_label,
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

/* Tests the libfsfat_volume_get_utf16_label_size function
 * Returns 1 if successful or 0 if not
 */
int fsfat_test_volume_get_utf16_label_size(
     libfsfat_volume_t *volume )
{
	libcerror_error_t *error = NULL;
	size_t utf16_label_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsfat_volume_get_utf16_label_size(
	          volume,
	          &utf16_label_size,
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
	result = libfsfat_volume_get_utf16_label_size(
	          NULL,
	          &utf16_label_size,
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

	result = libfsfat_volume_get_utf16_label_size(
	          volume,
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

/* Tests the libfsfat_volume_get_utf16_label function
 * Returns 1 if successful or 0 if not
 */
int fsfat_test_volume_get_utf16_label(
     libfsfat_volume_t *volume )
{
	uint16_t utf16_label[ 512 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsfat_volume_get_utf16_label(
	          volume,
	          utf16_label,
	          512,
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
	result = libfsfat_volume_get_utf16_label(
	          NULL,
	          utf16_label,
	          512,
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

	result = libfsfat_volume_get_utf16_label(
	          volume,
	          NULL,
	          512,
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

	result = libfsfat_volume_get_utf16_label(
	          volume,
	          utf16_label,
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

	result = libfsfat_volume_get_utf16_label(
	          volume,
	          utf16_label,
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

/* Tests the libfsfat_volume_get_root_directory function
 * Returns 1 if successful or 0 if not
 */
int fsfat_test_volume_get_root_directory(
     libfsfat_volume_t *volume )
{
	libcerror_error_t *error              = NULL;
	libfsfat_file_entry_t *root_directory = 0;
	int result                            = 0;

	/* Test regular cases
	 */
	result = libfsfat_volume_get_root_directory(
	          volume,
	          &root_directory,
	          &error );

	FSFAT_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	FSFAT_TEST_ASSERT_IS_NOT_NULL(
	 "root_directory",
	 root_directory );

	result = libfsfat_file_entry_free(
	          &root_directory,
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
	result = libfsfat_volume_get_root_directory(
	          NULL,
	          &root_directory,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "root_directory",
	 root_directory );

	FSFAT_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsfat_volume_get_root_directory(
	          volume,
	          NULL,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSFAT_TEST_ASSERT_IS_NULL(
	 "root_directory",
	 root_directory );

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

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc,
     wchar_t * const argv[] )
#else
int main(
     int argc,
     char * const argv[] )
#endif
{
	libbfio_handle_t *file_io_handle  = NULL;
	libcerror_error_t *error          = NULL;
	libfsfat_volume_t *volume         = NULL;
	system_character_t *option_offset = NULL;
	system_character_t *source        = NULL;
	system_integer_t option           = 0;
	size_t string_length              = 0;
	off64_t volume_offset             = 0;
	int result                        = 0;

	while( ( option = fsfat_test_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "o:" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM ".\n",
				 argv[ optind - 1 ] );

				return( EXIT_FAILURE );

			case (system_integer_t) 'o':
				option_offset = optarg;

				break;
		}
	}
	if( optind < argc )
	{
		source = argv[ optind ];
	}
	if( option_offset != NULL )
	{
		string_length = system_string_length(
		                 option_offset );

		result = fsfat_test_system_string_copy_from_64_bit_in_decimal(
		          option_offset,
		          string_length + 1,
		          (uint64_t *) &volume_offset,
		          &error );

		FSFAT_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        FSFAT_TEST_ASSERT_IS_NULL(
	         "error",
	         error );
	}
#if defined( HAVE_DEBUG_OUTPUT ) && defined( FSFAT_TEST_VOLUME_VERBOSE )
	libfsfat_notify_set_verbose(
	 1 );
	libfsfat_notify_set_stream(
	 stderr,
	 NULL );
#endif

	FSFAT_TEST_RUN(
	 "libfsfat_volume_initialize",
	 fsfat_test_volume_initialize );

	FSFAT_TEST_RUN(
	 "libfsfat_volume_free",
	 fsfat_test_volume_free );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )
	if( source != NULL )
	{
		result = libbfio_file_range_initialize(
		          &file_io_handle,
		          &error );

		FSFAT_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        FSFAT_TEST_ASSERT_IS_NOT_NULL(
	         "file_io_handle",
	         file_io_handle );

	        FSFAT_TEST_ASSERT_IS_NULL(
	         "error",
	         error );

		string_length = system_string_length(
		                 source );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libbfio_file_range_set_name_wide(
		          file_io_handle,
		          source,
		          string_length,
		          &error );
#else
		result = libbfio_file_range_set_name(
		          file_io_handle,
		          source,
		          string_length,
		          &error );
#endif
		FSFAT_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        FSFAT_TEST_ASSERT_IS_NULL(
	         "error",
	         error );

		result = libbfio_file_range_set(
		          file_io_handle,
		          volume_offset,
		          0,
		          &error );

		FSFAT_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        FSFAT_TEST_ASSERT_IS_NULL(
	         "error",
	         error );

		result = libfsfat_check_volume_signature_file_io_handle(
		          file_io_handle,
		          &error );

		FSFAT_TEST_ASSERT_NOT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		FSFAT_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	if( ( result != 0 )
	 && ( volume_offset == 0 ) )
	{
		FSFAT_TEST_RUN_WITH_ARGS(
		 "libfsfat_volume_open",
		 fsfat_test_volume_open,
		 source );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

		FSFAT_TEST_RUN_WITH_ARGS(
		 "libfsfat_volume_open_wide",
		 fsfat_test_volume_open_wide,
		 source );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

		FSFAT_TEST_RUN_WITH_ARGS(
		 "libfsfat_volume_open_file_io_handle",
		 fsfat_test_volume_open_file_io_handle,
		 source );

		FSFAT_TEST_RUN(
		 "libfsfat_volume_close",
		 fsfat_test_volume_close );

		FSFAT_TEST_RUN_WITH_ARGS(
		 "libfsfat_volume_open_close",
		 fsfat_test_volume_open_close,
		 source );
	}
	if( result != 0 )
	{
		/* Initialize volume for tests
		 */
		result = fsfat_test_volume_open_source(
		          &volume,
		          file_io_handle,
		          &error );

		FSFAT_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		FSFAT_TEST_ASSERT_IS_NOT_NULL(
		 "volume",
		 volume );

		FSFAT_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		FSFAT_TEST_RUN_WITH_ARGS(
		 "libfsfat_volume_signal_abort",
		 fsfat_test_volume_signal_abort,
		 volume );

#if defined( __GNUC__ ) && !defined( LIBFSFAT_DLL_IMPORT )

		/* TODO: add tests for libfsfat_volume_open_read */

		/* TODO: add tests for libfsfat_volume_read_block_groups */

#endif /* defined( __GNUC__ ) && !defined( LIBFSFAT_DLL_IMPORT ) */

		FSFAT_TEST_RUN_WITH_ARGS(
		 "libfsfat_volume_get_utf8_label_size",
		 fsfat_test_volume_get_utf8_label_size,
		 volume );

		FSFAT_TEST_RUN_WITH_ARGS(
		 "libfsfat_volume_get_utf8_label",
		 fsfat_test_volume_get_utf8_label,
		 volume );

		FSFAT_TEST_RUN_WITH_ARGS(
		 "libfsfat_volume_get_utf16_label_size",
		 fsfat_test_volume_get_utf16_label_size,
		 volume );

		FSFAT_TEST_RUN_WITH_ARGS(
		 "libfsfat_volume_get_utf16_label",
		 fsfat_test_volume_get_utf16_label,
		 volume );

		/* TODO: add tests for libfsntfs_volume_get_number_of_file_entries */

		/* TODO: add tests for libfsntfs_volume_get_file_entry_by_index */

		FSFAT_TEST_RUN_WITH_ARGS(
		 "libfsfat_volume_get_root_directory",
		 fsfat_test_volume_get_root_directory,
		 volume );

		/* Clean up
		 */
		result = fsfat_test_volume_close_source(
		          &volume,
		          &error );

		FSFAT_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 0 );

		FSFAT_TEST_ASSERT_IS_NULL(
		 "volume",
		 volume );

		FSFAT_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	if( file_io_handle != NULL )
	{
		result = libbfio_handle_free(
		          &file_io_handle,
		          &error );

		FSFAT_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		FSFAT_TEST_ASSERT_IS_NULL(
	         "file_io_handle",
	         file_io_handle );

	        FSFAT_TEST_ASSERT_IS_NULL(
	         "error",
	         error );
	}
#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( volume != NULL )
	{
		libfsfat_volume_free(
		 &volume,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

