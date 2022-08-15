/*
 * Python object wrapper of libfsfat_volume_t
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

#if !defined( _PYFSFAT_VOLUME_H )
#define _PYFSFAT_VOLUME_H

#include <common.h>
#include <types.h>

#include "pyfsfat_libbfio.h"
#include "pyfsfat_libfsfat.h"
#include "pyfsfat_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyfsfat_volume pyfsfat_volume_t;

struct pyfsfat_volume
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libfsfat volume
	 */
	libfsfat_volume_t *volume;

	/* The libbfio file IO handle
	 */
	libbfio_handle_t *file_io_handle;
};

extern PyMethodDef pyfsfat_volume_object_methods[];
extern PyTypeObject pyfsfat_volume_type_object;

int pyfsfat_volume_init(
     pyfsfat_volume_t *pyfsfat_volume );

void pyfsfat_volume_free(
      pyfsfat_volume_t *pyfsfat_volume );

PyObject *pyfsfat_volume_signal_abort(
           pyfsfat_volume_t *pyfsfat_volume,
           PyObject *arguments );

PyObject *pyfsfat_volume_open(
           pyfsfat_volume_t *pyfsfat_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsfat_volume_open_file_object(
           pyfsfat_volume_t *pyfsfat_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsfat_volume_close(
           pyfsfat_volume_t *pyfsfat_volume,
           PyObject *arguments );

PyObject *pyfsfat_volume_get_file_system_format(
           pyfsfat_volume_t *pyfsfat_volume,
           PyObject *arguments );

PyObject *pyfsfat_volume_get_label(
           pyfsfat_volume_t *pyfsfat_volume,
           PyObject *arguments );

PyObject *pyfsfat_volume_get_root_directory(
           pyfsfat_volume_t *pyfsfat_volume,
           PyObject *arguments );

PyObject *pyfsfat_volume_get_file_entry_by_identifier(
           pyfsfat_volume_t *pyfsfat_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsfat_volume_get_file_entry_by_path(
           pyfsfat_volume_t *pyfsfat_volume,
           PyObject *arguments,
           PyObject *keywords );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYFSFAT_VOLUME_H ) */

