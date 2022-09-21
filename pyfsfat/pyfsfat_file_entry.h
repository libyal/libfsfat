/*
 * Python object wrapper of libfsfat_file_entry_t
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

#if !defined( _PYFSFAT_FILE_ENTRY_H )
#define _PYFSFAT_FILE_ENTRY_H

#include <common.h>
#include <types.h>

#include "pyfsfat_libfsfat.h"
#include "pyfsfat_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyfsfat_file_entry pyfsfat_file_entry_t;

struct pyfsfat_file_entry
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libfsfat file entry
	 */
	libfsfat_file_entry_t *file_entry;

	/* The parent object
	 */
	PyObject *parent_object;
};

extern PyMethodDef pyfsfat_file_entry_object_methods[];
extern PyTypeObject pyfsfat_file_entry_type_object;

PyObject *pyfsfat_file_entry_new(
           libfsfat_file_entry_t *file_entry,
           PyObject *parent_object );

int pyfsfat_file_entry_init(
     pyfsfat_file_entry_t *pyfsfat_file_entry );

void pyfsfat_file_entry_free(
      pyfsfat_file_entry_t *pyfsfat_file_entry );

PyObject *pyfsfat_file_entry_get_identifier(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments );

PyObject *pyfsfat_file_entry_get_access_time(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments );

PyObject *pyfsfat_file_entry_get_access_time_as_integer(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments );

PyObject *pyfsfat_file_entry_get_creation_time(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments );

PyObject *pyfsfat_file_entry_get_creation_time_as_integer(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments );

PyObject *pyfsfat_file_entry_get_modification_time(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments );

PyObject *pyfsfat_file_entry_get_modification_time_as_integer(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments );

PyObject *pyfsfat_file_entry_get_file_attribute_flags(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments );

PyObject *pyfsfat_file_entry_get_name(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments );

PyObject *pyfsfat_file_entry_get_number_of_sub_file_entries(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments );

PyObject *pyfsfat_file_entry_get_sub_file_entry_by_index(
           PyObject *pyfsfat_file_entry,
           int sub_file_entry_index );

PyObject *pyfsfat_file_entry_get_sub_file_entry(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsfat_file_entry_get_sub_file_entries(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments );

PyObject *pyfsfat_file_entry_get_sub_file_entry_by_name(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsfat_file_entry_read_buffer(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsfat_file_entry_read_buffer_at_offset(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsfat_file_entry_seek_offset(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsfat_file_entry_get_offset(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments );

PyObject *pyfsfat_file_entry_get_size(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments );

PyObject *pyfsfat_file_entry_get_number_of_extents(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments );

PyObject *pyfsfat_file_entry_get_extent_by_index(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           int extent_index );

PyObject *pyfsfat_file_entry_get_extent(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments,
           PyObject *keywords );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYFSFAT_FILE_ENTRY_H ) */

