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

#if !defined( _LIBFSFAT_FILE_ENTRY_H )
#define _LIBFSFAT_FILE_ENTRY_H

#include <common.h>
#include <types.h>

#include "libfsfat_directory.h"
#include "libfsfat_directory_entry.h"
#include "libfsfat_extern.h"
#include "libfsfat_file_system.h"
#include "libfsfat_io_handle.h"
#include "libfsfat_libbfio.h"
#include "libfsfat_libcdata.h"
#include "libfsfat_libcerror.h"
#include "libfsfat_libcthreads.h"
#include "libfsfat_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsfat_internal_file_entry libfsfat_internal_file_entry_t;

struct libfsfat_internal_file_entry
{
	/* The IO handle
	 */
	libfsfat_io_handle_t *io_handle;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* The file system
	 */
	libfsfat_file_system_t *file_system;

	/* The directory entry
	 */
	libfsfat_directory_entry_t *directory_entry;

	/* The identifier
	 */
	uint64_t identifier;

	/* The file attribute flags
	 */
	uint16_t file_attribute_flags;

	/* The cluster number
	 */
	uint32_t cluster_number;

	/* Data extents array
	 */
	libcdata_array_t *data_extents_array;

	/* The cluster block stream
	 */
	libfdata_stream_t *cluster_block_stream;

	/* The data size
	 */
	size64_t data_size;

	/* The directory
	 */
	libfsfat_directory_t *directory;

	/* The flags
	 */
	uint8_t flags;

#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

int libfsfat_file_entry_initialize(
     libfsfat_file_entry_t **file_entry,
     libfsfat_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsfat_file_system_t *file_system,
     uint64_t identifier,
     libfsfat_directory_entry_t *directory_entry,
     libfsfat_directory_t *directory,
     libcerror_error_t **error );

LIBFSFAT_EXTERN \
int libfsfat_file_entry_free(
     libfsfat_file_entry_t **file_entry,
     libcerror_error_t **error );

int libfsfat_internal_file_entry_get_directory(
     libfsfat_internal_file_entry_t *internal_file_entry,
     libcerror_error_t **error );

LIBFSFAT_EXTERN \
int libfsfat_file_entry_get_identifier(
     libfsfat_file_entry_t *file_entry,
     uint64_t *identifier,
     libcerror_error_t **error );

LIBFSFAT_EXTERN \
int libfsfat_file_entry_get_access_time(
     libfsfat_file_entry_t *file_entry,
     uint64_t *fat_timestamp,
     libcerror_error_t **error );

LIBFSFAT_EXTERN \
int libfsfat_file_entry_get_creation_time(
     libfsfat_file_entry_t *file_entry,
     uint64_t *fat_timestamp,
     libcerror_error_t **error );

LIBFSFAT_EXTERN \
int libfsfat_file_entry_get_modification_time(
     libfsfat_file_entry_t *file_entry,
     uint64_t *fat_timestamp,
     libcerror_error_t **error );

LIBFSFAT_EXTERN \
int libfsfat_file_entry_get_file_attribute_flags(
     libfsfat_file_entry_t *file_entry,
     uint16_t *file_attribute_flags,
     libcerror_error_t **error );

LIBFSFAT_EXTERN \
int libfsfat_file_entry_get_utf8_name_size(
     libfsfat_file_entry_t *file_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBFSFAT_EXTERN \
int libfsfat_file_entry_get_utf8_name(
     libfsfat_file_entry_t *file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBFSFAT_EXTERN \
int libfsfat_file_entry_get_utf16_name_size(
     libfsfat_file_entry_t *file_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBFSFAT_EXTERN \
int libfsfat_file_entry_get_utf16_name(
     libfsfat_file_entry_t *file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBFSFAT_EXTERN \
int libfsfat_file_entry_get_number_of_sub_file_entries(
     libfsfat_file_entry_t *file_entry,
     int *number_of_sub_entries,
     libcerror_error_t **error );

int libfsfat_internal_file_entry_get_sub_file_entry_by_index(
     libfsfat_internal_file_entry_t *internal_file_entry,
     int sub_file_entry_index,
     libfsfat_file_entry_t **sub_file_entry,
     libcerror_error_t **error );

LIBFSFAT_EXTERN \
int libfsfat_file_entry_get_sub_file_entry_by_index(
     libfsfat_file_entry_t *file_entry,
     int sub_file_entry_index,
     libfsfat_file_entry_t **sub_file_entry,
     libcerror_error_t **error );

int libfsfat_internal_file_entry_get_sub_file_entry_by_utf8_name(
     libfsfat_internal_file_entry_t *internal_file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsfat_file_entry_t **sub_file_entry,
     libcerror_error_t **error );

LIBFSFAT_EXTERN \
int libfsfat_file_entry_get_sub_file_entry_by_utf8_name(
     libfsfat_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsfat_file_entry_t **sub_file_entry,
     libcerror_error_t **error );

LIBFSFAT_EXTERN \
int libfsfat_file_entry_get_sub_file_entry_by_utf16_name(
     libfsfat_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsfat_file_entry_t **sub_file_entry,
     libcerror_error_t **error );

LIBFSFAT_EXTERN \
ssize_t libfsfat_file_entry_read_buffer(
         libfsfat_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBFSFAT_EXTERN \
ssize_t libfsfat_file_entry_read_buffer_at_offset(
         libfsfat_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error );

LIBFSFAT_EXTERN \
off64_t libfsfat_file_entry_seek_offset(
         libfsfat_file_entry_t *file_entry,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

LIBFSFAT_EXTERN \
int libfsfat_file_entry_get_offset(
     libfsfat_file_entry_t *file_entry,
     off64_t *offset,
     libcerror_error_t **error );

LIBFSFAT_EXTERN \
int libfsfat_file_entry_get_size(
     libfsfat_file_entry_t *file_entry,
     size64_t *size,
     libcerror_error_t **error );

LIBFSFAT_EXTERN \
int libfsfat_file_entry_get_number_of_extents(
     libfsfat_file_entry_t *file_entry,
     int *number_of_extents,
     libcerror_error_t **error );

LIBFSFAT_EXTERN \
int libfsfat_file_entry_get_extent_by_index(
     libfsfat_file_entry_t *file_entry,
     int extent_index,
     off64_t *extent_offset,
     size64_t *extent_size,
     uint32_t *extent_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSFAT_FILE_ENTRY_H ) */

