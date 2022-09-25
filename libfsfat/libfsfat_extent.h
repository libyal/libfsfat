/*
 * Extent functions
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

#if !defined( _LIBFSFAT_EXTENT_H )
#define _LIBFSFAT_EXTENT_H

#include <common.h>
#include <types.h>

#include "libfsfat_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsfat_extent libfsfat_extent_t;

struct libfsfat_extent
{
	/* Offset
	 */
	off64_t offset;

	/* Size
	 */
	size64_t size;
};

int libfsfat_extent_initialize(
     libfsfat_extent_t **extent,
     libcerror_error_t **error );

int libfsfat_extent_free(
     libfsfat_extent_t **extent,
     libcerror_error_t **error );

int libfsfat_extent_get_values(
     libfsfat_extent_t *extent,
     off64_t *extent_offset,
     size64_t *extent_size,
     uint32_t *extent_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSFAT_EXTENT_H ) */

