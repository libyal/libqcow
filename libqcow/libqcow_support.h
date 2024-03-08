/*
 * Support functions
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

#if !defined( _LIBQCOW_SUPPORT_H )
#define _LIBQCOW_SUPPORT_H

#include <common.h>
#include <types.h>

#include "libqcow_extern.h"
#include "libqcow_libbfio.h"
#include "libqcow_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if !defined( HAVE_LOCAL_LIBQCOW )

LIBQCOW_EXTERN \
const char *libqcow_get_version(
             void );

LIBQCOW_EXTERN \
int libqcow_get_access_flags_read(
     void );

LIBQCOW_EXTERN \
int libqcow_get_access_flags_read_write(
     void );

LIBQCOW_EXTERN \
int libqcow_get_access_flags_write(
     void );

LIBQCOW_EXTERN \
int libqcow_get_codepage(
     int *codepage,
     libcerror_error_t **error );

LIBQCOW_EXTERN \
int libqcow_set_codepage(
     int codepage,
     libcerror_error_t **error );

#endif /* !defined( HAVE_LOCAL_LIBQCOW ) */

LIBQCOW_EXTERN \
int libqcow_check_file_signature(
     const char *filename,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

LIBQCOW_EXTERN \
int libqcow_check_file_signature_wide(
     const wchar_t *filename,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

LIBQCOW_EXTERN \
int libqcow_check_file_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBQCOW_SUPPORT_H ) */

