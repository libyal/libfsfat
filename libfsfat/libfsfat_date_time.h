/*
 * Date and time functions
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

#if !defined( _LIBFSFAT_DATE_TIME_H )
#define _LIBFSFAT_DATE_TIME_H

#include <common.h>
#include <types.h>

#include "libfsfat_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libfsfat_date_time_get_timestamp(
     uint16_t fat_date,
     uint16_t fat_time,
     uint16_t fat_time_fraction,
     uint16_t fat_time_utc_offset,
     uint64_t *fat_timestamp,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSFAT_DATE_TIME_H ) */

