/*
 * File entry functions
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
#include "libfsfat_extent.h"
#include "libfsfat_file_entry.h"
#include "libfsfat_file_system.h"
#include "libfsfat_io_handle.h"
#include "libfsfat_libbfio.h"
#include "libfsfat_libcdata.h"
#include "libfsfat_libcerror.h"
#include "libfsfat_libcthreads.h"
#include "libfsfat_libfdata.h"
#include "libfsfat_types.h"

/* Creates a file entry
 * Make sure the value file_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsfat_file_entry_initialize(
     libfsfat_file_entry_t **file_entry,
     libfsfat_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsfat_file_system_t *file_system,
     uint64_t identifier,
     libfsfat_directory_entry_t *directory_entry,
     libfsfat_directory_t *directory,
     libcerror_error_t **error )
{
	libfsfat_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsfat_file_entry_initialize";
	size64_t data_size                                  = 0;
	size64_t valid_data_size                            = 0;
	uint32_t cluster_number                             = 0;
	uint16_t file_attribute_flags                       = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( *file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file entry value already set.",
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
	if( directory_entry != NULL )
	{
		if( libfsfat_directory_entry_get_file_attribute_flags(
		     directory_entry,
		     &file_attribute_flags,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file attribute flags from directory entry.",
			 function );

			goto on_error;
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
			 "%s: unable to retrieve directory entry data start cluster.",
			 function );

			goto on_error;
		}
		if( libfsfat_directory_entry_get_data_size(
		     directory_entry,
		     &data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data size from directory entry.",
			 function );

			goto on_error;
		}
		if( libfsfat_directory_entry_get_valid_data_size(
		     directory_entry,
		     &valid_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data size from directory entry.",
			 function );

			goto on_error;
		}
		if( valid_data_size > 0 )
		{
			data_size = valid_data_size;
		}
	}
	internal_file_entry = memory_allocate_structure(
	                       libfsfat_internal_file_entry_t );

	if( internal_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file entry.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_file_entry,
	     0,
	     sizeof( libfsfat_internal_file_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file entry.",
		 function );

		memory_free(
		 internal_file_entry );

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( internal_file_entry->data_extents_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data extents array.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_file_entry->read_write_lock ),
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
	internal_file_entry->io_handle            = io_handle;
	internal_file_entry->file_io_handle       = file_io_handle;
	internal_file_entry->file_system          = file_system;
	internal_file_entry->identifier           = identifier;
	internal_file_entry->directory_entry      = directory_entry;
	internal_file_entry->file_attribute_flags = file_attribute_flags;
	internal_file_entry->cluster_number       = cluster_number;
	internal_file_entry->data_size            = data_size;
	internal_file_entry->directory            = directory;

	*file_entry = (libfsfat_file_entry_t *) internal_file_entry;

	return( 1 );

on_error:
	if( internal_file_entry != NULL )
	{
		memory_free(
		 internal_file_entry );
	}
	return( -1 );
}

/* Frees a file entry
 * Returns 1 if successful or -1 on error
 */
int libfsfat_file_entry_free(
     libfsfat_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	libfsfat_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsfat_file_entry_free";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( *file_entry != NULL )
	{
		internal_file_entry = (libfsfat_internal_file_entry_t *) *file_entry;
		*file_entry         = NULL;

#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_file_entry->read_write_lock ),
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
		if( internal_file_entry->directory_entry != NULL )
		{
			if( libfsfat_directory_entry_free(
			     &( internal_file_entry->directory_entry ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free directory entry.",
				 function );

				result = -1;
			}
		}
		if( ( internal_file_entry->directory != NULL )
		 && ( ( internal_file_entry->flags & LIBFSFAT_FILE_ENTRY_FLAG_MANAGE_DIRECTORY ) != 0 ) )
		{
			if( libfsfat_directory_free(
			     &( internal_file_entry->directory ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free directory.",
				 function );

				result = -1;
			}
		}
		if( internal_file_entry->cluster_block_stream != NULL )
		{
			if( libfdata_stream_free(
			     &( internal_file_entry->cluster_block_stream ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free cluster block stream.",
				 function );

				result = -1;
			}
		}
		if( libcdata_array_free(
		     &( internal_file_entry->data_extents_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsfat_extent_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free data extents array.",
			 function );

			result = -1;
		}
		/* The file_io_handle, io_handle and directory_entry references are freed elsewhere
		 */
		memory_free(
		 internal_file_entry );
	}
	return( result );
}

/* Determines the directory
 * Returns 1 if successful or -1 on error
 */
int libfsfat_internal_file_entry_get_directory(
     libfsfat_internal_file_entry_t *internal_file_entry,
     libcerror_error_t **error )
{
	static char *function   = "libfsfat_internal_file_entry_get_directory";
	uint32_t cluster_number = 0;

	if( internal_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( internal_file_entry->directory != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file entry - directory value already set.",
		 function );

		return( -1 );
	}
	if( ( internal_file_entry->file_attribute_flags & LIBFSFAT_FILE_ATTRIBUTE_FLAG_DIRECTORY ) != 0 )
	{
		if( libfsfat_directory_entry_get_data_start_cluster(
		     internal_file_entry->directory_entry,
		     &cluster_number,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve directory entry data start cluster.",
			 function );

			goto on_error;
		}
		if( libfsfat_file_system_read_directory(
		     internal_file_entry->file_system,
		     internal_file_entry->file_io_handle,
		     cluster_number,
		     &( internal_file_entry->directory ),
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
		internal_file_entry->flags |= LIBFSFAT_FILE_ENTRY_FLAG_MANAGE_DIRECTORY;
	}
	return( 1 );

on_error:
	if( internal_file_entry->directory != NULL )
	{
		libfsfat_directory_free(
		 &( internal_file_entry->directory ),
		 NULL );
	}
	return( -1 );
}

/* Retrieves the (virtual) identifier
 * The identifier is the offset of the short name directory entry
 * Returns 1 if successful or -1 on error
 */
int libfsfat_file_entry_get_identifier(
     libfsfat_file_entry_t *file_entry,
     uint64_t *identifier,
     libcerror_error_t **error )
{
	libfsfat_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsfat_file_entry_get_identifier";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsfat_internal_file_entry_t *) file_entry;

	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*identifier = internal_file_entry->identifier;

#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Retrieves the access date and time
 * The timestamp is an unsigned 64-bit integer containing the 10 milli seconds intervals since January 1, 1980
 * This value is retrieved from the short file name directory entry
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsfat_file_entry_get_access_time(
     libfsfat_file_entry_t *file_entry,
     uint64_t *fat_timestamp,
     libcerror_error_t **error )
{
	libfsfat_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsfat_file_entry_get_access_time";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsfat_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->directory_entry != NULL )
	{
		result = libfsfat_directory_entry_get_access_time(
		          internal_file_entry->directory_entry,
		          fat_timestamp,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve access time from directory entry.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the creation date and time
 * The timestamp is an unsigned 64-bit integer containing the 10 milli seconds intervals since January 1, 1980
 * This value is retrieved from the short file name directory entry
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsfat_file_entry_get_creation_time(
     libfsfat_file_entry_t *file_entry,
     uint64_t *fat_date_time,
     libcerror_error_t **error )
{
	libfsfat_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsfat_file_entry_get_creation_time";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsfat_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->directory_entry != NULL )
	{
		result = libfsfat_directory_entry_get_creation_time(
		          internal_file_entry->directory_entry,
		          fat_date_time,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve creation time from directory entry.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the modification date and time
 * The timestamp is an unsigned 64-bit integer containing the 10 milli seconds intervals since January 1, 1980
 * This value is retrieved from the short file name directory entry
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsfat_file_entry_get_modification_time(
     libfsfat_file_entry_t *file_entry,
     uint64_t *fat_date_time,
     libcerror_error_t **error )
{
	libfsfat_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsfat_file_entry_get_modification_time";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsfat_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->directory_entry != NULL )
	{
		result = libfsfat_directory_entry_get_modification_time(
		          internal_file_entry->directory_entry,
		          fat_date_time,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve modification time from directory entry.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the file attribute flags
 * This value is retrieved from the short file name directory entry
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsfat_file_entry_get_file_attribute_flags(
     libfsfat_file_entry_t *file_entry,
     uint16_t *file_attribute_flags,
     libcerror_error_t **error )
{
	libfsfat_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsfat_file_entry_get_file_attribute_flags";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsfat_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->directory_entry != NULL )
	{
		result = libfsfat_directory_entry_get_file_attribute_flags(
		          internal_file_entry->directory_entry,
		          file_attribute_flags,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve file attribute flags from directory entry.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * This value is retrieved from the directory entry
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsfat_file_entry_get_utf8_name_size(
     libfsfat_file_entry_t *file_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libfsfat_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsfat_file_entry_get_utf8_name_size";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsfat_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->directory_entry != NULL )
	{
		result = libfsfat_directory_entry_get_utf8_name_size(
		          internal_file_entry->directory_entry,
		          utf8_string_size,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve size of UTF-8 name from directory entry.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * This value is retrieved from the directory entry
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsfat_file_entry_get_utf8_name(
     libfsfat_file_entry_t *file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libfsfat_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsfat_file_entry_get_utf8_name";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsfat_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->directory_entry != NULL )
	{
		result = libfsfat_directory_entry_get_utf8_name(
		          internal_file_entry->directory_entry,
		          utf8_string,
		          utf8_string_size,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-8 name from directory entry.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * This value is retrieved from the directory entry
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsfat_file_entry_get_utf16_name_size(
     libfsfat_file_entry_t *file_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libfsfat_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsfat_file_entry_get_utf16_name_size";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsfat_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->directory_entry != NULL )
	{
		result = libfsfat_directory_entry_get_utf16_name_size(
		          internal_file_entry->directory_entry,
		          utf16_string_size,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve size of UTF-16 name from directory entry.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * This value is retrieved from the directory entry
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsfat_file_entry_get_utf16_name(
     libfsfat_file_entry_t *file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libfsfat_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsfat_file_entry_get_utf16_name";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsfat_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->directory_entry != NULL )
	{
		result = libfsfat_directory_entry_get_utf16_name(
		          internal_file_entry->directory_entry,
		          utf16_string,
		          utf16_string_size,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 name from directory entry.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the number of sub file entries
 * Returns 1 if successful or -1 on error
 */
int libfsfat_file_entry_get_number_of_sub_file_entries(
     libfsfat_file_entry_t *file_entry,
     int *number_of_sub_file_entries,
     libcerror_error_t **error )
{
	libfsfat_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsfat_file_entry_get_number_of_sub_file_entries";
	int result                                          = 1;
	int safe_number_of_sub_file_entries                 = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsfat_internal_file_entry_t *) file_entry;

	if( number_of_sub_file_entries == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of sub file entries.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->directory == NULL )
	{
		if( libfsfat_internal_file_entry_get_directory(
		     internal_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine directory.",
			 function );

			result = -1;
		}
	}
	if( internal_file_entry->directory != NULL )
	{
		if( libfsfat_directory_get_number_of_file_entries(
		     internal_file_entry->directory,
		     &safe_number_of_sub_file_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of file entries from directory.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*number_of_sub_file_entries = safe_number_of_sub_file_entries;

	return( result );
}

/* Retrieves the sub file entry for the specific index
 * Returns 1 if successful or -1 on error
 */
int libfsfat_internal_file_entry_get_sub_file_entry_by_index(
     libfsfat_internal_file_entry_t *internal_file_entry,
     int sub_file_entry_index,
     libfsfat_file_entry_t **sub_file_entry,
     libcerror_error_t **error )
{
	libfsfat_directory_entry_t *safe_directory_entry = NULL;
	libfsfat_directory_entry_t *sub_directory_entry  = NULL;
	static char *function                            = "libfsfat_internal_file_entry_get_sub_file_entry_by_index";
	uint64_t identifier                              = 0;

	if( internal_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( sub_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub file entry.",
		 function );

		return( -1 );
	}
	if( *sub_file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid sub file entry value already set.",
		 function );

		return( -1 );
	}
	if( internal_file_entry->directory == NULL )
	{
		if( libfsfat_internal_file_entry_get_directory(
		     internal_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine directory.",
			 function );

			goto on_error;
		}
	}
	if( libfsfat_directory_get_file_entry_by_index(
	     internal_file_entry->directory,
	     sub_file_entry_index,
	     &sub_directory_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub directory entry: %d.",
		 function,
		 sub_file_entry_index );

		goto on_error;
	}
	if( libfsfat_directory_entry_get_identifier(
	     sub_directory_entry,
	     &identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve identifier from sub directory entry: %d.",
		 function,
		 sub_file_entry_index );

		goto on_error;
	}
	if( libfsfat_directory_entry_clone(
	     &safe_directory_entry,
	     sub_directory_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sub directory entry: %d.",
		 function,
		 sub_file_entry_index );

		goto on_error;
	}
	/* libfsfat_file_entry_initialize takes over management of safe_directory_entry
	 */
	if( libfsfat_file_entry_initialize(
	     sub_file_entry,
	     internal_file_entry->io_handle,
	     internal_file_entry->file_io_handle,
	     internal_file_entry->file_system,
	     identifier,
	     safe_directory_entry,
	     NULL,
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
	if( safe_directory_entry != NULL )
	{
		libfsfat_directory_entry_free(
		 &safe_directory_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the sub file entry for the specific index
 * Returns 1 if successful or -1 on error
 */
int libfsfat_file_entry_get_sub_file_entry_by_index(
     libfsfat_file_entry_t *file_entry,
     int sub_file_entry_index,
     libfsfat_file_entry_t **sub_file_entry,
     libcerror_error_t **error )
{
	libfsfat_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsfat_file_entry_get_sub_file_entry_by_index";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsfat_internal_file_entry_t *) file_entry;

	if( sub_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub file entry.",
		 function );

		return( -1 );
	}
	if( *sub_file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid sub file entry value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsfat_internal_file_entry_get_sub_file_entry_by_index(
	     internal_file_entry,
	     sub_file_entry_index,
	     sub_file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub file entry: %d.",
		 function,
		 sub_file_entry_index );

		result = -1;
	}
#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the sub file entry for an UTF-8 encoded name
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libfsfat_internal_file_entry_get_sub_file_entry_by_utf8_name(
     libfsfat_internal_file_entry_t *internal_file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsfat_file_entry_t **sub_file_entry,
     libcerror_error_t **error )
{
	libfsfat_directory_entry_t *safe_directory_entry = NULL;
	libfsfat_directory_entry_t *sub_directory_entry  = NULL;
	static char *function                            = "libfsfat_internal_file_entry_get_sub_file_entry_by_utf8_name";
	uint64_t identifier                              = 0;
	int result                                       = 0;

	if( internal_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( sub_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub file entry.",
		 function );

		return( -1 );
	}
	if( *sub_file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid sub file entry value already set.",
		 function );

		return( -1 );
	}
	if( internal_file_entry->directory == NULL )
	{
		if( libfsfat_internal_file_entry_get_directory(
		     internal_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine directory.",
			 function );

			goto on_error;
		}
	}
	result = libfsfat_directory_get_file_entry_by_utf8_name(
	          internal_file_entry->directory,
	          utf8_string,
	          utf8_string_length,
	          &sub_directory_entry,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub directory entry.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	if( libfsfat_directory_entry_get_identifier(
	     sub_directory_entry,
	     &identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve identifier from sub directory entry.",
		 function );

		goto on_error;
	}
	if( libfsfat_directory_entry_clone(
	     &safe_directory_entry,
	     sub_directory_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sub directory entry.",
		 function );

		goto on_error;
	}
	/* libfsfat_file_entry_initialize takes over management of safe_directory_entry
	 */
	if( libfsfat_file_entry_initialize(
	     sub_file_entry,
	     internal_file_entry->io_handle,
	     internal_file_entry->file_io_handle,
	     internal_file_entry->file_system,
	     identifier,
	     safe_directory_entry,
	     NULL,
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
	if( safe_directory_entry != NULL )
	{
		libfsfat_directory_entry_free(
		 &safe_directory_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the sub file entry for an UTF-8 encoded name
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libfsfat_file_entry_get_sub_file_entry_by_utf8_name(
     libfsfat_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsfat_file_entry_t **sub_file_entry,
     libcerror_error_t **error )
{
	libfsfat_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsfat_file_entry_get_sub_file_entry_by_utf8_name";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsfat_internal_file_entry_t *) file_entry;

	if( sub_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub file entry.",
		 function );

		return( -1 );
	}
	if( *sub_file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid sub file entry value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsfat_internal_file_entry_get_sub_file_entry_by_utf8_name(
	     internal_file_entry,
	     utf8_string,
	     utf8_string_length,
	     sub_file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub file entry.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the sub file entry for an UTF-16 encoded name
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libfsfat_internal_file_entry_get_sub_file_entry_by_utf16_name(
     libfsfat_internal_file_entry_t *internal_file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsfat_file_entry_t **sub_file_entry,
     libcerror_error_t **error )
{
	libfsfat_directory_entry_t *safe_directory_entry = NULL;
	libfsfat_directory_entry_t *sub_directory_entry  = NULL;
	static char *function                            = "libfsfat_internal_file_entry_get_sub_file_entry_by_utf16_name";
	uint64_t identifier                              = 0;
	int result                                       = 0;

	if( internal_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( sub_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub file entry.",
		 function );

		return( -1 );
	}
	if( *sub_file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid sub file entry value already set.",
		 function );

		return( -1 );
	}
	if( internal_file_entry->directory == NULL )
	{
		if( libfsfat_internal_file_entry_get_directory(
		     internal_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine directory.",
			 function );

			goto on_error;
		}
	}
	result = libfsfat_directory_get_file_entry_by_utf16_name(
	          internal_file_entry->directory,
	          utf16_string,
	          utf16_string_length,
	          &sub_directory_entry,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub directory entry.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	if( libfsfat_directory_entry_get_identifier(
	     sub_directory_entry,
	     &identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve identifier from sub directory entry.",
		 function );

		goto on_error;
	}
	if( libfsfat_directory_entry_clone(
	     &safe_directory_entry,
	     sub_directory_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sub directory entry.",
		 function );

		goto on_error;
	}
	/* libfsfat_file_entry_initialize takes over management of safe_directory_entry
	 */
	if( libfsfat_file_entry_initialize(
	     sub_file_entry,
	     internal_file_entry->io_handle,
	     internal_file_entry->file_io_handle,
	     internal_file_entry->file_system,
	     identifier,
	     safe_directory_entry,
	     NULL,
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
	if( safe_directory_entry != NULL )
	{
		libfsfat_directory_entry_free(
		 &safe_directory_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the sub file entry for an UTF-16 encoded name
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libfsfat_file_entry_get_sub_file_entry_by_utf16_name(
     libfsfat_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsfat_file_entry_t **sub_file_entry,
     libcerror_error_t **error )
{
	libfsfat_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsfat_file_entry_get_sub_file_entry_by_utf16_name";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsfat_internal_file_entry_t *) file_entry;

	if( sub_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub file entry.",
		 function );

		return( -1 );
	}
	if( *sub_file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid sub file entry value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsfat_internal_file_entry_get_sub_file_entry_by_utf16_name(
	     internal_file_entry,
	     utf16_string,
	     utf16_string_length,
	     sub_file_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub file entry.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Reads data at the current offset
 * Returns the number of bytes read or -1 on error
 */
ssize_t libfsfat_file_entry_read_buffer(
         libfsfat_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libfsfat_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsfat_file_entry_read_buffer";
	ssize_t read_count                                  = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsfat_internal_file_entry_t *) file_entry;

	if( ( ( internal_file_entry->file_attribute_flags & LIBFSFAT_FILE_ATTRIBUTE_FLAG_DEVICE ) != 0 )
	 && ( ( internal_file_entry->file_attribute_flags & LIBFSFAT_FILE_ATTRIBUTE_FLAG_DIRECTORY ) != 0 )
	 && ( ( internal_file_entry->file_attribute_flags & LIBFSFAT_FILE_ATTRIBUTE_FLAG_VOLUME_LABEL ) != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid file entry - unsupported file attribute flags not a regular file.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->cluster_block_stream == NULL )
	{
		if( libfsfat_file_system_get_data_stream(
		     internal_file_entry->file_system,
		     internal_file_entry->cluster_number,
		     internal_file_entry->data_size,
		     internal_file_entry->data_extents_array,
		     &( internal_file_entry->cluster_block_stream ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data stream: %" PRIu32 ".",
			 function,
			 internal_file_entry->cluster_number );

			read_count = -1;
		}
	}
	if( read_count != -1 )
	{
		read_count = libfdata_stream_read_buffer(
		              internal_file_entry->cluster_block_stream,
		              (intptr_t *) internal_file_entry->file_io_handle,
		              buffer,
		              buffer_size,
		              0,
		              error );

		if( read_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read from cluster block stream.",
			 function );

			read_count = -1;
		}
	}
#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( read_count );
}

/* Reads data at a specific offset
 * Returns the number of bytes read or -1 on error
 */
ssize_t libfsfat_file_entry_read_buffer_at_offset(
         libfsfat_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error )
{
	libfsfat_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsfat_file_entry_read_buffer_at_offset";
	ssize_t read_count                                  = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsfat_internal_file_entry_t *) file_entry;

	if( ( ( internal_file_entry->file_attribute_flags & LIBFSFAT_FILE_ATTRIBUTE_FLAG_DEVICE ) != 0 )
	 && ( ( internal_file_entry->file_attribute_flags & LIBFSFAT_FILE_ATTRIBUTE_FLAG_DIRECTORY ) != 0 )
	 && ( ( internal_file_entry->file_attribute_flags & LIBFSFAT_FILE_ATTRIBUTE_FLAG_VOLUME_LABEL ) != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid file entry - unsupported file attribute flags not a regular file.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->cluster_block_stream == NULL )
	{
		if( libfsfat_file_system_get_data_stream(
		     internal_file_entry->file_system,
		     internal_file_entry->cluster_number,
		     internal_file_entry->data_size,
		     internal_file_entry->data_extents_array,
		     &( internal_file_entry->cluster_block_stream ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data stream: %" PRIu32 ".",
			 function,
			 internal_file_entry->cluster_number );

			read_count = -1;
		}
	}
	if( read_count != -1 )
	{
		read_count = libfdata_stream_read_buffer_at_offset(
		              internal_file_entry->cluster_block_stream,
		              (intptr_t *) internal_file_entry->file_io_handle,
		              buffer,
		              buffer_size,
		              offset,
		              0,
		              error );

		if( read_count < 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read from cluster block stream.",
			 function );

			read_count = -1;
		}
	}
#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( read_count );
}

/* Seeks a certain offset in the data
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libfsfat_file_entry_seek_offset(
         libfsfat_file_entry_t *file_entry,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	libfsfat_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsfat_file_entry_seek_offset";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsfat_internal_file_entry_t *) file_entry;

	if( ( ( internal_file_entry->file_attribute_flags & LIBFSFAT_FILE_ATTRIBUTE_FLAG_DEVICE ) != 0 )
	 && ( ( internal_file_entry->file_attribute_flags & LIBFSFAT_FILE_ATTRIBUTE_FLAG_DIRECTORY ) != 0 )
	 && ( ( internal_file_entry->file_attribute_flags & LIBFSFAT_FILE_ATTRIBUTE_FLAG_VOLUME_LABEL ) != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid file entry - unsupported file attribute flags not a regular file.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->cluster_block_stream == NULL )
	{
		if( libfsfat_file_system_get_data_stream(
		     internal_file_entry->file_system,
		     internal_file_entry->cluster_number,
		     internal_file_entry->data_size,
		     internal_file_entry->data_extents_array,
		     &( internal_file_entry->cluster_block_stream ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data stream: %" PRIu32 ".",
			 function,
			 internal_file_entry->cluster_number );

			offset = -1;
		}
	}
	if( offset != -1 )
	{
		offset = libfdata_stream_seek_offset(
		          internal_file_entry->cluster_block_stream,
		          offset,
		          whence,
		          error );

		if( offset == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_SEEK_FAILED,
			 "%s: unable to seek offset in cluster block stream.",
			 function );

			offset = -1;
		}
	}
#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( offset );
}

/* Retrieves the current offset of the data
 * Returns the offset if successful or -1 on error
 */
int libfsfat_file_entry_get_offset(
     libfsfat_file_entry_t *file_entry,
     off64_t *offset,
     libcerror_error_t **error )
{
	libfsfat_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsfat_file_entry_get_offset";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsfat_internal_file_entry_t *) file_entry;

	if( ( ( internal_file_entry->file_attribute_flags & LIBFSFAT_FILE_ATTRIBUTE_FLAG_DEVICE ) != 0 )
	 && ( ( internal_file_entry->file_attribute_flags & LIBFSFAT_FILE_ATTRIBUTE_FLAG_DIRECTORY ) != 0 )
	 && ( ( internal_file_entry->file_attribute_flags & LIBFSFAT_FILE_ATTRIBUTE_FLAG_VOLUME_LABEL ) != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid file entry - unsupported file attribute flags not a regular file.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->cluster_block_stream == NULL )
	{
		if( libfsfat_file_system_get_data_stream(
		     internal_file_entry->file_system,
		     internal_file_entry->cluster_number,
		     internal_file_entry->data_size,
		     internal_file_entry->data_extents_array,
		     &( internal_file_entry->cluster_block_stream ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data stream: %" PRIu32 ".",
			 function,
			 internal_file_entry->cluster_number );

			result = -1;
		}
	}
	if( result != -1 )
	{
		if( libfdata_stream_get_offset(
		     internal_file_entry->cluster_block_stream,
		     offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset from cluster block stream.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the data
 * Returns 1 if successful or -1 on error
 */
int libfsfat_file_entry_get_size(
     libfsfat_file_entry_t *file_entry,
     size64_t *size,
     libcerror_error_t **error )
{
	libfsfat_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsfat_file_entry_get_size";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsfat_internal_file_entry_t *) file_entry;

	if( size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*size = internal_file_entry->data_size;

#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Retrieves the number of extents of the data
 * Returns 1 if successful or -1 on error
 */
int libfsfat_file_entry_get_number_of_extents(
     libfsfat_file_entry_t *file_entry,
     int *number_of_extents,
     libcerror_error_t **error )
{
	libfsfat_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsfat_file_entry_get_number_of_extents";
	int result                                           = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsfat_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->cluster_block_stream == NULL )
	{
		if( libfsfat_file_system_get_data_stream(
		     internal_file_entry->file_system,
		     internal_file_entry->cluster_number,
		     internal_file_entry->data_size,
		     internal_file_entry->data_extents_array,
		     &( internal_file_entry->cluster_block_stream ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data stream: %" PRIu32 ".",
			 function,
			 internal_file_entry->cluster_number );

			result = -1;
		}
	}
	if( result != -1 )
	{
		if( libcdata_array_get_number_of_entries(
		     internal_file_entry->data_extents_array,
		     number_of_extents,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of segments from cluster block stream.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves a specific extent of the data
 * Returns 1 if successful or -1 on error
 */
int libfsfat_file_entry_get_extent_by_index(
     libfsfat_file_entry_t *file_entry,
     int extent_index,
     off64_t *extent_offset,
     size64_t *extent_size,
     uint32_t *extent_flags,
     libcerror_error_t **error )
{
	libfsfat_extent_t *extent                           = NULL;
	libfsfat_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsfat_file_entry_get_extent_by_index";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsfat_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->cluster_block_stream == NULL )
	{
		if( libfsfat_file_system_get_data_stream(
		     internal_file_entry->file_system,
		     internal_file_entry->cluster_number,
		     internal_file_entry->data_size,
		     internal_file_entry->data_extents_array,
		     &( internal_file_entry->cluster_block_stream ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data stream: %" PRIu32 ".",
			 function,
			 internal_file_entry->cluster_number );

			result = -1;
		}
	}
	if( result != -1 )
	{
		if( libcdata_array_get_entry_by_index(
		     internal_file_entry->data_extents_array,
		     extent_index,
		     (intptr_t **) &extent,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve segment: %d from cluster block stream.",
			 function,
			 extent_index );

			result = -1;
		}
		if( result == 1 )
		{
			if( libfsfat_extent_get_values(
			     extent,
			     extent_offset,
			     extent_size,
			     extent_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve extent: %d values.",
				 function,
				 extent_index );

				result = -1;
			}
		}
	}
#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

