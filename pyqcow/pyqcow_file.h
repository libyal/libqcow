/*
 * Python object wrapper of libqcow_file_t
 *
 * Copyright (C) 2010-2021, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _PYQCOW_FILE_H )
#define _PYQCOW_FILE_H

#include <common.h>
#include <types.h>

#include "pyqcow_libbfio.h"
#include "pyqcow_libqcow.h"
#include "pyqcow_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyqcow_file pyqcow_file_t;

struct pyqcow_file
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libqcow file
	 */
	libqcow_file_t *file;

	/* The libbfio file IO handle
	 */
	libbfio_handle_t *file_io_handle;
};

extern PyMethodDef pyqcow_file_object_methods[];
extern PyTypeObject pyqcow_file_type_object;

int pyqcow_file_init(
     pyqcow_file_t *pyqcow_file );

void pyqcow_file_free(
      pyqcow_file_t *pyqcow_file );

PyObject *pyqcow_file_signal_abort(
           pyqcow_file_t *pyqcow_file,
           PyObject *arguments );

PyObject *pyqcow_file_open(
           pyqcow_file_t *pyqcow_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyqcow_file_open_file_object(
           pyqcow_file_t *pyqcow_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyqcow_file_close(
           pyqcow_file_t *pyqcow_file,
           PyObject *arguments );

PyObject *pyqcow_file_is_locked(
           pyqcow_file_t *pyqcow_file,
           PyObject *arguments );

PyObject *pyqcow_file_read_buffer(
           pyqcow_file_t *pyqcow_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyqcow_file_read_buffer_at_offset(
           pyqcow_file_t *pyqcow_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyqcow_file_seek_offset(
           pyqcow_file_t *pyqcow_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyqcow_file_get_offset(
           pyqcow_file_t *pyqcow_file,
           PyObject *arguments );

PyObject *pyqcow_file_set_parent(
           pyqcow_file_t *pyqcow_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyqcow_file_set_password(
           pyqcow_file_t *pyqcow_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyqcow_file_get_media_size(
           pyqcow_file_t *pyqcow_file,
           PyObject *arguments );

PyObject *pyqcow_file_get_backing_filename(
           pyqcow_file_t *pyqcow_file,
           PyObject *arguments );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYQCOW_FILE_H ) */

