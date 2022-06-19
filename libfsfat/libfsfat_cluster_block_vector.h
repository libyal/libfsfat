/*
 * Cluster block vector functions
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

#if !defined( _LIBFSFAT_CLUSTER_BLOCK_VECTOR_H )
#define _LIBFSFAT_CLUSTER_BLOCK_VECTOR_H

#include <common.h>
#include <types.h>

#include "libfsfat_allocation_table.h"
#include "libfsfat_io_handle.h"
#include "libfsfat_libbfio.h"
#include "libfsfat_libcerror.h"
#include "libfsfat_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libfsfat_cluster_block_vector_initialize(
     libfdata_vector_t **cluster_block_vector,
     libfsfat_io_handle_t *io_handle,
     libfsfat_allocation_table_t *allocation_table,
     libcerror_error_t **error );

int libfsfat_cluster_block_vector_read_element_data(
     intptr_t *data_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfdata_cache_t *cache,
     int element_index,
     int element_data_file_index,
     off64_t cluster_block_offset,
     size64_t cluster_block_size,
     uint32_t range_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSFAT_CLUSTER_BLOCK_VECTOR_H ) */

