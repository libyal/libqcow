/*
 * Integer functions
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

#include <common.h>
#include <types.h>

#include "pyqcow_error.h"
#include "pyqcow_libcerror.h"
#include "pyqcow_libcstring.h"
#include "pyqcow_python.h"

#define PYQCOW_ERROR_FORMAT_STRING_SIZE	128
#define PYQCOW_ERROR_STRING_SIZE	512

/* Raises an error
 */
void pyqcow_error_raise(
      PyObject *exception_object,
      const char *format_string,
      const char *function,
      libcerror_error_t *error )
{
	char error_string[ PYQCOW_ERROR_STRING_SIZE ];
	char extended_format_string[ PYQCOW_ERROR_FORMAT_STRING_SIZE ];

	size_t error_string_index   = 0;
	size_t format_string_length = 0;

	if( function == NULL )
	{
		function = "pyqcow_error_raise";
	}
       	if( format_string == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: missing format string.",
		 function );

		return;
	}
	if( error != NULL )
	{
		format_string_length = libcstring_narrow_string_length(
		                        format_string );

		if( format_string_length < ( PYQCOW_ERROR_FORMAT_STRING_SIZE - 4 ) )
		{
			if( libcerror_error_backtrace_sprint(
			     error,
			     error_string,
			     PYQCOW_ERROR_STRING_SIZE ) != -1 )
			{
				if( memory_copy(
				     extended_format_string,
				     format_string,
				     format_string_length ) != NULL )
				{
					extended_format_string[ format_string_length++ ] = ' ';
					extended_format_string[ format_string_length++ ] = '%';
					extended_format_string[ format_string_length++ ] = 's';
					extended_format_string[ format_string_length ] = 0;

					for( error_string_index = 0;
					     error_string_index < PYQCOW_ERROR_STRING_SIZE;
					     error_string_index++ )
					{
						if( error_string[ error_string_index ] == 0 )
						{
							break;
						}
						if( error_string[ error_string_index ] == '\n' )
						{
							error_string[ error_string_index ] = ' ';
						}
					}
					PyErr_Format(
					 exception_object,
					 extended_format_string,
					 function,
					 error_string );

					return;
				}
			}
		}
	}
	PyErr_Format(
	 exception_object,
	 format_string,
	 function );

	return;
}

