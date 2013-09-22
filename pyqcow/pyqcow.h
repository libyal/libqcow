/*
 * Python bindings for libqcow (pyqcow)
 *
 * Copyright (c) 2010-2013, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _PYQCOW_H )
#define _PYQCOW_H

#include <common.h>
#include <types.h>

#include "pyqcow_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

#define PYQCOW_ERROR_STRING_SIZE		512

PyObject *pyqcow_get_version(
           PyObject *self,
           PyObject *arguments );

PyObject *pyqcow_check_file_signature(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyqcow_check_file_signature_file_object(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords );

PyMODINIT_FUNC initpyqcow(
                void );

#if defined( __cplusplus )
}
#endif

#endif

