/*
 * Extent functions
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
#include <memory.h>
#include <types.h>

#include "libfsfat_extent.h"
#include "libfsfat_libcerror.h"

/* Creates an extent
 * Make sure the value extent is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsfat_extent_initialize(
     libfsfat_extent_t **extent,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_extent_initialize";

	if( extent == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent.",
		 function );

		return( -1 );
	}
	if( *extent != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid extent value already set.",
		 function );

		return( -1 );
	}
	*extent = memory_allocate_structure(
	           libfsfat_extent_t );

	if( *extent == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create extent.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *extent,
	     0,
	     sizeof( libfsfat_extent_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear extent.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *extent != NULL )
	{
		memory_free(
		 *extent );

		*extent = NULL;
	}
	return( -1 );
}

/* Frees an extent
 * Returns 1 if successful or -1 on error
 */
int libfsfat_extent_free(
     libfsfat_extent_t **extent,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_extent_free";

	if( extent == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent.",
		 function );

		return( -1 );
	}
	if( *extent != NULL )
	{
		memory_free(
		 *extent );

		*extent = NULL;
	}
	return( 1 );
}

/* Retrieves the extents values
 * Returns 1 if successful or -1 on error
 */
int libfsfat_extent_get_values(
     libfsfat_extent_t *extent,
     off64_t *extent_offset,
     size64_t *extent_size,
     uint32_t *extent_flags,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_extent_get_values";

	if( extent == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent.",
		 function );

		return( -1 );
	}
	if( extent_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent offset.",
		 function );

		return( -1 );
	}
	if( extent_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent size.",
		 function );

		return( -1 );
	}
	if( extent_flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent flags.",
		 function );

		return( -1 );
	}
	*extent_offset = extent->offset;
	*extent_size   = extent->size;
	*extent_flags  = 0;

	return( 1 );
}

