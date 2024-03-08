/*
 * Tools signal functions test program
 *
 * Copyright (C) 2010-2024, Joachim Metz <joachim.metz@gmail.com>
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
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "qcow_test_libcerror.h"
#include "qcow_test_macros.h"
#include "qcow_test_unused.h"

#include "../qcowtools/qcowtools_signal.h"

void qcow_test_tools_signal_handler(
      qcowtools_signal_t signal QCOW_TEST_ATTRIBUTE_UNUSED )
{
	QCOW_TEST_UNREFERENCED_PARAMETER( signal )
}

/* Tests the qcowtools_signal_attach and function
 * Returns 1 if successful or 0 if not
 */
int qcow_test_tools_signal_attach(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	result = qcowtools_signal_attach(
	          qcow_test_tools_signal_handler,
	          &error );

	QCOW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	QCOW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the qcowtools_signal_detach and function
 * Returns 1 if successful or 0 if not
 */
int qcow_test_tools_signal_detach(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	result = qcowtools_signal_detach(
	          &error );

	QCOW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	QCOW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc QCOW_TEST_ATTRIBUTE_UNUSED,
     wchar_t * const argv[] QCOW_TEST_ATTRIBUTE_UNUSED )
#else
int main(
     int argc QCOW_TEST_ATTRIBUTE_UNUSED,
     char * const argv[] QCOW_TEST_ATTRIBUTE_UNUSED )
#endif
{
	QCOW_TEST_UNREFERENCED_PARAMETER( argc )
	QCOW_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( WINAPI )

	/* TODO add tests for qcowtools_signal_handler */
#endif

#if defined( WINAPI ) && defined( _MSC_VER )

	/* TODO add tests for qcowtools_signal_initialize_memory_debug */
#endif

	QCOW_TEST_RUN(
	 "qcowtools_signal_attach",
	 qcow_test_tools_signal_attach )

	QCOW_TEST_RUN(
	 "qcowtools_signal_detach",
	 qcow_test_tools_signal_detach )

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

