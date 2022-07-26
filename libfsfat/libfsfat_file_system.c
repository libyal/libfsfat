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
#include "libfsfat_definitions.h"
#include "libfsfat_directory.h"
#include "libfsfat_directory_entry.h"
#include "libfsfat_file_system.h"
#include "libfsfat_libbfio.h"
#include "libfsfat_libcdata.h"
#include "libfsfat_libcerror.h"
#include "libfsfat_libcthreads.h"
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
		/* The io_handle reference is freed elsewhere
		 */
		memory_free(
		 *file_system );

		*file_system = NULL;
	}
	return( result );
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
	if( libfsfat_allocation_table_read_file_io_handle(
	     file_system->allocation_table,
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
	libcdata_array_t *long_file_name_entries_array = NULL;
	libfsfat_directory_t *safe_directory           = NULL;
	libfsfat_directory_entry_t *directory_entry    = NULL;
	static char *function                          = "libfsfat_file_system_read_directory";
	off64_t cluster_end_offset                     = 0;
	off64_t cluster_offset                         = 0;
	int entry_index                                = 0;
	int result                                     = 0;
	uint8_t last_vfat_sequence_number              = 0;
	uint8_t vfat_sequence_number                   = 0;

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
	while( ( ( file_system->io_handle->file_system_type == LIBFSFAT_FILE_SYSTEM_TYPE_FAT12 )
	      && ( cluster_number < 0x00000ff8UL ) )
	    || ( ( file_system->io_handle->file_system_type == LIBFSFAT_FILE_SYSTEM_TYPE_FAT16 )
	      && ( cluster_number < 0x0000fff8UL ) )
	    || ( ( file_system->io_handle->file_system_type == LIBFSFAT_FILE_SYSTEM_TYPE_FAT32 )
	      && ( cluster_number < 0x0ffffff8UL ) ) )
	{
		cluster_offset     = file_system->io_handle->first_cluster_offset + ( (off64_t) cluster_number * file_system->io_handle->cluster_block_size );
		cluster_end_offset = cluster_offset + file_system->io_handle->cluster_block_size;

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
			if( directory_entry->entry_type == LIBFSFAT_DIRECTORY_ENTRY_TYPE_SHORT_NAME )
			{
				directory_entry->long_file_name_entries_array = long_file_name_entries_array;
				long_file_name_entries_array                  = NULL;

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

						directory_entry = NULL;

						goto on_error;
					}
				}
			}
			else if( directory_entry->entry_type == LIBFSFAT_DIRECTORY_ENTRY_TYPE_VFAT_LONG_NAME )
			{
				vfat_sequence_number = directory_entry->vfat_sequence_number & 0x1f;

				if( ( directory_entry->vfat_sequence_number & 0x40 ) != 0 )
				{
					if( long_file_name_entries_array != NULL )
					{
						if( libcdata_array_free(
						     &long_file_name_entries_array,
						     NULL,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
							 "%s: unable to free long file name entries array.",
							 function );

							directory_entry = NULL;

							goto on_error;
						}
					}
					if( libcdata_array_initialize(
					     &long_file_name_entries_array,
					     0,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
						 "%s: unable to create long file name entries array.",
						 function );

						directory_entry = NULL;

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

						return( -1 );
					}
				}
				if( libcdata_array_append_entry(
				     long_file_name_entries_array,
				     &entry_index,
				     (intptr_t *) directory_entry,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append directory entry to long file name entries array.",
					 function );

					directory_entry = NULL;

					goto on_error;
				}
				last_vfat_sequence_number = vfat_sequence_number;
			}
			directory_entry = NULL;

			cluster_offset += sizeof( fsfat_directory_entry_t );
		}
		if( result == 0 )
		{
			break;
		}
/* TODO lookup next cluster block */
	}
	if( long_file_name_entries_array != NULL )
	{
		if( libcdata_array_free(
		     &long_file_name_entries_array,
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free long file name entries array.",
			 function );

			goto on_error;
		}
	}
	*directory = safe_directory;

	return( 1 );

on_error:
	if( long_file_name_entries_array != NULL )
	{
		libcdata_array_free(
		 &long_file_name_entries_array,
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
