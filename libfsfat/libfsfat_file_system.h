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
#include "libfsfat_directory.h"
#include "libfsfat_io_handle.h"
#include "libfsfat_libbfio.h"
#include "libfsfat_libcerror.h"
#include "libfsfat_libcthreads.h"
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

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSFAT_FILE_SYSTEM_H ) */

