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

#if !defined( _LIBFSFAT_FILE_SYSTEM_H )
#define _LIBFSFAT_FILE_SYSTEM_H

#include <common.h>
#include <types.h>

#include "libfsfat_allocation_table.h"
#include "libfsfat_block_tree.h"
#include "libfsfat_directory.h"
#include "libfsfat_io_handle.h"
#include "libfsfat_libbfio.h"
#include "libfsfat_libcdata.h"
#include "libfsfat_libcerror.h"
#include "libfsfat_libcthreads.h"
#include "libfsfat_libfcache.h"
#include "libfsfat_libfdata.h"
#include "libfsfat_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsfat_file_system libfsfat_file_system_t;

struct libfsfat_file_system
{
	/* The IO handle
	 */
	libfsfat_io_handle_t *io_handle;

	/* The allocation table
	 */
	libfsfat_allocation_table_t *allocation_table;

	/* The reversed allocation table
	 */
	libfsfat_allocation_table_t *reversed_allocation_table;

	/* The cache timestamp
	 */
	int64_t cache_timestamp;

	/* The directory cache
	 */
	libfcache_cache_t *directory_cache;

	/* The root directory
	 */
	libfsfat_directory_t *root_directory;

#if defined( HAVE_LIBFSFAT_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

int libfsfat_file_system_initialize(
     libfsfat_file_system_t **file_system,
     libfsfat_io_handle_t *io_handle,
     libcerror_error_t **error );

int libfsfat_file_system_free(
     libfsfat_file_system_t **file_system,
     libcerror_error_t **error );

int libfsfat_file_system_check_if_cluster_block_first_read(
     libfsfat_file_system_t *file_system,
     libfsfat_block_tree_t *cluster_block_tree,
     uint32_t cluster_number,
     off64_t cluster_offset,
     libcerror_error_t **error );

int libfsfat_file_system_read_allocation_table(
     libfsfat_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     size64_t size,
     libcerror_error_t **error );

int libfsfat_file_system_read_directory(
     libfsfat_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     uint32_t cluster_number,
     libfsfat_directory_t **directory,
     libcerror_error_t **error );

int libfsfat_file_system_read_directory_by_range(
     libfsfat_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     size64_t size,
     libfsfat_directory_t **directory,
     libcerror_error_t **error );

int libfsfat_file_system_read_directory_entry_by_identifier(
     libfsfat_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     uint64_t identifier,
     libfsfat_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfsfat_file_system_read_root_directory(
     libfsfat_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     size64_t size,
     uint32_t cluster_number,
     libcerror_error_t **error );

int libfsfat_file_system_get_data_stream(
     libfsfat_file_system_t *file_system,
     uint32_t cluster_number,
     size64_t size,
     libcdata_array_t *data_extents_array,
     libfdata_stream_t **data_stream,
     libcerror_error_t **error );

int libfsfat_file_system_get_directory(
     libfsfat_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     uint32_t cluster_number,
     libfsfat_directory_t **directory,
     libcerror_error_t **error );

int libfsfat_file_system_get_root_directory(
     libfsfat_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     libfsfat_file_entry_t **file_entry,
     libcerror_error_t **error );

int libfsfat_file_system_get_file_entry_by_identifier(
     libfsfat_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     uint64_t identifier,
     libfsfat_file_entry_t **file_entry,
     libcerror_error_t **error );

int libfsfat_file_system_get_file_entry_by_utf8_path(
     libfsfat_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsfat_file_entry_t **file_entry,
     libcerror_error_t **error );

int libfsfat_file_system_get_file_entry_by_utf16_path(
     libfsfat_file_system_t *file_system,
     libbfio_handle_t *file_io_handle,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsfat_file_entry_t **file_entry,
     libcerror_error_t **error );

int libfsfat_file_system_get_utf8_volume_label_size(
     libfsfat_file_system_t *file_system,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libfsfat_file_system_get_utf8_volume_label(
     libfsfat_file_system_t *file_system,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libfsfat_file_system_get_utf16_volume_label_size(
     libfsfat_file_system_t *file_system,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libfsfat_file_system_get_utf16_volume_label(
     libfsfat_file_system_t *file_system,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSFAT_FILE_SYSTEM_H ) */

