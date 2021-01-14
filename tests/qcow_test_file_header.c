/*
 * Library file_header type test program
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

#include <common.h>
#include <byte_stream.h>
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "qcow_test_functions.h"
#include "qcow_test_libbfio.h"
#include "qcow_test_libcerror.h"
#include "qcow_test_libqcow.h"
#include "qcow_test_macros.h"
#include "qcow_test_memory.h"
#include "qcow_test_unused.h"

#include "../libqcow/libqcow_file_header.h"

uint8_t qcow_test_file_header_data1[ 512 ] = {
	0x51, 0x46, 0x49, 0xfb, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x68, 0x68, 0x03, 0xf8, 0x57, 0x00, 0x00, 0x00, 0x90,
	0x00, 0x00, 0x64, 0x69, 0x72, 0x74, 0x79, 0x20, 0x62, 0x69, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x01, 0x63, 0x6f, 0x72, 0x72, 0x75, 0x70, 0x74, 0x20, 0x62, 0x69, 0x74, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x6c, 0x61, 0x7a, 0x79, 0x20, 0x72, 0x65, 0x66, 0x63, 0x6f, 0x75, 0x6e, 0x74, 0x73,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBQCOW_DLL_IMPORT )

/* Tests the libqcow_file_header_initialize function
 * Returns 1 if successful or 0 if not
 */
int qcow_test_file_header_initialize(
     void )
{
	libcerror_error_t *error          = NULL;
	libqcow_file_header_t *file_header = NULL;
	int result                        = 0;

#if defined( HAVE_QCOW_TEST_MEMORY )
	int number_of_malloc_fail_tests   = 1;
	int number_of_memset_fail_tests   = 1;
	int test_number                   = 0;
#endif

	/* Test regular cases
	 */
	result = libqcow_file_header_initialize(
	          &file_header,
	          &error );

	QCOW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	QCOW_TEST_ASSERT_IS_NOT_NULL(
	 "file_header",
	 file_header );

	QCOW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libqcow_file_header_free(
	          &file_header,
	          &error );

	QCOW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	QCOW_TEST_ASSERT_IS_NULL(
	 "file_header",
	 file_header );

	QCOW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libqcow_file_header_initialize(
	          NULL,
	          &error );

	QCOW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	QCOW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	file_header = (libqcow_file_header_t *) 0x12345678UL;

	result = libqcow_file_header_initialize(
	          &file_header,
	          &error );

	file_header = NULL;

	QCOW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	QCOW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_QCOW_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libqcow_file_header_initialize with malloc failing
		 */
		qcow_test_malloc_attempts_before_fail = test_number;

		result = libqcow_file_header_initialize(
		          &file_header,
		          &error );

		if( qcow_test_malloc_attempts_before_fail != -1 )
		{
			qcow_test_malloc_attempts_before_fail = -1;

			if( file_header != NULL )
			{
				libqcow_file_header_free(
				 &file_header,
				 NULL );
			}
		}
		else
		{
			QCOW_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			QCOW_TEST_ASSERT_IS_NULL(
			 "file_header",
			 file_header );

			QCOW_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
	for( test_number = 0;
	     test_number < number_of_memset_fail_tests;
	     test_number++ )
	{
		/* Test libqcow_file_header_initialize with memset failing
		 */
		qcow_test_memset_attempts_before_fail = test_number;

		result = libqcow_file_header_initialize(
		          &file_header,
		          &error );

		if( qcow_test_memset_attempts_before_fail != -1 )
		{
			qcow_test_memset_attempts_before_fail = -1;

			if( file_header != NULL )
			{
				libqcow_file_header_free(
				 &file_header,
				 NULL );
			}
		}
		else
		{
			QCOW_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			QCOW_TEST_ASSERT_IS_NULL(
			 "file_header",
			 file_header );

			QCOW_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_QCOW_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( file_header != NULL )
	{
		libqcow_file_header_free(
		 &file_header,
		 NULL );
	}
	return( 0 );
}

/* Tests the libqcow_file_header_free function
 * Returns 1 if successful or 0 if not
 */
int qcow_test_file_header_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libqcow_file_header_free(
	          NULL,
	          &error );

	QCOW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	QCOW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libqcow_file_header_read_data function
 * Returns 1 if successful or 0 if not
 */
int qcow_test_file_header_read_data(
     void )
{
	libcerror_error_t *error          = NULL;
	libqcow_file_header_t *file_header = NULL;
	int result                        = 0;

	/* Initialize test
	 */
	result = libqcow_file_header_initialize(
	          &file_header,
	          &error );

	QCOW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	QCOW_TEST_ASSERT_IS_NOT_NULL(
	 "file_header",
	 file_header );

	QCOW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libqcow_file_header_read_data(
	          file_header,
	          qcow_test_file_header_data1,
	          512,
	          &error );

	QCOW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	QCOW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	QCOW_TEST_ASSERT_EQUAL_UINT64(
	 "file_header->media_size",
	 file_header->media_size,
	 (uint64_t) 4194304 );

	/* Test error cases
	 */
	result = libqcow_file_header_read_data(
	          NULL,
	          qcow_test_file_header_data1,
	          512,
	          &error );

	QCOW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	QCOW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libqcow_file_header_read_data(
	          file_header,
	          NULL,
	          512,
	          &error );

	QCOW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	QCOW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libqcow_file_header_read_data(
	          file_header,
	          qcow_test_file_header_data1,
	          0,
	          &error );

	QCOW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	QCOW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libqcow_file_header_read_data(
	          file_header,
	          qcow_test_file_header_data1,
	          (size_t) SSIZE_MAX + 1,
	          &error );

	QCOW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	QCOW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_QCOW_TEST_MEMORY )

	/* Test qcow_test_file_header_read_data with memcpy failing
	 */
	qcow_test_memcpy_attempts_before_fail = 0;

	result = libqcow_file_header_read_data(
	          file_header,
	          qcow_test_file_header_data1,
	          512,
	          &error );

	if( qcow_test_memcpy_attempts_before_fail != -1 )
	{
		qcow_test_memcpy_attempts_before_fail = -1;
	}
	else
	{
		QCOW_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		QCOW_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_QCOW_TEST_MEMORY ) */

	/* Test error case where the signature is invalid
	 */
	byte_stream_copy_from_uint32_big_endian(
	 qcow_test_file_header_data1,
	 0xffffffffUL );

	result = libqcow_file_header_read_data(
	          file_header,
	          qcow_test_file_header_data1,
	          512,
	          &error );

	byte_stream_copy_from_uint32_big_endian(
	 qcow_test_file_header_data1,
	 0x514649fbUL );

	QCOW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	QCOW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libqcow_file_header_free(
	          &file_header,
	          &error );

	QCOW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	QCOW_TEST_ASSERT_IS_NULL(
	 "file_header",
	 file_header );

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
	if( file_header != NULL )
	{
		libqcow_file_header_free(
		 &file_header,
		 NULL );
	}
	return( 0 );
}

/* Tests the libqcow_file_header_read_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int qcow_test_file_header_read_file_io_handle(
     void )
{
	libbfio_handle_t *file_io_handle  = NULL;
	libcerror_error_t *error          = NULL;
	libqcow_file_header_t *file_header = NULL;
	int result                        = 0;

	/* Initialize test
	 */
	result = libqcow_file_header_initialize(
	          &file_header,
	          &error );

	QCOW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	QCOW_TEST_ASSERT_IS_NOT_NULL(
	 "file_header",
	 file_header );

	QCOW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize file IO handle
	 */
	result = qcow_test_open_file_io_handle(
	          &file_io_handle,
	          qcow_test_file_header_data1,
	          512,
	          &error );

	QCOW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	QCOW_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_handle",
	 file_io_handle );

	QCOW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libqcow_file_header_read_file_io_handle(
	          file_header,
	          file_io_handle,
	          &error );

	QCOW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	QCOW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libqcow_file_header_read_file_io_handle(
	          NULL,
	          file_io_handle,
	          &error );

	QCOW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	QCOW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libqcow_file_header_read_file_io_handle(
	          file_header,
	          NULL,
	          &error );

	QCOW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	QCOW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up file IO handle
	 */
	result = qcow_test_close_file_io_handle(
	          &file_io_handle,
	          &error );

	QCOW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	QCOW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test data too small
	 */
	result = qcow_test_open_file_io_handle(
	          &file_io_handle,
	          qcow_test_file_header_data1,
	          8,
	          &error );

	QCOW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	QCOW_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_handle",
	 file_io_handle );

	QCOW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libqcow_file_header_read_file_io_handle(
	          file_header,
	          file_io_handle,
	          &error );

	QCOW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	QCOW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = qcow_test_close_file_io_handle(
	          &file_io_handle,
	          &error );

	QCOW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	QCOW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test data invalid
	 */
	result = qcow_test_open_file_io_handle(
	          &file_io_handle,
	          qcow_test_file_header_data1,
	          512,
	          &error );

	QCOW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	QCOW_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_handle",
	 file_io_handle );

	QCOW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	byte_stream_copy_from_uint32_big_endian(
	 qcow_test_file_header_data1,
	 0xffffffffUL );

	result = libqcow_file_header_read_file_io_handle(
	          file_header,
	          file_io_handle,
	          &error );

	byte_stream_copy_from_uint32_big_endian(
	 qcow_test_file_header_data1,
	 0x514649fbUL );

	QCOW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	QCOW_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = qcow_test_close_file_io_handle(
	          &file_io_handle,
	          &error );

	QCOW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	QCOW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libqcow_file_header_free(
	          &file_header,
	          &error );

	QCOW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	QCOW_TEST_ASSERT_IS_NULL(
	 "file_header",
	 file_header );

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
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	if( file_header != NULL )
	{
		libqcow_file_header_free(
		 &file_header,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) && !defined( LIBQCOW_DLL_IMPORT ) */

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

#if defined( __GNUC__ ) && !defined( LIBQCOW_DLL_IMPORT )

	QCOW_TEST_RUN(
	 "libqcow_file_header_initialize",
	 qcow_test_file_header_initialize );

	QCOW_TEST_RUN(
	 "libqcow_file_header_free",
	 qcow_test_file_header_free );

	QCOW_TEST_RUN(
	 "libqcow_file_header_read_data",
	 qcow_test_file_header_read_data );

	QCOW_TEST_RUN(
	 "libqcow_file_header_read_file_io_handle",
	 qcow_test_file_header_read_file_io_handle );

#endif /* defined( __GNUC__ ) && !defined( LIBQCOW_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

