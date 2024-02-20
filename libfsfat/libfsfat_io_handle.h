/*
 * Input/Output (IO) handle functions
 *
 * Copyright (C) 2021-2024, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBFSFAT_IO_HANDLE_H )
#define _LIBFSFAT_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include "libfsfat_libbfio.h"
#include "libfsfat_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsfat_io_handle libfsfat_io_handle_t;

struct libfsfat_io_handle
{
	/* The volume size
	 */
	size64_t volume_size;

	/* The file system format
	 */
	uint8_t file_system_format;

	/* The bytes per sector
	 */
	uint16_t bytes_per_sector;

	/* The cluster block size
	 */
	size_t cluster_block_size;

	/* The total number of clusters
	 */
	uint32_t total_number_of_clusters;

	/* The root directory offset
	 */
	off64_t root_directory_offset;

	/* The first cluster offset
	 */
	off64_t first_cluster_offset;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int libfsfat_io_handle_initialize(
     libfsfat_io_handle_t **io_handle,
     libcerror_error_t **error );

int libfsfat_io_handle_free(
     libfsfat_io_handle_t **io_handle,
     libcerror_error_t **error );

int libfsfat_io_handle_clear(
     libfsfat_io_handle_t *io_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSFAT_IO_HANDLE_H ) */

