/*
 * Info handle
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
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "byte_size_string.h"
#include "info_handle.h"
#include "qcowtools_libcerror.h"
#include "qcowtools_libcnotify.h"
#include "qcowtools_libqcow.h"

#define INFO_HANDLE_NOTIFY_STREAM		stdout

/* Creates an info handle
 * Make sure the value mount_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int info_handle_initialize(
     info_handle_t **info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_initialize";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( *info_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid info handle value already set.",
		 function );

		return( -1 );
	}
	*info_handle = memory_allocate_structure(
	                info_handle_t );

	if( *info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create info handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *info_handle,
	     0,
	     sizeof( info_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear info handle.",
		 function );

		goto on_error;
	}
	if( libqcow_file_initialize(
	     &( ( *info_handle )->input_file ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize input file.",
		 function );

		goto on_error;
	}
	( *info_handle )->notify_stream = INFO_HANDLE_NOTIFY_STREAM;

	return( 1 );

on_error:
	if( *info_handle != NULL )
	{
		memory_free(
		 *info_handle );

		*info_handle = NULL;
	}
	return( -1 );
}

/* Frees an info handle
 * Returns 1 if successful or -1 on error
 */
int info_handle_free(
     info_handle_t **info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_free";
	int result            = 1;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( *info_handle != NULL )
	{
		if( ( *info_handle )->input_file != NULL )
		{
			if( libqcow_file_free(
			     &( ( *info_handle )->input_file ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free input file.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *info_handle );

		*info_handle = NULL;
	}
	return( result );
}

/* Signals the info handle to abort
 * Returns 1 if successful or -1 on error
 */
int info_handle_signal_abort(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_signal_abort";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_file != NULL )
	{
		if( libqcow_file_signal_abort(
		     info_handle->input_file,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to signal input file to abort.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Opens the info handle
 * Returns 1 if successful, 0 if the keys could not be read or -1 on error
 */
int info_handle_open_input(
     info_handle_t *info_handle,
     const system_character_t *filename,
     libcerror_error_t **error )
{
	static char *function = "info_handle_open_input";
	int result            = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libqcow_file_open_wide(
	          info_handle->input_file,
	          filename,
	          LIBQCOW_OPEN_READ,
	          error );
#else
	result = libqcow_file_open(
	          info_handle->input_file,
	          filename,
	          LIBQCOW_OPEN_READ,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open input file.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Closes the info handle
 * Returns the 0 if succesful or -1 on error
 */
int info_handle_close(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_close";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->input_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid info handle - missing input file.",
		 function );

		return( -1 );
	}
	if( libqcow_file_close(
	     info_handle->input_file,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close input file.",
		 function );

		return( -1 );
	}
	return( 0 );
}

/* Prints the file information to a stream
 * Returns 1 if successful or -1 on error
 */
int info_handle_file_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	system_character_t byte_size_string[ 16 ];

	system_character_t *value_string = NULL;
	static char *function            = "qcowinfo_file_info_fprint";
	size64_t media_size              = 0;
	size_t value_string_size         = 0;
	uint32_t encryption_method       = 0;
	uint32_t format_version          = 0;
	int number_of_snapshots          = 0;
	int result                       = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "QEMU Copy-On-Write (QCOW) image information:\n" );

	if( libqcow_file_get_format_version(
	     info_handle->input_file,
	     &format_version,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve format version.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tFormat version\t\t: %" PRIu32 "\n",
	 format_version );

	if( libqcow_file_get_media_size(
	     info_handle->input_file,
	     &media_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve media size.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tMedia size\t\t:" );

	result = byte_size_string_create(
	          byte_size_string,
	          16,
	          media_size,
	          BYTE_SIZE_STRING_UNIT_MEBIBYTE,
	          NULL );

	if( result == 1 )
	{
		fprintf(
		 info_handle->notify_stream,
		 " %" PRIs_SYSTEM " (%" PRIu64 " bytes)",
		 byte_size_string,
		 media_size );
	}
	else
	{
		fprintf(
		 info_handle->notify_stream,
		 " %" PRIu64 " bytes",
		 media_size );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	if( libqcow_file_get_encryption_method(
	     info_handle->input_file,
	     &encryption_method,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve encryption method.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tEncryption method\t:" );

	switch( encryption_method )
	{
		case LIBQCOW_ENCRYPTION_METHOD_NONE:
			fprintf(
			 info_handle->notify_stream,
			 " None" );

			break;

		case LIBQCOW_ENCRYPTION_METHOD_AES_128_CBC:
			fprintf(
			 info_handle->notify_stream,
			 " AES-CBC 128-bit" );

			break;

		case LIBQCOW_ENCRYPTION_METHOD_LUKS:
			fprintf(
			 info_handle->notify_stream,
			 " Linux Unified Key Setup (LUKS)" );

			break;

		default:
			fprintf(
			 info_handle->notify_stream,
			 " Unknown (0x%08" PRIx32 ")",
			 encryption_method );

			break;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libqcow_file_get_utf16_backing_filename_size(
		  info_handle->input_file,
		  &value_string_size,
		  error );
#else
	result = libqcow_file_get_utf8_backing_filename_size(
		  info_handle->input_file,
		  &value_string_size,
		  error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve backing filename string size.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( value_string_size > (size_t) ( SSIZE_MAX / sizeof( system_character_t ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid backing filename size value exceeds maximum.",
			 function );

			goto on_error;
		}
		value_string = system_string_allocate(
				value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create backing filename string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libqcow_file_get_utf16_backing_filename(
			  info_handle->input_file,
			  (uint16_t *) value_string,
			  value_string_size,
			  error );
#else
		result = libqcow_file_get_utf8_backing_filename(
			  info_handle->input_file,
			  (uint8_t *) value_string,
			  value_string_size,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve backing filename.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 "\tBacking filename\t: %s\n",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
	if( libqcow_file_get_number_of_snapshots(
	     info_handle->input_file,
	     &number_of_snapshots,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of snapshots.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tNumber of snapshots\t: %d\n",
	 number_of_snapshots );

	if( number_of_snapshots > 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\n" );

/* TODO print snapshot information */
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	return( 1 );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	return( -1 );
}

