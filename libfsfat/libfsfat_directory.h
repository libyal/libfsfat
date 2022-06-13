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
#include "libfsfat_libbfio.h"
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

int libfsfat_directory_read_file_io_handle(
     libfsfat_directory_t *directory,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSFAT_DIRECTORY_H ) */

