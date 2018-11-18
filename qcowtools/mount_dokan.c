/*
 * Mount tool dokan functions
 *
 * Copyright (C) 2010-2018, Joachim Metz <joachim.metz@gmail.com>
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
#include <memory.h>
#include <types.h>
#include <wide_string.h>

#include "mount_dokan.h"
#include "mount_handle.h"
#include "qcowtools_libcerror.h"
#include "qcowtools_libcnotify.h"
#include "qcowtools_libqcow.h"
#include "qcowtools_unused.h"

extern mount_handle_t *qcowmount_mount_handle;

#if defined( HAVE_LIBDOKAN )

static wchar_t *mount_dokan_path_prefix      = L"\\QCOW";
static size_t mount_dokan_path_prefix_length = 5;

/* Opens a file or directory
 * Returns 0 if successful or a negative error code otherwise
 */
#if ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 )
int __stdcall mount_dokan_CreateFile(
               const wchar_t *path,
               DWORD desired_access,
               DWORD share_mode QCOWTOOLS_ATTRIBUTE_UNUSED,
               DWORD creation_disposition,
               DWORD attribute_flags QCOWTOOLS_ATTRIBUTE_UNUSED,
               DOKAN_FILE_INFO *file_info )
#else
NTSTATUS __stdcall mount_dokan_CreateFile(
                    const wchar_t *path,
                    DOKAN_IO_SECURITY_CONTEXT *security_context,
                    ACCESS_MASK desired_access,
                    ULONG file_attributes,
                    ULONG share_access,
                    ULONG creation_disposition,
                    ULONG creation_options,
                    DOKAN_FILE_INFO *file_info )
#endif
{
#if ! ( ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 ) )
	SECURITY_ATTRIBUTES security_attributes;

	ACCESS_MASK result_desired_access      = 0;
	DWORD result_creation_disposition      = 0;
	DWORD result_file_attributes_and_flags = 0;
#endif

	libcerror_error_t *error               = NULL;
	static char *function                  = "mount_dokan_CreateFile";
	size_t path_length                     = 0;
	int result                             = 0;

#if ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 )
	QCOWTOOLS_UNREFERENCED_PARAMETER( share_mode )
	QCOWTOOLS_UNREFERENCED_PARAMETER( attribute_flags )

#else
	security_attributes.nLength              = sizeof( SECURITY_ATTRIBUTES );
	security_attributes.lpSecurityDescriptor = security_context->AccessState.SecurityDescriptor;
	security_attributes.bInheritHandle       = FALSE;

	DokanMapKernelToUserCreateFileFlags(
	 desired_access,
	 file_attributes,
	 creation_options,
	 creation_disposition,
	 &result_desired_access,
	 &result_creation_disposition,
	 &result_creation_disposition );
#endif

	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	if( ( desired_access & GENERIC_WRITE ) != 0 )
	{
		return( -ERROR_WRITE_PROTECT );
	}
	/* Ignore the share_mode
	 */
	if( creation_disposition == CREATE_NEW )
	{
		return( -ERROR_FILE_EXISTS );
	}
	else if( creation_disposition == CREATE_ALWAYS )
	{
		return( -ERROR_ALREADY_EXISTS );
	}
	else if( creation_disposition == OPEN_ALWAYS )
	{
		return( -ERROR_FILE_NOT_FOUND );
	}
	else if( creation_disposition == TRUNCATE_EXISTING )
	{
		return( -ERROR_FILE_NOT_FOUND );
	}
	else if( creation_disposition != OPEN_EXISTING )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid creation disposition.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	if( file_info == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file info.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	path_length = wide_string_length(
	               path );

	if( path_length == 1 )
	{
		if( path[ 0 ] != (wchar_t) '\\' )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported path: %ls.",
			 function,
			 path );

			result = -ERROR_FILE_NOT_FOUND;

			goto on_error;
		}
	}
	else
	{
		if( ( path_length <= mount_dokan_path_prefix_length )
		 || ( path_length > ( mount_dokan_path_prefix_length + 3 ) )
		 || ( wide_string_compare(
		       path,
		       mount_dokan_path_prefix,
		       mount_dokan_path_prefix_length ) != 0 ) )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported path: %ls.",
			 function,
			 path );

			result = -ERROR_FILE_NOT_FOUND;

			goto on_error;
		}
	}
	return( 0 );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
#if ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 )
	return( result );
#else
	return( DokanNtStatusFromWin32( result ) );
#endif
}

#if ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 )

/* Opens a directory
 * Returns 0 if successful or a negative error code otherwise
 */
int __stdcall mount_dokan_OpenDirectory(
               const wchar_t *path,
               DOKAN_FILE_INFO *file_info QCOWTOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "mount_dokan_OpenDirectory";
	size_t path_length       = 0;
	int result               = 0;

	QCOWTOOLS_UNREFERENCED_PARAMETER( file_info )

	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	path_length = wide_string_length(
	               path );

	if( ( path_length != 1 )
	 || ( path[ 0 ] != (wchar_t) '\\' ) )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported path: %ls.",
		 function,
		 path );

		result = -ERROR_FILE_NOT_FOUND;

		goto on_error;
	}
	return( 0 );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
#if ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 )
	return( result );
#else
	return( DokanNtStatusFromWin32( result ) );
#endif
}

#endif /* ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 ) */

/* Closes a file or direcotry
 * Returns 0 if successful or a negative error code otherwise
 */
#if ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 )
int __stdcall mount_dokan_CloseFile(
               const wchar_t *path,
               DOKAN_FILE_INFO *file_info QCOWTOOLS_ATTRIBUTE_UNUSED )
#else
NTSTATUS __stdcall mount_dokan_CloseFile(
                    const wchar_t *path,
                    DOKAN_FILE_INFO *file_info QCOWTOOLS_ATTRIBUTE_UNUSED )
#endif
{
	libcerror_error_t *error = NULL;
	static char *function    = "mount_dokan_CloseFile";
	int result               = 0;

	QCOWTOOLS_UNREFERENCED_PARAMETER( file_info )

	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	return( 0 );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
#if ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 )
	return( result );
#else
	return( DokanNtStatusFromWin32( result ) );
#endif
}

/* Reads a buffer of data at the specified offset
 * Returns 0 if successful or a negative error code otherwise
 */
#if ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 )
int __stdcall mount_dokan_ReadFile(
               const wchar_t *path,
               void *buffer,
               DWORD number_of_bytes_to_read,
               DWORD *number_of_bytes_read,
               LONGLONG offset,
               DOKAN_FILE_INFO *file_info QCOWTOOLS_ATTRIBUTE_UNUSED )
#else
NTSTATUS __stdcall mount_dokan_ReadFile(
                    const wchar_t *path,
                    void *buffer,
                    DWORD number_of_bytes_to_read,
                    DWORD *number_of_bytes_read,
                    LONGLONG offset,
                    DOKAN_FILE_INFO *file_info QCOWTOOLS_ATTRIBUTE_UNUSED )
#endif
{
	libcerror_error_t *error = NULL;
	static char *function    = "mount_dokan_ReadFile";
	size_t path_length       = 0;
	ssize_t read_count       = 0;
	int image_index          = 0;
	int result               = 0;
	int string_index         = 0;

	QCOWTOOLS_UNREFERENCED_PARAMETER( file_info )

	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	if( number_of_bytes_to_read > (DWORD) INT32_MAX )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid number of bytes to read value exceeds maximum.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	if( number_of_bytes_read == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of bytes read.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	path_length = wide_string_length(
	               path );

	if( ( path_length <= mount_dokan_path_prefix_length )
         || ( path_length > ( mount_dokan_path_prefix_length + 3 ) )
	 || ( wide_string_compare(
	       path,
	       mount_dokan_path_prefix,
	       mount_dokan_path_prefix_length ) != 0 ) )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported path: %ls.",
		 function,
		 path );

		result = -ERROR_FILE_NOT_FOUND;

		goto on_error;
	}
	string_index = (int) mount_dokan_path_prefix_length;

	image_index = path[ string_index++ ] - (wchar_t) '0';

	if( string_index < (int) path_length )
	{
		image_index *= 10;
		image_index += path[ string_index++ ] - (wchar_t) '0';
	}
	if( string_index < (int) path_length )
	{
		image_index *= 10;
		image_index += path[ string_index++ ] - (wchar_t) '0';
	}
	image_index -= 1;

	if( mount_handle_seek_offset(
	     qcowmount_mount_handle,
	     image_index,
	     (off64_t) offset,
	     SEEK_SET,
	     &error ) == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset in mount handle.",
		 function );

		result = -ERROR_SEEK_ON_DEVICE;

		goto on_error;
	}
	read_count = mount_handle_read_buffer(
		      qcowmount_mount_handle,
		      image_index,
		      (uint8_t *) buffer,
		      (size_t) number_of_bytes_to_read,
		      &error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from mount handle.",
		 function );

		result = -ERROR_READ_FAULT;

		goto on_error;
	}
	if( read_count > (size_t) INT32_MAX )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid read count value exceeds maximum.",
		 function );

		result = -ERROR_READ_FAULT;

		goto on_error;
	}
	/* Dokan does not require the read function to return ERROR_HANDLE_EOF
	 */
	*number_of_bytes_read = (DWORD) read_count;

	return( 0 );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
#if ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 )
	return( result );
#else
	return( DokanNtStatusFromWin32( result ) );
#endif
}

/* Sets the values in a find data structure
 * Returns 1 if successful or -1 on error
 */
int mount_dokan_set_find_data(
     WIN32_FIND_DATAW *find_data,
     size64_t size,
     int number_of_sub_items,
     uint8_t use_mount_time,
     libcerror_error_t **error )
{
	static char *function = "mount_dokan_set_find_data";

	if( find_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid find data.",
		 function );

		return( -1 );
	}
/* TODO implement
	if( use_mount_time != 0 )
	{
	}
*/
	if( size > 0 )
	{
		find_data->nFileSizeHigh = (DWORD) ( size >> 32 );
		find_data->nFileSizeLow  = (DWORD) ( size & 0xffffffffUL );
	}
	find_data->dwFileAttributes = FILE_ATTRIBUTE_READONLY;

	if( number_of_sub_items > 0 )
	{
		find_data->dwFileAttributes |= FILE_ATTRIBUTE_DIRECTORY;
	}
	return( 1 );
}

/* Fills a directory entry
 * Returns 1 if successful or -1 on error
 */
int mount_dokan_filldir(
     PFillFindData fill_find_data,
     DOKAN_FILE_INFO *file_info,
     wchar_t *name,
     size_t name_size,
     WIN32_FIND_DATAW *find_data,
     mount_handle_t *mount_handle,
     int image_index,
     uint8_t use_mount_time,
     libcerror_error_t **error )
{
	static char *function   = "mount_dokan_filldir";
	size64_t media_size     = 0;
	int number_of_sub_items = 0;

	if( fill_find_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid fill find data.",
		 function );

		return( -1 );
	}
	if( name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name.",
		 function );

		return( -1 );
	}
	if( name_size > (size_t) MAX_PATH )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid name size value out of bounds.",
		 function );

		return( -1 );
	}
	if( mount_handle == NULL )
	{
		number_of_sub_items = 1;
	}
	else
	{
		if( mount_handle_get_media_size(
		     mount_handle,
		     image_index,
		     &media_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve media size.",
			 function );

			return( -1 );
		}
	}
	if( memory_set(
	     find_data,
	     0,
	     sizeof( WIN32_FIND_DATAW ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear find data.",
		 function );

		return( -1 );
	}
	if( wide_string_copy(
	     find_data->cFileName,
	     name,
	     name_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy filename.",
		 function );

		return( -1 );
	}
	if( name_size <= (size_t) 14 )
	{
		if( wide_string_copy(
		     find_data->cAlternateFileName,
		     name,
		     name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy alternate filename.",
			 function );

			return( -1 );
		}
	}
	if( mount_dokan_set_find_data(
	     find_data,
	     media_size,
	     number_of_sub_items,
	     use_mount_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set find data.",
		 function );

		return( -1 );
	}
	if( fill_find_data(
	     find_data,
	     file_info ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set directory entry.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads a directory
 * Returns 0 if successful or a negative error code otherwise
 */
#if ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 )
int __stdcall mount_dokan_FindFiles(
               const wchar_t *path,
               PFillFindData fill_find_data,
               DOKAN_FILE_INFO *file_info )
#else
NTSTATUS __stdcall mount_dokan_FindFiles(
                    const wchar_t *path,
                    PFillFindData fill_find_data,
                    DOKAN_FILE_INFO *file_info )
#endif
{
	WIN32_FIND_DATAW find_data;

	wchar_t mount_dokan_path[ 10 ];

	libcerror_error_t *error = NULL;
	static char *function    = "mount_dokan_FindFiles";
	size_t path_length       = 0;
	int image_index          = 0;
	int number_of_images     = 0;
	int result               = 0;
	int string_index         = 0;

	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	path_length = wide_string_length(
	               path );

	if( ( path_length != 1 )
	 || ( path[ 0 ] != (wchar_t) '\\' ) )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported path: %ls.",
		 function,
		 path );

		result = -ERROR_FILE_NOT_FOUND;

		goto on_error;
	}
	if( wide_string_copy(
	     mount_dokan_path,
	     mount_dokan_path_prefix,
	     mount_dokan_path_prefix_length ) == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy path prefix.",
		 function );

		result = -ERROR_GEN_FAILURE;

		goto on_error;
	}
	if( mount_handle_get_number_of_images(
	     qcowmount_mount_handle,
	     &number_of_images,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of images.",
		 function );

		result = -ERROR_GEN_FAILURE;

		goto on_error;
	}
	if( ( number_of_images < 0 )
	 || ( number_of_images > 99 ) )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported number of images.",
		 function );

		result = -ERROR_GEN_FAILURE;

		goto on_error;
	}
	if( mount_dokan_filldir(
	     fill_find_data,
	     file_info,
	     L".",
	     2,
	     &find_data,
	     NULL,
	     -1,
	     1,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set find data.",
		 function );

		result = -ERROR_GEN_FAILURE;

		goto on_error;
	}
	if( mount_dokan_filldir(
	     fill_find_data,
	     file_info,
	     L"..",
	     3,
	     &find_data,
	     NULL,
	     -1,
	     0,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set find data.",
		 function );

		result = -ERROR_GEN_FAILURE;

		goto on_error;
	}
	for( image_index = 1;
	     image_index <= number_of_images;
	     image_index++ )
	{
		string_index = (int) mount_dokan_path_prefix_length;

		if( image_index >= 100 )
		{
			mount_dokan_path[ string_index++ ] = (wchar_t) ( '0' + ( image_index / 100 ) );
		}
		if( image_index >= 10 )
		{
			mount_dokan_path[ string_index++ ] = (wchar_t) ( '0' + ( image_index / 10 ) );
		}
		mount_dokan_path[ string_index++ ] = (wchar_t) ( '0' + ( image_index % 10 ) );
		mount_dokan_path[ string_index++ ] = 0;

		if( mount_dokan_filldir(
		     fill_find_data,
		     file_info,
		     &( mount_dokan_path[ 1 ] ),
		     string_index - 1,
		     &find_data,
		     qcowmount_mount_handle,
		     image_index - 1,
		     1,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set find data.",
			 function );

			result = -ERROR_GEN_FAILURE;

			goto on_error;
		}
	}
	return( 0 );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
#if ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 )
	return( result );
#else
	return( DokanNtStatusFromWin32( result ) );
#endif
}

/* Sets the values in a file information structure
 * Returns 1 if successful or -1 on error
 */
int mount_dokan_set_file_information(
     BY_HANDLE_FILE_INFORMATION *file_information,
     size64_t size,
     int number_of_sub_items,
     uint8_t use_mount_time,
     libcerror_error_t **error )
{
	static char *function = "mount_dokan_set_file_information";

	if( file_information == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file information.",
		 function );

		return( -1 );
	}
/* TODO implement
	if( use_mount_time != 0 )
	{
	}
*/
	if( size > 0 )
	{
		file_information->nFileSizeHigh = (DWORD) ( size >> 32 );
		file_information->nFileSizeLow  = (DWORD) ( size & 0xffffffffUL );
	}
	file_information->dwFileAttributes = FILE_ATTRIBUTE_READONLY;

	if( number_of_sub_items > 0 )
	{
		file_information->dwFileAttributes |= FILE_ATTRIBUTE_DIRECTORY;
	}
	return( 1 );
}

/* Retrieves the file information
 * Returns 0 if successful or a negative error code otherwise
 */
#if ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 )
int __stdcall mount_dokan_GetFileInformation(
               const wchar_t *path,
               BY_HANDLE_FILE_INFORMATION *file_information,
               DOKAN_FILE_INFO *file_info )
#else
NTSTATUS __stdcall mount_dokan_GetFileInformation(
                    const wchar_t *path,
                    BY_HANDLE_FILE_INFORMATION *file_information,
                    DOKAN_FILE_INFO *file_info )
#endif
{
	libcerror_error_t *error = NULL;
	static char *function    = "mount_dokan_GetFileInformation";
	size64_t media_size      = 0;
	size_t path_length       = 0;
	int image_index          = 0;
	int number_of_sub_items  = 0;
	int result               = 0;
	int string_index         = 0;
	uint8_t use_mount_time   = 0;

	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	if( file_info == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file info.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	path_length = wide_string_length(
	               path );

	if( path_length == 1 )
	{
		if( path[ 0 ] != (wchar_t) '\\' )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported path: %ls.",
			 function,
			 path );

			result = -ERROR_FILE_NOT_FOUND;

			goto on_error;
		}
		number_of_sub_items = 1;
		use_mount_time      = 1;
	}
	else
	{
		if( ( path_length <= mount_dokan_path_prefix_length )
		 || ( path_length > ( mount_dokan_path_prefix_length + 3 ) )
		 || ( wide_string_compare(
		       path,
		       mount_dokan_path_prefix,
		       mount_dokan_path_prefix_length ) != 0 ) )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported path: %ls.",
			 function,
			 path );

			result = -ERROR_FILE_NOT_FOUND;

			goto on_error;
		}
		string_index = (int) mount_dokan_path_prefix_length;

		image_index = path[ string_index++ ] - (wchar_t) '0';

		if( string_index < (int) path_length )
		{
			image_index *= 10;
			image_index += path[ string_index++ ] - (wchar_t) '0';
		}
		if( string_index < (int) path_length )
		{
			image_index *= 10;
			image_index += path[ string_index++ ] - (wchar_t) '0';
		}
		image_index -= 1;

		if( mount_handle_get_media_size(
		     qcowmount_mount_handle,
		     image_index,
		     &media_size,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve media size.",
			 function );

			result = -ERROR_GEN_FAILURE;

			goto on_error;
		}
		use_mount_time = 1;
	}
	if( mount_dokan_set_file_information(
	     file_information,
	     media_size,
	     number_of_sub_items,
	     use_mount_time,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set file info.",
		 function );

		result = -ERROR_GEN_FAILURE;

		goto on_error;
	}
	return( 0 );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
#if ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 )
	return( result );
#else
	return( DokanNtStatusFromWin32( result ) );
#endif
}

/* Retrieves the volume information
 * Returns 0 if successful or a negative error code otherwise
 */
#if ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 )
int __stdcall mount_dokan_GetVolumeInformation(
               wchar_t *volume_name,
               DWORD volume_name_size,
               DWORD *volume_serial_number,
               DWORD *maximum_filename_length,
               DWORD *file_system_flags,
               wchar_t *file_system_name,
               DWORD file_system_name_size,
               DOKAN_FILE_INFO *file_info QCOWTOOLS_ATTRIBUTE_UNUSED )
#else
NTSTATUS __stdcall mount_dokan_GetVolumeInformation(
                    wchar_t *volume_name,
                    DWORD volume_name_size,
                    DWORD *volume_serial_number,
                    DWORD *maximum_filename_length,
                    DWORD *file_system_flags,
                    wchar_t *file_system_name,
                    DWORD file_system_name_size,
                    DOKAN_FILE_INFO *file_info QCOWTOOLS_ATTRIBUTE_UNUSED )
#endif
{
	libcerror_error_t *error = NULL;
	static char *function    = "mount_dokan_GetVolumeInformation";
	int result               = 0;

	QCOWTOOLS_UNREFERENCED_PARAMETER( file_info )

	if( ( volume_name != NULL )
	 && ( volume_name_size > (DWORD) ( sizeof( wchar_t ) * 5 ) ) )
	{
		/* Using wcsncpy seems to cause strange behavior here
		 */
		if( memory_copy(
		     volume_name,
		     L"QCOW",
		     sizeof( wchar_t ) * 5 ) == NULL )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy volume name.",
			 function );

			result = -ERROR_GEN_FAILURE;

			goto on_error;
		}
	}
	if( volume_serial_number != NULL )
	{
		/* If this value contains 0 it can crash the system is this an issue in Dokan?
		 */
		*volume_serial_number = 0x19831116;
	}
	if( maximum_filename_length != NULL )
	{
		*maximum_filename_length = 256;
	}
	if( file_system_flags != NULL )
	{
		*file_system_flags = FILE_CASE_SENSITIVE_SEARCH
		                   | FILE_CASE_PRESERVED_NAMES
		                   | FILE_UNICODE_ON_DISK
		                   | FILE_READ_ONLY_VOLUME;
	}
	if( ( file_system_name != NULL )
	 && ( file_system_name_size > (DWORD) ( sizeof( wchar_t ) * 6 ) ) )
	{
		/* Using wcsncpy seems to cause strange behavior here
		 */
		if( memory_copy(
		     file_system_name,
		     L"Dokan",
		     sizeof( wchar_t ) * 6 ) == NULL )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy file system name.",
			 function );

			result = -ERROR_GEN_FAILURE;

			goto on_error;
		}
	}
	return( 0 );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
#if ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 )
	return( result );
#else
	return( DokanNtStatusFromWin32( result ) );
#endif
}

#if ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 )

/* Unmount the image
 * Returns 0 if successful or a negative error code otherwise
 */
int __stdcall mount_dokan_Unmount(
               DOKAN_FILE_INFO *file_info QCOWTOOLS_ATTRIBUTE_UNUSED )
{
	static char *function = "mount_dokan_Unmount";

	QCOWTOOLS_UNREFERENCED_PARAMETER( file_info )

	return( 0 );
}

#endif /* ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 ) */

#endif /* defined( HAVE_LIBDOKAN ) */

