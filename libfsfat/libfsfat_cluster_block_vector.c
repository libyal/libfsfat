/*
 * Cluster block vector functions
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
#include <types.h>

#include "libfsfat_allocation_table.h"
#include "libfsfat_cluster_block.h"
#include "libfsfat_cluster_block_vector.h"
#include "libfsfat_definitions.h"
#include "libfsfat_io_handle.h"
#include "libfsfat_libbfio.h"
#include "libfsfat_libcerror.h"
#include "libfsfat_libfdata.h"
#include "libfsfat_unused.h"

/* Creates a cluster block vector
 * Make sure the value cluster_block_vector is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsfat_cluster_block_vector_initialize(
     libfdata_vector_t **cluster_block_vector,
     libfsfat_io_handle_t *io_handle,
     libfsfat_allocation_table_t *allocation_table,
     libcerror_error_t **error )
{
	libfdata_vector_t *safe_cluster_block_vector = NULL;
	static char *function                        = "libfsfat_cluster_block_vector_initialize";
	int segment_index                            = 0;

	if( cluster_block_vector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid cluster block vector.",
		 function );

		return( -1 );
	}
	if( *cluster_block_vector != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid cluster block vector value already set.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( io_handle->cluster_block_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid IO handle - cluster block size value out of bounds.",
		 function );

		return( -1 );
	}
/* TODO
	if( libfdata_vector_initialize(
	     &safe_cluster_block_vector,
	     (size64_t) io_handle->cluster_block_size,
	     (intptr_t *) io_handle,
	     NULL,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_vector_t *, libfdata_cache_t *, int, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libfsfat_cluster_block_vector_read_element_data,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create cluster block vector.",
		 function );

		goto on_error;
	}
	if( libfdata_vector_append_segment(
	     safe_cluster_block_vector,
	     &segment_index,
	     0,
	     data_run->start_offset,
	     data_run->size,
	     data_run->range_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append attribute: %d data run: %d vector segment.",
		 function,
		 attribute_index,
		 data_run_index );

		goto on_error;
	}
*/
	return( 1 );

on_error:
	if( safe_cluster_block_vector != NULL )
	{
		libfdata_vector_free(
		 &safe_cluster_block_vector,
		 NULL );
	}
	return( -1 );
}

/* Reads a cluster block
 * Callback function for the cluster block vector
 * Returns 1 if successful or -1 on error
 */
int libfsfat_cluster_block_vector_read_element_data(
     intptr_t *data_handle LIBFSFAT_ATTRIBUTE_UNUSED,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfdata_cache_t *cache,
     int element_index LIBFSFAT_ATTRIBUTE_UNUSED,
     int element_data_file_index LIBFSFAT_ATTRIBUTE_UNUSED,
     off64_t cluster_block_offset,
     size64_t cluster_block_size,
     uint32_t range_flags LIBFSFAT_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBFSFAT_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libfsfat_cluster_block_t *cluster_block = NULL;
	static char *function                   = "libfsfat_cluster_block_vector_read_element_data";

	LIBFSFAT_UNREFERENCED_PARAMETER( data_handle )
	LIBFSFAT_UNREFERENCED_PARAMETER( element_index )
	LIBFSFAT_UNREFERENCED_PARAMETER( element_data_file_index )
	LIBFSFAT_UNREFERENCED_PARAMETER( range_flags )
	LIBFSFAT_UNREFERENCED_PARAMETER( read_flags )

	if( ( cluster_block_size == 0 )
	 || ( cluster_block_size > (size64_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid cluster block size value out of bounds.",
		 function );

		return( -1 );
	}
	if( libfsfat_cluster_block_initialize(
	     &cluster_block,
	     (size_t) cluster_block_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create cluster block.",
		 function );

		goto on_error;
	}
	if( libfsfat_cluster_block_read_file_io_handle(
	     cluster_block,
	     file_io_handle,
	     cluster_block_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read cluster block at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 cluster_block_offset,
		 cluster_block_offset );

		goto on_error;
	}
	if( libfdata_vector_set_element_value_by_index(
	     vector,
	     (intptr_t *) file_io_handle,
	     cache,
	     element_index,
	     (intptr_t *) cluster_block,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfsfat_cluster_block_free,
	     LIBFDATA_VECTOR_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set cluster block as element value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( cluster_block != NULL )
	{
		libfsfat_cluster_block_free(
		 &cluster_block,
		 NULL );
	}
	return( -1 );
}

