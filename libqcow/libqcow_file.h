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

#if !defined( _LIBQCOW_INTERNAL_FILE_H )
#define _LIBQCOW_INTERNAL_FILE_H

#include <common.h>
#include <types.h>

#include "libqcow_cluster_table.h"
#include "libqcow_encryption.h"
#include "libqcow_extern.h"
#include "libqcow_file_header.h"
#include "libqcow_io_handle.h"
#include "libqcow_libbfio.h"
#include "libqcow_libcerror.h"
#include "libqcow_libcthreads.h"
#include "libqcow_libfcache.h"
#include "libqcow_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libqcow_internal_file libqcow_internal_file_t;

struct libqcow_internal_file
{
	/* The current (storage media) offset
	 */
	off64_t current_offset;

	/* The IO handle
	 */
	libqcow_io_handle_t *io_handle;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* Value to indicate if the file IO handle was created inside the library
	 */
	uint8_t file_io_handle_created_in_library;

	/* Value to indicate if the file IO handle was opened inside the library
	 */
	uint8_t file_io_handle_opened_in_library;

	/* The (file) size
	 */
	size64_t size;

	/* The file header
	 */
	libqcow_file_header_t *file_header;

	/* The backing filename
	 */
	uint8_t *backing_filename;

	/* The backing filename size
	 */
	size_t backing_filename_size;

	/* The encryption method
	 */
	uint32_t encryption_method;

	/* The encryption context
	 */
	libqcow_encryption_context_t *encryption_context;

	/* The key data
	 */
	uint8_t key_data[ 16 ];

	/* Value to indicate the key data is set
	 */
	uint8_t key_data_is_set;

	/* The level 1 index bit shift
 	 */
	uint32_t level1_index_bit_shift;

	/* The level 1 table
	 */
	libqcow_cluster_table_t *level1_table;

	/* The level 2 index bit mask
 	 */
	uint64_t level2_index_bit_mask;

	/* The level 2 table vector
	 */
	libfdata_vector_t *level2_table_vector;

	/* The level2 table cache
	 */
	libfcache_cache_t *level2_table_cache;

	/* The cluster block size
 	 */
	size64_t cluster_block_size;

	/* The cluster block bit mask
 	 */
	uint64_t cluster_block_bit_mask;

	/* The offset bit mask
 	 */
	uint64_t offset_bit_mask;

	/* The compression flag bit mask
 	 */
	uint64_t compression_flag_bit_mask;

	/* The compression (offset) bit mask
 	 */
	uint64_t compression_bit_mask;

	/* The compression (size) bit shift
 	 */
	uint64_t compression_bit_shift;

	/* The cluster block vector
	 */
	libfdata_vector_t *cluster_block_vector;

	/* The cluster block cache
	 */
	libfcache_cache_t *cluster_block_cache;

	/* The compressed cluster block cache
	 */
	libfcache_cache_t *compressed_cluster_block_cache;

	/* Value to indicate if the file is locked
	 */
	uint8_t is_locked;

	/* The parent (backing) file
	 */
	libqcow_file_t *parent_file;

#if defined( HAVE_LIBQCOW_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

LIBQCOW_EXTERN \
int libqcow_file_initialize(
     libqcow_file_t **file,
     libcerror_error_t **error );

LIBQCOW_EXTERN \
int libqcow_file_free(
     libqcow_file_t **file,
     libcerror_error_t **error );

LIBQCOW_EXTERN \
int libqcow_file_signal_abort(
     libqcow_file_t *file,
     libcerror_error_t **error );

LIBQCOW_EXTERN \
int libqcow_file_open(
     libqcow_file_t *file,
     const char *filename,
     int access_flags,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

LIBQCOW_EXTERN \
int libqcow_file_open_wide(
     libqcow_file_t *file,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

LIBQCOW_EXTERN \
int libqcow_file_open_file_io_handle(
     libqcow_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error );

LIBQCOW_EXTERN \
int libqcow_file_close(
     libqcow_file_t *file,
     libcerror_error_t **error );

int libqcow_internal_file_open_read(
     libqcow_internal_file_t *internal_file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libqcow_internal_file_open_read_backing_filename(
     libqcow_internal_file_t *internal_file,
     libbfio_handle_t *file_io_handle,
     off64_t backing_filename_offset,
     uint32_t backing_filename_size,
     libcerror_error_t **error );

LIBQCOW_EXTERN \
int libqcow_file_is_locked(
     libqcow_file_t *file,
     libcerror_error_t **error );

ssize_t libqcow_internal_file_read_buffer_from_file_io_handle(
         libqcow_internal_file_t *internal_file,
         libbfio_handle_t *file_io_handle,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBQCOW_EXTERN \
ssize_t libqcow_file_read_buffer(
         libqcow_file_t *file,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBQCOW_EXTERN \
ssize_t libqcow_file_read_buffer_at_offset(
         libqcow_file_t *file,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error );

off64_t libqcow_internal_file_seek_offset(
         libqcow_internal_file_t *internal_file,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

LIBQCOW_EXTERN \
off64_t libqcow_file_seek_offset(
         libqcow_file_t *file,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

LIBQCOW_EXTERN \
int libqcow_file_get_offset(
     libqcow_file_t *file,
     off64_t *offset,
     libcerror_error_t **error );

LIBQCOW_EXTERN \
int libqcow_file_set_parent_file(
     libqcow_file_t *file,
     libqcow_file_t *parent_file,
     libcerror_error_t **error );

LIBQCOW_EXTERN \
int libqcow_file_set_keys(
     libqcow_file_t *file,
     const uint8_t *key,
     size_t key_size,
     libcerror_error_t **error );

LIBQCOW_EXTERN \
int libqcow_file_set_utf8_password(
     libqcow_file_t *file,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error );

LIBQCOW_EXTERN \
int libqcow_file_set_utf16_password(
     libqcow_file_t *file,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error );

LIBQCOW_EXTERN \
int libqcow_file_get_media_size(
     libqcow_file_t *file,
     size64_t *media_size,
     libcerror_error_t **error );

LIBQCOW_EXTERN \
int libqcow_file_get_format_version(
     libqcow_file_t *file,
     uint32_t *format_version,
     libcerror_error_t **error );

LIBQCOW_EXTERN \
int libqcow_file_get_encryption_method(
     libqcow_file_t *file,
     uint32_t *encryption_method,
     libcerror_error_t **error );

LIBQCOW_EXTERN \
int libqcow_file_get_utf8_backing_filename_size(
     libqcow_file_t *file,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBQCOW_EXTERN \
int libqcow_file_get_utf8_backing_filename(
     libqcow_file_t *file,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBQCOW_EXTERN \
int libqcow_file_get_utf16_backing_filename_size(
     libqcow_file_t *file,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBQCOW_EXTERN \
int libqcow_file_get_utf16_backing_filename(
     libqcow_file_t *file,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBQCOW_EXTERN \
int libqcow_file_get_number_of_snapshots(
     libqcow_file_t *file,
     int *number_of_snapshots,
     libcerror_error_t **error );

/* TODO add libqcow_file_get_snapshot_by_index */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBQCOW_INTERNAL_FILE_H ) */

