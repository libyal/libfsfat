/*
 * The directory functions
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

#if !defined( _LIBFSFAT_DIRECTORY_H )
#define _LIBFSFAT_DIRECTORY_H

#include <common.h>
#include <types.h>

#include "libfsfat_directory_entry.h"
#include "libfsfat_libcdata.h"
#include "libfsfat_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsfat_directory libfsfat_directory_t;

struct libfsfat_directory
{
	/* The (directory) entries array
	 */
	libcdata_array_t *entries_array;

	/* The file entries array
	 */
	libcdata_array_t *file_entries_array;

	/* The self (directory) entry
	 */
	libfsfat_directory_entry_t *self_entry;

	/* The parent (directory) entry
	 */
	libfsfat_directory_entry_t *parent_entry;

	/* The volume label (directory) entry
	 */
	libfsfat_directory_entry_t *volume_label_entry;
};

int libfsfat_directory_initialize(
     libfsfat_directory_t **directory,
     libcerror_error_t **error );

int libfsfat_directory_free(
     libfsfat_directory_t **directory,
     libcerror_error_t **error );

int libfsfat_directory_get_number_of_file_entries(
     libfsfat_directory_t *directory,
     int *number_of_file_entries,
     libcerror_error_t **error );

int libfsfat_directory_get_file_entry_by_index(
     libfsfat_directory_t *directory,
     int file_entry_index,
     libfsfat_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfsfat_directory_get_file_entry_by_utf8_name(
     libfsfat_directory_t *directory,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsfat_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfsfat_directory_get_file_entry_by_utf16_name(
     libfsfat_directory_t *directory,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsfat_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfsfat_directory_get_utf8_volume_label_size(
     libfsfat_directory_t *directory,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libfsfat_directory_get_utf8_volume_label(
     libfsfat_directory_t *directory,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libfsfat_directory_get_utf16_volume_label_size(
     libfsfat_directory_t *directory,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libfsfat_directory_get_utf16_volume_label(
     libfsfat_directory_t *directory,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSFAT_DIRECTORY_H ) */

