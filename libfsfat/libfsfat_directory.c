/*
 * The directory functions
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

#include "libfsfat_definitions.h"
#include "libfsfat_directory.h"
#include "libfsfat_directory_entry.h"
#include "libfsfat_libbfio.h"
#include "libfsfat_libcdata.h"
#include "libfsfat_libcerror.h"

#include "fsfat_directory_entry.h"

/* Creates a directory
 * Make sure the value directory is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsfat_directory_initialize(
     libfsfat_directory_t **directory,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_directory_initialize";

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
	*directory = memory_allocate_structure(
	              libfsfat_directory_t );

	if( *directory == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create directory.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *directory,
	     0,
	     sizeof( libfsfat_directory_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear directory.",
		 function );

		memory_free(
		 *directory );

		*directory = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *directory )->entries_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create entries array.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( ( *directory )->file_entries_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file entries array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *directory != NULL )
	{
		if( ( *directory )->entries_array != NULL )
		{
			libcdata_array_free(
			 &( ( *directory )->entries_array ),
			 NULL,
			 NULL );
		}
		memory_free(
		 *directory );

		*directory = NULL;
	}
	return( -1 );
}

/* Frees a directory
 * Returns 1 if successful or -1 on error
 */
int libfsfat_directory_free(
     libfsfat_directory_t **directory,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_directory_free";
	int result            = 1;

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
		if( libcdata_array_free(
		     &( ( *directory )->file_entries_array ),
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file entries array.",
			 function );

			result = -1;
		}
		if( libcdata_array_free(
		     &( ( *directory )->entries_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsfat_directory_entry_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free entries array.",
			 function );

			result = -1;
		}
		memory_free(
		 *directory );

		*directory = NULL;
	}
	return( result );
}

/* Reads a directory
 * Returns 1 if successful or -1 on error
 */
int libfsfat_directory_read_file_io_handle(
     libfsfat_directory_t *directory,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	libcdata_array_t *long_file_name_entries_array = NULL;
	libfsfat_directory_entry_t *directory_entry    = NULL;
	static char *function                          = "libfsfat_directory_read_file_io_handle";
	int entry_index                                = 0;
	int result                                     = 0;
	uint8_t last_vfat_sequence_number              = 0;
	uint8_t vfat_sequence_number                   = 0;

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
/* TODO determine stream containing directory entries */
	do
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
/* TODO read directory entry from stream */
		result = libfsfat_directory_entry_read_file_io_handle(
		          directory_entry,
		          file_io_handle,
		          file_offset,
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
		}
		else
		{
			if( libcdata_array_append_entry(
			     directory->entries_array,
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
					if( directory->volume_label_entry != NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
						 "%s: invalid directory - volume label entry value already set.",
						 function );

						goto on_error;
					}
					directory->volume_label_entry = directory_entry;
				}
				else
				{
					if( libcdata_array_append_entry(
					     directory->file_entries_array,
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
					last_vfat_sequence_number = vfat_sequence_number;
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
			}
			directory_entry = NULL;
		}
		file_offset += sizeof( fsfat_directory_entry_t );
	}
	while( result != 0 );

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
	return( -1 );
}

