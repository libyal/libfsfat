/*
 * Debug functions
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

#if !defined( _LIBFSFAT_DEBUG_H )
#define _LIBFSFAT_DEBUG_H

#include <common.h>
#include <types.h>

#include "libfsfat_libbfio.h"
#include "libfsfat_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_DEBUG_OUTPUT )

const char *libfsfat_debug_print_exfat_cluster_type(
             uint32_t cluster_type );

const char *libfsfat_debug_print_exfat_directory_entry_type(
             uint8_t entry_type );

const char *libfsfat_debug_print_fat12_cluster_type(
             uint16_t cluster_type );

const char *libfsfat_debug_print_fat16_cluster_type(
             uint16_t cluster_type );

const char *libfsfat_debug_print_fat32_cluster_type(
             uint32_t cluster_type );

void libfsfat_debug_print_file_attribute_flags(
      uint16_t file_attribute_flags );

int libfsfat_debug_print_fat_date_time_value(
     const char *function_name,
     const char *value_name,
     uint16_t fat_date,
     uint16_t fat_time,
     uint8_t fat_time_fraction,
     uint8_t fat_time_utc_offset,
     uint32_t string_format_flags,
     libcerror_error_t **error );

int libfsfat_debug_print_read_offsets(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

#endif /* defined( HAVE_DEBUG_OUTPUT ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSFAT_DEBUG_H ) */

