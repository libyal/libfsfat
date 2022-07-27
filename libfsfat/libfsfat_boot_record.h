/*
 * The boot record functions
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

#if !defined( _LIBFSFAT_BOOT_RECORD_H )
#define _LIBFSFAT_BOOT_RECORD_H

#include <common.h>
#include <types.h>

#include "libfsfat_libbfio.h"
#include "libfsfat_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsfat_boot_record libfsfat_boot_record_t;

struct libfsfat_boot_record
{
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

	/* The volume serial number
	 */
	uint32_t volume_serial_number;

	/* The allocation table offset
	 */
	off64_t allocation_table_offset;

	/* The allocation table size
	 */
	size64_t allocation_table_size;

	/* The root directory cluster
	 */
	uint32_t root_directory_cluster;

	/* The root directory offset
	 */
	off64_t root_directory_offset;

	/* The root directory size
	 */
	size64_t root_directory_size;

	/* The first cluster offset
	 */
	off64_t first_cluster_offset;
};

int libfsfat_boot_record_initialize(
     libfsfat_boot_record_t **boot_record,
     libcerror_error_t **error );

int libfsfat_boot_record_free(
     libfsfat_boot_record_t **boot_record,
     libcerror_error_t **error );

int libfsfat_boot_record_read_data(
     libfsfat_boot_record_t *boot_record,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsfat_boot_record_read_file_io_handle(
     libfsfat_boot_record_t *boot_record,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSFAT_BOOT_RECORD_H ) */

