/*
 * Library notification functions test program
 *
 * Copyright (C) 2010-2016, Joachim Metz <joachim.metz@gmail.com>
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

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "qcow_test_libcerror.h"
#include "qcow_test_libqcow.h"
#include "qcow_test_macros.h"
#include "qcow_test_unused.h"

/* Tests the libqcow_notify_set_verbose function
 * Returns 1 if successful or 0 if not
 */
int qcow_test_notify_set_verbose(
     void )
{
	/* Test invocation of function only
	 */
	libqcow_notify_set_verbose(
	 0 );

	return( 1 );
}

/* Tests the libqcow_notify_set_stream function
 * Returns 1 if successful or 0 if not
 */
int qcow_test_notify_set_stream(
     void )
{
	/* Test invocation of function only
	 */
	libqcow_notify_set_stream(
	 NULL,
	 NULL );

	return( 1 );
}

/* Tests the libqcow_notify_stream_open function
 * Returns 1 if successful or 0 if not
 */
int qcow_test_notify_stream_open(
     void )
{
	/* Test invocation of function only
	 */
	libqcow_notify_stream_open(
	 NULL,
	 NULL );

	return( 1 );
}

/* Tests the libqcow_notify_stream_close function
 * Returns 1 if successful or 0 if not
 */
int qcow_test_notify_stream_close(
     void )
{
	/* Test invocation of function only
	 */
	libqcow_notify_stream_close(
	 NULL );

	return( 1 );
}

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
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

	QCOW_TEST_RUN(
	 "libqcow_notify_set_verbose",
	 qcow_test_notify_set_verbose )

	QCOW_TEST_RUN(
	 "libqcow_notify_set_stream",
	 qcow_test_notify_set_stream )

	QCOW_TEST_RUN(
	 "libqcow_notify_stream_open",
	 qcow_test_notify_stream_open )

	QCOW_TEST_RUN(
	 "libqcow_notify_stream_close",
	 qcow_test_notify_stream_close )

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

