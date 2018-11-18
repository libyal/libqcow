/*
 * Mount tool fuse functions
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
#include <narrow_string.h>
#include <types.h>

#if defined( HAVE_ERRNO_H ) || defined( WINAPI )
#include <errno.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if !defined( WINAPI )
#if defined( TIME_WITH_SYS_TIME )
#include <sys/time.h>
#include <time.h>
#elif defined( HAVE_SYS_TIME_H )
#include <sys/time.h>
#else
#include <time.h>
#endif
#endif

#include "mount_fuse.h"
#include "mount_handle.h"
#include "qcowtools_libcerror.h"
#include "qcowtools_libcnotify.h"
#include "qcowtools_libqcow.h"
#include "qcowtools_unused.h"

extern mount_handle_t *qcowmount_mount_handle;

#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )

#if ( SIZEOF_OFF_T != 8 ) && ( SIZEOF_OFF_T != 4 )
#error Size of off_t not supported
#endif

static char *mount_fuse_path_prefix         = "/qcow";
static size_t mount_fuse_path_prefix_length = 5;

#if defined( HAVE_TIME )
time_t mount_timestamp                      = 0;
#endif

/* Opens a file or directory
 * Returns 0 if successful or a negative errno value otherwise
 */
int mount_fuse_open(
     const char *path,
     struct fuse_file_info *file_info )
{
	libcerror_error_t *error = NULL;
	static char *function    = "mount_fuse_open";
	size_t path_length       = 0;
	int result               = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: %s\n",
		 function,
		 path );
	}
#endif
	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -EINVAL;

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

		result = -EINVAL;

		goto on_error;
	}
	path_length = narrow_string_length(
	               path );

	if( ( path_length <= mount_fuse_path_prefix_length )
         || ( path_length > ( mount_fuse_path_prefix_length + 3 ) )
	 || ( narrow_string_compare(
	       path,
	       mount_fuse_path_prefix,
	       mount_fuse_path_prefix_length ) != 0 ) )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported path: %s.",
		 function,
		 path );

		result = -ENOENT;

		goto on_error;
	}
	if( ( file_info->flags & 0x03 ) != O_RDONLY )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		result = -EACCES;

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
	return( result );
}

/* Reads a buffer of data at the specified offset
 * Returns number of bytes read if successful or a negative errno value otherwise
 */
int mount_fuse_read(
     const char *path,
     char *buffer,
     size_t size,
     off_t offset,
     struct fuse_file_info *file_info )
{
	libcerror_error_t *error = NULL;
	static char *function    = "mount_fuse_read";
	size_t path_length       = 0;
	ssize_t read_count       = 0;
	int image_index          = 0;
	int result               = 0;
	int string_index         = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: %s\n",
		 function,
		 path );
	}
#endif
	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	if( size > (size_t) INT_MAX )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		result = -EINVAL;

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

		result = -EINVAL;

		goto on_error;
	}
	path_length = narrow_string_length(
	               path );

	if( ( path_length <= mount_fuse_path_prefix_length )
         || ( path_length > ( mount_fuse_path_prefix_length + 3 ) )
	 || ( narrow_string_compare(
	       path,
	       mount_fuse_path_prefix,
	       mount_fuse_path_prefix_length ) != 0 ) )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported path: %s.",
		 function,
		 path );

		result = -ENOENT;

		goto on_error;
	}
	string_index = (int) mount_fuse_path_prefix_length;

	image_index = path[ string_index++ ] - '0';

	if( string_index < (int) path_length )
	{
		image_index *= 10;
		image_index += path[ string_index++ ] - '0';
	}
	if( string_index < (int) path_length )
	{
		image_index *= 10;
		image_index += path[ string_index++ ] - '0';
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

		result = -EIO;

		goto on_error;
	}
	read_count = mount_handle_read_buffer(
	              qcowmount_mount_handle,
	              image_index,
	              (uint8_t *) buffer,
	              size,
	              &error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from mount handle.",
		 function );

		result = -EIO;

		goto on_error;
	}
	return( (int) read_count );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	return( result );
}

/* Sets the values in a stat info structure
 * Returns 1 if successful or -1 on error
 */
int mount_fuse_set_stat_info(
     struct stat *stat_info,
     size64_t size,
     int number_of_sub_items,
     uint8_t use_mount_time,
     libcerror_error_t **error )
{
	static char *function = "mount_fuse_set_stat_info";

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: %s\n",
		 function,
		 path );
	}
#endif
	if( stat_info == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stat info.",
		 function );

		return( -1 );
	}
#if defined( HAVE_TIME )
	if( use_mount_time != 0 )
	{
		if( mount_timestamp == 0 )
		{
			if( time(
			     &mount_timestamp ) == (time_t) -1 )
			{
				mount_timestamp = 0;
			}
		}
		stat_info->st_atime = mount_timestamp;
		stat_info->st_mtime = mount_timestamp;
		stat_info->st_ctime = mount_timestamp;
	}
#endif
	if( size != 0 )
	{
#if SIZEOF_OFF_T <= 4
		if( size > (size64_t) UINT32_MAX )
#else
		if( size > (size64_t) INT64_MAX )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid size value out of bounds.",
			 function );

			return( -1 );
		}
		stat_info->st_size = (off_t) size;
	}
	if( number_of_sub_items > 0 )
	{
		stat_info->st_mode  = S_IFDIR | 0555;
		stat_info->st_nlink = 2;
	}
	else
	{
		stat_info->st_mode  = S_IFREG | 0444;
		stat_info->st_nlink = 1;
	}
#if defined( HAVE_GETEUID )
	stat_info->st_uid = geteuid();
#endif
#if defined( HAVE_GETEGID )
	stat_info->st_gid = getegid();
#endif
	return( 1 );
}

/* Fills a directory entry
 * Returns 1 if successful or -1 on error
 */
int mount_fuse_filldir(
     void *buffer,
     fuse_fill_dir_t filler,
     char *name,
     size_t name_size,
     struct stat *stat_info,
     mount_handle_t *mount_handle,
     int image_index,
     uint8_t use_mount_time,
     libcerror_error_t **error )
{
	static char *function   = "mount_fuse_filldir";
	size64_t media_size     = 0;
	int number_of_sub_items = 0;

	if( filler == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filler.",
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
	     stat_info,
	     0,
	     sizeof( struct stat ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear stat info.",
		 function );

		return( -1 );
	}
	if( mount_fuse_set_stat_info(
	     stat_info,
	     media_size,
	     number_of_sub_items,
	     use_mount_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set stat info.",
		 function );

		return( -1 );
	}
	if( filler(
	     buffer,
	     name,
	     stat_info,
	     0 ) == 1 )
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
 * Returns 0 if successful or a negative errno value otherwise
 */
int mount_fuse_readdir(
     const char *path,
     void *buffer,
     fuse_fill_dir_t filler,
     off_t offset QCOWTOOLS_ATTRIBUTE_UNUSED,
     struct fuse_file_info *file_info QCOWTOOLS_ATTRIBUTE_UNUSED )
{
	char mount_fuse_path[ 10 ];

	libcerror_error_t *error = NULL;
	struct stat *stat_info   = NULL;
	static char *function    = "mount_fuse_readdir";
	size_t path_length       = 0;
	int image_index          = 0;
	int number_of_images     = 0;
	int result               = 0;
	int string_index         = 0;

	QCOWTOOLS_UNREFERENCED_PARAMETER( offset )
	QCOWTOOLS_UNREFERENCED_PARAMETER( file_info )

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: %s\n",
		 function,
		 path );
	}
#endif
	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	path_length = narrow_string_length(
	               path );

	if( ( path_length != 1 )
	 || ( path[ 0 ] != '/' ) )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported path: %s.",
		 function,
		 path );

		result = -ENOENT;

		goto on_error;
	}
	if( narrow_string_copy(
	     mount_fuse_path,
	     mount_fuse_path_prefix,
	     mount_fuse_path_prefix_length ) == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy path prefix.",
		 function );

		result = -errno;

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

		result = -EIO;

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

		result = -ENOENT;

		goto on_error;
	}
	stat_info = memory_allocate_structure(
	             struct stat );

	if( stat_info == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create stat info.",
		 function );

		result = errno;

		goto on_error;
	}
	if( mount_fuse_filldir(
	     buffer,
	     filler,
	     ".",
	     2,
	     stat_info,
	     NULL,
	     -1,
	     1,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set directory entry.",
		 function );

		result = -EIO;

		goto on_error;
	}
	if( mount_fuse_filldir(
	     buffer,
	     filler,
	     "..",
	     3,
	     stat_info,
	     NULL,
	     -1,
	     0,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set directory entry.",
		 function );

		result = -EIO;

		goto on_error;
	}
	for( image_index = 1;
	     image_index <= number_of_images;
	     image_index++ )
	{
		string_index = mount_fuse_path_prefix_length;

		if( image_index >= 100 )
		{
			mount_fuse_path[ string_index++ ] = '0' + (char) ( image_index / 100 );
		}
		if( image_index >= 10 )
		{
			mount_fuse_path[ string_index++ ] = '0' + (char) ( image_index / 10 );
		}
		mount_fuse_path[ string_index++ ] = '0' + (char) ( image_index % 10 );
		mount_fuse_path[ string_index++ ] = 0;

		if( mount_fuse_filldir(
		     buffer,
		     filler,
		     &( mount_fuse_path[ 1 ] ),
		     string_index - 1,
		     stat_info,
		     qcowmount_mount_handle,
		     image_index - 1,
		     1,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set directory entry.",
			 function );

			result = -EIO;

			goto on_error;
		}
	}
	memory_free(
	 stat_info );

	return( 0 );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( stat_info != NULL )
	{
		memory_free(
		 stat_info );
	}
	return( result );
}

/* Retrieves the file stat info
 * Returns 0 if successful or a negative errno value otherwise
 */
int mount_fuse_getattr(
     const char *path,
     struct stat *stat_info )
{
	libcerror_error_t *error = NULL;
	static char *function    = "mount_fuse_getattr";
	size64_t media_size      = 0;
	size_t path_length       = 0;
	int image_index          = 0;
	int number_of_sub_items  = 0;
	int result               = -ENOENT;
	int string_index         = 0;
	uint8_t use_mount_time   = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: %s\n",
		 function,
		 path );
	}
#endif
	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	if( stat_info == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stat info.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	if( memory_set(
	     stat_info,
	     0,
	     sizeof( struct stat ) ) == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear stat info.",
		 function );

		result = errno;

		goto on_error;
	}
	path_length = narrow_string_length(
	               path );

	if( path_length == 1 )
	{
		if( path[ 0 ] == '/' )
		{
			number_of_sub_items = 1;
			use_mount_time      = 1;
			result              = 0;
		}
	}
	else if( ( path_length > mount_fuse_path_prefix_length )
	      && ( path_length <= ( mount_fuse_path_prefix_length + 3 ) ) )
	{
		if( narrow_string_compare(
		     path,
		     mount_fuse_path_prefix,
		     mount_fuse_path_prefix_length ) == 0 )
		{
			string_index = mount_fuse_path_prefix_length;

			image_index = path[ string_index++ ] - '0';

			if( string_index < (int) path_length )
			{
				image_index *= 10;
				image_index += path[ string_index++ ] - '0';
			}
			if( string_index < (int) path_length )
			{
				image_index *= 10;
				image_index += path[ string_index++ ] - '0';
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

				result = -EIO;

				goto on_error;
			}
			use_mount_time = 1;
			result         = 0;
		}
	}
	if( result == 0 )
	{
		if( mount_fuse_set_stat_info(
		     stat_info,
		     media_size,
		     number_of_sub_items,
		     use_mount_time,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set stat info.",
			 function );

			result = -EIO;

			goto on_error;
		}
	}
	return( result );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	return( result );
}

/* Cleans up when fuse is done
 */
void mount_fuse_destroy(
      void *private_data QCOWTOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "mount_fuse_destroy";

	QCOWTOOLS_UNREFERENCED_PARAMETER( private_data )

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s\n",
		 function );
	}
#endif
	if( qcowmount_mount_handle != NULL )
	{
		if( mount_handle_free(
		     &qcowmount_mount_handle,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free mount handle.",
			 function );

			goto on_error;
		}
	}
	return;

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	return;
}

#endif /* defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE ) */

