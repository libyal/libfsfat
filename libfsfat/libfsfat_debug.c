/*
 * Debug functions
 *
 * Copyright (C) 2021-2022, Joachim Metz <joachim.metz@gmail.com>
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
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "libfsfat_date_time.h"
#include "libfsfat_debug.h"
#include "libfsfat_definitions.h"
#include "libfsfat_libbfio.h"
#include "libfsfat_libcerror.h"
#include "libfsfat_libcnotify.h"
#include "libfsfat_libfdatetime.h"

#if defined( HAVE_DEBUG_OUTPUT )

/* Prints the exFAT cluster type
 */
const char *libfsfat_debug_print_exfat_cluster_type(
             uint32_t cluster_type )
{
	if( cluster_type == 0UL )
	{
		return( "Free" );
	}
	if( cluster_type == 1UL )
	{
		return( "Invalid" );
	}
	if( ( cluster_type >= 0xfffffff0UL )
	 && ( cluster_type <= 0xfffffff6UL ) )
	{
		return( "Reserved" );
	}
	if( cluster_type == 0xfffffff7UL )
	{
		return( "Bad" );
	}
	if( cluster_type >= 0xfffffff8UL )
	{
		return( "End of chain" );
	}
	return( "Used" );
}

/* Prints the exFAT (known) directory entry types
 */
const char *libfsfat_debug_print_exfat_directory_entry_type(
             uint8_t entry_type )
{
	switch( entry_type )
	{
		case 0:
			return( "terminator" );

		case 0x81:
			return( "allocation bitmap" );

		case 0x82:
			return( "Up-Case table" );

		case 0x83:
			return( "volume label" );

		case 0x85:
			return( "file entry" );

		case 0xc0:
			return( "data stream" );

		case 0xc1:
			return( "file entry name" );

		default:
			break;
	}
	return( "UNKNOWN" );
}

/* Prints the FAT-12 cluster type
 */
const char *libfsfat_debug_print_fat12_cluster_type(
             uint16_t cluster_type )
{
	if( cluster_type == 0 )
	{
		return( "Free" );
	}
	if( cluster_type == 1 )
	{
		return( "Invalid" );
	}
	if( ( cluster_type >= 0x0ff0 )
	 && ( cluster_type <= 0x0ff6 ) )
	{
		return( "Reserved" );
	}
	if( cluster_type == 0x0ff7 )
	{
		return( "Bad" );
	}
	if( ( cluster_type >= 0x0ff8 )
	 && ( cluster_type <= 0x0fff ) )
	{
		return( "End of chain" );
	}
	return( "Used" );
}

/* Prints the FAT-16 cluster type
 */
const char *libfsfat_debug_print_fat16_cluster_type(
             uint16_t cluster_type )
{
	if( cluster_type == 0 )
	{
		return( "Free" );
	}
	if( cluster_type == 1 )
	{
		return( "Invalid" );
	}
	if( ( cluster_type >= 0xfff0 )
	 && ( cluster_type <= 0xfff6 ) )
	{
		return( "Reserved" );
	}
	if( cluster_type == 0xfff7 )
	{
		return( "Bad" );
	}
	if( cluster_type >= 0xfff8 )
	{
		return( "End of chain" );
	}
	return( "Used" );
}

/* Prints the FAT-32 cluster type
 */
const char *libfsfat_debug_print_fat32_cluster_type(
             uint32_t cluster_type )
{
	if( cluster_type == 0UL )
	{
		return( "Free" );
	}
	if( cluster_type == 1UL )
	{
		return( "Invalid" );
	}
	if( ( cluster_type >= 0x0ffffff0UL )
	 && ( cluster_type <= 0x0ffffff6UL ) )
	{
		return( "Reserved" );
	}
	if( cluster_type == 0x0ffffff7UL )
	{
		return( "Bad" );
	}
	if( ( cluster_type >= 0x0ffffff8UL )
	 && ( cluster_type <= 0x0fffffffUL ) )
	{
		return( "End of chain" );
	}
	if( cluster_type >= 0x10000000UL )
	{
		return( "Unknown" );
	}
	return( "Used" );
}

/* Prints the file attribute flags
 */
void libfsfat_debug_print_file_attribute_flags(
      uint16_t file_attribute_flags )
{
	if( ( file_attribute_flags & LIBFSFAT_FILE_ATTRIBUTE_FLAG_READ_ONLY ) != 0 )
	{
		libcnotify_printf(
		 "\tIs read-only (FILE_ATTRIBUTE_READ_ONLY)\n" );
	}
	if( ( file_attribute_flags & LIBFSFAT_FILE_ATTRIBUTE_FLAG_HIDDEN ) != 0 )
	{
		libcnotify_printf(
		 "\tIs hidden (FILE_ATTRIBUTE_HIDDEN)\n" );
	}
	if( ( file_attribute_flags & LIBFSFAT_FILE_ATTRIBUTE_FLAG_SYSTEM ) != 0 )
	{
		libcnotify_printf(
		 "\tIs system (FILE_ATTRIBUTE_SYSTEM)\n" );
	}
	if( ( file_attribute_flags & LIBFSFAT_FILE_ATTRIBUTE_FLAG_VOLUME_LABEL ) != 0 )
	{
		libcnotify_printf(
		 "\tIs volume label (FILE_ATTRIBUTE_VOLUME_LABEL)\n" );
	}
	if( ( file_attribute_flags & LIBFSFAT_FILE_ATTRIBUTE_FLAG_DIRECTORY ) != 0 )
	{
		libcnotify_printf(
		 "\tIs directory (FILE_ATTRIBUTE_DIRECTORY)\n" );
	}
	if( ( file_attribute_flags & LIBFSFAT_FILE_ATTRIBUTE_FLAG_ARCHIVE ) != 0 )
	{
		libcnotify_printf(
		 "\tShould be archived (FILE_ATTRIBUTE_ARCHIVE)\n" );
	}
	if( ( file_attribute_flags & LIBFSFAT_FILE_ATTRIBUTE_FLAG_DEVICE ) != 0 )
	{
		libcnotify_printf(
		 "\tIs device (FILE_ATTRIBUTE_DEVICE)\n" );
	}
}

/* Prints a FAT date time value
 * Returns 1 if successful or -1 on error
 */
int libfsfat_debug_print_fat_date_time_value(
     const char *function_name,
     const char *value_name,
     uint16_t fat_date,
     uint16_t fat_time,
     uint8_t fat_time_fraction,
     uint8_t fat_time_utc_offset,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	char date_time_string[ 32 ];

	libfdatetime_posix_time_t *posix_time = NULL;
	static char *function                 = "libfsfat_debug_print_fat_date_time_value";
	uint64_t fat_timestamp                = 0;

	if( libfsfat_date_time_get_timestamp(
	     fat_date,
	     fat_time,
	     fat_time_fraction,
	     fat_time_utc_offset,
	     &fat_timestamp,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve timestamp.",
		 function );

		goto on_error;
	}
	if( libfdatetime_posix_time_initialize(
	     &posix_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create POSIX time.",
		 function );

		goto on_error;
	}
	if( libfdatetime_posix_time_copy_from_64bit(
	     posix_time,
	     ( fat_timestamp + 31553280000UL ) * 10000,
	     LIBFDATETIME_POSIX_TIME_VALUE_TYPE_MICRO_SECONDS_64BIT_UNSIGNED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy POSIX time from 64-bit.",
		 function );

		goto on_error;
	}
	if( libfdatetime_posix_time_copy_to_utf8_string(
	     posix_time,
	     (uint8_t *) date_time_string,
	     32,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy POSIX time to string.",
		 function );

		goto on_error;
	}
/* TODO print UTC or local time indicator */

	libcnotify_printf(
	 "%s: %s: %s.%02" PRIu64 "\n",
	 function_name,
	 value_name,
	 date_time_string,
	 fat_timestamp % 100 );

	if( libfdatetime_posix_time_free(
	     &posix_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free POSIX time.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( posix_time != NULL )
	{
		libfdatetime_posix_time_free(
		 &posix_time,
		 NULL );
	}
	return( -1 );
}

/* Prints the read offsets
 * Returns 1 if successful or -1 on error
 */
int libfsfat_debug_print_read_offsets(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_debug_print_read_offsets";
	off64_t offset        = 0;
	size64_t size         = 0;
	int number_of_offsets = 0;
	int offset_iterator   = 0;

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
	if( libbfio_handle_get_number_of_offsets_read(
	     file_io_handle,
	     &number_of_offsets,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of offsets read.",
		 function );

		return( -1 );
	}
	libcnotify_printf(
	 "Offsets read:\n" );

	for( offset_iterator = 0;
	     offset_iterator < number_of_offsets;
	     offset_iterator++ )
	{
		if( libbfio_handle_get_offset_read(
		     file_io_handle,
		     offset_iterator,
		     &offset,
		     &size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset: %d.",
			 function,
			 ( offset_iterator + 1 ) );

			return( -1 );
		}
		libcnotify_printf(
		 "%08" PRIi64 " ( 0x%08" PRIx64 " ) - %08" PRIi64 " ( 0x%08" PRIx64 " ) size: %" PRIu64 "\n",
		 offset,
		 offset,
		 offset + (off64_t) size,
		 offset + (off64_t) size,
		 size );
	}
	libcnotify_printf(
	 "\n" );

	return( 1 );
}

#endif /* defined( HAVE_DEBUG_OUTPUT ) */

