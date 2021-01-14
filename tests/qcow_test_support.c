/*
 * Library support functions test program
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
#include <file_stream.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "qcow_test_functions.h"
#include "qcow_test_getopt.h"
#include "qcow_test_libbfio.h"
#include "qcow_test_libcerror.h"
#include "qcow_test_libqcow.h"
#include "qcow_test_macros.h"
#include "qcow_test_memory.h"
#include "qcow_test_unused.h"

#if !defined( LIBQCOW_HAVE_BFIO )

LIBQCOW_EXTERN \
int libqcow_check_file_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

#endif /* !defined( LIBQCOW_HAVE_BFIO ) */

/* Tests the libqcow_get_version function
 * Returns 1 if successful or 0 if not
 */
int qcow_test_get_version(
     void )
{
	const char *version_string = NULL;
	int result                 = 0;

	version_string = libqcow_get_version();

	result = narrow_string_compare(
	          version_string,
	          LIBQCOW_VERSION_STRING,
	          9 );

	QCOW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	return( 1 );

on_error:
	return( 0 );
}

/* Tests the libqcow_get_access_flags_read function
 * Returns 1 if successful or 0 if not
 */
int qcow_test_get_access_flags_read(
     void )
{
	int access_flags = 0;

	access_flags = libqcow_get_access_flags_read();

	QCOW_TEST_ASSERT_EQUAL_INT(
	 "access_flags",
	 access_flags,
	 LIBQCOW_ACCESS_FLAG_READ );

	return( 1 );

on_error:
	return( 0 );
}

/* Tests the libqcow_get_codepage function
 * Returns 1 if successful or 0 if not
 */
int qcow_test_get_codepage(
     void )
{
	libcerror_error_t *error = NULL;
	int codepage             = 0;
	int result               = 0;

	result = libqcow_get_codepage(
	          &codepage,
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
	result = libqcow_get_codepage(
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

/* Tests the libqcow_set_codepage function
 * Returns 1 if successful or 0 if not
 */
int qcow_test_set_codepage(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	result = libqcow_set_codepage(
	          0,
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
	result = libqcow_set_codepage(
	          -1,
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

/* Tests the libqcow_check_file_signature function
 * Returns 1 if successful or 0 if not
 */
int qcow_test_check_file_signature(
     const system_character_t *source )
{
	char narrow_source[ 256 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	if( source != NULL )
	{
		/* Initialize test
		 */
		result = qcow_test_get_narrow_source(
		          source,
		          narrow_source,
		          256,
		          &error );

		QCOW_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		QCOW_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Test check file signature
		 */
		result = libqcow_check_file_signature(
		          narrow_source,
		          &error );

		QCOW_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		QCOW_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	result = libqcow_check_file_signature(
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

	result = libqcow_check_file_signature(
	          "",
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

	if( source != NULL )
	{
#if defined( HAVE_QCOW_TEST_MEMORY )

		/* Test libqcow_check_file_signature with malloc failing in libbfio_file_initialize
		 */
		qcow_test_malloc_attempts_before_fail = 0;

		result = libqcow_check_file_signature(
		          narrow_source,
		          &error );

		if( qcow_test_malloc_attempts_before_fail != -1 )
		{
			qcow_test_malloc_attempts_before_fail = -1;
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
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Tests the libqcow_check_file_signature_wide function
 * Returns 1 if successful or 0 if not
 */
int qcow_test_check_file_signature_wide(
     const system_character_t *source )
{
	wchar_t wide_source[ 256 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	if( source != NULL )
	{
		/* Initialize test
		 */
		result = qcow_test_get_wide_source(
		          source,
		          wide_source,
		          256,
		          &error );

		QCOW_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		QCOW_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Test check file signature
		 */
		result = libqcow_check_file_signature_wide(
		          wide_source,
		          &error );

		QCOW_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		QCOW_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	result = libqcow_check_file_signature_wide(
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

	result = libqcow_check_file_signature_wide(
	          L"",
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

	if( source != NULL )
	{
#if defined( HAVE_QCOW_TEST_MEMORY )

		/* Test libqcow_check_file_signature_wide with malloc failing in libbfio_file_initialize
		 */
		qcow_test_malloc_attempts_before_fail = 0;

		result = libqcow_check_file_signature_wide(
		          wide_source,
		          &error );

		if( qcow_test_malloc_attempts_before_fail != -1 )
		{
			qcow_test_malloc_attempts_before_fail = -1;
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
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Tests the libqcow_check_file_signature_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int qcow_test_check_file_signature_file_io_handle(
     const system_character_t *source )
{
	uint8_t empty_block[ 8192 ];

	libbfio_handle_t *file_io_handle = NULL;
	libcerror_error_t *error         = NULL;
	void *memset_result              = NULL;
	size_t source_length             = 0;
	int result                       = 0;

	/* Initialize test
	 */
	memset_result = memory_set(
	                 empty_block,
	                 0,
	                 sizeof( uint8_t ) * 8192 );

	QCOW_TEST_ASSERT_IS_NOT_NULL(
	 "memset_result",
	 memset_result );

	if( source != NULL )
	{
		/* Initialize test
		 */
		result = libbfio_file_initialize(
		          &file_io_handle,
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

		source_length = system_string_length(
		                 source );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libbfio_file_set_name_wide(
		          file_io_handle,
		          source,
		          source_length,
		          &error );
#else
		result = libbfio_file_set_name(
		          file_io_handle,
		          source,
		          source_length,
		          &error );
#endif
		QCOW_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		QCOW_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		result = libbfio_handle_open(
		          file_io_handle,
		          LIBBFIO_OPEN_READ,
		          &error );

		QCOW_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		QCOW_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Test check file signature
		 */
		result = libqcow_check_file_signature_file_io_handle(
		          file_io_handle,
		          &error );

		QCOW_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		QCOW_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	result = libqcow_check_file_signature_file_io_handle(
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

	/* Clean up
	 */
	if( source != NULL )
	{
		result = libbfio_handle_close(
		          file_io_handle,
		          &error );

		QCOW_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 0 );

		QCOW_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		result = libbfio_handle_free(
		          &file_io_handle,
		          &error );

		QCOW_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		QCOW_TEST_ASSERT_IS_NULL(
		 "file_io_handle",
		 file_io_handle );

		QCOW_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test check file signature with data too small
	 */
	result = qcow_test_open_file_io_handle(
	          &file_io_handle,
	          empty_block,
	          sizeof( uint8_t ) * 1,
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

	result = libqcow_check_file_signature_file_io_handle(
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

	/* Test check file signature with empty block
	 */
	result = qcow_test_open_file_io_handle(
	          &file_io_handle,
	          empty_block,
	          sizeof( uint8_t ) * 8192,
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

	result = libqcow_check_file_signature_file_io_handle(
	          file_io_handle,
	          &error );

	QCOW_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	QCOW_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

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
	return( 0 );
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc,
     wchar_t * const argv[] )
#else
int main(
     int argc,
     char * const argv[] )
#endif
{
	system_character_t *source = NULL;
	system_integer_t option    = 0;

	while( ( option = qcow_test_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "p:" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM ".\n",
				 argv[ optind - 1 ] );

				return( EXIT_FAILURE );

			case (system_integer_t) 'p':
				break;
		}
	}
	if( optind < argc )
	{
		source = argv[ optind ];
	}
	QCOW_TEST_RUN(
	 "libqcow_get_version",
	 qcow_test_get_version );

	QCOW_TEST_RUN(
	 "libqcow_get_access_flags_read",
	 qcow_test_get_access_flags_read );

	QCOW_TEST_RUN(
	 "libqcow_get_codepage",
	 qcow_test_get_codepage );

	QCOW_TEST_RUN(
	 "libqcow_set_codepage",
	 qcow_test_set_codepage );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	QCOW_TEST_RUN_WITH_ARGS(
	 "libqcow_check_file_signature",
	 qcow_test_check_file_signature,
	 source );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

	QCOW_TEST_RUN_WITH_ARGS(
	 "libqcow_check_file_signature_wide",
	 qcow_test_check_file_signature_wide,
	 source );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

	QCOW_TEST_RUN_WITH_ARGS(
	 "libqcow_check_file_signature_file_io_handle",
	 qcow_test_check_file_signature_file_io_handle,
	 source );

#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

