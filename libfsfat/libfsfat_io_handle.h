/*
 * Input/Output (IO) handle functions
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
	/* The format version
	 */
	int format_version;

	/* The block size
	 */
	uint32_t block_size;

	/* Inode size
	 */
	uint16_t inode_size;

	/* Group descriptor size
	 */
	uint16_t group_descriptor_size;

	/* The format revision
	 */
	uint32_t format_revision;

	/* The compatible features flags
	 */
	uint32_t compatible_features_flags;

	/* The incompatible features flags
	 */
	uint32_t incompatible_features_flags;

	/* The read-only compatible features flags
	 */
	uint32_t read_only_compatible_features_flags;

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

