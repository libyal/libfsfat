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

#include <common.h>
#include <narrow_string.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyfsfat_datetime.h"
#include "pyfsfat_error.h"
#include "pyfsfat_file_entries.h"
#include "pyfsfat_file_entry.h"
#include "pyfsfat_integer.h"
#include "pyfsfat_libcerror.h"
#include "pyfsfat_libfsfat.h"
#include "pyfsfat_python.h"
#include "pyfsfat_unused.h"

PyMethodDef pyfsfat_file_entry_object_methods[] = {

	{ "get_identifier",
	  (PyCFunction) pyfsfat_file_entry_get_identifier,
	  METH_NOARGS,
	  "get_identifier() -> Integer\n"
	  "\n"
	  "Retrieves the identifier." },

	{ "get_access_time",
	  (PyCFunction) pyfsfat_file_entry_get_access_time,
	  METH_NOARGS,
	  "get_access_time() -> Datetime\n"
	  "\n"
	  "Retrieves the access date and time." },

	{ "get_access_time_as_integer",
	  (PyCFunction) pyfsfat_file_entry_get_access_time_as_integer,
	  METH_NOARGS,
	  "get_access_time_as_integer() -> Integer or None\n"
	  "\n"
	  "Retrieves the access date and time as a 64-bit integer containing a POSIX timestamp value." },

	{ "get_creation_time",
	  (PyCFunction) pyfsfat_file_entry_get_creation_time,
	  METH_NOARGS,
	  "get_creation_time() -> Datetime\n"
	  "\n"
	  "Retrieves the creation date and time." },

	{ "get_creation_time_as_integer",
	  (PyCFunction) pyfsfat_file_entry_get_creation_time_as_integer,
	  METH_NOARGS,
	  "get_creation_time_as_integer() -> Integer or None\n"
	  "\n"
	  "Retrieves the creation date and time as a 64-bit integer containing a POSIX timestamp value." },

	{ "get_modification_time",
	  (PyCFunction) pyfsfat_file_entry_get_modification_time,
	  METH_NOARGS,
	  "get_modification_time() -> Datetime\n"
	  "\n"
	  "Retrieves the modification date and time." },

	{ "get_modification_time_as_integer",
	  (PyCFunction) pyfsfat_file_entry_get_modification_time_as_integer,
	  METH_NOARGS,
	  "get_modification_time_as_integer() -> Integer or None\n"
	  "\n"
	  "Retrieves the modification date and time as a 64-bit integer containing a POSIX timestamp value." },

	{ "get_file_attribute_flags",
	  (PyCFunction) pyfsfat_file_entry_get_file_attribute_flags,
	  METH_NOARGS,
	  "get_file_attribute_flags() -> Integer or None\n"
	  "\n"
	  "Retrieves the file attribute flags." },

	{ "get_name",
	  (PyCFunction) pyfsfat_file_entry_get_name,
	  METH_NOARGS,
	  "get_name() -> Unicode string or None\n"
	  "\n"
	  "Retrieves the name." },

	{ "get_number_of_sub_file_entries",
	  (PyCFunction) pyfsfat_file_entry_get_number_of_sub_file_entries,
	  METH_NOARGS,
	  "get_number_of_sub_file_entries() -> Integer\n"
	  "\n"
	  "Retrieves the number of sub file entries." },

	{ "get_sub_file_entry",
	  (PyCFunction) pyfsfat_file_entry_get_sub_file_entry,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_sub_file_entry(sub_file_entry_index) -> Object\n"
	  "\n"
	  "Retrieves the sub file entry for the specific index specified by the index." },

	{ "get_sub_file_entry_by_name",
	  (PyCFunction) pyfsfat_file_entry_get_sub_file_entry_by_name,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_sub_file_entry_by_name(name) -> Object or None\n"
	  "\n"
	  "Retrieves the sub file entry for an UTF-8 encoded name specified by the name." },

	{ "read_buffer",
	  (PyCFunction) pyfsfat_file_entry_read_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "read_buffer(size) -> Binary string\n"
	  "\n"
	  "Reads a buffer of data at the current offset." },

	{ "read_buffer_at_offset",
	  (PyCFunction) pyfsfat_file_entry_read_buffer_at_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "read_buffer_at_offset(size, offset) -> Binary string\n"
	  "\n"
	  "Reads a buffer of data at a specific offset." },

	{ "seek_offset",
	  (PyCFunction) pyfsfat_file_entry_seek_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "seek_offset(offset, whence) -> None\n"
	  "\n"
	  "Seeks an offset within the data." },

	{ "get_offset",
	  (PyCFunction) pyfsfat_file_entry_get_offset,
	  METH_NOARGS,
	  "get_offset() -> Integer\n"
	  "\n"
	  "Retrieves the current offset of the data." },

	{ "read",
	  (PyCFunction) pyfsfat_file_entry_read_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "read(size) -> Binary string\n"
	  "\n"
	  "Reads a buffer of data at the current offset." },

	{ "seek",
	  (PyCFunction) pyfsfat_file_entry_seek_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "seek(offset, whence) -> None\n"
	  "\n"
	  "Seeks an offset within the data." },

	{ "tell",
	  (PyCFunction) pyfsfat_file_entry_get_offset,
	  METH_NOARGS,
	  "tell() -> Integer\n"
	  "\n"
	  "Retrieves the current offset of the data." },

	{ "get_size",
	  (PyCFunction) pyfsfat_file_entry_get_size,
	  METH_NOARGS,
	  "get_size() -> Integer\n"
	  "\n"
	  "Retrieves the size of the data." },

	{ "get_number_of_extents",
	  (PyCFunction) pyfsfat_file_entry_get_number_of_extents,
	  METH_NOARGS,
	  "get_number_of_extents() -> Integer\n"
	  "\n"
	  "Retrieves the number of extents of the data." },

	{ "get_extent",
	  (PyCFunction) pyfsfat_file_entry_get_extent,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_extent(extent_index) -> Tuple(Integer, Integer, Integer)\n"
	  "\n"
	  "Retrieves a specific extent.\t"
          "The extent is a tuple of offset, size and flags." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyfsfat_file_entry_object_get_set_definitions[] = {

	{ "identifier",
	  (getter) pyfsfat_file_entry_get_identifier,
	  (setter) 0,
	  "The identifier.",
	  NULL },

	{ "access_time",
	  (getter) pyfsfat_file_entry_get_access_time,
	  (setter) 0,
	  "The access date and time.",
	  NULL },

	{ "creation_time",
	  (getter) pyfsfat_file_entry_get_creation_time,
	  (setter) 0,
	  "The creation date and time.",
	  NULL },

	{ "modification_time",
	  (getter) pyfsfat_file_entry_get_modification_time,
	  (setter) 0,
	  "The modification date and time.",
	  NULL },

	{ "file_attribute_flags",
	  (getter) pyfsfat_file_entry_get_file_attribute_flags,
	  (setter) 0,
	  "The file attribute flags.",
	  NULL },

	{ "name",
	  (getter) pyfsfat_file_entry_get_name,
	  (setter) 0,
	  "The name.",
	  NULL },

	{ "number_of_sub_file_entries",
	  (getter) pyfsfat_file_entry_get_number_of_sub_file_entries,
	  (setter) 0,
	  "The number of sub file entries.",
	  NULL },

	{ "sub_file_entries",
	  (getter) pyfsfat_file_entry_get_sub_file_entries,
	  (setter) 0,
	  "The sub file entry for the specific indexes.",
	  NULL },

	{ "size",
	  (getter) pyfsfat_file_entry_get_size,
	  (setter) 0,
	  "The size of the data.",
	  NULL },

	{ "number_of_extents",
	  (getter) pyfsfat_file_entry_get_number_of_extents,
	  (setter) 0,
	  "The number of extents of the data.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyfsfat_file_entry_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyfsfat.file_entry",
	/* tp_basicsize */
	sizeof( pyfsfat_file_entry_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyfsfat_file_entry_free,
	/* tp_print */
	0,
	/* tp_getattr */
	0,
	/* tp_setattr */
	0,
	/* tp_compare */
	0,
	/* tp_repr */
	0,
	/* tp_as_number */
	0,
	/* tp_as_sequence */
	0,
	/* tp_as_mapping */
	0,
	/* tp_hash */
	0,
	/* tp_call */
	0,
	/* tp_str */
	0,
	/* tp_getattro */
	0,
	/* tp_setattro */
	0,
	/* tp_as_buffer */
	0,
	/* tp_flags */
	Py_TPFLAGS_DEFAULT,
	/* tp_doc */
	"pyfsfat file entry object (wraps libfsfat_file_entry_t)",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	0,
	/* tp_iternext */
	0,
	/* tp_methods */
	pyfsfat_file_entry_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pyfsfat_file_entry_object_get_set_definitions,
	/* tp_base */
	0,
	/* tp_dict */
	0,
	/* tp_descr_get */
	0,
	/* tp_descr_set */
	0,
	/* tp_dictoffset */
	0,
	/* tp_init */
	(initproc) pyfsfat_file_entry_init,
	/* tp_alloc */
	0,
	/* tp_new */
	0,
	/* tp_free */
	0,
	/* tp_is_gc */
	0,
	/* tp_bases */
	NULL,
	/* tp_mro */
	NULL,
	/* tp_cache */
	NULL,
	/* tp_subclasses */
	NULL,
	/* tp_weaklist */
	NULL,
	/* tp_del */
	0
};

/* Creates a new file entry object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_file_entry_new(
           libfsfat_file_entry_t *file_entry,
           PyObject *parent_object )
{
	pyfsfat_file_entry_t *pyfsfat_file_entry = NULL;
	static char *function                    = "pyfsfat_file_entry_new";

	if( file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	/* PyObject_New does not invoke tp_init
	 */
	pyfsfat_file_entry = PyObject_New(
	                      struct pyfsfat_file_entry,
	                      &pyfsfat_file_entry_type_object );

	if( pyfsfat_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize file entry.",
		 function );

		goto on_error;
	}
	pyfsfat_file_entry->file_entry    = file_entry;
	pyfsfat_file_entry->parent_object = parent_object;

	if( pyfsfat_file_entry->parent_object != NULL )
	{
		Py_IncRef(
		 pyfsfat_file_entry->parent_object );
	}
	return( (PyObject *) pyfsfat_file_entry );

on_error:
	if( pyfsfat_file_entry != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyfsfat_file_entry );
	}
	return( NULL );
}

/* Initializes a file entry object
 * Returns 0 if successful or -1 on error
 */
int pyfsfat_file_entry_init(
     pyfsfat_file_entry_t *pyfsfat_file_entry )
{
	static char *function = "pyfsfat_file_entry_init";

	if( pyfsfat_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	/* Make sure libfsfat file entry is set to NULL
	 */
	pyfsfat_file_entry->file_entry = NULL;

	PyErr_Format(
	 PyExc_NotImplementedError,
	 "%s: initialize of file entry not supported.",
	 function );

	return( -1 );
}

/* Frees a file entry object
 */
void pyfsfat_file_entry_free(
      pyfsfat_file_entry_t *pyfsfat_file_entry )
{
	struct _typeobject *ob_type = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pyfsfat_file_entry_free";
	int result                  = 0;

	if( pyfsfat_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pyfsfat_file_entry );

	if( ob_type == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: missing ob_type.",
		 function );

		return;
	}
	if( ob_type->tp_free == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid ob_type - missing tp_free.",
		 function );

		return;
	}
	if( pyfsfat_file_entry->file_entry != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libfsfat_file_entry_free(
		          &( pyfsfat_file_entry->file_entry ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyfsfat_error_raise(
			 error,
			 PyExc_MemoryError,
			 "%s: unable to free libfsfat file entry.",
			 function );

			libcerror_error_free(
			 &error );
		}
	}
	if( pyfsfat_file_entry->parent_object != NULL )
	{
		Py_DecRef(
		 pyfsfat_file_entry->parent_object );
	}
	ob_type->tp_free(
	 (PyObject*) pyfsfat_file_entry );
}

/* Retrieves the identifier
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_file_entry_get_identifier(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments PYFSFAT_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsfat_file_entry_get_identifier";
	uint64_t value_64bit     = 0;
	int result               = 0;

	PYFSFAT_UNREFERENCED_PARAMETER( arguments )

	if( pyfsfat_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsfat_file_entry_get_identifier(
	          pyfsfat_file_entry->file_entry,
	          &value_64bit,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve identifier.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyfsfat_integer_unsigned_new_from_64bit(
	                  value_64bit );

	return( integer_object );
}

/* Retrieves the access date and time
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_file_entry_get_access_time(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments PYFSFAT_ATTRIBUTE_UNUSED )
{
	PyObject *datetime_object = NULL;
	libcerror_error_t *error  = NULL;
	static char *function     = "pyfsfat_file_entry_get_access_time";
	uint64_t fat_timestamp    = 0;
	int result                = 0;

	PYFSFAT_UNREFERENCED_PARAMETER( arguments )

	if( pyfsfat_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsfat_file_entry_get_access_time(
	          pyfsfat_file_entry->file_entry,
	          &fat_timestamp,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve access date and time.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	datetime_object = pyfsfat_datetime_new_from_posix_time_in_micro_seconds(
	                   ( fat_timestamp + 31553280000UL ) * 10000 );

	return( datetime_object );
}

/* Retrieves the access date and time as an integer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_file_entry_get_access_time_as_integer(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments PYFSFAT_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsfat_file_entry_get_access_time_as_integer";
	uint64_t fat_timestamp   = 0;
	int result               = 0;

	PYFSFAT_UNREFERENCED_PARAMETER( arguments )

	if( pyfsfat_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsfat_file_entry_get_access_time(
	          pyfsfat_file_entry->file_entry,
	          &fat_timestamp,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve access date and time.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	integer_object = pyfsfat_integer_unsigned_new_from_64bit(
	                  fat_timestamp );

	return( integer_object );
}

/* Retrieves the creation date and time
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_file_entry_get_creation_time(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments PYFSFAT_ATTRIBUTE_UNUSED )
{
	PyObject *datetime_object = NULL;
	libcerror_error_t *error  = NULL;
	static char *function     = "pyfsfat_file_entry_get_creation_time";
	uint64_t fat_timestamp    = 0;
	int result                = 0;

	PYFSFAT_UNREFERENCED_PARAMETER( arguments )

	if( pyfsfat_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsfat_file_entry_get_creation_time(
	          pyfsfat_file_entry->file_entry,
	          &fat_timestamp,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve creation date and time.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	datetime_object = pyfsfat_datetime_new_from_posix_time_in_micro_seconds(
	                   ( fat_timestamp + 31553280000UL ) * 10000 );

	return( datetime_object );
}

/* Retrieves the creation date and time as an integer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_file_entry_get_creation_time_as_integer(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments PYFSFAT_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsfat_file_entry_get_creation_time_as_integer";
	uint64_t fat_timestamp   = 0;
	int result               = 0;

	PYFSFAT_UNREFERENCED_PARAMETER( arguments )

	if( pyfsfat_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsfat_file_entry_get_creation_time(
	          pyfsfat_file_entry->file_entry,
	          &fat_timestamp,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve creation date and time.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	integer_object = pyfsfat_integer_unsigned_new_from_64bit(
	                  fat_timestamp );

	return( integer_object );
}

/* Retrieves the modification date and time
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_file_entry_get_modification_time(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments PYFSFAT_ATTRIBUTE_UNUSED )
{
	PyObject *datetime_object = NULL;
	libcerror_error_t *error  = NULL;
	static char *function     = "pyfsfat_file_entry_get_modification_time";
	uint64_t fat_timestamp    = 0;
	int result                = 0;

	PYFSFAT_UNREFERENCED_PARAMETER( arguments )

	if( pyfsfat_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsfat_file_entry_get_modification_time(
	          pyfsfat_file_entry->file_entry,
	          &fat_timestamp,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve modification date and time.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	datetime_object = pyfsfat_datetime_new_from_posix_time_in_micro_seconds(
	                   ( fat_timestamp + 31553280000UL ) * 10000 );

	return( datetime_object );
}

/* Retrieves the modification date and time as an integer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_file_entry_get_modification_time_as_integer(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments PYFSFAT_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsfat_file_entry_get_modification_time_as_integer";
	uint64_t fat_timestamp   = 0;
	int result               = 0;

	PYFSFAT_UNREFERENCED_PARAMETER( arguments )

	if( pyfsfat_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsfat_file_entry_get_modification_time(
	          pyfsfat_file_entry->file_entry,
	          &fat_timestamp,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve modification date and time.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	integer_object = pyfsfat_integer_unsigned_new_from_64bit(
	                  fat_timestamp );

	return( integer_object );
}

/* Retrieves the file attribute flags
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_file_entry_get_file_attribute_flags(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments PYFSFAT_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object      = NULL;
	libcerror_error_t *error      = NULL;
	static char *function         = "pyfsfat_file_entry_get_file_attribute_flags";
	uint16_t file_attribute_flags = 0;
	int result                    = 0;

	PYFSFAT_UNREFERENCED_PARAMETER( arguments )

	if( pyfsfat_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsfat_file_entry_get_file_attribute_flags(
	          pyfsfat_file_entry->file_entry,
	          &file_attribute_flags,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve file attribute flags.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) file_attribute_flags );
#else
	integer_object = PyInt_FromLong(
	                  (long) file_attribute_flags );
#endif
	return( integer_object );
}

/* Retrieves the name
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_file_entry_get_name(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments PYFSFAT_ATTRIBUTE_UNUSED )
{
	PyObject *string_object  = NULL;
	libcerror_error_t *error = NULL;
	uint8_t *name            = NULL;
	const char *errors       = NULL;
	static char *function    = "pyfsfat_file_entry_get_name";
	size_t name_size         = 0;
	int result               = 0;

	PYFSFAT_UNREFERENCED_PARAMETER( arguments )

	if( pyfsfat_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsfat_file_entry_get_utf8_name_size(
	          pyfsfat_file_entry->file_entry,
	          &name_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve name size.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( ( result == 0 )
	      || ( name_size == 0 ) )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	name = (uint8_t *) PyMem_Malloc(
	                    sizeof( uint8_t ) * name_size );

	if( name == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to create name.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsfat_file_entry_get_utf8_name(
		  pyfsfat_file_entry->file_entry,
		  name,
		  name_size,
		  &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve name.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	string_object = PyUnicode_DecodeUTF8(
			 (char *) name,
			 (Py_ssize_t) name_size - 1,
			 errors );

	PyMem_Free(
	 name );

	return( string_object );

on_error:
	if( name != NULL )
	{
		PyMem_Free(
		 name );
	}
	return( NULL );
}

/* Retrieves the number of sub file entries
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_file_entry_get_number_of_sub_file_entries(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments PYFSFAT_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object       = NULL;
	libcerror_error_t *error       = NULL;
	static char *function          = "pyfsfat_file_entry_get_number_of_sub_file_entries";
	int number_of_sub_file_entries = 0;
	int result                     = 0;

	PYFSFAT_UNREFERENCED_PARAMETER( arguments )

	if( pyfsfat_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsfat_file_entry_get_number_of_sub_file_entries(
	          pyfsfat_file_entry->file_entry,
	          &number_of_sub_file_entries,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of sub file entries.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) number_of_sub_file_entries );
#else
	integer_object = PyInt_FromLong(
	                  (long) number_of_sub_file_entries );
#endif
	return( integer_object );
}

/* Retrieves a specific sub file entry for the specific index by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_file_entry_get_sub_file_entry_by_index(
           PyObject *pyfsfat_file_entry,
           int sub_file_entry_index )
{
	PyObject *file_entry_object           = NULL;
	libcerror_error_t *error              = NULL;
	libfsfat_file_entry_t *sub_file_entry = NULL;
	static char *function                 = "pyfsfat_file_entry_get_sub_file_entry_by_index";
	int result                            = 0;

	if( pyfsfat_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsfat_file_entry_get_sub_file_entry_by_index(
	          ( (pyfsfat_file_entry_t *) pyfsfat_file_entry )->file_entry,
	          sub_file_entry_index,
	          &sub_file_entry,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve sub file entry for the specific index: %d.",
		 function,
		 sub_file_entry_index );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	file_entry_object = pyfsfat_file_entry_new(
	                     sub_file_entry,
	                     ( (pyfsfat_file_entry_t *) pyfsfat_file_entry )->parent_object );

	if( file_entry_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create sub file entry object.",
		 function );

		goto on_error;
	}
	return( file_entry_object );

on_error:
	if( sub_file_entry != NULL )
	{
		libfsfat_file_entry_free(
		 &sub_file_entry,
		 NULL );
	}
	return( NULL );
}

/* Retrieves a specific sub file entry for the specific index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_file_entry_get_sub_file_entry(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *file_entry_object = NULL;
	static char *keyword_list[] = { "sub_file_entry_index", NULL };
	int sub_file_entry_index    = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &sub_file_entry_index ) == 0 )
	{
		return( NULL );
	}
	file_entry_object = pyfsfat_file_entry_get_sub_file_entry_by_index(
	                     (PyObject *) pyfsfat_file_entry,
	                     sub_file_entry_index );

	return( file_entry_object );
}

/* Retrieves a sequence and iterator object for the sub file entries
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_file_entry_get_sub_file_entries(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments PYFSFAT_ATTRIBUTE_UNUSED )
{
	PyObject *sequence_object      = NULL;
	libcerror_error_t *error       = NULL;
	static char *function          = "pyfsfat_file_entry_get_sub_file_entries";
	int number_of_sub_file_entries = 0;
	int result                     = 0;

	PYFSFAT_UNREFERENCED_PARAMETER( arguments )

	if( pyfsfat_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsfat_file_entry_get_number_of_sub_file_entries(
	          pyfsfat_file_entry->file_entry,
	          &number_of_sub_file_entries,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of sub file entries.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	sequence_object = pyfsfat_file_entries_new(
	                   (PyObject *) pyfsfat_file_entry,
	                   &pyfsfat_file_entry_get_sub_file_entry_by_index,
	                   number_of_sub_file_entries );

	if( sequence_object == NULL )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to create sequence object.",
		 function );

		return( NULL );
	}
	return( sequence_object );
}

/* Retrieves the sub file entry for an UTF-8 encoded name specified by the name
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_file_entry_get_sub_file_entry_by_name(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *file_entry_object           = NULL;
	libcerror_error_t *error              = NULL;
	libfsfat_file_entry_t *sub_file_entry = NULL;
	static char *function                 = "pyfsfat_file_entry_get_sub_file_entry_by_name";
	static char *keyword_list[]           = { "name", NULL };
	char *utf8_name                       = NULL;
	size_t utf8_name_length               = 0;
	int result                            = 0;

	if( pyfsfat_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s",
	     keyword_list,
	     &utf8_name ) == 0 )
	{
		goto on_error;
	}
	utf8_name_length = narrow_string_length(
	                    utf8_name );

	Py_BEGIN_ALLOW_THREADS

	result = libfsfat_file_entry_get_sub_file_entry_by_utf8_name(
	          pyfsfat_file_entry->file_entry,
	          (uint8_t *) utf8_name,
	          utf8_name_length,
	          &sub_file_entry,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve sub file entry for an UTF-8 encoded name.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	file_entry_object = pyfsfat_file_entry_new(
	                     sub_file_entry,
	                     pyfsfat_file_entry->parent_object );

	if( file_entry_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create sub file entry object.",
		 function );

		goto on_error;
	}
	return( file_entry_object );

on_error:
	if( sub_file_entry != NULL )
	{
		libfsfat_file_entry_free(
		 &sub_file_entry,
		 NULL );
	}
	return( NULL );
}

/* Reads data at the current offset into a buffer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_file_entry_read_buffer(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *integer_object    = NULL;
	PyObject *string_object     = NULL;
	libcerror_error_t *error    = NULL;
	char *buffer                = NULL;
	static char *function       = "pyfsfat_file_entry_read_buffer";
	static char *keyword_list[] = { "size", NULL };
	ssize_t read_count          = 0;
	int64_t read_size           = 0;
	int result                  = 0;

	if( pyfsfat_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|O",
	     keyword_list,
	     &integer_object ) == 0 )
	{
		return( NULL );
	}
	if( integer_object == NULL )
	{
		result = 0;
	}
	else
	{
		result = PyObject_IsInstance(
		          integer_object,
		          (PyObject *) &PyLong_Type );

		if( result == -1 )
		{
			pyfsfat_error_fetch_and_raise(
			 PyExc_RuntimeError,
			 "%s: unable to determine if integer object is of type long.",
			 function );

			return( NULL );
		}
#if PY_MAJOR_VERSION < 3
		else if( result == 0 )
		{
			PyErr_Clear();

			result = PyObject_IsInstance(
			          integer_object,
			          (PyObject *) &PyInt_Type );

			if( result == -1 )
			{
				pyfsfat_error_fetch_and_raise(
				 PyExc_RuntimeError,
				 "%s: unable to determine if integer object is of type int.",
				 function );

				return( NULL );
			}
		}
#endif
	}
	if( result != 0 )
	{
		if( pyfsfat_integer_signed_copy_to_64bit(
		     integer_object,
		     &read_size,
		     &error ) != 1 )
		{
			pyfsfat_error_raise(
			 error,
			 PyExc_ValueError,
			 "%s: unable to convert integer object into read size.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
	}
	else if( ( integer_object == NULL )
	      || ( integer_object == Py_None ) )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libfsfat_file_entry_get_size(
		          pyfsfat_file_entry->file_entry,
		          (size64_t *) &read_size,
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyfsfat_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to retrieve size.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
	}
	else
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: unsupported integer object type.",
		 function );

		return( NULL );
	}
	if( read_size == 0 )
	{
#if PY_MAJOR_VERSION >= 3
		string_object = PyBytes_FromString(
		                 "" );
#else
		string_object = PyString_FromString(
		                 "" );
#endif
		return( string_object );
	}
	if( read_size < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid read size value less than zero.",
		 function );

		return( NULL );
	}
	/* Make sure the data fits into a memory buffer
	 */
	if( ( read_size > (int64_t) INT_MAX )
	 || ( read_size > (int64_t) SSIZE_MAX ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value exceeds maximum.",
		 function );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	string_object = PyBytes_FromStringAndSize(
	                 NULL,
	                 (Py_ssize_t) read_size );

	buffer = PyBytes_AsString(
	          string_object );
#else
	/* Note that a size of 0 is not supported
	 */
	string_object = PyString_FromStringAndSize(
	                 NULL,
	                 (Py_ssize_t) read_size );

	buffer = PyString_AsString(
	          string_object );
#endif
	Py_BEGIN_ALLOW_THREADS

	read_count = libfsfat_file_entry_read_buffer(
	              pyfsfat_file_entry->file_entry,
	              (uint8_t *) buffer,
	              (size_t) read_size,
	              &error );

	Py_END_ALLOW_THREADS

	if( read_count == -1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to read data.",
		 function );

		libcerror_error_free(
		 &error );

		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	/* Need to resize the string here in case read_size was not fully read.
	 */
#if PY_MAJOR_VERSION >= 3
	if( _PyBytes_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#else
	if( _PyString_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#endif
	{
		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	return( string_object );
}

/* Reads data at a specific offset into a buffer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_file_entry_read_buffer_at_offset(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *integer_object    = NULL;
	PyObject *string_object     = NULL;
	libcerror_error_t *error    = NULL;
	char *buffer                = NULL;
	static char *function       = "pyfsfat_file_entry_read_buffer_at_offset";
	static char *keyword_list[] = { "size", "offset", NULL };
	ssize_t read_count          = 0;
	off64_t read_offset         = 0;
	int64_t read_size           = 0;
	int result                  = 0;

	if( pyfsfat_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "OL",
	     keyword_list,
	     &integer_object,
	     &read_offset ) == 0 )
	{
		return( NULL );
	}
	result = PyObject_IsInstance(
	          integer_object,
	          (PyObject *) &PyLong_Type );

	if( result == -1 )
	{
		pyfsfat_error_fetch_and_raise(
		 PyExc_RuntimeError,
		 "%s: unable to determine if integer object is of type long.",
		 function );

		return( NULL );
	}
#if PY_MAJOR_VERSION < 3
	else if( result == 0 )
	{
		PyErr_Clear();

		result = PyObject_IsInstance(
		          integer_object,
		          (PyObject *) &PyInt_Type );

		if( result == -1 )
		{
			pyfsfat_error_fetch_and_raise(
			 PyExc_RuntimeError,
			 "%s: unable to determine if integer object is of type int.",
			 function );

			return( NULL );
		}
	}
#endif
	if( result != 0 )
	{
		if( pyfsfat_integer_signed_copy_to_64bit(
		     integer_object,
		     &read_size,
		     &error ) != 1 )
		{
			pyfsfat_error_raise(
			 error,
			 PyExc_ValueError,
			 "%s: unable to convert integer object into read size.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
	}
	else
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: unsupported integer object type.",
		 function );

		return( NULL );
	}
	if( read_size == 0 )
	{
#if PY_MAJOR_VERSION >= 3
		string_object = PyBytes_FromString(
		                 "" );
#else
		string_object = PyString_FromString(
		                 "" );
#endif
		return( string_object );
	}
	if( read_size < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid read size value less than zero.",
		 function );

		return( NULL );
	}
	/* Make sure the data fits into a memory buffer
	 */
	if( ( read_size > (int64_t) INT_MAX )
	 || ( read_size > (int64_t) SSIZE_MAX ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value exceeds maximum.",
		 function );

		return( NULL );
	}
	if( read_offset < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid read offset value less than zero.",
		 function );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	string_object = PyBytes_FromStringAndSize(
	                 NULL,
	                 (Py_ssize_t) read_size );

	buffer = PyBytes_AsString(
	          string_object );
#else
	/* Note that a size of 0 is not supported
	 */
	string_object = PyString_FromStringAndSize(
	                 NULL,
	                 (Py_ssize_t) read_size );

	buffer = PyString_AsString(
	          string_object );
#endif
	Py_BEGIN_ALLOW_THREADS

	read_count = libfsfat_file_entry_read_buffer_at_offset(
	              pyfsfat_file_entry->file_entry,
	              (uint8_t *) buffer,
	              (size_t) read_size,
	              (off64_t) read_offset,
	              &error );

	Py_END_ALLOW_THREADS

	if( read_count == -1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to read data.",
		 function );

		libcerror_error_free(
		 &error );

		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	/* Need to resize the string here in case read_size was not fully read.
	 */
#if PY_MAJOR_VERSION >= 3
	if( _PyBytes_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#else
	if( _PyString_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#endif
	{
		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	return( string_object );
}

/* Seeks a certain offset
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_file_entry_seek_offset(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	static char *function       = "pyfsfat_file_entry_seek_offset";
	static char *keyword_list[] = { "offset", "whence", NULL };
	off64_t offset              = 0;
	int whence                  = 0;

	if( pyfsfat_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "L|i",
	     keyword_list,
	     &offset,
	     &whence ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	offset = libfsfat_file_entry_seek_offset(
	          pyfsfat_file_entry->file_entry,
	          offset,
	          whence,
	          &error );

	Py_END_ALLOW_THREADS

	if( offset == -1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to seek offset.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Retrieves the current offset of the data
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_file_entry_get_offset(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments PYFSFAT_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsfat_file_entry_get_offset";
	off64_t offset           = 0;
	int result               = 0;

	PYFSFAT_UNREFERENCED_PARAMETER( arguments )

	if( pyfsfat_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsfat_file_entry_get_offset(
	          pyfsfat_file_entry->file_entry,
	          &offset,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve current offset of the data.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	integer_object = pyfsfat_integer_signed_new_from_64bit(
	                  (int64_t) offset );

	return( integer_object );
}

/* Retrieves the size of the data
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_file_entry_get_size(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments PYFSFAT_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsfat_file_entry_get_size";
	size64_t size            = 0;
	int result               = 0;

	PYFSFAT_UNREFERENCED_PARAMETER( arguments )

	if( pyfsfat_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsfat_file_entry_get_size(
	          pyfsfat_file_entry->file_entry,
	          &size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: failed to retrieve size of the data.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyfsfat_integer_unsigned_new_from_64bit(
	                  (uint64_t) size );

	return( integer_object );
}

/* Retrieves the number of extents of the data
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_file_entry_get_number_of_extents(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments PYFSFAT_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsfat_file_entry_get_number_of_extents";
	int number_of_extents    = 0;
	int result               = 0;

	PYFSFAT_UNREFERENCED_PARAMETER( arguments )

	if( pyfsfat_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsfat_file_entry_get_number_of_extents(
	          pyfsfat_file_entry->file_entry,
	          &number_of_extents,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of extents of the data.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) number_of_extents );
#else
	integer_object = PyInt_FromLong(
	                  (long) number_of_extents );
#endif
	return( integer_object );
}

/* Retrieves a specific extent by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_file_entry_get_extent_by_index(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           int extent_index )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	PyObject *tuple_object   = NULL;
	static char *function    = "pyfsfat_file_entry_get_extent_by_index";
	off64_t extent_offset    = 0;
	size64_t extent_size     = 0;
	uint32_t extent_flags    = 0;
	int result               = 0;

	if( pyfsfat_file_entry == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file entry.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsfat_file_entry_get_extent_by_index(
	          pyfsfat_file_entry->file_entry,
	          extent_index,
	          &extent_offset,
	          &extent_size,
	          &extent_flags,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve extent: %d.",
		 function,
		 extent_index );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	tuple_object = PyTuple_New(
                        3 );

	integer_object = pyfsfat_integer_signed_new_from_64bit(
	                  (int64_t) extent_offset );

	/* Tuple set item does not increment the reference count of the integer object
	 */
	if( PyTuple_SetItem(
	     tuple_object,
	     0,
	     integer_object ) != 0 )
	{
		goto on_error;
	}
	integer_object = pyfsfat_integer_unsigned_new_from_64bit(
	                  (uint64_t) extent_size );

	/* Tuple set item does not increment the reference count of the integer object
	 */
	if( PyTuple_SetItem(
	     tuple_object,
	     1,
	     integer_object ) != 0 )
	{
		goto on_error;
	}
	integer_object = pyfsfat_integer_unsigned_new_from_64bit(
	                  (uint64_t) extent_flags );

	/* Tuple set item does not increment the reference count of the integer object
	 */
	if( PyTuple_SetItem(
	     tuple_object,
	     2,
	     integer_object ) != 0 )
	{
		goto on_error;
	}
	return( tuple_object );

on_error:
	if( integer_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) integer_object );
	}
	if( tuple_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) tuple_object );
	}
	return( NULL );
}

/* Retrieves a specific extent
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_file_entry_get_extent(
           pyfsfat_file_entry_t *pyfsfat_file_entry,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *sequence_object   = NULL;
	static char *keyword_list[] = { "extent_index", NULL };
	int extent_index            = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &extent_index ) == 0 )
	{
		return( NULL );
	}
	sequence_object = pyfsfat_file_entry_get_extent_by_index(
	                   pyfsfat_file_entry,
	                   extent_index );

	return( sequence_object );
}

