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

#include <common.h>
#include <narrow_string.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyfsfat_datetime.h"
#include "pyfsfat_error.h"
#include "pyfsfat_file_entry.h"
#include "pyfsfat_file_object_io_handle.h"
#include "pyfsfat_libbfio.h"
#include "pyfsfat_libcerror.h"
#include "pyfsfat_libfsfat.h"
#include "pyfsfat_python.h"
#include "pyfsfat_unused.h"
#include "pyfsfat_volume.h"

#if !defined( LIBFSFAT_HAVE_BFIO )

LIBFSFAT_EXTERN \
int libfsfat_volume_open_file_io_handle(
     libfsfat_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libfsfat_error_t **error );

#endif /* !defined( LIBFSFAT_HAVE_BFIO ) */

PyMethodDef pyfsfat_volume_object_methods[] = {

	{ "signal_abort",
	  (PyCFunction) pyfsfat_volume_signal_abort,
	  METH_NOARGS,
	  "signal_abort() -> None\n"
	  "\n"
	  "Signals the volume to abort the current activity." },

	{ "open",
	  (PyCFunction) pyfsfat_volume_open,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(filename, mode='r') -> None\n"
	  "\n"
	  "Opens a volume." },

	{ "open_file_object",
	  (PyCFunction) pyfsfat_volume_open_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "open_file_object(file_object, mode='r') -> None\n"
	  "\n"
	  "Opens a volume using a file-like object." },

	{ "close",
	  (PyCFunction) pyfsfat_volume_close,
	  METH_NOARGS,
	  "close() -> None\n"
	  "\n"
	  "Closes a volume." },

	{ "get_label",
	  (PyCFunction) pyfsfat_volume_get_label,
	  METH_NOARGS,
	  "get_label() -> Unicode string\n"
	  "\n"
	  "Retrieves the label." },

	{ "get_last_mount_path",
	  (PyCFunction) pyfsfat_volume_get_last_mount_path,
	  METH_NOARGS,
	  "get_last_mount_path() -> Unicode string\n"
	  "\n"
	  "Retrieves the last mount path." },

	{ "get_last_mount_time",
	  (PyCFunction) pyfsfat_volume_get_last_mount_time,
	  METH_NOARGS,
	  "get_last_mount_time() -> Datetime or None\n"
	  "\n"
	  "Retrieves the last mount time." },

	{ "get_last_mount_time_as_integer",
	  (PyCFunction) pyfsfat_volume_get_last_mount_time_as_integer,
	  METH_NOARGS,
	  "get_last_mount_time_as_integer() -> Integer or None\n"
	  "\n"
	  "Retrieves the last mount time as a 32-bit integer containing a POSIX timestamp value." },

	{ "get_last_written_time",
	  (PyCFunction) pyfsfat_volume_get_last_written_time,
	  METH_NOARGS,
	  "get_last_written_time() -> Datetime or None\n"
	  "\n"
	  "Retrieves the last written time." },

	{ "get_last_written_time_as_integer",
	  (PyCFunction) pyfsfat_volume_get_last_written_time_as_integer,
	  METH_NOARGS,
	  "get_last_written_time_as_integer() -> Integer or None\n"
	  "\n"
	  "Retrieves the last written time as a 32-bit integer containing a POSIX timestamp value." },

	{ "get_number_of_file_entries",
	  (PyCFunction) pyfsfat_volume_get_number_of_file_entries,
	  METH_NOARGS,
	  "get_number_of_file_entries() -> Integer\n"
	  "\n"
	  "Retrieves the number of file entries (MFT entries)." },

	{ "get_file_entry_by_inode",
	  (PyCFunction) pyfsfat_volume_get_file_entry_by_inode,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_file_entry_by_inode(inode_number) -> Object or None\n"
	  "\n"
	  "Retrieves the file entry specified by the inode number." },

	{ "get_root_directory",
	  (PyCFunction) pyfsfat_volume_get_root_directory,
	  METH_NOARGS,
	  "get_root_directory() -> Object\n"
	  "\n"
	  "Retrieves the root directory file entry." },

	{ "get_file_entry_by_path",
	  (PyCFunction) pyfsfat_volume_get_file_entry_by_path,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_file_entry_by_path(path) -> Object or None\n"
	  "\n"
	  "Retrieves the file entry for an UTF-8 encoded path specified by the path." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyfsfat_volume_object_get_set_definitions[] = {

	{ "label",
	  (getter) pyfsfat_volume_get_label,
	  (setter) 0,
	  "The label.",
	  NULL },

	{ "last_mount_path",
	  (getter) pyfsfat_volume_get_last_mount_path,
	  (setter) 0,
	  "The last mount path.",
	  NULL },

	{ "last_mount_time",
	  (getter) pyfsfat_volume_get_last_mount_time,
	  (setter) 0,
	  "The last mount time.",
	  NULL },

	{ "last_written_time",
	  (getter) pyfsfat_volume_get_last_written_time,
	  (setter) 0,
	  "The last written time.",
	  NULL },

	{ "number_of_file_entries",
	  (getter) pyfsfat_volume_get_number_of_file_entries,
	  (setter) 0,
	  "The number of file entries (MFT entries).",
	  NULL },

	{ "root_directory",
	  (getter) pyfsfat_volume_get_root_directory,
	  (setter) 0,
	  "The root directory file entry.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyfsfat_volume_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyfsfat.volume",
	/* tp_basicsize */
	sizeof( pyfsfat_volume_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyfsfat_volume_free,
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
	"pyfsfat volume object (wraps libfsfat_volume_t)",
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
	pyfsfat_volume_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pyfsfat_volume_object_get_set_definitions,
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
	(initproc) pyfsfat_volume_init,
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

/* Initializes a volume object
 * Returns 0 if successful or -1 on error
 */
int pyfsfat_volume_init(
     pyfsfat_volume_t *pyfsfat_volume )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsfat_volume_init";

	if( pyfsfat_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	/* Make sure libfsfat volume is set to NULL
	 */
	pyfsfat_volume->volume         = NULL;
	pyfsfat_volume->file_io_handle = NULL;

	if( libfsfat_volume_initialize(
	     &( pyfsfat_volume->volume ),
	     &error ) != 1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to initialize volume.",
		 function );

		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 0 );
}

/* Frees a volume object
 */
void pyfsfat_volume_free(
      pyfsfat_volume_t *pyfsfat_volume )
{
	struct _typeobject *ob_type = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pyfsfat_volume_free";
	int result                  = 0;

	if( pyfsfat_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pyfsfat_volume );

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
	if( pyfsfat_volume->file_io_handle != NULL )
	{
		if( pyfsfat_volume_close(
		     pyfsfat_volume,
		     NULL ) == NULL )
		{
			return;
		}
	}
	if( pyfsfat_volume->volume != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libfsfat_volume_free(
		          &( pyfsfat_volume->volume ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyfsfat_error_raise(
			 error,
			 PyExc_MemoryError,
			 "%s: unable to free libfsfat volume.",
			 function );

			libcerror_error_free(
			 &error );
		}
	}
	ob_type->tp_free(
	 (PyObject*) pyfsfat_volume );
}

/* Signals the volume to abort the current activity
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_volume_signal_abort(
           pyfsfat_volume_t *pyfsfat_volume,
           PyObject *arguments PYFSFAT_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsfat_volume_signal_abort";
	int result               = 0;

	PYFSFAT_UNREFERENCED_PARAMETER( arguments )

	if( pyfsfat_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsfat_volume_signal_abort(
	          pyfsfat_volume->volume,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to signal abort.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Opens a volume
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_volume_open(
           pyfsfat_volume_t *pyfsfat_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *string_object     = NULL;
	libcerror_error_t *error    = NULL;
	const char *filename_narrow = NULL;
	static char *function       = "pyfsfat_volume_open";
	static char *keyword_list[] = { "filename", "mode", NULL };
	char *mode                  = NULL;
	int result                  = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	const wchar_t *filename_wide = NULL;
#else
	PyObject *utf8_string_object = NULL;
#endif

	if( pyfsfat_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * On Windows the narrow character strings contains an extended ASCII string with a codepage. Hence we get a conversion
	 * exception. This will also fail if the default encoding is not set correctly. We cannot use "u" here either since that
	 * does not allow us to pass non Unicode string objects and Python (at least 2.7) does not seems to automatically upcast them.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O|s",
	     keyword_list,
	     &string_object,
	     &mode ) == 0 )
	{
		return( NULL );
	}
	if( ( mode != NULL )
	 && ( mode[ 0 ] != 'r' ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: unsupported mode: %s.",
		 function,
		 mode );

		return( NULL );
	}
	PyErr_Clear();

	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyUnicode_Type );

	if( result == -1 )
	{
		pyfsfat_error_fetch_and_raise(
		 PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type Unicode.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		filename_wide = (wchar_t *) PyUnicode_AsUnicode(
		                             string_object );
		Py_BEGIN_ALLOW_THREADS

		result = libfsfat_volume_open_wide(
		          pyfsfat_volume->volume,
		          filename_wide,
		          LIBFSFAT_OPEN_READ,
		          &error );

		Py_END_ALLOW_THREADS
#else
		utf8_string_object = PyUnicode_AsUTF8String(
		                      string_object );

		if( utf8_string_object == NULL )
		{
			pyfsfat_error_fetch_and_raise(
			 PyExc_RuntimeError,
			 "%s: unable to convert Unicode string to UTF-8.",
			 function );

			return( NULL );
		}
#if PY_MAJOR_VERSION >= 3
		filename_narrow = PyBytes_AsString(
		                   utf8_string_object );
#else
		filename_narrow = PyString_AsString(
		                   utf8_string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libfsfat_volume_open(
		          pyfsfat_volume->volume,
		          filename_narrow,
		          LIBFSFAT_OPEN_READ,
		          &error );

		Py_END_ALLOW_THREADS

		Py_DecRef(
		 utf8_string_object );
#endif
		if( result != 1 )
		{
			pyfsfat_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to open volume.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyBytes_Type );
#else
	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyString_Type );
#endif
	if( result == -1 )
	{
		pyfsfat_error_fetch_and_raise(
		 PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type string.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
		filename_narrow = PyBytes_AsString(
		                   string_object );
#else
		filename_narrow = PyString_AsString(
		                   string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libfsfat_volume_open(
		          pyfsfat_volume->volume,
		          filename_narrow,
		          LIBFSFAT_OPEN_READ,
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyfsfat_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to open volume.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	PyErr_Format(
	 PyExc_TypeError,
	 "%s: unsupported string object type.",
	 function );

	return( NULL );
}

/* Opens a volume using a file-like object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_volume_open_file_object(
           pyfsfat_volume_t *pyfsfat_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *file_object       = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pyfsfat_volume_open_file_object";
	static char *keyword_list[] = { "file_object", "mode", NULL };
	char *mode                  = NULL;
	int result                  = 0;

	if( pyfsfat_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O|s",
	     keyword_list,
	     &file_object,
	     &mode ) == 0 )
	{
		return( NULL );
	}
	if( ( mode != NULL )
	 && ( mode[ 0 ] != 'r' ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: unsupported mode: %s.",
		 function,
		 mode );

		return( NULL );
	}
	PyErr_Clear();

	result = PyObject_HasAttrString(
	          file_object,
	          "read" );

	if( result != 1 )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: unsupported file object - missing read attribute.",
		 function );

		return( NULL );
	}
	PyErr_Clear();

	result = PyObject_HasAttrString(
	          file_object,
	          "seek" );

	if( result != 1 )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: unsupported file object - missing seek attribute.",
		 function );

		return( NULL );
	}
	if( pyfsfat_volume->file_io_handle != NULL )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: invalid volume - file IO handle already set.",
		 function );

		return( NULL );
	}
	if( pyfsfat_file_object_initialize(
	     &( pyfsfat_volume->file_io_handle ),
	     file_object,
	     &error ) != 1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to initialize file IO handle.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsfat_volume_open_file_io_handle(
	          pyfsfat_volume->volume,
	          pyfsfat_volume->file_io_handle,
	          LIBFSFAT_OPEN_READ,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to open volume.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );

on_error:
	if( pyfsfat_volume->file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &( pyfsfat_volume->file_io_handle ),
		 NULL );
	}
	return( NULL );
}

/* Closes a volume
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_volume_close(
           pyfsfat_volume_t *pyfsfat_volume,
           PyObject *arguments PYFSFAT_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsfat_volume_close";
	int result               = 0;

	PYFSFAT_UNREFERENCED_PARAMETER( arguments )

	if( pyfsfat_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsfat_volume_close(
	          pyfsfat_volume->volume,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 0 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to close volume.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( pyfsfat_volume->file_io_handle != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libbfio_handle_free(
		          &( pyfsfat_volume->file_io_handle ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyfsfat_error_raise(
			 error,
			 PyExc_MemoryError,
			 "%s: unable to free libbfio file IO handle.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Retrieves the label
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_volume_get_label(
           pyfsfat_volume_t *pyfsfat_volume,
           PyObject *arguments PYFSFAT_ATTRIBUTE_UNUSED )
{
	PyObject *string_object  = NULL;
	libcerror_error_t *error = NULL;
	const char *errors       = NULL;
	static char *function    = "pyfsfat_volume_get_label";
	char *utf8_string        = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	PYFSFAT_UNREFERENCED_PARAMETER( arguments )

	if( pyfsfat_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsfat_volume_get_utf8_label_size(
	          pyfsfat_volume->volume,
	          &utf8_string_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to determine size of label as UTF-8 string.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( ( result == 0 )
	      || ( utf8_string_size == 0 ) )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	utf8_string = (char *) PyMem_Malloc(
	                        sizeof( char ) * utf8_string_size );

	if( utf8_string == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create UTF-8 string.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsfat_volume_get_utf8_label(
	          pyfsfat_volume->volume,
	          (uint8_t *) utf8_string,
	          utf8_string_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve label as UTF-8 string.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Pass the string length to PyUnicode_DecodeUTF8 otherwise it makes
	 * the end of string character is part of the string.
	 */
	string_object = PyUnicode_DecodeUTF8(
	                 utf8_string,
	                 (Py_ssize_t) utf8_string_size - 1,
	                 errors );

	if( string_object == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to convert UTF-8 string into Unicode object.",
		 function );

		goto on_error;
	}
	PyMem_Free(
	 utf8_string );

	return( string_object );

on_error:
	if( utf8_string != NULL )
	{
		PyMem_Free(
		 utf8_string );
	}
	return( NULL );
}

/* Retrieves the last mount path
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_volume_get_last_mount_path(
           pyfsfat_volume_t *pyfsfat_volume,
           PyObject *arguments PYFSFAT_ATTRIBUTE_UNUSED )
{
	PyObject *string_object  = NULL;
	libcerror_error_t *error = NULL;
	const char *errors       = NULL;
	static char *function    = "pyfsfat_volume_get_last_mount_path";
	char *utf8_string        = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	PYFSFAT_UNREFERENCED_PARAMETER( arguments )

	if( pyfsfat_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsfat_volume_get_utf8_last_mount_path_size(
	          pyfsfat_volume->volume,
	          &utf8_string_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to determine size of last mount path as UTF-8 string.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( ( result == 0 )
	      || ( utf8_string_size == 0 ) )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	utf8_string = (char *) PyMem_Malloc(
	                        sizeof( char ) * utf8_string_size );

	if( utf8_string == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create UTF-8 string.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsfat_volume_get_utf8_last_mount_path(
	          pyfsfat_volume->volume,
	          (uint8_t *) utf8_string,
	          utf8_string_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve last mount path as UTF-8 string.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Pass the string length to PyUnicode_DecodeUTF8 otherwise it makes
	 * the end of string character is part of the string.
	 */
	string_object = PyUnicode_DecodeUTF8(
	                 utf8_string,
	                 (Py_ssize_t) utf8_string_size - 1,
	                 errors );

	if( string_object == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to convert UTF-8 string into Unicode object.",
		 function );

		goto on_error;
	}
	PyMem_Free(
	 utf8_string );

	return( string_object );

on_error:
	if( utf8_string != NULL )
	{
		PyMem_Free(
		 utf8_string );
	}
	return( NULL );
}

/* Retrieves the last_mount date and time
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_volume_get_last_mount_time(
           pyfsfat_volume_t *pyfsfat_volume,
           PyObject *arguments PYFSFAT_ATTRIBUTE_UNUSED )
{
	PyObject *datetime_object = NULL;
	libcerror_error_t *error  = NULL;
	static char *function     = "pyfsfat_volume_get_last_mount_time";
	int32_t posix_time        = 0;
	int result                = 0;

	PYFSFAT_UNREFERENCED_PARAMETER( arguments )

	if( pyfsfat_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsfat_volume_get_last_mount_time(
	          pyfsfat_volume->volume,
	          &posix_time,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve last mount date and time.",
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
	datetime_object = pyfsfat_datetime_new_from_posix_time(
	                   posix_time );

	return( datetime_object );
}

/* Retrieves the last_mount date and time as an integer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_volume_get_last_mount_time_as_integer(
           pyfsfat_volume_t *pyfsfat_volume,
           PyObject *arguments PYFSFAT_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsfat_volume_get_last_mount_time_as_integer";
	int32_t posix_time       = 0;
	int result               = 0;

	PYFSFAT_UNREFERENCED_PARAMETER( arguments )

	if( pyfsfat_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsfat_volume_get_last_mount_time(
	          pyfsfat_volume->volume,
	          &posix_time,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve last mount date and time.",
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
	integer_object = PyLong_FromUnsignedLong(
	                  (unsigned long) posix_time );

	return( integer_object );
}

/* Retrieves the last_written date and time
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_volume_get_last_written_time(
           pyfsfat_volume_t *pyfsfat_volume,
           PyObject *arguments PYFSFAT_ATTRIBUTE_UNUSED )
{
	PyObject *datetime_object = NULL;
	libcerror_error_t *error  = NULL;
	static char *function     = "pyfsfat_volume_get_last_written_time";
	int32_t posix_time        = 0;
	int result                = 0;

	PYFSFAT_UNREFERENCED_PARAMETER( arguments )

	if( pyfsfat_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsfat_volume_get_last_written_time(
	          pyfsfat_volume->volume,
	          &posix_time,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve last written date and time.",
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
	datetime_object = pyfsfat_datetime_new_from_posix_time(
	                   posix_time );

	return( datetime_object );
}

/* Retrieves the last_written date and time as an integer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_volume_get_last_written_time_as_integer(
           pyfsfat_volume_t *pyfsfat_volume,
           PyObject *arguments PYFSFAT_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsfat_volume_get_last_written_time_as_integer";
	int32_t posix_time       = 0;
	int result               = 0;

	PYFSFAT_UNREFERENCED_PARAMETER( arguments )

	if( pyfsfat_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsfat_volume_get_last_written_time(
	          pyfsfat_volume->volume,
	          &posix_time,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve last written date and time.",
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
	integer_object = PyLong_FromUnsignedLong(
	                  (unsigned long) posix_time );

	return( integer_object );
}

/* Retrieves the number of file entries (MFT entries)
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_volume_get_number_of_file_entries(
           pyfsfat_volume_t *pyfsfat_volume,
           PyObject *arguments PYFSFAT_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfsfat_volume_get_number_of_file_entries";
	uint32_t value_32bit     = 0;
	int result               = 0;

	PYFSFAT_UNREFERENCED_PARAMETER( arguments )

	if( pyfsfat_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsfat_volume_get_number_of_file_entries(
	          pyfsfat_volume->volume,
	          &value_32bit,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of file entries.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = PyLong_FromUnsignedLong(
	                  (unsigned long) value_32bit );

	return( integer_object );
}

/* Retrieves a specific of file entry by inode
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_volume_get_file_entry_by_inode(
           pyfsfat_volume_t *pyfsfat_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *file_entry_object       = NULL;
	libcerror_error_t *error          = NULL;
	libfsfat_file_entry_t *file_entry = NULL;
	static char *function             = "pyfsfat_volume_get_file_entry_by_inode";
	static char *keyword_list[]       = { "file_entry_inode", NULL };
	unsigned long long inode_number   = 0;
	int result                        = 0;

	if( pyfsfat_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "K",
	     keyword_list,
	     &inode_number ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsfat_volume_get_file_entry_by_inode(
	          ( (pyfsfat_volume_t *) pyfsfat_volume )->volume,
	          (uint64_t) inode_number,
	          &file_entry,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve of file entry: %d.",
		 function,
		 inode_number );

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
	                     file_entry,
	                     (PyObject *) pyfsfat_volume );

	if( file_entry_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create file entry object.",
		 function );

		goto on_error;
	}
	return( file_entry_object );

on_error:
	if( file_entry != NULL )
	{
		libfsfat_file_entry_free(
		 &file_entry,
		 NULL );
	}
	return( NULL );
}

/* Retrieves the root root directory file entry
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_volume_get_root_directory(
           pyfsfat_volume_t *pyfsfat_volume,
           PyObject *arguments PYFSFAT_ATTRIBUTE_UNUSED )
{
	PyObject *directory_object            = NULL;
	libcerror_error_t *error              = NULL;
	libfsfat_file_entry_t *root_directory = NULL;
	static char *function                 = "pyfsfat_volume_get_root_directory";
	int result                            = 0;

	PYFSFAT_UNREFERENCED_PARAMETER( arguments )

	if( pyfsfat_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfsfat_volume_get_root_directory(
	          pyfsfat_volume->volume,
	          &root_directory,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve root root directory file entry.",
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
	directory_object = pyfsfat_file_entry_new(
	                    root_directory,
	                    (PyObject *) pyfsfat_volume );

	if( directory_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create root directory file entry object.",
		 function );

		goto on_error;
	}
	return( directory_object );

on_error:
	if( root_directory != NULL )
	{
		libfsfat_file_entry_free(
		 &root_directory,
		 NULL );
	}
	return( NULL );
}

/* Retrieves the file entry for an UTF-8 encoded path specified by the path
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfsfat_volume_get_file_entry_by_path(
           pyfsfat_volume_t *pyfsfat_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *file_entry_object       = NULL;
	libcerror_error_t *error          = NULL;
	libfsfat_file_entry_t *file_entry = NULL;
	static char *function             = "pyfsfat_volume_get_file_entry_by_path";
	static char *keyword_list[]       = { "path", NULL };
	char *utf8_path                   = NULL;
	size_t utf8_path_length           = 0;
	int result                        = 0;

	if( pyfsfat_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s",
	     keyword_list,
	     &utf8_path ) == 0 )
	{
		goto on_error;
	}
	utf8_path_length = narrow_string_length(
	                    utf8_path );

	Py_BEGIN_ALLOW_THREADS

	result = libfsfat_volume_get_file_entry_by_utf8_path(
	          pyfsfat_volume->volume,
	          (uint8_t *) utf8_path,
	          utf8_path_length,
	          &file_entry,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfsfat_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve file entry for an UTF-8 encoded path.",
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
	                     file_entry,
	                     (PyObject *) pyfsfat_volume );

	if( file_entry_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create file entry object.",
		 function );

		goto on_error;
	}
	return( file_entry_object );

on_error:
	if( file_entry != NULL )
	{
		libfsfat_file_entry_free(
		 &file_entry,
		 NULL );
	}
	return( NULL );
}

