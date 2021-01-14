/*
 * File functions
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
#include <memory.h>
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include "libqcow_cluster_block.h"
#include "libqcow_cluster_table.h"
#include "libqcow_codepage.h"
#include "libqcow_compression.h"
#include "libqcow_debug.h"
#include "libqcow_definitions.h"
#include "libqcow_encryption.h"
#include "libqcow_file_header.h"
#include "libqcow_i18n.h"
#include "libqcow_io_handle.h"
#include "libqcow_file.h"
#include "libqcow_libbfio.h"
#include "libqcow_libcerror.h"
#include "libqcow_libcnotify.h"
#include "libqcow_libcthreads.h"
#include "libqcow_libfcache.h"
#include "libqcow_libfdata.h"
#include "libqcow_libuna.h"

/* Creates a file
 * Make sure the value file is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libqcow_file_initialize(
     libqcow_file_t **file,
     libcerror_error_t **error )
{
	libqcow_internal_file_t *internal_file = NULL;
	static char *function                  = "libqcow_file_initialize";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( *file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file value already set.",
		 function );

		return( -1 );
	}
	internal_file = memory_allocate_structure(
	                 libqcow_internal_file_t );

	if( internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_file,
	     0,
	     sizeof( libqcow_internal_file_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file.",
		 function );

		memory_free(
		 internal_file );

		return( -1 );
	}
	if( libqcow_io_handle_initialize(
	     &( internal_file->io_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create IO handle.",
		 function );

		goto on_error;
	}
	if( libqcow_i18n_initialize(
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initalize internationalization (i18n).",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_file->read_write_lock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize read/write lock.",
		 function );

		goto on_error;
	}
#endif
	internal_file->is_locked = 1;

	*file = (libqcow_file_t *) internal_file;

	return( 1 );

on_error:
	if( internal_file != NULL )
	{
		if( internal_file->io_handle != NULL )
		{
			libqcow_io_handle_free(
			 &( internal_file->io_handle ),
			 NULL );
		}
		memory_free(
		 internal_file );
	}
	return( -1 );
}

/* Frees a file
 * Returns 1 if successful or -1 on error
 */
int libqcow_file_free(
     libqcow_file_t **file,
     libcerror_error_t **error )
{
	libqcow_internal_file_t *internal_file = NULL;
	static char *function                  = "libqcow_file_free";
	int result                             = 1;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( *file != NULL )
	{
		internal_file = (libqcow_internal_file_t *) *file;

		if( internal_file->file_io_handle != NULL )
		{
			if( libqcow_file_close(
			     *file,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close file.",
				 function );

				result = -1;
			}
		}
		*file = NULL;

#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_file->read_write_lock ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free read/write lock.",
			 function );

			result = -1;
		}
#endif
		if( libqcow_io_handle_free(
		     &( internal_file->io_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free IO handle.",
			 function );

			result = -1;
		}
		if( memory_set(
		     internal_file->key_data,
		     0,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear key data.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_file );
	}
	return( result );
}

/* Signals a file to abort its current activity
 * Returns 1 if successful or -1 on error
 */
int libqcow_file_signal_abort(
     libqcow_file_t *file,
     libcerror_error_t **error )
{
	libqcow_internal_file_t *internal_file = NULL;
	static char *function                  = "libqcow_file_signal_abort";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libqcow_internal_file_t *) file;

	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	internal_file->io_handle->abort = 1;

	return( 1 );
}

/* Opens a file
 * Returns 1 if successful or -1 on error
 */
int libqcow_file_open(
     libqcow_file_t *file,
     const char *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle       = NULL;
	libqcow_internal_file_t *internal_file = NULL;
	static char *function                  = "libqcow_file_open";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBQCOW_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBQCOW_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBQCOW_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	internal_file = (libqcow_internal_file_t *) file;

	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set track offsets read in file IO handle.",
                 function );

		goto on_error;
	}
#endif
	if( libbfio_file_set_name(
	     file_io_handle,
	     filename,
	     narrow_string_length(
	      filename ) + 1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set filename in file IO handle.",
                 function );

		goto on_error;
	}
	if( libqcow_file_open_file_io_handle(
	     file,
	     file_io_handle,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file: %s.",
		 function,
		 filename );

		goto on_error;
	}
#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	internal_file->file_io_handle_created_in_library = 1;

#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Opens a file
 * Returns 1 if successful or -1 on error
 */
int libqcow_file_open_wide(
     libqcow_file_t *file,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle       = NULL;
	libqcow_internal_file_t *internal_file = NULL;
	static char *function                  = "libqcow_file_open_wide";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBQCOW_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBQCOW_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBQCOW_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	internal_file = (libqcow_internal_file_t *) file;

	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set track offsets read in file IO handle.",
                 function );

		goto on_error;
	}
#endif
	if( libbfio_file_set_name_wide(
	     file_io_handle,
	     filename,
	     wide_string_length(
	      filename ) + 1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set filename in file IO handle.",
                 function );

		goto on_error;
	}
	if( libqcow_file_open_file_io_handle(
	     file,
	     file_io_handle,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file: %ls.",
		 function,
		 filename );

		goto on_error;
	}
#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	internal_file->file_io_handle_created_in_library = 1;

#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Opens a file using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
int libqcow_file_open_file_io_handle(
     libqcow_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error )
{
	libqcow_internal_file_t *internal_file = NULL;
	static char *function                  = "libqcow_file_open_file_io_handle";
	int bfio_access_flags                  = 0;
	int file_io_handle_is_open             = 0;
	int file_io_handle_opened_in_library   = 0;
	int result                             = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libqcow_internal_file_t *) file;

	if( internal_file->file_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - file IO handle already set.",
		 function );

		return( -1 );
	}
	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBQCOW_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBQCOW_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBQCOW_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBQCOW_ACCESS_FLAG_READ ) != 0 )
	{
		bfio_access_flags = LIBBFIO_ACCESS_FLAG_READ;
	}
	file_io_handle_is_open = libbfio_handle_is_open(
	                          file_io_handle,
	                          error );

	if( file_io_handle_is_open == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to determine if file IO handle is open.",
		 function );

		goto on_error;
	}
	else if( file_io_handle_is_open == 0 )
	{
		if( libbfio_handle_open(
		     file_io_handle,
		     bfio_access_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open file IO handle.",
			 function );

			goto on_error;
		}
		file_io_handle_opened_in_library = 1;
	}
#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libqcow_internal_file_open_read(
	          internal_file,
	          file_io_handle,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from file IO handle.",
		 function );
	}
	else
	{
		internal_file->file_io_handle                   = file_io_handle;
		internal_file->file_io_handle_opened_in_library = (uint8_t) file_io_handle_opened_in_library;
	}
#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );

on_error:
	if( file_io_handle_opened_in_library != 0 )
	{
		libbfio_handle_close(
		 file_io_handle,
		 error );
	}
	return( -1 );
}

/* Closes a file
 * Returns 0 if successful or -1 on error
 */
int libqcow_file_close(
     libqcow_file_t *file,
     libcerror_error_t **error )
{
	libqcow_internal_file_t *internal_file = NULL;
	static char *function                  = "libqcow_file_close";
	int result                             = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libqcow_internal_file_t *) file;

	if( internal_file->file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( internal_file->file_io_handle_created_in_library != 0 )
		{
			if( libqcow_debug_print_read_offsets(
			     internal_file->file_io_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print the read offsets.",
				 function );

				result = -1;
			}
		}
	}
#endif
	if( internal_file->file_io_handle_opened_in_library != 0 )
	{
		if( libbfio_handle_close(
		     internal_file->file_io_handle,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close file IO handle.",
			 function );

			result = -1;
		}
		internal_file->file_io_handle_opened_in_library = 0;
	}
	if( internal_file->file_io_handle_created_in_library != 0 )
	{
		if( libbfio_handle_free(
		     &( internal_file->file_io_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file IO handle.",
			 function );

			result = -1;
		}
		internal_file->file_io_handle_created_in_library = 0;
	}
	internal_file->file_io_handle = NULL;
	internal_file->current_offset = 0;
	internal_file->is_locked      = 1;

	if( libqcow_io_handle_clear(
	     internal_file->io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		result = -1;
	}
	if( libqcow_file_header_free(
	     &( internal_file->file_header ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file header.",
		 function );

		result = -1;
	}
	if( internal_file->backing_filename != NULL )
	{
		memory_free(
		 internal_file->backing_filename );

		internal_file->backing_filename      = NULL;
		internal_file->backing_filename_size = 0;
	}
	if( libqcow_cluster_table_free(
	     &( internal_file->level1_table ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free level 1 table.",
		 function );

		result = -1;
	}
	if( libfdata_vector_free(
	     &( internal_file->level2_table_vector ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free level2 table vector.",
		 function );

		result = -1;
	}
	if( libfcache_cache_free(
	     &( internal_file->level2_table_cache ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free level2 table cache.",
		 function );

		result = -1;
	}
	if( libfdata_vector_free(
	     &( internal_file->cluster_block_vector ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free cluster block vector.",
		 function );

		result = -1;
	}
	if( libfcache_cache_free(
	     &( internal_file->cluster_block_cache ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free cluster block cache.",
		 function );

		result = -1;
	}
	if( libfcache_cache_free(
	     &( internal_file->compressed_cluster_block_cache ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free compressed cluster block cache.",
		 function );

		result = -1;
	}
	if( internal_file->encryption_context != NULL )
	{
		if( libqcow_encryption_free(
		     &( internal_file->encryption_context ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free encryption context.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Opens a file for reading
 * Returns 1 if successful or -1 on error
 */
int libqcow_internal_file_open_read(
     libqcow_internal_file_t *internal_file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function                      = "libqcow_internal_file_open_read";
	size_t level1_table_size                   = 0;
	size_t level2_table_size                   = 0;
	uint32_t number_of_level1_table_references = 0;
	uint32_t number_of_level2_table_bits       = 0;
	int entry_index                            = 0;

	if( internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_file->file_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - file header value already set.",
		 function );

		return( -1 );
	}
	if( internal_file->backing_filename != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - backing filename value already set.",
		 function );

		return( -1 );
	}
	if( internal_file->level1_table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - level 1 table already set.",
		 function );

		return( -1 );
	}
	if( internal_file->level2_table_vector != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - level2 table vector already set.",
		 function );

		return( -1 );
	}
	if( internal_file->level2_table_cache != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - level2 table cache already set.",
		 function );

		return( -1 );
	}
	if( internal_file->cluster_block_vector != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - cluster block vector already set.",
		 function );

		return( -1 );
	}
	if( internal_file->cluster_block_cache != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - cluster block cache already set.",
		 function );

		return( -1 );
	}
	if( internal_file->compressed_cluster_block_cache != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - compressed cluster block cache already set.",
		 function );

		return( -1 );
	}
	if( libbfio_handle_get_size(
	     file_io_handle,
	     &( internal_file->size ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file size.",
		 function );

		goto on_error;
	}
	if( internal_file->size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file size.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading file header:\n" );
	}
#endif
	if( libqcow_file_header_initialize(
	     &( internal_file->file_header ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file header.",
		 function );

		goto on_error;
	}
	if( libqcow_file_header_read_file_io_handle(
	     internal_file->file_header,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header.",
		 function );

		goto on_error;
	}

	internal_file->encryption_method  = internal_file->file_header->encryption_method;

	number_of_level1_table_references = internal_file->file_header->number_of_level1_table_references;

	if( internal_file->file_header->format_version == 1 )
	{
		if( internal_file->file_header->number_of_cluster_block_bits > 63 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid number of cluster block bits value out of bounds.",
			 function );

			goto on_error;
		}
		if( internal_file->file_header->number_of_level2_table_bits > 63 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid number of cluster block bits value out of bounds.",
			 function );

			goto on_error;
		}
		number_of_level2_table_bits = internal_file->file_header->number_of_level2_table_bits;

		internal_file->offset_bit_mask           = 0x7fffffffffffffffULL;
		internal_file->compression_flag_bit_mask = (uint64_t) 1UL << 63;
		internal_file->compression_bit_shift     = 63 - internal_file->file_header->number_of_cluster_block_bits;
	}
	else if( ( internal_file->file_header->format_version == 2 )
	      || ( internal_file->file_header->format_version == 3 ) )
	{
		if( ( internal_file->file_header->number_of_cluster_block_bits <= 8 )
		 || ( internal_file->file_header->number_of_cluster_block_bits > 63 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid number of cluster block bits value out of bounds.",
			 function );

			goto on_error;
		}
		number_of_level2_table_bits = internal_file->file_header->number_of_cluster_block_bits - 3;

		internal_file->offset_bit_mask           = 0x3fffffffffffffffULL;
		internal_file->compression_flag_bit_mask = (uint64_t) 1UL << 62;
		internal_file->compression_bit_shift     = 62 - ( internal_file->file_header->number_of_cluster_block_bits - 8 );
	}
	internal_file->level1_index_bit_shift = internal_file->file_header->number_of_cluster_block_bits + number_of_level2_table_bits;

	if( internal_file->level1_index_bit_shift > 63 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid level1 index bit shift value out of bounds.",
		 function );

		goto on_error;
	}
	internal_file->level2_index_bit_mask  = ~( (uint64_t) -1 << number_of_level2_table_bits );
	internal_file->cluster_block_bit_mask = ~( (uint64_t) -1 << internal_file->file_header->number_of_cluster_block_bits );
	internal_file->compression_bit_mask   = ~( (uint64_t) -1 << internal_file->compression_bit_shift );
	internal_file->cluster_block_size     = (size64_t) 1 << internal_file->file_header->number_of_cluster_block_bits;

	level2_table_size = (size_t) 1 << number_of_level2_table_bits;

	if( internal_file->file_header->format_version == 1 )
	{
		if( internal_file->cluster_block_size > ( (size64_t) MEMORY_MAXIMUM_ALLOCATION_SIZE / level2_table_size ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid cluster block size value out of bounds.",
			 function );

			goto on_error;
		}
		level1_table_size = (size_t) ( internal_file->cluster_block_size * level2_table_size );

		if( ( internal_file->file_header->media_size / level1_table_size ) > ( (size64_t) UINT32_MAX - 1 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid level1 table size value out of bounds: %zd.",
			 function, level1_table_size );

			goto on_error;
		}
		number_of_level1_table_references = (uint32_t) ( internal_file->file_header->media_size / level1_table_size );

		if( ( internal_file->file_header->media_size % level1_table_size ) != 0 )
		{
			number_of_level1_table_references += 1;
		}
		level1_table_size = (size_t) number_of_level1_table_references;
	}
	else if( ( internal_file->file_header->format_version == 2 )
	      || ( internal_file->file_header->format_version == 3 ) )
	{
		if( internal_file->cluster_block_size > (size64_t) MEMORY_MAXIMUM_ALLOCATION_SIZE )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid cluster block size value out of bounds.",
			 function );

			goto on_error;
		}
		level1_table_size = (size_t) number_of_level1_table_references;
	}
	if( ( level1_table_size == 0 )
	 || ( level1_table_size > ( (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE / 8 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid level1 table size value out of bounds.",
		 function );

		goto on_error;
	}
	if( ( level2_table_size == 0 )
	 || ( level2_table_size > ( (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE / 8 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid level2 table size value out of bounds.",
		 function );

		goto on_error;
	}
	level1_table_size *= 8;
	level2_table_size *= 8;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: level 1 table size\t\t\t: %" PRIzd "\n",
		 function,
		 level1_table_size );

		libcnotify_printf(
		 "%s: level 2 table size\t\t\t: %" PRIzd "\n",
		 function,
		 level2_table_size );

		libcnotify_printf(
		 "%s: cluster block size\t\t\t: %" PRIu64 "\n",
		 function,
		 internal_file->cluster_block_size );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( ( internal_file->file_header->backing_filename_offset > 0 )
	 && ( internal_file->file_header->backing_filename_size > 0 ) )
	{
		if( libqcow_internal_file_open_read_backing_filename(
		     internal_file,
		     file_io_handle,
		     internal_file->file_header->backing_filename_offset,
		     internal_file->file_header->backing_filename_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read backing filename.",
			 function );

			goto on_error;
		}
	}
	if( internal_file->encryption_method != LIBQCOW_ENCRYPTION_METHOD_NONE )
	{
		if( libqcow_encryption_initialize(
		     &( internal_file->encryption_context ),
		     internal_file->encryption_method,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create encryption context.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: key:\n",
			 function );
			libcnotify_print_data(
			 internal_file->key_data,
			 16,
			 0 );
		}
#endif
		if( libqcow_encryption_set_keys(
		     internal_file->encryption_context,
		     internal_file->key_data,
		     16,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set key data in encryption context.",
			 function );

			goto on_error;
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading level 1 table:\n" );
	}
#endif
	if( libqcow_cluster_table_initialize(
	     &( internal_file->level1_table ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create level 1 table.",
		 function );

		goto on_error;
	}
	if( libqcow_cluster_table_read(
	     internal_file->level1_table,
	     file_io_handle,
	     internal_file->file_header->level1_table_offset,
	     level1_table_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read level 1 table.",
		 function );

		goto on_error;
	}
/* TODO clone function ? */
	if( libfdata_vector_initialize(
	     &( internal_file->level2_table_vector ),
	     (size64_t) level2_table_size,
	     (intptr_t *) internal_file->io_handle,
	     NULL,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_vector_t *, libfdata_cache_t *, int, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libqcow_io_handle_read_level2_table,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create level2 table vector.",
		 function );

		goto on_error;
	}
	if( libfdata_vector_append_segment(
	     internal_file->level2_table_vector,
	     &entry_index,
	     0,
	     0,
	     internal_file->size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append segment to level2 table vector.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( internal_file->level2_table_cache ),
	     LIBQCOW_MAXIMUM_CACHE_ENTRIES_LEVEL2_TABLES,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create level2 table cache.",
		 function );

		goto on_error;
	}
/* TODO clone function ? */
	if( libfdata_vector_initialize(
	     &( internal_file->cluster_block_vector ),
	     internal_file->cluster_block_size,
	     (intptr_t *) internal_file->io_handle,
	     NULL,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_vector_t *, libfdata_cache_t *, int, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libqcow_io_handle_read_cluster_block,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create cluster block vector.",
		 function );

		goto on_error;
	}
	if( libfdata_vector_append_segment(
	     internal_file->cluster_block_vector,
	     &entry_index,
	     0,
	     0,
	     internal_file->size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append segment to cluster block vector.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( internal_file->cluster_block_cache ),
	     LIBQCOW_MAXIMUM_CACHE_ENTRIES_CLUSTER_BLOCKS,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create cluster block cache.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( internal_file->compressed_cluster_block_cache ),
	     LIBQCOW_MAXIMUM_CACHE_ENTRIES_CLUSTER_BLOCKS,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create compressed cluster block cache.",
		 function );

		goto on_error;
	}
	internal_file->is_locked = 0;

	return( 1 );

on_error:
	if( internal_file->cluster_block_cache != NULL )
	{
		libfcache_cache_free(
		 &( internal_file->cluster_block_cache ),
		 NULL );
	}
	if( internal_file->cluster_block_vector != NULL )
	{
		libfdata_vector_free(
		 &( internal_file->cluster_block_vector ),
		 NULL );
	}
	if( internal_file->level2_table_cache != NULL )
	{
		libfcache_cache_free(
		 &( internal_file->level2_table_cache ),
		 NULL );
	}
	if( internal_file->level2_table_vector != NULL )
	{
		libfdata_vector_free(
		 &( internal_file->level2_table_vector ),
		 NULL );
	}
	if( internal_file->level1_table != NULL )
	{
		libqcow_cluster_table_free(
		 &( internal_file->level1_table ),
		 NULL );
	}
	if( internal_file->encryption_context != NULL )
	{
		libqcow_encryption_free(
		 &( internal_file->encryption_context ),
		 NULL );
	}
	if( internal_file->backing_filename != NULL )
	{
		memory_free(
		 internal_file->backing_filename );

		internal_file->backing_filename = NULL;
	}
	internal_file->backing_filename_size = 0;

	if( internal_file->file_header != NULL )
	{
		libqcow_file_header_free(
		 &( internal_file->file_header ),
		 NULL );
	}
	return( -1 );
}

/* Reads the backing filename
 * Returns 1 if successful or -1 on error
 */
int libqcow_internal_file_open_read_backing_filename(
     libqcow_internal_file_t *internal_file,
     libbfio_handle_t *file_io_handle,
     off64_t backing_filename_offset,
     uint32_t backing_filename_size,
     libcerror_error_t **error )
{
	static char *function = "libqcow_internal_file_open_read_backing_filename";
	ssize_t read_count    = 0;

	if( internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( internal_file->backing_filename != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - backing filename value already set.",
		 function );

		return( -1 );
	}
	if( ( backing_filename_size == 0 )
	 || ( backing_filename_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid backing filename size value out of bounds.",
		 function );

		goto on_error;
	}
	internal_file->backing_filename = (uint8_t *) memory_allocate(
	                                               sizeof( uint8_t ) * backing_filename_size );

	if( internal_file->backing_filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create backing filename.",
		 function );

		goto on_error;
	}
	internal_file->backing_filename_size = (size_t) backing_filename_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading backing filename at offset: %" PRIu64 " (0x%08" PRIx64 ")\n",
		 function,
		 backing_filename_offset,
		 backing_filename_offset );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
		      file_io_handle,
		      internal_file->backing_filename,
		      (size_t) backing_filename_size,
		      backing_filename_offset,
		      error );

	if( read_count != (ssize_t) backing_filename_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read backing filename data at offset: %" PRIu64 " (0x%08" PRIx64 ").",
		 function,
		 backing_filename_offset,
		 backing_filename_offset );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: backing filename data:\n",
		 function );
		libcnotify_print_data(
		 internal_file->backing_filename,
		 (size_t) backing_filename_size,
		 0 );
	}
#endif
	return( 1 );

on_error:
	if( internal_file->backing_filename != NULL )
	{
		memory_free(
		 internal_file->backing_filename );

		internal_file->backing_filename = NULL;
	}
	internal_file->backing_filename_size = 0;

	return( -1 );
}

/* Determines if the file is locked
 * Returns 1 if locked, 0 if not or -1 on error
 */
int libqcow_file_is_locked(
     libqcow_file_t *file,
     libcerror_error_t **error )
{
	libqcow_internal_file_t *internal_file = NULL;
	static char *function                  = "libqcow_file_is_locked";
	uint8_t is_locked                      = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libqcow_internal_file_t *) file;

#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	is_locked = internal_file->is_locked;

#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( is_locked );
}

/* Retrieves the cluster block offset for a specific offset
 * Returns 1 if successful or -1 on error
 */
int libqcow_internal_file_get_cluster_block_offset(
     libqcow_internal_file_t *internal_file,
     libbfio_handle_t *file_io_handle,
     off64_t offset,
     uint64_t *cluster_block_offset,
     uint64_t *cluster_block_data_offset,
     uint8_t *cluster_block_is_compressed,
     libcerror_error_t **error )
{
	libqcow_cluster_table_t *level2_table = NULL;
	static char *function                 = "libqcow_internal_file_get_cluster_block_offset";
	off64_t element_data_offset           = 0;
	uint64_t level1_table_index           = 0;
	uint64_t level2_table_index           = 0;
	uint64_t level2_table_offset          = 0;
	uint64_t safe_cluster_block_offset    = 0;
	uint8_t is_compressed                 = 0;

	if( internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( internal_file->file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file header.",
		 function );

		return( -1 );
	}
	if( offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( cluster_block_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid cluster block offset.",
		 function );

		return( -1 );
	}
	if( cluster_block_data_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid cluster block data offset.",
		 function );

		return( -1 );
	}
	if( cluster_block_is_compressed == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid cluster block is compressed.",
		 function );

		return( -1 );
	}
	level1_table_index = offset >> internal_file->level1_index_bit_shift;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: level 1 table index\t: %" PRIu64 "\n",
		 function,
		 level1_table_index );
	}
#endif
	if( level1_table_index > (uint64_t) INT_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid level 1 table index value out of bounds.",
		 function );

		return( -1 );
	}
	if( libqcow_cluster_table_get_reference_by_index(
	     internal_file->level1_table,
	     (int) level1_table_index,
	     &level2_table_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve level 2 table offset: %" PRIi64 " from level 1 table.",
		 function,
		 level1_table_index );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: level 2 table offset\t: 0x%08" PRIx64 "\n",
		 function,
		 level2_table_offset );

		libcnotify_printf(
		 "\n" );
	}
#endif
	level2_table_offset &= internal_file->offset_bit_mask;

	/* If level2_table_offset is 0 the level 2 table is sparse
	 */
	if( level2_table_offset > 0 )
	{
		if( libfdata_vector_get_element_value_at_offset(
		     internal_file->level2_table_vector,
		     (intptr_t *) file_io_handle,
		     (libfdata_cache_t *) internal_file->level2_table_cache,
		     (off64_t) level2_table_offset,
		     &element_data_offset,
		     (intptr_t **) &level2_table,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve level2 table at offset: 0x%08" PRIx64 ".",
			 function,
			 level2_table_offset );

			return( -1 );
		}
		level2_table_index = ( internal_file->current_offset >> internal_file->file_header->number_of_cluster_block_bits ) & internal_file->level2_index_bit_mask;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: level 2 table index\t: %" PRIu64 "\n",
			 function,
			 level2_table_index );
		}
#endif
		if( level2_table_index > (uint64_t) INT_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid level 2 table index value out of bounds.",
			 function );

			return( -1 );
		}
		if( libqcow_cluster_table_get_reference_by_index(
		     level2_table,
		     (int) level2_table_index,
		     &safe_cluster_block_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve level 2 table entry: %" PRIu64 ".",
			 function,
			 level2_table_index );

			return( -1 );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: table2 entry: %" PRIu64 "\t\t: 0x%08" PRIx64 "\n",
		 function,
		 level2_table_index,
		 safe_cluster_block_offset );

		libcnotify_printf(
		 "%s: cluster block offset\t: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 safe_cluster_block_offset & internal_file->offset_bit_mask,
		 safe_cluster_block_offset & internal_file->offset_bit_mask );
	}
#endif
	if( ( safe_cluster_block_offset & internal_file->compression_flag_bit_mask ) != 0 )
	{
		if( internal_file->encryption_method != LIBQCOW_ENCRYPTION_METHOD_NONE )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: simultaneous encryption and compression not supported.",
			 function );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: is compressed\n",
			 function );
		}
#endif
		is_compressed = 1;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	*cluster_block_offset        = safe_cluster_block_offset & internal_file->offset_bit_mask;
	*cluster_block_data_offset   = offset & internal_file->cluster_block_bit_mask;
	*cluster_block_is_compressed = is_compressed;

	return( 1 );
}

/* Reads a cluster block
 * Returns 1 if successful or -1 on error
 */
int libqcow_internal_file_read_cluster_block(
     libqcow_internal_file_t *internal_file,
     libbfio_handle_t *file_io_handle,
     uint64_t cluster_block_offset,
     uint64_t cluster_block_data_offset,
     uint8_t cluster_block_is_compressed,
     libqcow_cluster_block_t **cluster_block,
     libcerror_error_t **error )
{
	libfcache_cache_t *cluster_block_cache       = NULL;
	libqcow_cluster_block_t *safe_cluster_block  = NULL;
	static char *function                        = "libqcow_internal_file_read_cluster_block";
	size_t cluster_block_size                    = 0;
	size_t safe_cluster_block_data_size          = 0;
	off64_t element_data_offset                  = 0;
	uint64_t block_key                           = 0;
	uint64_t compressed_cluster_block_end_offset = 0;
	int cache_entry_index                        = 0;

	if( internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( internal_file->file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file header.",
		 function );

		return( -1 );
	}
	if( internal_file->cluster_block_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid file - cluster block size value out of bounds.",
		 function );

		return( -1 );
	}
	if( cluster_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid cluster block.",
		 function );

		return( -1 );
	}
	if( cluster_block_is_compressed != 0 )
	{
		/* Handle compressed cluster block
		 */
		cluster_block_size    = (size_t) ( cluster_block_offset >> internal_file->compression_bit_shift );
		cluster_block_offset &= internal_file->compression_bit_mask;

		if( ( internal_file->file_header->format_version == 2 )
		 || ( internal_file->file_header->format_version == 3 ) )
		{
			cluster_block_size += 1;
			cluster_block_size *= 512;

			/* Make sure the compressed block size stays within the bounds
			 * of the cluster block size and the size of the file
			 */
			compressed_cluster_block_end_offset = cluster_block_offset / internal_file->cluster_block_size;

			if( ( cluster_block_offset % internal_file->cluster_block_size ) != 0 )
			{
				compressed_cluster_block_end_offset += 1;
			}
			compressed_cluster_block_end_offset += 1;
			compressed_cluster_block_end_offset *= internal_file->cluster_block_size;

			if( compressed_cluster_block_end_offset > internal_file->size )
			{
				compressed_cluster_block_end_offset = internal_file->size;
			}
			if( ( cluster_block_offset + cluster_block_size ) > compressed_cluster_block_end_offset )
			{
				cluster_block_size = (size_t) ( compressed_cluster_block_end_offset - cluster_block_offset );
			}
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: compressed cluster block offset\t\t: 0x%08" PRIx64 "\n",
			 function,
			 cluster_block_offset );

			libcnotify_printf(
			 "%s: compressed cluster block size\t\t: %" PRIzd "\n",
			 function,
			 cluster_block_size );
		}
#endif
		cluster_block_cache = internal_file->compressed_cluster_block_cache;
	}
	else
	{
		/* For version 2 and 3 make sure the sure the last cluster block size
		 * stays within the bounds of the size of the file
		 */
		if( ( ( internal_file->file_header->format_version == 2 )
		  ||  ( internal_file->file_header->format_version == 3 ) )
		 && ( ( cluster_block_offset + internal_file->cluster_block_size ) > internal_file->size ) )
		{
			cluster_block_size = (size_t) ( internal_file->size - cluster_block_offset );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: last cluster block offset\t\t: 0x%08" PRIx64 "\n",
				 function,
				 cluster_block_offset );

				libcnotify_printf(
				 "%s: last cluster block size\t\t\t: %" PRIzd "\n",
				 function,
				 cluster_block_size );
			}
#endif
		}
		cluster_block_cache = internal_file->cluster_block_cache;
	}
	if( cluster_block_size != 0 )
	{
/* TODO check cache  */
		if( libqcow_cluster_block_initialize(
		     &safe_cluster_block,
		     cluster_block_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create cluster block.",
			 function );

			goto on_error;
		}
		if( libqcow_cluster_block_read(
		     safe_cluster_block,
		     file_io_handle,
		     cluster_block_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read cluster block at offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 cluster_block_offset,
			 cluster_block_offset );

			goto on_error;
		}
	}
	else
	{
		if( libfdata_vector_get_element_value_at_offset(
		     internal_file->cluster_block_vector,
		     (intptr_t *) file_io_handle,
		     (libfdata_cache_t *) cluster_block_cache,
		     (off64_t) cluster_block_offset,
		     &element_data_offset,
		     (intptr_t **) cluster_block,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve cluster block at offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 cluster_block_offset,
			 cluster_block_offset );

			goto on_error;
		}
	}
	if( cluster_block_is_compressed != 0 )
	{
		safe_cluster_block->compressed_data = safe_cluster_block->data;

		safe_cluster_block->data = (uint8_t *) memory_allocate(
		                                        sizeof( uint8_t ) * internal_file->cluster_block_size );

		if( safe_cluster_block->data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create cluster block data.",
			 function );

			goto on_error;
		}
		safe_cluster_block->data_size = internal_file->cluster_block_size;

		safe_cluster_block_data_size = safe_cluster_block->data_size;

		if( libqcow_decompress_data(
		     safe_cluster_block->compressed_data,
		     cluster_block_size,
		     LIBQCOW_COMPRESSION_METHOD_DEFLATE,
		     safe_cluster_block->data,
		     &safe_cluster_block_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
			 LIBCERROR_ENCRYPTION_ERROR_GENERIC,
			 "%s: unable to decompress cluster block data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 cluster_block_offset,
			 cluster_block_offset );

			goto on_error;
		}
/* TODO check safe_cluster_block_data_size
		if( safe_cluster_block_data_size != safe_cluster_block->data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid cluster block size value out of bounds.",
			 function );

			goto on_error;
		}
*/
	}
	if( safe_cluster_block != NULL )
	{
		cache_entry_index = ( cluster_block_offset & internal_file->cluster_block_bit_mask ) % LIBQCOW_MAXIMUM_CACHE_ENTRIES_CLUSTER_BLOCKS;

		if( libfcache_cache_set_value_by_index(
		     cluster_block_cache,
		     cache_entry_index,
		     0,
		     cluster_block_offset,
		     0,
		     (intptr_t *) safe_cluster_block,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libqcow_cluster_block_free,
		     LIBFCACHE_CACHE_VALUE_FLAG_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set value in cache entry: %d.",
			 function,
			 cache_entry_index );

			goto on_error;
		}
		*cluster_block = safe_cluster_block;
	}
	if( internal_file->encryption_method != LIBQCOW_ENCRYPTION_METHOD_NONE )
	{
		if( *cluster_block == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing cluster block.",
			 function );

			goto on_error;
		}
		if( ( *cluster_block )->encrypted_data == NULL )
		{
			( *cluster_block )->encrypted_data = ( *cluster_block )->data;

			( *cluster_block )->data = (uint8_t *) memory_allocate(
			                                        sizeof( uint8_t ) * ( *cluster_block )->data_size );

			if( ( *cluster_block )->data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create cluster block data.",
				 function );

				goto on_error;
			}
			block_key = (uint64_t) ( internal_file->current_offset - cluster_block_data_offset ) / 512;

			if( libqcow_encryption_crypt(
			     internal_file->encryption_context,
			     LIBQCOW_ENCYPTION_CRYPT_MODE_DECRYPT,
			     ( *cluster_block )->encrypted_data,
			     ( *cluster_block )->data_size,
			     ( *cluster_block )->data,
			     ( *cluster_block )->data_size,
			     block_key,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
				 LIBCERROR_ENCRYPTION_ERROR_GENERIC,
				 "%s: unable to decrypt cluster block data.",
				 function );

				goto on_error;
			}
		}
	}
	return( 1 );

on_error:
	if( safe_cluster_block != NULL )
	{
		libqcow_cluster_block_free(
		 &safe_cluster_block,
		 NULL );
	}
	return( -1 );
}

/* Reads (media) data from the current offset into a buffer using a Basic File IO (bfio) handle
 * This function is not multi-thread safe acquire write lock before call
 * Returns the number of bytes read or -1 on error
 */
ssize_t libqcow_internal_file_read_buffer_from_file_io_handle(
         libqcow_internal_file_t *internal_file,
         libbfio_handle_t *file_io_handle,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libqcow_cluster_block_t *cluster_block = NULL;
	static char *function                  = "libqcow_internal_file_read_buffer_from_file_io_handle";
	size_t buffer_offset                   = 0;
	size_t read_size                       = 0;
	ssize_t read_count                     = 0;
	uint64_t cluster_block_data_offset     = 0;
	uint64_t cluster_block_file_offset     = 0;
	uint8_t cluster_block_is_compressed    = 0;

	if( internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( internal_file->file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file header.",
		 function );

		return( -1 );
	}
	if( internal_file->current_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid file - current offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( internal_file->cluster_block_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid file - cluster block size value out of bounds.",
		 function );

		return( -1 );
	}
	if( internal_file->backing_filename != NULL )
	{
		if( internal_file->parent_file == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid file - missing parent file.",
			 function );

			return( -1 );
		}
	}
	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( buffer_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( (size64_t) internal_file->current_offset >= internal_file->file_header->media_size )
	{
		return( 0 );
	}
	while( buffer_offset < buffer_size )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: current offset\t: %" PRIi64 " (0x%08" PRIx64 ")\n",
			 function,
			 internal_file->current_offset,
			 internal_file->current_offset );
		}
#endif
		if( libqcow_internal_file_get_cluster_block_offset(
		     internal_file,
		     file_io_handle,
		     internal_file->current_offset,
		     &cluster_block_file_offset,
		     &cluster_block_data_offset,
		     &cluster_block_is_compressed,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve cluster block offset for offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 internal_file->current_offset,
			 internal_file->current_offset );

			return( -1 );
		}
		read_size = internal_file->cluster_block_size - (size_t) cluster_block_data_offset;

		if( ( (size64_t) internal_file->current_offset + read_size ) > internal_file->file_header->media_size )
		{
			read_size = (size_t) ( internal_file->file_header->media_size - internal_file->current_offset );
		}
		if( ( buffer_offset + read_size ) > buffer_size )
		{
			read_size = buffer_size - buffer_offset;
		}
		if( cluster_block_file_offset > 0 )
		{
			if( libqcow_internal_file_read_cluster_block(
			     internal_file,
			     file_io_handle,
			     cluster_block_file_offset,
			     cluster_block_data_offset,
			     cluster_block_is_compressed,
			     &cluster_block,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read cluster block.",
				 function );

				return( -1 );
			}
			if( cluster_block == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid cluster block.",
				 function );

				return( -1 );
			}
			if( cluster_block->data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid cluster block - missing data.",
				 function );

				return( -1 );
			}
			if( memory_copy(
			     &( ( (uint8_t *) buffer )[ buffer_offset ] ),
			     &( cluster_block->data[ cluster_block_data_offset ] ),
			     read_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy cluster block data to buffer.",
				 function );

				return( -1 );
			}
		}
		else if( internal_file->parent_file != NULL )
		{
			read_count = libqcow_file_read_buffer_at_offset(
				      internal_file->parent_file,
				      &( ( (uint8_t *) buffer )[ buffer_offset ] ),
				      buffer_size - buffer_offset,
				      internal_file->current_offset,
				      error );

			if( read_count == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read buffer from parent file.",
				 function );

				return( -1 );
			}
			read_size = (size_t) read_count;
		}
		else
		{
			/* Handle sparse cluster block
			 */
			if( memory_set(
			     &( ( (uint8_t *) buffer )[ buffer_offset ] ),
			     0,
			     read_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to set sparse data in buffer.",
				 function );

				return( -1 );
			}
		}
		internal_file->current_offset += read_size;
		buffer_offset                 += read_size;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "\n" );
		}
#endif
		if( (size64_t) internal_file->current_offset >= internal_file->file_header->media_size )
		{
			break;
		}
	}
	return( (ssize_t) buffer_offset );
}

/* Reads (media) data from the current offset into a buffer
 * Returns the number of bytes read or -1 on error
 */
ssize_t libqcow_file_read_buffer(
         libqcow_file_t *file,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libqcow_internal_file_t *internal_file = NULL;
	static char *function                  = "libqcow_file_read_buffer";
	ssize_t read_count                     = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libqcow_internal_file_t *) file;

	if( internal_file->file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	read_count = libqcow_internal_file_read_buffer_from_file_io_handle(
		      internal_file,
		      internal_file->file_io_handle,
		      buffer,
		      buffer_size,
		      error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read buffer.",
		 function );

		read_count = -1;
	}
#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( read_count );
}

/* Reads (media) data at a specific offset
 * Returns the number of bytes read or -1 on error
 */
ssize_t libqcow_file_read_buffer_at_offset(
         libqcow_file_t *file,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error )
{
	libqcow_internal_file_t *internal_file = NULL;
	static char *function                  = "libqcow_file_read_buffer_at_offset";
	ssize_t read_count                     = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libqcow_internal_file_t *) file;

	if( internal_file->file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libqcow_internal_file_seek_offset(
	     internal_file,
	     offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		read_count = -1;
	}
	else
	{
		read_count = libqcow_internal_file_read_buffer_from_file_io_handle(
			      internal_file,
			      internal_file->file_io_handle,
			      buffer,
			      buffer_size,
			      error );

		if( read_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read buffer.",
			 function );

			read_count = -1;
		}
	}
#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( read_count );
}

/* Seeks a certain offset of the (media) data
 * This function is not multi-thread safe acquire write lock before call
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libqcow_internal_file_seek_offset(
         libqcow_internal_file_t *internal_file,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	static char *function = "libqcow_internal_file_seek_offset";

	if( internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( internal_file->file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file header.",
		 function );

		return( -1 );
	}
	if( internal_file->backing_filename != NULL )
	{
		if( internal_file->parent_file == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid file - missing parent file.",
			 function );

			return( -1 );
		}
	}
	if( ( whence != SEEK_CUR )
	 && ( whence != SEEK_END )
	 && ( whence != SEEK_SET ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported whence.",
		 function );

		return( -1 );
	}
	if( whence == SEEK_CUR )
	{
		offset += internal_file->current_offset;
	}
	else if( whence == SEEK_END )
	{
		offset += (off64_t) internal_file->file_header->media_size;
	}
	if( offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid offset value out of bounds.",
		 function );

		return( -1 );
	}
	internal_file->current_offset = offset;

	return( offset );
}

/* Seeks a certain offset of the (media) data
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libqcow_file_seek_offset(
         libqcow_file_t *file,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	libqcow_internal_file_t *internal_file = NULL;
	static char *function                  = "libqcow_file_seek_offset";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libqcow_internal_file_t *) file;

	if( internal_file->file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	offset = libqcow_internal_file_seek_offset(
	          internal_file,
	          offset,
	          whence,
	          error );

	if( offset == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		offset = -1;
	}
#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( offset );
}

/* Retrieves the current offset of the (media) data
 * Returns 1 if successful or -1 on error
 */
int libqcow_file_get_offset(
     libqcow_file_t *file,
     off64_t *offset,
     libcerror_error_t **error )
{
	libqcow_internal_file_t *internal_file = NULL;
	static char *function                  = "libqcow_file_get_offset";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libqcow_internal_file_t *) file;

	if( internal_file->file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file IO handle.",
		 function );

		return( -1 );
	}
	if( offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*offset = internal_file->current_offset;

#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Sets the parent (backing) file of a differential image
 * Returns 1 if successful or -1 on error
 */
int libqcow_file_set_parent_file(
     libqcow_file_t *file,
     libqcow_file_t *parent_file,
     libcerror_error_t **error )
{
	libqcow_internal_file_t *internal_file = NULL;
	static char *function                  = "libqcow_file_set_parent_file";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libqcow_internal_file_t *) file;

	if( internal_file->backing_filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing backing filename.",
		 function );

		return( -1 );
	}
	if( internal_file->parent_file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - parent file already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	internal_file->parent_file = parent_file;

#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Set the keys
 * This function needs to be used before one of the open functions
 * Returns 1 if successful or -1 on error
 */
int libqcow_file_set_keys(
     libqcow_file_t *file,
     const uint8_t *key,
     size_t key_size,
     libcerror_error_t **error )
{
	libqcow_internal_file_t *internal_file = NULL;
	static char *function                  = "libqcow_file_set_keys";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libqcow_internal_file_t *) file;

	if( internal_file->file_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - file IO handle already set.",
		 function );

		return( -1 );
	}
	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	if( key_size != 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported key size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( memory_copy(
	     internal_file->key_data,
	     key,
	     key_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy key.",
		 function );

		goto on_error;
	}
	internal_file->key_data_is_set = 1;

#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
	memory_set(
	 internal_file->key_data,
	 0,
	 16 );

#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_file->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Sets an UTF-8 formatted password
 * This function needs to be used before one of the open functions
 * Returns 1 if successful, 0 if password is invalid or -1 on error
 */
int libqcow_file_set_utf8_password(
     libqcow_file_t *file,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	libqcow_internal_file_t *internal_file = NULL;
	static char *function                  = "libqcow_file_set_utf8_password";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libqcow_internal_file_t *) file;

	if( internal_file->file_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - file IO handle already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( memory_set(
	     internal_file->key_data,
	     0,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear key data.",
		 function );

		goto on_error;
	}
	if( libuna_byte_stream_copy_from_utf8(
	     internal_file->key_data,
	     16,
	     LIBQCOW_CODEPAGE_US_ASCII,
	     utf8_string,
	     utf8_string_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy password.",
		 function );

		goto on_error;
	}
	internal_file->key_data_is_set = 1;

#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
	memory_set(
	 internal_file->key_data,
	 0,
	 16 );

#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_file->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Sets an UTF-16 formatted password
 * This function needs to be used before one of the open functions
 * Returns 1 if successful, 0 if password is invalid or -1 on error
 */
int libqcow_file_set_utf16_password(
     libqcow_file_t *file,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	libqcow_internal_file_t *internal_file = NULL;
	static char *function                  = "libqcow_file_set_utf16_password";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libqcow_internal_file_t *) file;

	if( internal_file->file_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - file IO handle already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( memory_set(
	     internal_file->key_data,
	     0,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear key data.",
		 function );

		goto on_error;
	}
	if( libuna_byte_stream_copy_from_utf16(
	     internal_file->key_data,
	     16,
	     LIBQCOW_CODEPAGE_US_ASCII,
	     utf16_string,
	     utf16_string_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy password.",
		 function );

		goto on_error;
	}
	internal_file->key_data_is_set = 1;

#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );

on_error:
	memory_set(
	 internal_file->key_data,
	 0,
	 16 );

#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_file->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Retrieves the media size
 * Returns 1 if successful or -1 on error
 */
int libqcow_file_get_media_size(
     libqcow_file_t *file,
     size64_t *media_size,
     libcerror_error_t **error )
{
	libqcow_internal_file_t *internal_file = NULL;
	static char *function                  = "libqcow_file_get_media_size";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libqcow_internal_file_t *) file;

	if( internal_file->file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file header.",
		 function );

		return( -1 );
	}
	if( media_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid media size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*media_size = internal_file->file_header->media_size;

#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Retrieves the format version
 * Returns 1 if successful or -1 on error
 */
int libqcow_file_get_format_version(
     libqcow_file_t *file,
     uint32_t *format_version,
     libcerror_error_t **error )
{
	libqcow_internal_file_t *internal_file = NULL;
	static char *function                  = "libqcow_file_get_format_version";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libqcow_internal_file_t *) file;

	if( internal_file->file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file header.",
		 function );

		return( -1 );
	}
	if( format_version == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid format version.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*format_version = internal_file->file_header->format_version;

#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Retrieves the encryption method
 * Returns 1 if successful or -1 on error
 */
int libqcow_file_get_encryption_method(
     libqcow_file_t *file,
     uint32_t *encryption_method,
     libcerror_error_t **error )
{
	libqcow_internal_file_t *internal_file = NULL;
	static char *function                  = "libqcow_file_get_encryption_method";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libqcow_internal_file_t *) file;

	if( internal_file->file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file IO handle.",
		 function );

		return( -1 );
	}
	if( encryption_method == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encryption method.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*encryption_method = internal_file->encryption_method;

#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded backing filename
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libqcow_file_get_utf8_backing_filename_size(
     libqcow_file_t *file,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libqcow_internal_file_t *internal_file = NULL;
	static char *function                  = "libqcow_file_get_utf8_backing_filename_size";
	int result                             = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libqcow_internal_file_t *) file;

	if( internal_file->file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file->backing_filename != NULL )
	{
		result = libuna_utf8_string_size_from_utf8_stream(
		          internal_file->backing_filename,
		          internal_file->backing_filename_size,
		          utf8_string_size,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-8 string size of backing filename.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-8 encoded backing filename
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libqcow_file_get_utf8_backing_filename(
     libqcow_file_t *file,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libqcow_internal_file_t *internal_file = NULL;
	static char *function                  = "libqcow_file_get_utf8_backing_filename";
	int result                             = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libqcow_internal_file_t *) file;

	if( internal_file->file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file->backing_filename != NULL )
	{
		result = libuna_utf8_string_copy_from_utf8_stream(
		          utf8_string,
		          utf8_string_size,
		          internal_file->backing_filename,
		          internal_file->backing_filename_size,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-8 string of backing filename.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-16 encoded backing filename
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libqcow_file_get_utf16_backing_filename_size(
     libqcow_file_t *file,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libqcow_internal_file_t *internal_file = NULL;
	static char *function                  = "libqcow_file_get_utf16_backing_filename_size";
	int result                             = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libqcow_internal_file_t *) file;

	if( internal_file->file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file->backing_filename != NULL )
	{
		result = libuna_utf16_string_size_from_utf8_stream(
		          internal_file->backing_filename,
		          internal_file->backing_filename_size,
		          utf16_string_size,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 string size of backing filename.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-16 encoded backing filename
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libqcow_file_get_utf16_backing_filename(
     libqcow_file_t *file,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libqcow_internal_file_t *internal_file = NULL;
	static char *function                  = "libqcow_file_get_utf16_backing_filename";
	int result                             = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libqcow_internal_file_t *) file;

	if( internal_file->file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file->backing_filename != NULL )
	{
		result = libuna_utf16_string_copy_from_utf8_stream(
		          utf16_string,
		          utf16_string_size,
		          internal_file->backing_filename,
		          internal_file->backing_filename_size,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 string of backing filename.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the number of snapshots
 * Returns 1 if successful or -1 on error
 */
int libqcow_file_get_number_of_snapshots(
     libqcow_file_t *file,
     int *number_of_snapshots,
     libcerror_error_t **error )
{
	libqcow_internal_file_t *internal_file = NULL;
	static char *function                  = "libqcow_file_get_number_of_snapshots";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libqcow_internal_file_t *) file;

	if( internal_file->file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file header.",
		 function );

		return( -1 );
	}
	if( number_of_snapshots == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of snapshots.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	/* TODO retrieve number of snapshots from array */
	*number_of_snapshots = (int) internal_file->file_header->number_of_snapshots;

#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

