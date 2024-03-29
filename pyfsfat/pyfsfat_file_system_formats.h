/*
 * Python object definition of the libfsfat file system formats
 *
 * Copyright (C) 2021-2024, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _PYFSFAT_FILE_SYSTEM_FORMATS_H )
#define _PYFSFAT_FILE_SYSTEM_FORMATS_H

#include <common.h>
#include <types.h>

#include "pyfsfat_libfsfat.h"
#include "pyfsfat_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyfsfat_file_system_formats pyfsfat_file_system_formats_t;

struct pyfsfat_file_system_formats
{
	/* Python object initialization
	 */
	PyObject_HEAD
};

extern PyTypeObject pyfsfat_file_system_formats_type_object;

int pyfsfat_file_system_formats_init_type(
     PyTypeObject *type_object );

PyObject *pyfsfat_file_system_formats_new(
           void );

int pyfsfat_file_system_formats_init(
     pyfsfat_file_system_formats_t *definitions_object );

void pyfsfat_file_system_formats_free(
      pyfsfat_file_system_formats_t *definitions_object );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYFSFAT_FILE_SYSTEM_FORMATS_H ) */

