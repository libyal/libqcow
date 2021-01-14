/*
 * File header functions
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

#if !defined( _LIBQCOW_FILE_HEADER_H )
#define _LIBQCOW_FILE_HEADER_H

#include <common.h>
#include <types.h>

#include "libqcow_libbfio.h"
#include "libqcow_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libqcow_file_header libqcow_file_header_t;

struct libqcow_file_header
{
	/* The format version
	 */
	uint32_t format_version;

	/* The backing filename offset
	 */
	uint64_t backing_filename_offset;

	/* The backing filename size
	 */
	uint32_t backing_filename_size;

	/* The (storage) media size
	 */
	size64_t media_size;

	/* The number of cluster block bits
 	 */
	uint32_t number_of_cluster_block_bits;

	/* The number of level 1 table references
 	 */
	uint32_t number_of_level1_table_references;

	/* The number of level 2 table bits
 	 */
	uint32_t number_of_level2_table_bits;

	/* The encryption method
 	 */
	uint32_t encryption_method;

	/* The level 1 table offset
 	 */
	off64_t level1_table_offset;

	/* The number of snapshots
 	 */
	uint32_t number_of_snapshots;

	/* The snapshots offset
 	 */
	off64_t snapshots_offset;
};

int libqcow_file_header_initialize(
     libqcow_file_header_t **file_header,
     libcerror_error_t **error );

int libqcow_file_header_free(
     libqcow_file_header_t **file_header,
     libcerror_error_t **error );

int libqcow_file_header_read_data(
     libqcow_file_header_t *file_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libqcow_file_header_read_file_io_handle(
     libqcow_file_header_t *file_header,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBQCOW_FILE_HEADER_H ) */

