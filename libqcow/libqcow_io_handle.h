/*
 * Input/Output (IO) handle functions
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

#if !defined( _LIBQCOW_IO_HANDLE_H )
#define _LIBQCOW_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include "libqcow_libbfio.h"
#include "libqcow_libcerror.h"
#include "libqcow_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern const uint8_t qcow_file_signature[ 4 ];

typedef struct libqcow_io_handle libqcow_io_handle_t;

struct libqcow_io_handle
{
	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int libqcow_io_handle_initialize(
     libqcow_io_handle_t **io_handle,
     libcerror_error_t **error );

int libqcow_io_handle_free(
     libqcow_io_handle_t **io_handle,
     libcerror_error_t **error );

int libqcow_io_handle_clear(
     libqcow_io_handle_t *io_handle,
     libcerror_error_t **error );

int libqcow_io_handle_read_level2_table(
     intptr_t *data_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfdata_cache_t *cache,
     int element_index,
     int element_data_file_index,
     off64_t element_data_offset,
     size64_t element_data_size,
     uint32_t element_data_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

int libqcow_io_handle_read_cluster_block(
     intptr_t *data_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfdata_cache_t *cache,
     int element_index,
     int element_data_file_index,
     off64_t element_data_offset,
     size64_t element_data_size,
     uint32_t element_data_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBQCOW_IO_HANDLE_H ) */

