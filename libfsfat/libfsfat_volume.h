/*
 * Volume functions
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

#if !defined( _LIBFSFAT_INTERNAL_VOLUME_H )
#define _LIBFSFAT_INTERNAL_VOLUME_H

#include <common.h>
#include <types.h>

#include "libfsfat_extern.h"
#include "libfsfat_file_system.h"
#include "libfsfat_io_handle.h"
#include "libfsfat_libbfio.h"
#include "libfsfat_libcerror.h"
#include "libfsfat_libcthreads.h"
#include "libfsfat_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsfat_internal_volume libfsfat_internal_volume_t;

struct libfsfat_internal_volume
{
	/* The IO handle
	 */
	libfsfat_io_handle_t *io_handle;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* Value to indicate if the file IO handle was created inside the library
	 */
	uint8_t file_io_handle_created_in_library;

	/* Value to indicate if the file IO handle was opened inside the library
	 */
	uint8_t file_io_handle_opened_in_library;

	/* The file system
	 */
	libfsfat_file_system_t *file_system;

#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

LIBFSFAT_EXTERN \
int libfsfat_volume_initialize(
     libfsfat_volume_t **volume,
     libcerror_error_t **error );

LIBFSFAT_EXTERN \
int libfsfat_volume_free(
     libfsfat_volume_t **volume,
     libcerror_error_t **error );

LIBFSFAT_EXTERN \
int libfsfat_volume_signal_abort(
     libfsfat_volume_t *volume,
     libcerror_error_t **error );

LIBFSFAT_EXTERN \
int libfsfat_volume_open(
     libfsfat_volume_t *volume,
     const char *filename,
     int access_flags,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

LIBFSFAT_EXTERN \
int libfsfat_volume_open_wide(
     libfsfat_volume_t *volume,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

LIBFSFAT_EXTERN \
int libfsfat_volume_open_file_io_handle(
     libfsfat_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error );

LIBFSFAT_EXTERN \
int libfsfat_volume_close(
     libfsfat_volume_t *volume,
     libcerror_error_t **error );

int libfsfat_internal_volume_open_read(
     libfsfat_internal_volume_t *internal_volume,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

LIBFSFAT_EXTERN \
int libfsfat_volume_get_file_system_format(
     libfsfat_volume_t *volume,
     uint8_t *file_system_format,
     libcerror_error_t **error );

LIBFSFAT_EXTERN \
int libfsfat_volume_get_utf8_label_size(
     libfsfat_volume_t *volume,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBFSFAT_EXTERN \
int libfsfat_volume_get_utf8_label(
     libfsfat_volume_t *volume,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBFSFAT_EXTERN \
int libfsfat_volume_get_utf16_label_size(
     libfsfat_volume_t *volume,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBFSFAT_EXTERN \
int libfsfat_volume_get_utf16_label(
     libfsfat_volume_t *volume,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBFSFAT_EXTERN \
int libfsfat_volume_get_root_directory(
     libfsfat_volume_t *volume,
     libfsfat_file_entry_t **file_entry,
     libcerror_error_t **error );

LIBFSFAT_EXTERN \
int libfsfat_volume_get_file_entry_by_identifier(
     libfsfat_volume_t *volume,
     uint64_t identifier,
     libfsfat_file_entry_t **file_entry,
     libcerror_error_t **error );

LIBFSFAT_EXTERN \
int libfsfat_volume_get_file_entry_by_utf8_path(
     libfsfat_volume_t *volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsfat_file_entry_t **file_entry,
     libcerror_error_t **error );

LIBFSFAT_EXTERN \
int libfsfat_volume_get_file_entry_by_utf16_path(
     libfsfat_volume_t *volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsfat_file_entry_t **file_entry,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSFAT_INTERNAL_VOLUME_H ) */

