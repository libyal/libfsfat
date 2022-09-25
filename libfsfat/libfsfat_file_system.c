/*
 * File system functions
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

#include "libfsfat_allocation_table.h"
#include "libfsfat_cluster_block_data.h"
#include "libfsfat_block_descriptor.h"
#include "libfsfat_block_tree.h"
#include "libfsfat_block_tree_node.h"
#include "libfsfat_definitions.h"
#include "libfsfat_directory.h"
#include "libfsfat_directory_entry.h"
#include "libfsfat_extent.h"
#include "libfsfat_file_entry.h"
#include "libfsfat_file_system.h"
#include "libfsfat_libbfio.h"
#include "libfsfat_libcdata.h"
#include "libfsfat_libcerror.h"
#include "libfsfat_libcnotify.h"
#include "libfsfat_libcthreads.h"
#include "libfsfat_libfcache.h"
#include "libfsfat_libfdata.h"
#include "libfsfat_libuna.h"
#include "libfsfat_types.h"

#include "fsfat_directory_entry.h"

/* Creates a file system
 * Make sure the value file_system is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsfat_file_system_initialize(
     libfsfat_file_system_t **file_system,
     libfsfat_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_file_system_initialize";

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( *file_system != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file system value already set.",
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
	*file_system = memory_allocate_structure(
	                libfsfat_file_system_t );

	if( *file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file system.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *file_system,
	     0,
	     sizeof( libfsfat_file_system_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file system.",
		 function );

		memory_free(
		 *file_system );

		*file_system = NULL;

		return( -1 );
	}
	if( libfcache_date_time_get_timestamp(
	     &( ( *file_system )->cache_timestamp ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve cache timestamp.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( ( *file_system )->directory_cache ),
	     LIBFSFAT_MAXIMUM_CACHE_ENTRIES_DIRECTORIES,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create directory cache.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( ( *file_system )->read_write_lock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize read/write lock.",
		 function );

		goto on_error;
	}
#endif
	( *file_system )->io_handle = io_handle;

	return( 1 );

on_error:
	if( *file_system != NULL )
	{
		if( ( *file_system )->directory_cache != NULL )
		{
			libfcache_cache_free(
			 &( ( *file_system )->directory_cache ),
			 NULL );
		}
		memory_free(
		 *file_system );

		*file_system = NULL;
	}
	return( -1 );
}

/* Frees a file system
 * Returns 1 if successful or -1 on error
 */
int libfsfat_file_system_free(
     libfsfat_file_system_t **file_system,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_file_system_free";
	int result            = 1;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( *file_system != NULL )
	{
#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( ( *file_system )->read_write_lock ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free read/write lock.",
			 function );

			result = -1;
		}
#endif
		if( ( *file_system )->root_directory != NULL )
		{
			if( libfsfat_directory_free(
			     &( ( *file_system )->root_directory ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free root directory.",
				 function );

				result = -1;
			}
		}
		if( ( *file_system )->allocation_table != NULL )
		{
			if( libfsfat_allocation_table_free(
			     &( ( *file_system )->allocation_table ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free allocation table.",
				 function );

				result = -1;
			}
		}
		if( ( *file_system )->reversed_allocation_table != NULL )
		{
			if( libfsfat_allocation_table_free(
			     &( ( *file_system )->reversed_allocation_table ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free reversed allocation table.",
				 function );

				result = -1;
			}
		}
		if( libfcache_cache_free(
		     &( ( *file_system )->directory_cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free directory cache.",
			 function );

			result = -1;
		}
		/* The io_handle reference is freed elsewhere
		 */
		memory_free(
		 *file_system );

		*file_system = NULL;
	}
	return( result );
}

/* Checks if this is the first time the cluster block is being read
 * Returns 1 if successful or -1 on error
 */
int libfsfat_file_system_check_if_cluster_block_first_read(
     libfsfat_file_system_t *file_system,
     libfsfat_block_tree_t *cluster_block_tree,
     uint32_t cluster_number,
     off64_t cluster_offset,
     libcerror_error_t **error )
{
	libfsfat_block_descriptor_t *existing_block_descriptor = NULL;
	libfsfat_block_descriptor_t *new_block_descriptor      = NULL;
	libfsfat_block_tree_node_t *leaf_block_tree_node       = NULL;
	static char *function                                  = "libfsfat_file_system_check_if_cluster_block_first_read";
	int leaf_value_index                                   = 0;
	int result                                             = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( libfsfat_block_descriptor_initialize(
	     &new_block_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create block descriptor.",
		 function );

		goto on_error;
	}
	new_block_descriptor->cluster_number = cluster_number;

	result = libfsfat_block_tree_insert_block_descriptor_by_offset(
	          cluster_block_tree,
	          cluster_offset,
	          new_block_descriptor,
	          &leaf_value_index,
	          &leaf_block_tree_node,
	          &existing_block_descriptor,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to insert block descriptor in cluster block tree.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid cluster number: %" PRIu32 " value already exists.",
		 function,
		 cluster_number );

		goto on_error;
	}
	new_block_descriptor = NULL;

	return( 1 );

on_error:
	if( new_block_descriptor != NULL )
	{
		libfsfat_block_descriptor_free(
		 &new_block_descriptor,
		 NULL );
	}
	return( -1 );
}

/* Reads an allocation table
 * Returns 1 if successful or -1 on error
 */
int libfsfat_file_system_read_allocation_table(
     libfsfat_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     size64_t size,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_file_system_read_allocation_table";

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( file_system->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file system - missing IO handle.",
		 function );

		return( -1 );
	}
	if( file_system->allocation_table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file system - allocation table value already set.",
		 function );

		return( -1 );
	}
	if( libfsfat_allocation_table_initialize(
	     &( file_system->allocation_table ),
	     file_system->io_handle->total_number_of_clusters,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create allocation table.",
		 function );

		goto on_error;
	}
	if( ( file_system->io_handle->file_system_format == LIBFSFAT_FILE_SYSTEM_FORMAT_FAT12 )
	 || ( file_system->io_handle->file_system_format == LIBFSFAT_FILE_SYSTEM_FORMAT_FAT16 )
	 || ( file_system->io_handle->file_system_format == LIBFSFAT_FILE_SYSTEM_FORMAT_FAT32 ) )
	{
		if( libfsfat_allocation_table_initialize(
		     &( file_system->reversed_allocation_table ),
		     file_system->io_handle->total_number_of_clusters,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create reversed allocation table.",
			 function );

			goto on_error;
		}
	}
	if( libfsfat_allocation_table_read_file_io_handle(
	     file_system->allocation_table,
	     file_system->reversed_allocation_table,
	     file_system->io_handle,
	     file_io_handle,
	     file_offset,
	     size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read allocation table.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( file_system->reversed_allocation_table != NULL )
	{
		libfsfat_allocation_table_free(
		 &( file_system->reversed_allocation_table ),
		 NULL );
	}
	if( file_system->allocation_table != NULL )
	{
		libfsfat_allocation_table_free(
		 &( file_system->allocation_table ),
		 NULL );
	}
	return( -1 );
}

/* Reads a directory
 * Returns 1 if successful or -1 on error
 */
int libfsfat_file_system_read_directory(
     libfsfat_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     uint32_t cluster_number,
     libfsfat_directory_t **directory,
     libcerror_error_t **error )
{
	libcdata_array_t *name_entries_array           = NULL;
	libfsfat_block_tree_t *cluster_block_tree      = NULL;
	libfsfat_directory_t *safe_directory           = NULL;
	libfsfat_directory_entry_t *current_file_entry = NULL;
	libfsfat_directory_entry_t *data_stream_entry  = NULL;
	libfsfat_directory_entry_t *directory_entry    = NULL;
	static char *function                          = "libfsfat_file_system_read_directory";
	off64_t cluster_end_offset                     = 0;
	off64_t cluster_offset                         = 0;
	uint32_t last_cluster_number                   = 0;
	uint8_t last_vfat_sequence_number              = 0;
	uint8_t vfat_sequence_number                   = 0;
	int entry_index                                = 0;
	int result                                     = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( file_system->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file system - missing IO handle.",
		 function );

		return( -1 );
	}
	switch( file_system->io_handle->file_system_format )
	{
		case LIBFSFAT_FILE_SYSTEM_FORMAT_FAT12:
			last_cluster_number = 0x00000ff0UL;
			break;

		case LIBFSFAT_FILE_SYSTEM_FORMAT_FAT16:
			last_cluster_number = 0x0000fff0UL;
			break;

		case LIBFSFAT_FILE_SYSTEM_FORMAT_FAT32:
			last_cluster_number = 0x0ffffff0UL;
			break;

		case LIBFSFAT_FILE_SYSTEM_FORMAT_EXFAT:
			last_cluster_number = 0xfffffff0UL;
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported file system format.",
			 function );

			goto on_error;
	}
	if( directory == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory.",
		 function );

		return( -1 );
	}
	if( *directory != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid directory value already set.",
		 function );

		return( -1 );
	}
	if( libfsfat_block_tree_initialize(
	     &cluster_block_tree,
	     file_system->io_handle->volume_size,
	     file_system->io_handle->cluster_block_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create cluster block tree.",
		 function );

		goto on_error;
	}
	if( libfsfat_directory_initialize(
	     &safe_directory,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create directory.",
		 function );

		goto on_error;
	}
	while( ( cluster_number >= 2 )
	    && ( cluster_number < last_cluster_number ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: reading cluster number: %" PRIu32 "\n",
			 function,
			 cluster_number );
		}
#endif
		cluster_offset     = file_system->io_handle->first_cluster_offset + ( (off64_t) ( cluster_number - 2 ) * file_system->io_handle->cluster_block_size );
		cluster_end_offset = cluster_offset + file_system->io_handle->cluster_block_size;

		if( libfsfat_file_system_check_if_cluster_block_first_read(
		     file_system,
		     cluster_block_tree,
		     cluster_number,
		     cluster_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to check if first read of cluster number: %" PRIu32 ".",
			 function,
			 cluster_number );

			goto on_error;
		}
		while( cluster_offset < cluster_end_offset )
		{
			if( libfsfat_directory_entry_initialize(
			     &directory_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create directory entry.",
				 function );

				goto on_error;
			}
			result = libfsfat_directory_entry_read_file_io_handle(
			          directory_entry,
			          file_io_handle,
			          cluster_offset,
			          file_system->io_handle->file_system_format,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read directory entry.",
				 function );

				goto on_error;
			}
			else if( result == 0 )
			{
				if( libfsfat_directory_entry_free(
				     &directory_entry,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free directory entry.",
					 function );

					goto on_error;
				}
				break;
			}
			if( directory_entry->entry_type == LIBFSFAT_DIRECTORY_ENTRY_TYPE_UNALLOCATED )
			{
				if( libfsfat_directory_entry_free(
				     &directory_entry,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free directory entry.",
					 function );

					goto on_error;
				}
				cluster_offset += sizeof( fsfat_directory_entry_t );

				continue;
			}
			if( ( directory_entry->name_data[ 0 ] == '.' )
			 && ( directory_entry->name_data[ 1 ] == ' ' )
			 && ( directory_entry->name_data[ 2 ] == ' ' )
			 && ( directory_entry->name_data[ 3 ] == ' ' )
			 && ( directory_entry->name_data[ 4 ] == ' ' )
			 && ( directory_entry->name_data[ 5 ] == ' ' )
			 && ( directory_entry->name_data[ 6 ] == ' ' )
			 && ( directory_entry->name_data[ 7 ] == ' ' )
			 && ( directory_entry->name_data[ 8 ] == ' ' )
			 && ( directory_entry->name_data[ 9 ] == ' ' )
			 && ( directory_entry->name_data[ 10 ] == ' ' ) )
			{
				if( safe_directory->self_entry != NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
					 "%s: invalid directory - self entry value already set.",
					 function );

					goto on_error;
				}
				safe_directory->self_entry = directory_entry;
			}
			else if( ( directory_entry->name_data[ 0 ] == '.' )
			      && ( directory_entry->name_data[ 1 ] == '.' )
			      && ( directory_entry->name_data[ 2 ] == ' ' )
			      && ( directory_entry->name_data[ 3 ] == ' ' )
			      && ( directory_entry->name_data[ 4 ] == ' ' )
			      && ( directory_entry->name_data[ 5 ] == ' ' )
			      && ( directory_entry->name_data[ 6 ] == ' ' )
			      && ( directory_entry->name_data[ 7 ] == ' ' )
			      && ( directory_entry->name_data[ 8 ] == ' ' )
			      && ( directory_entry->name_data[ 9 ] == ' ' )
			      && ( directory_entry->name_data[ 10 ] == ' ' ) )
			{
				if( safe_directory->parent_entry != NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
					 "%s: invalid directory - parent entry value already set.",
					 function );

					goto on_error;
				}
				safe_directory->parent_entry = directory_entry;
			}
			else if( directory_entry->entry_type == LIBFSFAT_DIRECTORY_ENTRY_TYPE_SHORT_NAME )
			{
				if( ( directory_entry->file_attribute_flags & LIBFSFAT_FILE_ATTRIBUTE_FLAG_VOLUME_LABEL ) != 0 )
				{
					if( safe_directory->volume_label_entry != NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
						 "%s: invalid directory - volume label entry value already set.",
						 function );

						goto on_error;
					}
					safe_directory->volume_label_entry = directory_entry;
				}
				else
				{
					directory_entry->identifier = (uint64_t) cluster_offset;

					if( name_entries_array != NULL )
					{
						if( libfsfat_directory_entry_get_name_from_vfat_long_file_name_entries(
						     directory_entry,
						     name_entries_array,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
							 "%s: unable to determine name from VFAT long file name entries.",
							 function );

							goto on_error;
						}
						if( libcdata_array_free(
						     &name_entries_array,
						     NULL,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
							 "%s: unable to free VFAT long file name entries array.",
							 function );

							goto on_error;
						}
					}
					if( libcdata_array_append_entry(
					     safe_directory->file_entries_array,
					     &entry_index,
					     (intptr_t *) directory_entry,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
						 "%s: unable to append directory entry to file entries array.",
						 function );

						goto on_error;
					}
				}
			}
			else if( directory_entry->entry_type == LIBFSFAT_DIRECTORY_ENTRY_TYPE_VFAT_LONG_NAME )
			{
				vfat_sequence_number = directory_entry->vfat_sequence_number & 0x1f;

				if( ( directory_entry->vfat_sequence_number & 0x40 ) != 0 )
				{
					if( name_entries_array != NULL )
					{
						if( libcdata_array_free(
						     &name_entries_array,
						     NULL,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
							 "%s: unable to free VFAT long file name entries array.",
							 function );

							goto on_error;
						}
					}
					if( libcdata_array_initialize(
					     &name_entries_array,
					     0,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
						 "%s: unable to create VFAT long file name entries array.",
						 function );

						goto on_error;
					}
				}
				else
				{
					if( ( vfat_sequence_number + 1 ) != last_vfat_sequence_number )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
						 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
						 "%s: invalid VFAT sequence number value out of bounds.",
						 function );

						goto on_error;
					}
				}
				if( libcdata_array_append_entry(
				     name_entries_array,
				     &entry_index,
				     (intptr_t *) directory_entry,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append directory entry to VFAT long file name entries array.",
					 function );

					goto on_error;
				}
				last_vfat_sequence_number = vfat_sequence_number;
			}
			else if( directory_entry->entry_type == LIBFSFAT_DIRECTORY_ENTRY_TYPE_EXFAT_DATA_STREAM )
			{
				if( data_stream_entry != NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
					 "%s: invalid current file entry - data stream entry value already set.",
					 function );

					goto on_error;
				}
				data_stream_entry = directory_entry;

				if( current_file_entry == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing current file entry.",
					 function );

					goto on_error;
				}
				current_file_entry->data_start_cluster = directory_entry->data_start_cluster;
				current_file_entry->data_size          = directory_entry->data_size;
				current_file_entry->valid_data_size    = directory_entry->valid_data_size;
			}
			else if( directory_entry->entry_type == LIBFSFAT_DIRECTORY_ENTRY_TYPE_EXFAT_FILE_ENTRY )
			{
				directory_entry->identifier = (uint64_t) cluster_offset;

				if( libcdata_array_append_entry(
				     safe_directory->file_entries_array,
				     &entry_index,
				     (intptr_t *) directory_entry,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append directory entry to file entries array.",
					 function );

					goto on_error;
				}
				if( libcdata_array_initialize(
				     &( directory_entry->name_entries_array ),
				     0,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create name entries array.",
					 function );

					goto on_error;
				}
				current_file_entry = directory_entry;
				data_stream_entry  = NULL;
			}
			else if( directory_entry->entry_type == LIBFSFAT_DIRECTORY_ENTRY_TYPE_EXFAT_FILE_ENTRY_NAME )
			{
				if( current_file_entry == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing current file entry.",
					 function );

					goto on_error;
				}
				if( libcdata_array_append_entry(
				     current_file_entry->name_entries_array,
				     &entry_index,
				     (intptr_t *) directory_entry,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append directory entry to name entries array.",
					 function );

					goto on_error;
				}
			}
			else if( directory_entry->entry_type == LIBFSFAT_DIRECTORY_ENTRY_TYPE_EXFAT_VOLUME_LABEL )
			{
				if( safe_directory->volume_label_entry != NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
					 "%s: invalid directory - volume label entry value already set.",
					 function );

					goto on_error;
				}
				safe_directory->volume_label_entry = directory_entry;
			}
			if( libcdata_array_append_entry(
			     safe_directory->entries_array,
			     &entry_index,
			     (intptr_t *) directory_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append directory entry to array.",
				 function );

				goto on_error;
			}
			directory_entry = NULL;

			cluster_offset += sizeof( fsfat_directory_entry_t );
		}
		if( result == 0 )
		{
			break;
		}
		if( libfsfat_allocation_table_get_cluster_number_by_index(
		     file_system->allocation_table,
		     (int) cluster_number,
		     &cluster_number,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve cluster number: %" PRIu32 ".",
			 function,
			 cluster_number );

			goto on_error;
		}
	}
	if( name_entries_array != NULL )
	{
		if( libcdata_array_free(
		     &name_entries_array,
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free name entries array.",
			 function );

			goto on_error;
		}
	}
	if( libfsfat_block_tree_free(
	     &cluster_block_tree,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfsfat_block_descriptor_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free cluster block tree.",
		 function );

		goto on_error;
	}
	*directory = safe_directory;

	return( 1 );

on_error:
	if( name_entries_array != NULL )
	{
		libcdata_array_free(
		 &name_entries_array,
		 NULL,
		 NULL );
	}
	if( directory_entry != NULL )
	{
		libfsfat_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	if( safe_directory != NULL )
	{
		libfsfat_directory_free(
		 &safe_directory,
		 NULL );
	}
	if( cluster_block_tree != NULL )
	{
		libfsfat_block_tree_free(
		 &cluster_block_tree,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfsfat_block_descriptor_free,
		 NULL );
	}
	return( -1 );
}

/* Reads a directory
 * Returns 1 if successful or -1 on error
 */
int libfsfat_file_system_read_directory_by_range(
     libfsfat_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     size64_t size,
     libfsfat_directory_t **directory,
     libcerror_error_t **error )
{
	libcdata_array_t *name_entries_array        = NULL;
	libfsfat_directory_t *safe_directory        = NULL;
	libfsfat_directory_entry_t *directory_entry = NULL;
	static char *function                       = "libfsfat_file_system_read_directory_by_range";
	off64_t file_end_offset                     = 0;
	int entry_index                             = 0;
	int result                                  = 0;
	uint8_t last_vfat_sequence_number           = 0;
	uint8_t vfat_sequence_number                = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( file_system->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file system - missing IO handle.",
		 function );

		return( -1 );
	}
	if( directory == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory.",
		 function );

		return( -1 );
	}
	if( *directory != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid directory value already set.",
		 function );

		return( -1 );
	}
	if( libfsfat_directory_initialize(
	     &safe_directory,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create directory.",
		 function );

		goto on_error;
	}
	file_end_offset = file_offset + size;

	while( file_offset < file_end_offset )
	{
		if( libfsfat_directory_entry_initialize(
		     &directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create directory entry.",
			 function );

			goto on_error;
		}
		directory_entry->identifier = (uint64_t) file_offset;

		result = libfsfat_directory_entry_read_file_io_handle(
		          directory_entry,
		          file_io_handle,
		          file_offset,
		          file_system->io_handle->file_system_format,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read directory entry.",
			 function );

			goto on_error;
		}
		else if( result == 0 )
		{
			if( libfsfat_directory_entry_free(
			     &directory_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free directory entry.",
				 function );

				goto on_error;
			}
			break;
		}
		if( directory_entry->entry_type == LIBFSFAT_DIRECTORY_ENTRY_TYPE_UNALLOCATED )
		{
			if( libfsfat_directory_entry_free(
			     &directory_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free directory entry.",
				 function );

				goto on_error;
			}
			file_offset += sizeof( fsfat_directory_entry_t );

			continue;
		}
		if( directory_entry->entry_type == LIBFSFAT_DIRECTORY_ENTRY_TYPE_SHORT_NAME )
		{
			if( directory_entry->file_attribute_flags == LIBFSFAT_FILE_ATTRIBUTE_FLAG_VOLUME_LABEL )
			{
				if( safe_directory->volume_label_entry != NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
					 "%s: invalid directory - volume label entry value already set.",
					 function );

					goto on_error;
				}
				safe_directory->volume_label_entry = directory_entry;
			}
			else
			{
				directory_entry->identifier = (uint64_t) file_offset;

				if( name_entries_array != NULL )
				{
					if( libfsfat_directory_entry_get_name_from_vfat_long_file_name_entries(
					     directory_entry,
					     name_entries_array,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to determine name from VFAT long file name entries.",
						 function );

						goto on_error;
					}
					if( libcdata_array_free(
					     &name_entries_array,
					     NULL,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
						 "%s: unable to free VFAT long file name entries array.",
						 function );

						goto on_error;
					}
				}
				if( libcdata_array_append_entry(
				     safe_directory->file_entries_array,
				     &entry_index,
				     (intptr_t *) directory_entry,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append directory entry to file entries array.",
					 function );

					goto on_error;
				}
			}
		}
		else if( directory_entry->entry_type == LIBFSFAT_DIRECTORY_ENTRY_TYPE_VFAT_LONG_NAME )
		{
			vfat_sequence_number = directory_entry->vfat_sequence_number & 0x1f;

			if( ( directory_entry->vfat_sequence_number & 0x40 ) != 0 )
			{
				if( name_entries_array != NULL )
				{
					if( libcdata_array_free(
					     &name_entries_array,
					     NULL,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
						 "%s: unable to free VFAT long file name entries array.",
						 function );

						goto on_error;
					}
				}
				if( libcdata_array_initialize(
				     &name_entries_array,
				     0,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create VFAT long file name entries array.",
					 function );

					goto on_error;
				}
			}
			else
			{
				if( ( vfat_sequence_number + 1 ) != last_vfat_sequence_number )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid VFAT sequence number value out of bounds.",
					 function );

					goto on_error;
				}
			}
			if( libcdata_array_append_entry(
			     name_entries_array,
			     &entry_index,
			     (intptr_t *) directory_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append directory entry to VFAT long file name entries array.",
				 function );

				goto on_error;
			}
			last_vfat_sequence_number = vfat_sequence_number;
		}
		if( libcdata_array_append_entry(
		     safe_directory->entries_array,
		     &entry_index,
		     (intptr_t *) directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append directory entry to array.",
			 function );

			goto on_error;
		}
		directory_entry = NULL;

		file_offset += sizeof( fsfat_directory_entry_t );
	}
	if( name_entries_array != NULL )
	{
		if( libcdata_array_free(
		     &name_entries_array,
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free name entries array.",
			 function );

			goto on_error;
		}
	}
	*directory = safe_directory;

	return( 1 );

on_error:
	if( name_entries_array != NULL )
	{
		libcdata_array_free(
		 &name_entries_array,
		 NULL,
		 NULL );
	}
	if( directory_entry != NULL )
	{
		libfsfat_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	if( safe_directory != NULL )
	{
		libfsfat_directory_free(
		 &safe_directory,
		 NULL );
	}
	return( -1 );
}

/* Reads a directory entry for a specific identifier
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsfat_file_system_read_directory_entry_by_identifier(
     libfsfat_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     uint64_t identifier,
     libfsfat_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	libcdata_array_t *name_entries_array             = NULL;
	libfsfat_directory_entry_t *current_file_entry   = NULL;
	libfsfat_directory_entry_t *data_stream_entry    = NULL;
	libfsfat_directory_entry_t *safe_directory_entry = NULL;
	static char *function                            = "libfsfat_file_system_read_directory_entry_by_identifier";
	off64_t cluster_end_offset                       = 0;
	off64_t cluster_offset                           = 0;
	uint32_t cluster_number                          = 0;
	uint8_t last_vfat_sequence_number                = 0;
	uint8_t vfat_sequence_number                     = 0;
	int entry_index                                  = 0;
	int result                                       = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( file_system->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file system - missing IO handle.",
		 function );

		return( -1 );
	}
	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( libfsfat_directory_entry_initialize(
	     &current_file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create directory entry.",
		 function );

		goto on_error;
	}
	result = libfsfat_directory_entry_read_file_io_handle(
	          current_file_entry,
	          file_io_handle,
	          (off64_t) identifier,
	          file_system->io_handle->file_system_format,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read directory entry.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		if( libfsfat_directory_entry_free(
		     &current_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free directory entry.",
			 function );

			goto on_error;
		}
		return( 0 );
	}
	if( ( current_file_entry->entry_type != LIBFSFAT_DIRECTORY_ENTRY_TYPE_SHORT_NAME )
	 && ( current_file_entry->entry_type != LIBFSFAT_DIRECTORY_ENTRY_TYPE_EXFAT_FILE_ENTRY ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported directory entry type.",
		 function );

		goto on_error;
	}
	current_file_entry->identifier = identifier;

	cluster_number     = ( identifier / file_system->io_handle->cluster_block_size );
	cluster_offset     = (off64_t) cluster_number * file_system->io_handle->cluster_block_size;
	cluster_end_offset = cluster_offset + file_system->io_handle->cluster_block_size;
	cluster_number    += 2;

	if( libcdata_array_initialize(
	     &name_entries_array,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create name entries array.",
		 function );

		goto on_error;
	}
	if( current_file_entry->entry_type == LIBFSFAT_DIRECTORY_ENTRY_TYPE_SHORT_NAME )
	{
		cluster_offset = identifier - sizeof( fsfat_directory_entry_t );

		while( cluster_number > 2 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: reading cluster number: %" PRIu32 "\n",
				 function,
				 cluster_number );
			}
#endif
			while( cluster_offset <= cluster_end_offset )
			{
				if( libfsfat_directory_entry_initialize(
				     &safe_directory_entry,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create directory entry.",
					 function );

					goto on_error;
				}
				result = libfsfat_directory_entry_read_file_io_handle(
				          safe_directory_entry,
				          file_io_handle,
				          cluster_offset,
				          file_system->io_handle->file_system_format,
				          error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read directory entry.",
					 function );

					goto on_error;
				}
				if( safe_directory_entry->entry_type == LIBFSFAT_DIRECTORY_ENTRY_TYPE_VFAT_LONG_NAME )
				{
					vfat_sequence_number = safe_directory_entry->vfat_sequence_number & 0x1f;

					if( ( safe_directory_entry->vfat_sequence_number & 0x40 ) != 0 )
					{
						result = 0;
					}
					if( ( last_vfat_sequence_number != 0 )
					 && ( ( last_vfat_sequence_number + 1 ) != vfat_sequence_number ) )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
						 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
						 "%s: invalid VFAT sequence number value out of bounds.",
						 function );

						goto on_error;
					}
					if( libcdata_array_prepend_entry(
					     name_entries_array,
					     (intptr_t *) safe_directory_entry,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
						 "%s: unable to prepend directory entry to VFAT long file name entries array.",
						 function );

						goto on_error;
					}
					safe_directory_entry = NULL;

					last_vfat_sequence_number = vfat_sequence_number;
				}
				else
				{
					if( safe_directory_entry->entry_type == LIBFSFAT_DIRECTORY_ENTRY_TYPE_SHORT_NAME )
					{
						result = 0;
					}
					if( libfsfat_directory_entry_free(
					     &safe_directory_entry,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
						 "%s: unable to free directory entry.",
						 function );

						goto on_error;
					}
					break;
				}
				cluster_offset -= sizeof( fsfat_directory_entry_t );
			}
			if( result == 0 )
			{
				break;
			}
			if( libfsfat_allocation_table_get_cluster_number_by_index(
			     file_system->reversed_allocation_table,
			     (int) cluster_number,
			     &cluster_number,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve cluster number: %" PRIu32 ".",
				 function,
				 cluster_number );

				goto on_error;
			}
			cluster_offset     = file_system->io_handle->first_cluster_offset + ( (off64_t) ( cluster_number - 2 ) * file_system->io_handle->cluster_block_size );
			cluster_end_offset = cluster_offset + file_system->io_handle->cluster_block_size;
		}
		if( last_vfat_sequence_number != 0 )
		{
			if( libfsfat_directory_entry_get_name_from_vfat_long_file_name_entries(
			     current_file_entry,
			     name_entries_array,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine name from VFAT long file name entries.",
				 function );

				goto on_error;
			}
		}
	}
	else if( current_file_entry->entry_type == LIBFSFAT_DIRECTORY_ENTRY_TYPE_EXFAT_FILE_ENTRY )
	{
		cluster_offset = identifier + sizeof( fsfat_directory_entry_t );

		do
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: reading cluster number: %" PRIu32 "\n",
				 function,
				 cluster_number );
			}
#endif
			while( cluster_offset < cluster_end_offset )
			{
				if( libfsfat_directory_entry_initialize(
				     &safe_directory_entry,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create directory entry.",
					 function );

					goto on_error;
				}
				result = libfsfat_directory_entry_read_file_io_handle(
				          safe_directory_entry,
				          file_io_handle,
				          cluster_offset,
				          file_system->io_handle->file_system_format,
				          error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read directory entry.",
					 function );

					goto on_error;
				}
				if( safe_directory_entry->entry_type == LIBFSFAT_DIRECTORY_ENTRY_TYPE_EXFAT_DATA_STREAM )
				{
					if( data_stream_entry != NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
						 "%s: invalid current file entry - data stream entry value already set.",
						 function );

						goto on_error;
					}
					data_stream_entry = safe_directory_entry;

					current_file_entry->data_start_cluster = safe_directory_entry->data_start_cluster;
					current_file_entry->data_size          = safe_directory_entry->data_size;
					current_file_entry->valid_data_size    = safe_directory_entry->valid_data_size;
				}
				else if( safe_directory_entry->entry_type == LIBFSFAT_DIRECTORY_ENTRY_TYPE_EXFAT_FILE_ENTRY_NAME )
				{
					if( libcdata_array_append_entry(
					     name_entries_array,
					     &entry_index,
					     (intptr_t *) safe_directory_entry,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
						 "%s: unable to append directory entry to name entries array.",
						 function );

						goto on_error;
					}
					safe_directory_entry = NULL;
				}
				else
				{
					if( safe_directory_entry->entry_type == LIBFSFAT_DIRECTORY_ENTRY_TYPE_EXFAT_FILE_ENTRY )
					{
						result = 0;
					}
					if( libfsfat_directory_entry_free(
					     &safe_directory_entry,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
						 "%s: unable to free directory entry.",
						 function );

						goto on_error;
					}
					break;
				}
				cluster_offset += sizeof( fsfat_directory_entry_t );
			}
			if( result == 0 )
			{
				break;
			}
			if( libfsfat_allocation_table_get_cluster_number_by_index(
			     file_system->allocation_table,
			     (int) cluster_number,
			     &cluster_number,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve cluster number: %" PRIu32 ".",
				 function,
				 cluster_number );

				goto on_error;
			}
			cluster_offset     = file_system->io_handle->first_cluster_offset + ( (off64_t) ( cluster_number - 2 ) * file_system->io_handle->cluster_block_size );
			cluster_end_offset = cluster_offset + file_system->io_handle->cluster_block_size;
		}
		while( ( cluster_number >= 2 )
		    && ( cluster_number < 0xfffffff0UL ) );

		if( libfsfat_directory_entry_get_name_from_exfat_file_name_entries(
		     current_file_entry,
		     name_entries_array,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine name from exFAT file name entries.",
			 function );

			goto on_error;
		}
	}
	if( libcdata_array_free(
	     &name_entries_array,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfsfat_directory_entry_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free name entries array.",
		 function );

		goto on_error;
	}
	*directory_entry = current_file_entry;

	return( 1 );

on_error:
	if( name_entries_array != NULL )
	{
		libcdata_array_free(
		 &name_entries_array,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfsfat_directory_entry_free,
		 NULL );
	}
	if( safe_directory_entry != NULL )
	{
		libfsfat_directory_entry_free(
		 &safe_directory_entry,
		 NULL );
	}
	if( current_file_entry != NULL )
	{
		libfsfat_directory_entry_free(
		 &current_file_entry,
		 NULL );
	}
	return( -1 );
}

/* Reads the root directory
 * Returns 1 if successful or -1 on error
 */
int libfsfat_file_system_read_root_directory(
     libfsfat_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     size64_t size,
     uint32_t cluster_number,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_file_system_read_root_directory";

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( file_system->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file system - missing IO handle.",
		 function );

		return( -1 );
	}
	if( file_system->root_directory != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file system - root directory value already set.",
		 function );

		return( -1 );
	}
	if( size > 0 )
	{
		if( libfsfat_file_system_read_directory_by_range(
		     file_system,
		     file_io_handle,
		     file_offset,
		     size,
		     &( file_system->root_directory ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read root directory by range.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( libfsfat_file_system_read_directory(
		     file_system,
		     file_io_handle,
		     cluster_number,
		     &( file_system->root_directory ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read root directory by cluster number.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves a data stream
 * Returns 1 if successful or -1 on error
 */
int libfsfat_file_system_get_data_stream(
     libfsfat_file_system_t *file_system,
     uint32_t cluster_number,
     size64_t size,
     libcdata_array_t *data_extents_array,
     libfdata_stream_t **data_stream,
     libcerror_error_t **error )
{
	libfdata_stream_t *safe_data_stream       = NULL;
	libfsfat_block_tree_t *cluster_block_tree = NULL;
	libfsfat_extent_t *extent                 = NULL;
	static char *function                     = "libfsfat_file_system_get_data_stream";
	size64_t segment_size                     = 0;
	off64_t cluster_offset                    = 0;
	off64_t segment_end_offset                = 0;
	off64_t segment_start_offset              = 0;
	uint32_t last_cluster_number              = 0;
	int entry_index                           = 0;
	int segment_index                         = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( file_system->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file system - missing IO handle.",
		 function );

		return( -1 );
	}
	switch( file_system->io_handle->file_system_format )
	{
		case LIBFSFAT_FILE_SYSTEM_FORMAT_FAT12:
			last_cluster_number = 0x00000ff0UL;
			break;

		case LIBFSFAT_FILE_SYSTEM_FORMAT_FAT16:
			last_cluster_number = 0x0000fff0UL;
			break;

		case LIBFSFAT_FILE_SYSTEM_FORMAT_FAT32:
			last_cluster_number = 0x0ffffff0UL;
			break;

		case LIBFSFAT_FILE_SYSTEM_FORMAT_EXFAT:
			last_cluster_number = 0xfffffff0UL;
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported file system format.",
			 function );

			goto on_error;
	}
	if( ( file_system->io_handle->total_number_of_clusters == 0 )
	 || ( (size_t) file_system->io_handle->total_number_of_clusters > (size_t) ( MEMORY_MAXIMUM_ALLOCATION_SIZE / sizeof( uint32_t ) ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid IO handle - total number of clusters value out of bounds.",
		 function );

		return( -1 );
	}
	if( data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data stream.",
		 function );

		return( -1 );
	}
	if( *data_stream != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data stream value already set.",
		 function );

		return( -1 );
	}
	if( libfsfat_block_tree_initialize(
	     &cluster_block_tree,
	     file_system->io_handle->volume_size,
	     file_system->io_handle->cluster_block_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create cluster block tree.",
		 function );

		goto on_error;
	}
	if( libfdata_stream_initialize(
	     &safe_data_stream,
	     NULL,
	     NULL,
	     NULL,
	     NULL,
	     (ssize_t (*)(intptr_t *, intptr_t *, int, int, uint8_t *, size_t, uint32_t, uint8_t, libcerror_error_t **)) &libfsfat_cluster_block_data_read_segment_data,
	     NULL,
	     (off64_t (*)(intptr_t *, intptr_t *, int, int, off64_t, libcerror_error_t **)) &libfsfat_cluster_block_data_seek_segment_offset,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data stream.",
		 function );

		goto on_error;
	}
	while( ( cluster_number >= 2 )
	    && ( cluster_number < last_cluster_number ) )
	{
		if( size == 0 )
		{
			break;
		}
		cluster_offset = file_system->io_handle->first_cluster_offset + ( (off64_t) ( cluster_number - 2 ) * file_system->io_handle->cluster_block_size );

		if( libfsfat_file_system_check_if_cluster_block_first_read(
		     file_system,
		     cluster_block_tree,
		     cluster_number,
		     cluster_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to check if first read of cluster number: %" PRIu32 ".",
			 function,
			 cluster_number );

			goto on_error;
		}
		if( ( segment_start_offset != 0 )
		 && ( cluster_offset > segment_end_offset ) )
		{
			segment_size = segment_end_offset - segment_start_offset;

			if( libfsfat_extent_initialize(
			     &extent,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create extent.",
				 function );

				goto on_error;
			}
			extent->offset = segment_start_offset;
			extent->size   = segment_size;

			if( libcdata_array_append_entry(
			     data_extents_array,
			     &entry_index,
			     (intptr_t *) extent,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append data extent to array.",
				 function );

				goto on_error;
			}
			extent = NULL;

			if( segment_size > size )
			{
				segment_size = size;
			}
			if( libfdata_stream_append_segment(
			     safe_data_stream,
			     &segment_index,
			     0,
			     segment_start_offset,
			     segment_size,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append stream segment.",
				 function );

				goto on_error;
			}
			size -= segment_size;

			segment_start_offset = 0;
		}
		if( segment_start_offset == 0 )
		{
			segment_start_offset = cluster_offset;
			segment_end_offset   = cluster_offset;
		}
		segment_end_offset += file_system->io_handle->cluster_block_size;

		if( libfsfat_allocation_table_get_cluster_number_by_index(
		     file_system->allocation_table,
		     (int) cluster_number,
		     &cluster_number,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve cluster number: %" PRIu32 ".",
			 function,
			 cluster_number );

			goto on_error;
		}
	}
	if( ( size > 0 )
	 && ( segment_start_offset != 0 ) )
	{
		segment_size = segment_end_offset - segment_start_offset;

		if( libfsfat_extent_initialize(
		     &extent,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create extent.",
			 function );

			goto on_error;
		}
		extent->offset = segment_start_offset;
		extent->size   = segment_size;

		if( libcdata_array_append_entry(
		     data_extents_array,
		     &entry_index,
		     (intptr_t *) extent,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append data extent to array.",
			 function );

			goto on_error;
		}
		extent = NULL;

		if( segment_size > size )
		{
			segment_size = size;
		}
		if( libfdata_stream_append_segment(
		     safe_data_stream,
		     &segment_index,
		     0,
		     segment_start_offset,
		     segment_size,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append stream segment.",
			 function );

			goto on_error;
		}
	}
	if( libfsfat_block_tree_free(
	     &cluster_block_tree,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfsfat_block_descriptor_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free cluster block tree.",
		 function );

		goto on_error;
	}
	*data_stream = safe_data_stream;

	return( 1 );

on_error:
	if( extent != NULL )
	{
		libfsfat_extent_free(
		 &extent,
		 NULL );
	}
	if( safe_data_stream != NULL )
	{
		libfdata_stream_free(
		 &safe_data_stream,
		 NULL );
	}
	if( cluster_block_tree != NULL )
	{
		libfsfat_block_tree_free(
		 &cluster_block_tree,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfsfat_block_descriptor_free,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a directory
 * Returns 1 if successful or -1 on error
 */
int libfsfat_file_system_get_directory(
     libfsfat_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     uint32_t cluster_number,
     libfsfat_directory_t **directory,
     libcerror_error_t **error )
{
	libfcache_cache_value_t *cache_value = NULL;
	libfsfat_directory_t *safe_directory = NULL;
	static char *function                = "libfsfat_file_system_get_directory";
	int result                           = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	result = libfcache_cache_get_value_by_identifier(
	          file_system->directory_cache,
	          0,
	          (off64_t) cluster_number,
	          file_system->cache_timestamp,
	          &cache_value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from cache.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: cache: 0x%08" PRIjx " hit for cluster number: %" PRIu32 "\n",
			 function,
			 (intptr_t) file_system->directory_cache,
			 cluster_number );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		if( libfcache_cache_value_get_value(
		     cache_value,
		     (intptr_t **) directory,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sector data from cache.",
			 function );

			goto on_error;
		}
	}
	else
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: cache: 0x%08" PRIjx " miss for cluster number: %" PRIu32 "\n",
			 function,
			 (intptr_t) file_system->directory_cache,
			 cluster_number );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		if( libfsfat_file_system_read_directory(
		     file_system,
		     file_io_handle,
		     cluster_number,
		     &safe_directory,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read directory: %" PRIu32 ".",
			 function,
			 cluster_number );

			goto on_error;
		}
		if( libfcache_cache_set_value_by_identifier(
		     file_system->directory_cache,
		     0,
		     (off64_t) cluster_number,
		     file_system->cache_timestamp,
		     (intptr_t *) safe_directory,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsfat_directory_free,
		     LIBFCACHE_CACHE_VALUE_FLAG_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set directory in cache.",
			 function );

			goto on_error;
		}
		*directory     = safe_directory;
		safe_directory = NULL;
	}
	return( 1 );

on_error:
	if( safe_directory != NULL )
	{
		libfsfat_directory_free(
		 &safe_directory,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the root directory file entry
 * Returns 1 if successful or -1 on error
 */
int libfsfat_file_system_get_root_directory(
     libfsfat_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     libfsfat_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_file_system_get_root_directory";

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( file_system->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file system - missing IO handle.",
		 function );

		return( -1 );
	}
	if( libfsfat_file_entry_initialize(
	     file_entry,
	     file_system->io_handle,
	     file_io_handle,
	     file_system,
	     file_system->io_handle->root_directory_offset,
	     NULL,
	     file_system->root_directory,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file entry.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the file entry of a specific (virtual) identifier
 * The identifier is the offset of the short name directory entry
 * Returns 1 if successful or -1 on error
 */
int libfsfat_file_system_get_file_entry_by_identifier(
     libfsfat_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     uint64_t identifier,
     libfsfat_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	libfsfat_directory_t *directory             = NULL;
	libfsfat_directory_entry_t *directory_entry = NULL;
	static char *function                       = "libfsfat_file_system_get_file_entry_by_identifier";

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( file_system->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file system - missing IO handle.",
		 function );

		return( -1 );
	}
	if( (off64_t) identifier == file_system->io_handle->root_directory_offset )
	{
		directory = file_system->root_directory;
	}
	else
	{
		if( libfsfat_file_system_read_directory_entry_by_identifier(
		     file_system,
		     file_io_handle,
		     identifier,
		     &directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read directory entry: %" PRIu64 ".",
			 function,
			 identifier );

			goto on_error;
		}
	}
	/* libfsfat_file_entry_initialize takes over management of safe_directory_entry and directory
	 */
	if( libfsfat_file_entry_initialize(
	     file_entry,
	     file_system->io_handle,
	     file_io_handle,
	     file_system,
	     identifier,
	     directory_entry,
	     directory,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file entry.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( directory_entry != NULL )
	{
		libfsfat_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the file entry for an UTF-8 encoded path
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libfsfat_file_system_get_file_entry_by_utf8_path(
     libfsfat_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsfat_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	libfsfat_directory_t *directory                  = NULL;
	libfsfat_directory_entry_t *directory_entry      = NULL;
	libfsfat_directory_entry_t *safe_directory_entry = NULL;
	const uint8_t *utf8_string_segment               = NULL;
	static char *function                            = "libfsfat_file_system_get_file_entry_by_utf8_path";
	libuna_unicode_character_t unicode_character     = 0;
	size_t utf8_string_index                         = 0;
	size_t utf8_string_segment_length                = 0;
	uint64_t identifier                              = 0;
	uint32_t cluster_number                          = 0;
	int result                                       = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: file system.",
		 function );

		return( -1 );
	}
	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf8_string_length > 0 )
	{
		/* Ignore a leading separator
		 */
		if( utf8_string[ utf8_string_index ] == (uint8_t) LIBFSFAT_SEPARATOR )
		{
			utf8_string_index++;
		}
	}
	directory = file_system->root_directory;

	if( ( utf8_string_length == 0 )
	 || ( utf8_string_length == 1 ) )
	{
		result = 1;
	}
	else while( utf8_string_index < utf8_string_length )
	{
		if( directory != file_system->root_directory )
		{
			if( libfsfat_file_system_get_directory(
			     file_system,
			     file_io_handle,
			     cluster_number,
			     &directory,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve directory: %" PRIu32 ".",
				 function,
				 cluster_number );

				goto on_error;
			}
		}
		utf8_string_segment        = &( utf8_string[ utf8_string_index ] );
		utf8_string_segment_length = utf8_string_index;

		while( utf8_string_index < utf8_string_length )
		{
			if( libuna_unicode_character_copy_from_utf8(
			     &unicode_character,
			     utf8_string,
			     utf8_string_length,
			     &utf8_string_index,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-8 string to Unicode character.",
				 function );

				goto on_error;
			}
			if( ( unicode_character == (libuna_unicode_character_t) LIBFSFAT_SEPARATOR )
			 || ( unicode_character == 0 ) )
			{
				utf8_string_segment_length += 1;

				break;
			}
		}
		utf8_string_segment_length = utf8_string_index - utf8_string_segment_length;

		if( utf8_string_segment_length == 0 )
		{
			result = 0;
		}
		else
		{
			result = libfsfat_directory_get_file_entry_by_utf8_name(
			          directory,
			          utf8_string_segment,
			          utf8_string_segment_length,
			          &directory_entry,
			          error );
		}
		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve directory entry by UTF-8 name.",
			 function );

			goto on_error;
		}
		else if( result == 0 )
		{
			break;
		}
		if( directory == file_system->root_directory )
		{
			directory = NULL;
		}
		if( libfsfat_directory_entry_get_data_start_cluster(
		     directory_entry,
		     &cluster_number,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data start cluster from directory entry.",
			 function );

			goto on_error;
		}
	}
	if( directory_entry == NULL )
	{
		identifier = file_system->io_handle->root_directory_offset;
	}
	else
	{
		if( libfsfat_directory_entry_get_identifier(
		     directory_entry,
		     &identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve identifier from directory entry.",
			 function );

			goto on_error;
		}
		if( libfsfat_directory_entry_clone(
		     &safe_directory_entry,
		     directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create directory entry.",
			 function );

			goto on_error;
		}
		directory = NULL;
	}
	/* libfsfat_file_entry_initialize takes over management of safe_directory_entry and directory
	 */
	if( libfsfat_file_entry_initialize(
	     file_entry,
	     file_system->io_handle,
	     file_io_handle,
	     file_system,
	     identifier,
	     safe_directory_entry,
	     directory,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file entry.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( safe_directory_entry != NULL )
	{
		libfsfat_directory_entry_free(
		 &safe_directory_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the file entry for an UTF-16 encoded path
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libfsfat_file_system_get_file_entry_by_utf16_path(
     libfsfat_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsfat_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	libfsfat_directory_t *directory                  = NULL;
	libfsfat_directory_entry_t *directory_entry      = NULL;
	libfsfat_directory_entry_t *safe_directory_entry = NULL;
	const uint16_t *utf16_string_segment             = NULL;
	static char *function                            = "libfsfat_file_system_get_file_entry_by_utf16_path";
	libuna_unicode_character_t unicode_character     = 0;
	size_t utf16_string_index                        = 0;
	size_t utf16_string_segment_length               = 0;
	uint64_t identifier                              = 0;
	uint32_t cluster_number                          = 0;
	int result                                       = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: file system.",
		 function );

		return( -1 );
	}
	if( utf16_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf16_string_length > 0 )
	{
		/* Ignore a leading separator
		 */
		if( utf16_string[ utf16_string_index ] == (uint16_t) LIBFSFAT_SEPARATOR )
		{
			utf16_string_index++;
		}
	}
	directory = file_system->root_directory;

	if( ( utf16_string_length == 0 )
	 || ( utf16_string_length == 1 ) )
	{
		result = 1;
	}
	else while( utf16_string_index < utf16_string_length )
	{
		if( directory != file_system->root_directory )
		{
			if( libfsfat_file_system_get_directory(
			     file_system,
			     file_io_handle,
			     cluster_number,
			     &directory,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve directory: %" PRIu32 ".",
				 function,
				 cluster_number );

				goto on_error;
			}
		}
		utf16_string_segment        = &( utf16_string[ utf16_string_index ] );
		utf16_string_segment_length = utf16_string_index;

		while( utf16_string_index < utf16_string_length )
		{
			if( libuna_unicode_character_copy_from_utf16(
			     &unicode_character,
			     utf16_string,
			     utf16_string_length,
			     &utf16_string_index,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-16 string to Unicode character.",
				 function );

				goto on_error;
			}
			if( ( unicode_character == (libuna_unicode_character_t) LIBFSFAT_SEPARATOR )
			 || ( unicode_character == 0 ) )
			{
				utf16_string_segment_length += 1;

				break;
			}
		}
		utf16_string_segment_length = utf16_string_index - utf16_string_segment_length;

		if( utf16_string_segment_length == 0 )
		{
			result = 0;
		}
		else
		{
			result = libfsfat_directory_get_file_entry_by_utf16_name(
			          directory,
			          utf16_string_segment,
			          utf16_string_segment_length,
			          &directory_entry,
			          error );
		}
		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve directory entry by UTF-16 name.",
			 function );

			goto on_error;
		}
		else if( result == 0 )
		{
			break;
		}
		if( directory == file_system->root_directory )
		{
			directory = NULL;
		}
		if( libfsfat_directory_entry_get_data_start_cluster(
		     directory_entry,
		     &cluster_number,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data start cluster from directory entry.",
			 function );

			goto on_error;
		}
	}
	if( directory_entry == NULL )
	{
		identifier = file_system->io_handle->root_directory_offset;
	}
	else
	{
		if( libfsfat_directory_entry_get_identifier(
		     directory_entry,
		     &identifier,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve identifier from directory entry.",
			 function );

			goto on_error;
		}
		if( libfsfat_directory_entry_clone(
		     &safe_directory_entry,
		     directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create directory entry.",
			 function );

			goto on_error;
		}
		directory = NULL;
	}
	/* libfsfat_file_entry_initialize takes over management of safe_directory_entry and directory
	 */
	if( libfsfat_file_entry_initialize(
	     file_entry,
	     file_system->io_handle,
	     file_io_handle,
	     file_system,
	     identifier,
	     safe_directory_entry,
	     directory,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file entry.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( safe_directory_entry != NULL )
	{
		libfsfat_directory_entry_free(
		 &safe_directory_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the size of the UTF-8 encoded volume label
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsfat_file_system_get_utf8_volume_label_size(
     libfsfat_file_system_t *file_system,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_file_system_get_utf8_volume_label_size";

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( libfsfat_directory_get_utf8_volume_label_size(
	     file_system->root_directory,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-8 volume label.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded volume label
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsfat_file_system_get_utf8_volume_label(
     libfsfat_file_system_t *file_system,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_file_system_get_utf8_volume_label";

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( libfsfat_directory_get_utf8_volume_label(
	     file_system->root_directory,
	     utf8_string,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 volume label.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded volume label
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsfat_file_system_get_utf16_volume_label_size(
     libfsfat_file_system_t *file_system,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_file_system_get_utf16_volume_label_size";

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( libfsfat_directory_get_utf16_volume_label_size(
	     file_system->root_directory,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-16 volume label.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded volume label
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsfat_file_system_get_utf16_volume_label(
     libfsfat_file_system_t *file_system,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_file_system_get_utf16_volume_label";

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( libfsfat_directory_get_utf16_volume_label(
	     file_system->root_directory,
	     utf16_string,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 volume label.",
		 function );

		return( -1 );
	}
	return( 1 );
}

