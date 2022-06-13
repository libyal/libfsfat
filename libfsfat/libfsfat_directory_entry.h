/*
 * The directory entry functions
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

#if !defined( _LIBFSFAT_DIRECTORY_ENTRY_H )
#define _LIBFSFAT_DIRECTORY_ENTRY_H

#include <common.h>
#include <types.h>

#include "libfsfat_libbfio.h"
#include "libfsfat_libcdata.h"
#include "libfsfat_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsfat_directory_entry libfsfat_directory_entry_t;

struct libfsfat_directory_entry
{
	/* The entry type
	 */
	int entry_type;

	/* The name and extension or name segments data
	 */
	uint8_t name_data[ 10 + 12 + 4 ];

	/* The name
	 */
	uint8_t *name;

	/* The name size
	 */
	size_t name_size;

	/* The VFAT sequence number
	 */
	uint8_t vfat_sequence_number;

	/* The file attribute flags
	 */
	uint8_t file_attribute_flags;

	/* The creation date and time
	 */
	uint8_t creation_time[ 4 ];

	/* The access date and time
	 */
	uint8_t access_time[ 4 ];

	/* The modification date and time
	 */
	uint8_t modification_time[ 4 ];

	/* The (VFAT) long file name entries
	 */
	libcdata_array_t *long_file_name_entries_array;
};

int libfsfat_directory_entry_initialize(
     libfsfat_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfsfat_directory_entry_free(
     libfsfat_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfsfat_directory_entry_read_data(
     libfsfat_directory_entry_t *directory_entry,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsfat_directory_entry_read_file_io_handle(
     libfsfat_directory_entry_t *directory_entry,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

int libfsfat_directory_entry_get_access_time(
     libfsfat_directory_entry_t *directory_entry,
     uint32_t *fat_date_time,
     libcerror_error_t **error );

int libfsfat_directory_entry_get_creation_time(
     libfsfat_directory_entry_t *directory_entry,
     uint32_t *fat_date_time,
     libcerror_error_t **error );

int libfsfat_directory_entry_get_modification_time(
     libfsfat_directory_entry_t *directory_entry,
     uint32_t *fat_date_time,
     libcerror_error_t **error );

int libfsfat_directory_entry_get_file_attribute_flags(
     libfsfat_directory_entry_t *directory_entry,
     uint8_t *file_attribute_flags,
     libcerror_error_t **error );

int libfsfat_directory_entry_get_name(
     libfsfat_directory_entry_t *directory_entry,
     libcerror_error_t **error );

int libfsfat_directory_entry_get_utf8_name_size(
     libfsfat_directory_entry_t *directory_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libfsfat_directory_entry_get_utf8_name(
     libfsfat_directory_entry_t *directory_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libfsfat_directory_entry_get_utf16_name_size(
     libfsfat_directory_entry_t *directory_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libfsfat_directory_entry_get_utf16_name(
     libfsfat_directory_entry_t *directory_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSFAT_DIRECTORY_ENTRY_H ) */

