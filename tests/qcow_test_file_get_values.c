/*
 * Library file type get values testing program
 *
 * Copyright (C) 2010-2017, Joachim Metz <joachim.metz@gmail.com>
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
#include <file_stream.h>
#include <system_string.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "qcow_test_libcerror.h"
#include "qcow_test_libcsystem.h"
#include "qcow_test_libqcow.h"
#include "qcow_test_unused.h"

/* Define to make qcow_test_file_get_values generate verbose output
#define QCOW_TEST_FILE_GET_VALUES_VERBOSE
 */

/* Tests retrieving offset from the file
 * Returns 1 if successful, 0 if not or -1 on error
 */
int qcow_test_file_get_offset(
     libqcow_file_t *file,
     libcerror_error_t **error )
{
	off64_t offset = 0;
	int result     = 0;

	fprintf(
	 stdout,
	 "Testing libqcow_file_get_offset\t" );

	result = libqcow_file_get_offset(
	          file,
	          &offset,
	          error );

	if( result == 1 )
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve offset.\n" );

		return( -1 );
	}
	return( 1 );
}

/* Tests retrieving media size from the file
 * Returns 1 if successful, 0 if not or -1 on error
 */
int qcow_test_file_get_media_size(
     libqcow_file_t *file,
     libcerror_error_t **error )
{
	size64_t media_size = 0;
	int result          = 0;

	fprintf(
	 stdout,
	 "Testing libqcow_file_get_media_size\t" );

	result = libqcow_file_get_media_size(
	          file,
	          &media_size,
	          error );

	if( result == 1 )
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve media size.\n" );

		return( -1 );
	}
	return( 1 );
}

/* Tests retrieving format version from the file
 * Returns 1 if successful, 0 if not or -1 on error
 */
int qcow_test_file_get_format_version(
     libqcow_file_t *file,
     libcerror_error_t **error )
{
	uint32_t format_version = 0;
	int result              = 0;

	fprintf(
	 stdout,
	 "Testing libqcow_file_get_format_version\t" );

	result = libqcow_file_get_format_version(
	          file,
	          &format_version,
	          error );

	if( result == 1 )
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve format version.\n" );

		return( -1 );
	}
	return( 1 );
}

/* Tests retrieving encryption method from the file
 * Returns 1 if successful, 0 if not or -1 on error
 */
int qcow_test_file_get_encryption_method(
     libqcow_file_t *file,
     libcerror_error_t **error )
{
	uint32_t encryption_method = 0;
	int result                 = 0;

	fprintf(
	 stdout,
	 "Testing libqcow_file_get_encryption_method\t" );

	result = libqcow_file_get_encryption_method(
	          file,
	          &encryption_method,
	          error );

	if( result == 1 )
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve encryption method.\n" );

		return( -1 );
	}
	return( 1 );
}

/* Tests retrieving values from the file
 * Returns 1 if successful, 0 if not or -1 on error
 */
int qcow_test_file_get_values(
     system_character_t *source,
     libcerror_error_t **error )
{
	libqcow_file_t *file = NULL;
	size_t string_length = 0;
	int result           = 0;

	if( libqcow_file_initialize(
	     &file,
	     error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to create file.\n" );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libqcow_file_open_wide(
	     file,
	     source,
	     LIBQCOW_OPEN_READ,
	     error ) != 1 )
#else
	if( libqcow_file_open(
	     file,
	     source,
	     LIBQCOW_OPEN_READ,
	     error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Unable to open file.\n" );

		goto on_error;
	}
	result = qcow_test_file_get_offset(
	          file,
	          error );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve offset.\n" );

		goto on_error;
	}
	result = qcow_test_file_get_media_size(
	          file,
	          error );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve media size.\n" );

		goto on_error;
	}
	result = qcow_test_file_get_format_version(
	          file,
	          error );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve format version.\n" );

		goto on_error;
	}
	result = qcow_test_file_get_encryption_method(
	          file,
	          error );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve encryption method.\n" );

		goto on_error;
	}
	if( libqcow_file_close(
	     file,
	     error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close file.\n" );

		goto on_error;
	}
	if( libqcow_file_free(
	     &file,
	     error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free file.\n" );

		goto on_error;
	}
	return( result );

on_error:
	if( file != NULL )
	{
		libqcow_file_close(
		 file,
		 NULL );
		libqcow_file_free(
		 &file,
		 NULL );
	}
	return( -1 );
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libcerror_error_t *error   = NULL;
	system_character_t *source = NULL;
	system_integer_t option    = 0;
	int result                 = 0;

	while( ( option = libcsystem_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "" ) ) ) != (system_integer_t) -1 )
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
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing source file or device.\n" );

		return( EXIT_FAILURE );
	}
	source = argv[ optind ];

#if defined( HAVE_DEBUG_OUTPUT ) && defined( QCOW_TEST_FILE_GET_VALUES_VERBOSE )
	libqcow_notify_set_verbose(
	 1 );
	libqcow_notify_set_stream(
	 stderr,
	 NULL );
#endif
	result = qcow_test_file_get_values(
	          source,
	          &error );

	if( result != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to retrieve values from file.\n" );

		goto on_error;
	}
	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcerror_error_backtrace_fprint(
		 error,
		 stderr );
		libcerror_error_free(
		 &error );
	}
	return( EXIT_FAILURE );
}

