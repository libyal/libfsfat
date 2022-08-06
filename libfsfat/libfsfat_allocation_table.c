/*
 * Allocation table functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfsfat_allocation_table.h"
#include "libfsfat_debug.h"
#include "libfsfat_definitions.h"
#include "libfsfat_io_handle.h"
#include "libfsfat_libbfio.h"
#include "libfsfat_libcerror.h"
#include "libfsfat_libcnotify.h"

/* Creates an allocation table
 * Make sure the value allocation_table is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsfat_allocation_table_initialize(
     libfsfat_allocation_table_t **allocation_table,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_allocation_table_initialize";

	if( allocation_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid allocation table.",
		 function );

		return( -1 );
	}
	if( *allocation_table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid allocation table value already set.",
		 function );

		return( -1 );
	}
	*allocation_table = memory_allocate_structure(
	                     libfsfat_allocation_table_t );

	if( *allocation_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create allocation table.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *allocation_table,
	     0,
	     sizeof( libfsfat_allocation_table_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear allocation table.",
		 function );

		memory_free(
		 *allocation_table );

		*allocation_table = NULL;

		return( -1 );
	}
	return( 1 );

on_error:
	if( *allocation_table != NULL )
	{
		if( ( *allocation_table )->cluster_identifiers != NULL )
		{
			memory_free(
			 ( *allocation_table )->cluster_identifiers );
		}
		memory_free(
		 *allocation_table );

		*allocation_table = NULL;
	}
	return( -1 );
}

/* Frees an allocation table
 * Returns 1 if successful or -1 on error
 */
int libfsfat_allocation_table_free(
     libfsfat_allocation_table_t **allocation_table,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_allocation_table_free";

	if( allocation_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid allocation table.",
		 function );

		return( -1 );
	}
	if( *allocation_table != NULL )
	{
		if( ( *allocation_table )->cluster_identifiers != NULL )
		{
			memory_free(
			 ( *allocation_table )->cluster_identifiers );
		}
		memory_free(
		 *allocation_table );

		*allocation_table = NULL;
	}
	return( 1 );
}

/* Reads an allocation table
 * Returns 1 if successful or -1 on error
 */
int libfsfat_allocation_table_read_file_io_handle(
     libfsfat_allocation_table_t *allocation_table,
     libfsfat_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     size64_t size,
     libcerror_error_t **error )
{
	uint8_t *table_data                  = NULL;
	static char *function                = "libfsfat_allocation_table_read_file_io_handle";
	size_t cluster_identifiers_data_size = 0;
	size_t read_size                     = 0;
	size_t table_data_offset             = 0;
	size_t table_offset                  = 0;
	ssize_t read_count                   = 0;
	uint32_t cluster_number              = 0;
	int table_index                      = 0;

	if( allocation_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid allocation table.",
		 function );

		return( -1 );
	}
	if( allocation_table->cluster_identifiers != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid allocation table - cluster identifiers already set.",
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
	if( io_handle->bytes_per_sector == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - missing bytes per sector.",
		 function );

		return( -1 );
	}
	if( ( io_handle->total_number_of_clusters == 0 )
	 || ( (size_t) io_handle->total_number_of_clusters > (size_t) ( MEMORY_MAXIMUM_ALLOCATION_SIZE / sizeof( uint32_t ) ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid IO handle - total number of clusters value out of bounds.",
		 function );

		return( -1 );
	}
	cluster_identifiers_data_size = sizeof( uint32_t ) * io_handle->total_number_of_clusters;

	allocation_table->cluster_identifiers = (uint32_t *) memory_allocate(
	                                                      cluster_identifiers_data_size );

	if( allocation_table->cluster_identifiers == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create cluster identifiers.",
		 function );

		goto on_error;
	}
	allocation_table->number_of_cluster_identifiers = io_handle->total_number_of_clusters;

	if( memory_set(
	     allocation_table->cluster_identifiers,
	     0,
	     cluster_identifiers_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear cluster identifiers.",
		 function );

		goto on_error;
	}
	/* Add 2 bytes to ensure we can read 3 bytes from the table data buffer
	 */
	table_data = (uint8_t *) memory_allocate(
	                          (size_t) io_handle->bytes_per_sector + 2 );

	if( table_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create table data.",
		 function );

		goto on_error;
	}
	table_data[ io_handle->bytes_per_sector     ] = 0;
	table_data[ io_handle->bytes_per_sector + 1 ] = 0;

	read_size = io_handle->bytes_per_sector;

	while( table_offset < size )
	{
		if( read_size > ( size - table_offset ) )
		{
			read_size = (size_t) size - table_offset;
		}
		read_count = libbfio_handle_read_buffer_at_offset(
		              file_io_handle,
		              table_data,
		              read_size,
		              file_offset,
		              error );

		if( read_count != (ssize_t) read_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read allocation table data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 file_offset,
			 file_offset );

			goto on_error;
		}
		file_offset  += read_size;
		table_offset += read_size;

		table_data_offset = 0;

		while( table_data_offset < read_size )
		{
			if( table_index >= allocation_table->number_of_cluster_identifiers )
			{
				break;
			}
			if( io_handle->file_system_format == LIBFSFAT_FILE_SYSTEM_FORMAT_FAT12 )
			{
				byte_stream_copy_to_uint24_little_endian(
				 &( table_data[ table_data_offset ] ),
				 cluster_number );

				table_data_offset += 3;

#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					if( ( cluster_number & 0x00000fffUL ) >= 0x00000800UL )
					{
						libcnotify_printf(
						 "%s: cluster: %04d number\t: 0x%03" PRIx32 " (%s)\n",
						 function,
						 table_index,
						 cluster_number & 0x00000fffUL,
						 libfsfat_debug_print_fat12_cluster_type(
						  cluster_number & 0x00000fffUL ) );
					}
					else if( ( cluster_number & 0x00000fffUL ) != 0 )
					{
						libcnotify_printf(
						 "%s: cluster: %04d number\t: %" PRIu32 "\n",
						 function,
						 table_index,
						 cluster_number & 0x00000fffUL );
					}
				}
#endif
				allocation_table->cluster_identifiers[ table_index++ ] = cluster_number & 0x00000fffUL;

				if( table_index >= allocation_table->number_of_cluster_identifiers )
				{
					break;
				}
				cluster_number >>= 12;

#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					if( cluster_number >= 0x00000800UL )
					{
						libcnotify_printf(
						 "%s: cluster: %04d number\t: 0x%03" PRIx32 " (%s)\n",
						 function,
						 table_index,
						 cluster_number,
						 libfsfat_debug_print_fat12_cluster_type(
						  cluster_number ) );
					}
					else if( cluster_number != 0 )
					{
						libcnotify_printf(
						 "%s: cluster: %04d number\t: %" PRIu32 "\n",
						 function,
						 table_index,
						 cluster_number );
					}
				}
#endif
				allocation_table->cluster_identifiers[ table_index++ ] = cluster_number;
			}
			else if( io_handle->file_system_format == LIBFSFAT_FILE_SYSTEM_FORMAT_FAT16 )
			{
				byte_stream_copy_to_uint16_little_endian(
				 &( table_data[ table_data_offset ] ),
				 cluster_number );

				table_data_offset += 2;

#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					if( cluster_number >= 0x00008000UL )
					{
						libcnotify_printf(
						 "%s: cluster: %04d number\t: 0x%04" PRIx32 " (%s)\n",
						 function,
						 table_index,
						 cluster_number,
						 libfsfat_debug_print_fat16_cluster_type(
						  cluster_number ) );
					}
					else if( cluster_number != 0 )
					{
						libcnotify_printf(
						 "%s: cluster: %04d number\t: %" PRIu32 "\n",
						 function,
						 table_index,
						 cluster_number );
					}
				}
#endif
				allocation_table->cluster_identifiers[ table_index++ ] = cluster_number;
			}
			else
			{
				byte_stream_copy_to_uint32_little_endian(
				 &( table_data[ table_data_offset ] ),
				 cluster_number );

				table_data_offset += 4;

#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					if( cluster_number >= 0x08000000UL )
					{
						if( io_handle->file_system_format == LIBFSFAT_FILE_SYSTEM_FORMAT_FAT32 )
						{
							libcnotify_printf(
							 "%s: cluster: %04d number\t: 0x%08" PRIx32 " (%s)\n",
							 function,
							 table_index,
							 cluster_number,
							 libfsfat_debug_print_fat32_cluster_type(
							  cluster_number ) );
						}
						else
						{
							libcnotify_printf(
							 "%s: cluster: %04d number\t: 0x%08" PRIx32 " (%s)\n",
							 function,
							 table_index,
							 cluster_number,
							 libfsfat_debug_print_exfat_cluster_type(
							  cluster_number ) );
						}
					}
					else if( cluster_number != 0 )
					{
						libcnotify_printf(
						 "%s: cluster: %04d number\t: %" PRIu32 "\n",
						 function,
						 table_index,
						 cluster_number );
					}
				}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

				allocation_table->cluster_identifiers[ table_index++ ] = cluster_number;
			}
		}
	}
	memory_free(
	 table_data );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );

on_error:
	if( table_data != NULL )
	{
		memory_free(
		 table_data );
	}
	if( allocation_table->cluster_identifiers != NULL )
	{
		memory_free(
		 allocation_table->cluster_identifiers );

		allocation_table->cluster_identifiers = NULL;
	}
	allocation_table->number_of_cluster_identifiers = 0;

	return( -1 );
}

/* Retrieves a specific cluster identifier from the allocation table
 * Returns 1 if successful or -1 on error
 */
int libfsfat_allocation_table_get_cluster_identifier_by_index(
     libfsfat_allocation_table_t *allocation_table,
     int entry_index,
     uint32_t *cluster_identifier,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_allocation_table_resize";

	if( allocation_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid allocation table.",
		 function );

		return( -1 );
	}
	if( ( entry_index < 0 )
	 || ( entry_index >= allocation_table->number_of_cluster_identifiers ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid entry index value out of bounds.",
		 function );

		return( -1 );
	}
	if( cluster_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid cluster identifier.",
		 function );

		return( -1 );
	}
	*cluster_identifier = allocation_table->cluster_identifiers[ entry_index ];

	return( 1 );
}

