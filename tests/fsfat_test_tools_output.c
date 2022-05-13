/*
 * Tools output functions test program
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

#include <stdio.h>

#if defined( HAVE_IO_H ) || defined( WINAPI )
#include <io.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "fsfat_test_libcerror.h"
#include "fsfat_test_macros.h"
#include "fsfat_test_unused.h"

#include "../fsfattools/fsfattools_output.h"

/* Tests the fsfattools_output_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsfat_test_tools_output_initialize(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	result = fsfattools_output_initialize(
	          _IONBF,
	          &error );

	FSFAT_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

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
	return( 0 );
}

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

	FSFAT_TEST_RUN(
	 "fsfattools_output_initialize",
	 fsfat_test_tools_output_initialize )

	/* TODO add tests for fsfattools_output_copyright_fprint */

	/* TODO add tests for fsfattools_output_version_fprint */

	/* TODO add tests for fsfattools_output_version_detailed_fprint */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

