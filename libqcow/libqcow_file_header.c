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

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libqcow_debug.h"
#include "libqcow_file_header.h"
#include "libqcow_io_handle.h"
#include "libqcow_libcerror.h"
#include "libqcow_libcnotify.h"

#include "qcow_file_header.h"

/* Creates file header
 * Make sure the value file_header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libqcow_file_header_initialize(
     libqcow_file_header_t **file_header,
     libcerror_error_t **error )
{
	static char *function = "libqcow_file_header_initialize";

	if( file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file header.",
		 function );

		return( -1 );
	}
	if( *file_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file header value already set.",
		 function );

		return( -1 );
	}
	*file_header = memory_allocate_structure(
	                libqcow_file_header_t );

	if( *file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *file_header,
	     0,
	     sizeof( libqcow_file_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *file_header != NULL )
	{
		memory_free(
		 *file_header );

		*file_header = NULL;
	}
	return( -1 );
}

/* Frees file header
 * Returns 1 if successful or -1 on error
 */
int libqcow_file_header_free(
     libqcow_file_header_t **file_header,
     libcerror_error_t **error )
{
	static char *function = "libqcow_file_header_free";

	if( file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file header.",
		 function );

		return( -1 );
	}
	if( *file_header != NULL )
	{
		memory_free(
		 *file_header );

		*file_header = NULL;
	}
	return( 1 );
}

/* Reads the file header data
 * Returns 1 if successful or -1 on error
 */
int libqcow_file_header_read_data(
     libqcow_file_header_t *file_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function               = "libqcow_file_header_read_data";
	size_t file_header_data_size        = 0;
	uint64_t compatible_feature_flags   = 0;
	uint64_t incompatible_feature_flags = 0;
	uint64_t safe_level1_table_offset   = 0;
	uint64_t safe_snapshots_offset      = 0;
	uint64_t supported_feature_flags    = 0;
	uint32_t file_header_size           = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit                = 0;
	uint32_t value_32bit                = 0;
	uint16_t value_16bit                = 0;
#endif

	if( file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file header.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( ( data_size < 8 )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( memory_compare(
	     ( (qcow_file_header_v1_t *) data )->signature,
	     qcow_file_signature,
	     4 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported file signature.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_big_endian(
	 ( (qcow_file_header_v1_t *) data )->format_version,
	 file_header->format_version );

	if( file_header->format_version == 1 )
	{
		file_header_data_size = sizeof( qcow_file_header_v1_t );
	}
	else if( file_header->format_version == 2 )
	{
		file_header_data_size = sizeof( qcow_file_header_v2_t );
	}
	else if( file_header->format_version == 3 )
	{
		file_header_data_size = sizeof( qcow_file_header_v3_t );
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format version: %" PRIu32 ".",
		 function,
		 file_header->format_version );

		return( -1 );
	}
	if( data_size < file_header_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: file header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 file_header_data_size,
		 0 );
	}
#endif
	byte_stream_copy_to_uint64_big_endian(
	 ( (qcow_file_header_v1_t *) data )->backing_filename_offset,
	 file_header->backing_filename_offset );

	byte_stream_copy_to_uint32_big_endian(
	 ( (qcow_file_header_v1_t *) data )->backing_filename_size,
	 file_header->backing_filename_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint32_big_endian(
		 ( (qcow_file_header_v1_t *) data )->signature,
		 value_32bit );
		libcnotify_printf(
		 "%s: signature\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: format version\t\t\t\t: %" PRIu32 "\n",
		 function,
		 file_header->format_version );

		libcnotify_printf(
		 "%s: backing filename offset\t\t\t: %" PRIu32 "\n",
		 function,
		 file_header->backing_filename_offset );

		libcnotify_printf(
		 "%s: backing filename size\t\t\t: %" PRIu32 "\n",
		 function,
		 file_header->backing_filename_size );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( file_header->format_version == 3 )
	{
		if( data_size < sizeof( qcow_file_header_v3_t ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid data size value out of bounds.",
			 function );

			return( -1 );
		}
	}
	if( file_header->format_version == 1 )
	{
		byte_stream_copy_to_uint64_big_endian(
		 ( (qcow_file_header_v1_t *) data )->media_size,
		 file_header->media_size );

		file_header->number_of_cluster_block_bits = (uint32_t) ( (qcow_file_header_v1_t *) data )->number_of_cluster_block_bits;

		file_header->number_of_level2_table_bits = (uint32_t) ( (qcow_file_header_v1_t *) data )->number_of_level2_table_bits;

		byte_stream_copy_to_uint32_big_endian(
		 ( (qcow_file_header_v1_t *) data )->encryption_method,
		 file_header->encryption_method );

		byte_stream_copy_to_uint64_big_endian(
		 ( (qcow_file_header_v2_t *) data )->level1_table_offset,
		 safe_level1_table_offset );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint32_big_endian(
			 ( (qcow_file_header_v1_t *) data )->modification_time,
			 value_32bit );
			libcnotify_printf(
			 "%s: modification time\t\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "%s: media size\t\t\t\t: %" PRIu64 "\n",
			 function,
			 file_header->media_size );

			libcnotify_printf(
			 "%s: number of cluster block bits\t\t: %" PRIu32 "\n",
			 function,
			 file_header->number_of_cluster_block_bits );

			libcnotify_printf(
			 "%s: number of level 2 table bits\t\t: %" PRIu32 "\n",
			 function,
			 file_header->number_of_level2_table_bits );

			byte_stream_copy_to_uint16_big_endian(
			 ( (qcow_file_header_v1_t *) data )->unknown1,
			 value_16bit );
			libcnotify_printf(
			 "%s: unknown1\t\t\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 value_16bit );

			libcnotify_printf(
			 "%s: encryption method\t\t\t: %" PRIu32 "\n",
			 function,
			 file_header->encryption_method );

			libcnotify_printf(
			 "%s: level 1 table offset\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 safe_level1_table_offset );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	}
	else if( ( file_header->format_version == 2 )
	      || ( file_header->format_version == 3 ) )
	{
		byte_stream_copy_to_uint32_big_endian(
		 ( (qcow_file_header_v2_t *) data )->number_of_cluster_block_bits,
		 file_header->number_of_cluster_block_bits );

		byte_stream_copy_to_uint32_big_endian(
		 ( (qcow_file_header_v2_t *) data )->number_of_level1_table_references,
		 file_header->number_of_level1_table_references );

		byte_stream_copy_to_uint64_big_endian(
		 ( (qcow_file_header_v2_t *) data )->level1_table_offset,
		 safe_level1_table_offset );

		byte_stream_copy_to_uint64_big_endian(
		 ( (qcow_file_header_v2_t *) data )->media_size,
		 file_header->media_size );

		byte_stream_copy_to_uint32_big_endian(
		 ( (qcow_file_header_v2_t *) data )->encryption_method,
		 file_header->encryption_method );

		byte_stream_copy_to_uint32_big_endian(
		 ( (qcow_file_header_v2_t *) data )->number_of_snapshots,
		 file_header->number_of_snapshots );

		byte_stream_copy_to_uint64_big_endian(
		 ( (qcow_file_header_v2_t *) data )->snapshots_offset,
		 safe_snapshots_offset );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: number of cluster block bits\t\t: %" PRIu32 "\n",
			 function,
			 file_header->number_of_cluster_block_bits );

			libcnotify_printf(
			 "%s: media size\t\t\t\t: %" PRIu64 "\n",
			 function,
			 file_header->media_size );

			libcnotify_printf(
			 "%s: encryption method\t\t\t: %" PRIu32 "\n",
			 function,
			 file_header->encryption_method );

			libcnotify_printf(
			 "%s: number of level 1 table references\t: %" PRIu32 "\n",
			 function,
			 file_header->number_of_level1_table_references );

			libcnotify_printf(
			 "%s: level 1 table offset\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 safe_level1_table_offset );

			byte_stream_copy_to_uint64_big_endian(
			 ( (qcow_file_header_v2_t *) data )->reference_count_table_offset,
			 value_64bit );
			libcnotify_printf(
			 "%s: reference count table offset\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint32_big_endian(
			 ( (qcow_file_header_v2_t *) data )->reference_count_table_clusters,
			 value_32bit );
			libcnotify_printf(
			 "%s: reference count table clusters\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "%s: number of snapshots\t\t\t: %" PRIu32 "\n",
			 function,
			 file_header->number_of_snapshots );

			libcnotify_printf(
			 "%s: snapshots offset\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 safe_snapshots_offset );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	}
	if( safe_level1_table_offset > (uint64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid level1 table offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( file_header->format_version == 3 )
	{
		byte_stream_copy_to_uint64_big_endian(
		 ( (qcow_file_header_v3_t *) data )->incompatible_feature_flags,
		 incompatible_feature_flags );

		byte_stream_copy_to_uint64_big_endian(
		 ( (qcow_file_header_v3_t *) data )->compatible_feature_flags,
		 compatible_feature_flags );

		byte_stream_copy_to_uint32_big_endian(
		 ( (qcow_file_header_v3_t *) data )->header_size,
		 file_header_size );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: incompatible feature flags\t\t: 0x%08" PRIx64 "\n",
			 function,
			 incompatible_feature_flags );

			libcnotify_printf(
			 "%s: compatible feature flags\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 compatible_feature_flags );

			byte_stream_copy_to_uint64_big_endian(
			 ( (qcow_file_header_v3_t *) data )->auto_clear_feature_flags,
			 value_64bit );
			libcnotify_printf(
			 "%s: auto-clear feature flags\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint32_big_endian(
			 ( (qcow_file_header_v3_t *) data )->reference_count_order,
			 value_32bit );
			libcnotify_printf(
			 "%s: reference count order\t\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "%s: file header size\t\t\t\t: %" PRIu32 "\n",
			 function,
			 file_header_size );

			if( file_header_size >= 104 )
			{
				libcnotify_printf(
				 "%s: compression type\t\t\t\t: %" PRIu8 "\n",
				 function,
				 ( (qcow_file_header_v3_t *) data )->compression_type );

				libcnotify_printf(
				 "%s: unknown1:\n",
				 function );
				libcnotify_print_data(
				 ( (qcow_file_header_v3_t *) data )->unknown1,
				 7,
				 0 );
			}
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		supported_feature_flags = 0x00000001UL;

		if( ( incompatible_feature_flags & ~( supported_feature_flags ) ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported incompatible features flags: 0x%08" PRIx64 ".",
			 function,
			 incompatible_feature_flags );

			return( -1 );
		}
		supported_feature_flags = 0x00000001UL;

		if( ( compatible_feature_flags & ~( supported_feature_flags ) ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported compatible features flags: 0x%08" PRIx64 ".",
			 function,
			 compatible_feature_flags );

			return( -1 );
		}
		if( ( file_header_size != 104 )
		 && ( file_header_size != 112 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported file header size: %d.",
			 function,
			 file_header_size );

			return( -1 );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( ( file_header->format_version != 3 )
		 || ( file_header_size <= 104 ) )
		{
			libcnotify_printf(
			 "\n" );
		}
	}
#endif
	file_header->level1_table_offset = (off64_t) safe_level1_table_offset;
	file_header->snapshots_offset    = (off64_t) safe_snapshots_offset;

	return( 1 );
}

/* Reads the file header
 * Returns 1 if successful or -1 on error
 */
int libqcow_file_header_read_file_io_handle(
     libqcow_file_header_t *file_header,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	uint8_t file_header_data[ 512 ];

	static char *function = "libqcow_file_header_read_file_io_handle";
	ssize_t read_count    = 0;

	if( file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file header.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading file header at offset: 0 (0x00000000)\n",
		 function );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              file_header_data,
	              512,
	              0,
	              error );

	if( read_count != (ssize_t) 512 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header data at offset: 0 (0x00000000).",
		 function );

		return( -1 );
	}
	if( libqcow_file_header_read_data(
	     file_header,
	     file_header_data,
	     512,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header.",
		 function );

		return( -1 );
	}
	return( 1 );
}

