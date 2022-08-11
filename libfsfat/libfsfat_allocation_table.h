/*
 * Allocation table functions
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

#if !defined( _LIBFSFAT_ALLOCATION_TABLE_H )
#define _LIBFSFAT_ALLOCATION_TABLE_H

#include <common.h>
#include <types.h>

#include "libfsfat_io_handle.h"
#include "libfsfat_libbfio.h"
#include "libfsfat_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsfat_allocation_table libfsfat_allocation_table_t;

struct libfsfat_allocation_table
{
	/* The number of cluster numbers
	 */
	int number_of_cluster_numbers;

	/* The cluster numbers
	 */
	uint32_t *cluster_numbers;
};

int libfsfat_allocation_table_initialize(
     libfsfat_allocation_table_t **allocation_table,
     uint32_t total_number_of_clusters,
     libcerror_error_t **error );

int libfsfat_allocation_table_free(
     libfsfat_allocation_table_t **allocation_table,
     libcerror_error_t **error );

int libfsfat_allocation_table_read_file_io_handle(
     libfsfat_allocation_table_t *allocation_table,
     libfsfat_allocation_table_t *reversed_allocation_table,
     libfsfat_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     size64_t size,
     libcerror_error_t **error );

int libfsfat_allocation_table_get_cluster_number_by_index(
     libfsfat_allocation_table_t *allocation_table,
     int entry_index,
     uint32_t *cluster_number,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSFAT_ALLOCATION_TABLE_H ) */

