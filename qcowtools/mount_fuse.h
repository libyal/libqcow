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

#if !defined( _MOUNT_FUSE_H )
#define _MOUNT_FUSE_H

#include <common.h>
#include <types.h>

#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )
#define FUSE_USE_VERSION	26

#if defined( HAVE_LIBFUSE )
#include <fuse.h>

#elif defined( HAVE_LIBOSXFUSE )
#include <osxfuse/fuse.h>
#endif

#endif /* defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE ) */

#include "mount_handle.h"
#include "qcowtools_libcerror.h"
#include "qcowtools_libqcow.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )

int mount_fuse_open(
     const char *path,
     struct fuse_file_info *file_info );

int mount_fuse_read(
     const char *path,
     char *buffer,
     size_t size,
     off_t offset,
     struct fuse_file_info *file_info );

int mount_fuse_set_stat_info(
     struct stat *stat_info,
     size64_t size,
     int number_of_sub_items,
     uint8_t use_mount_time,
     libcerror_error_t **error );

int mount_fuse_filldir(
     void *buffer,
     fuse_fill_dir_t filler,
     char *name,
     size_t name_size,
     struct stat *stat_info,
     mount_handle_t *mount_handle,
     int image_index,
     uint8_t use_mount_time,
     libcerror_error_t **error );

int mount_fuse_readdir(
     const char *path,
     void *buffer,
     fuse_fill_dir_t filler,
     off_t offset,
     struct fuse_file_info *file_info );

int mount_fuse_getattr(
     const char *path,
     struct stat *stat_info );

void mount_fuse_destroy(
      void *private_data );

#endif /* defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _MOUNT_FUSE_H ) */

