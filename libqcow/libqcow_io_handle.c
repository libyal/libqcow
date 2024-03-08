/*
 * Input/Output (IO) handle functions
 *
 * Copyright (C) 2010-2024, Joachim Metz <joachim.metz@gmail.com>
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
#include <types.h>

#include "libqcow_cluster_block.h"
#include "libqcow_cluster_table.h"
#include "libqcow_encryption.h"
#include "libqcow_file_header.h"
#include "libqcow_io_handle.h"
#include "libqcow_libbfio.h"
#include "libqcow_libcerror.h"
#include "libqcow_libcnotify.h"
#include "libqcow_libfdata.h"
#include "libqcow_unused.h"

#include "qcow_file_header.h"

const uint8_t qcow_file_signature[ 4 ] = { 0x51, 0x46, 0x49, 0xfb };

/* Creates an IO handle
 * Make sure the value io_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libqcow_io_handle_initialize(
     libqcow_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libqcow_io_handle_initialize";

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( *io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid IO handle value already set.",
		 function );

		return( -1 );
	}
	*io_handle = memory_allocate_structure(
	              libqcow_io_handle_t );

	if( *io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create IO handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *io_handle,
	     0,
	     sizeof( libqcow_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *io_handle != NULL )
	{
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( -1 );
}

/* Frees an IO handle
 * Returns 1 if successful or -1 on error
 */
int libqcow_io_handle_free(
     libqcow_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libqcow_io_handle_free";
	int result            = 1;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( *io_handle != NULL )
	{
		if( libqcow_io_handle_clear(
		     *io_handle,
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
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( result );
}

/* Clears the IO handle
 * Returns 1 if successful or -1 on error
 */
int libqcow_io_handle_clear(
     libqcow_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	static char *function = "libqcow_io_handle_clear";

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     io_handle,
	     0,
	     sizeof( libqcow_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads a level 2 table
 * Callback function for the level 2 table vector
 * Returns 1 if successful or -1 on error
 */
int libqcow_io_handle_read_level2_table(
     intptr_t *data_handle LIBQCOW_ATTRIBUTE_UNUSED,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfdata_cache_t *cache,
     int element_index,
     int element_data_file_index LIBQCOW_ATTRIBUTE_UNUSED,
     off64_t element_data_offset,
     size64_t element_data_size,
     uint32_t element_data_flags LIBQCOW_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBQCOW_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libqcow_cluster_table_t *level2_table = NULL;
	static char *function                 = "libqcow_io_handle_read_level2_table";

	LIBQCOW_UNREFERENCED_PARAMETER( data_handle );
	LIBQCOW_UNREFERENCED_PARAMETER( element_data_file_index );
	LIBQCOW_UNREFERENCED_PARAMETER( element_data_flags );
	LIBQCOW_UNREFERENCED_PARAMETER( read_flags );

	if( element_data_size > (size64_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid element data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( libqcow_cluster_table_initialize(
	     &level2_table,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create level 2 table.",
		 function );

		goto on_error;
	}
	if( libqcow_cluster_table_read(
	     level2_table,
	     file_io_handle,
	     element_data_offset,
	     (size_t) element_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read level 2 table.",
		 function );

		goto on_error;
	}
	if( libfdata_vector_set_element_value_by_index(
	     vector,
	     (intptr_t *) file_io_handle,
	     cache,
	     element_index,
	     (intptr_t *) level2_table,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libqcow_cluster_table_free,
	     LIBFDATA_VECTOR_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set level 2 table as element value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( level2_table != NULL )
	{
		libqcow_cluster_table_free(
		 &level2_table,
		 NULL );
	}
	return( -1 );
}

/* Reads a cluster block
 * Callback function for the cluster block vector
 * Returns 1 if successful or -1 on error
 */
int libqcow_io_handle_read_cluster_block(
     intptr_t *data_handle LIBQCOW_ATTRIBUTE_UNUSED,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfdata_cache_t *cache,
     int element_index,
     int element_data_file_index LIBQCOW_ATTRIBUTE_UNUSED,
     off64_t element_data_offset,
     size64_t element_data_size,
     uint32_t element_data_flags LIBQCOW_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBQCOW_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libqcow_cluster_block_t *cluster_block = NULL;
	static char *function                  = "libqcow_io_handle_read_cluster_block";

	LIBQCOW_UNREFERENCED_PARAMETER( data_handle );
	LIBQCOW_UNREFERENCED_PARAMETER( element_data_file_index );
	LIBQCOW_UNREFERENCED_PARAMETER( element_data_flags );
	LIBQCOW_UNREFERENCED_PARAMETER( read_flags );

	if( element_data_size > (size64_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid element data size value exceeds maximum.",
		 function );

		goto on_error;
	}
	if( libqcow_cluster_block_initialize(
	     &cluster_block,
	     (size_t) element_data_size,
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
	     cluster_block,
	     file_io_handle,
             element_data_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read cluster block.",
		 function );

		goto on_error;
	}
	if( libfdata_vector_set_element_value_by_index(
	     vector,
	     (intptr_t *) file_io_handle,
	     cache,
	     element_index,
	     (intptr_t *) cluster_block,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libqcow_cluster_block_free,
	     LIBFDATA_VECTOR_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set cluster block as element value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( cluster_block != NULL )
	{
		libqcow_cluster_block_free(
		 &cluster_block,
		 NULL );
	}
	return( -1 );
}

