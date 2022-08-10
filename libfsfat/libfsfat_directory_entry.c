/*
 * The directory entry functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfsfat_date_time.h"
#include "libfsfat_debug.h"
#include "libfsfat_definitions.h"
#include "libfsfat_directory_entry.h"
#include "libfsfat_libbfio.h"
#include "libfsfat_libcdata.h"
#include "libfsfat_libcerror.h"
#include "libfsfat_libcnotify.h"
#include "libfsfat_libfdatetime.h"
#include "libfsfat_libuna.h"
#include "libfsfat_name.h"

#include "fsfat_directory_entry.h"

const uint8_t libfsfat_directory_entry_empty[ 32 ] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

/* Creates a directory entry
 * Make sure the value directory_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsfat_directory_entry_initialize(
     libfsfat_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_directory_entry_initialize";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( *directory_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid directory entry value already set.",
		 function );

		return( -1 );
	}
	*directory_entry = memory_allocate_structure(
	                    libfsfat_directory_entry_t );

	if( *directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create directory entry.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *directory_entry,
	     0,
	     sizeof( libfsfat_directory_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear directory entry.",
		 function );

		memory_free(
		 *directory_entry );

		*directory_entry = NULL;

		return( -1 );
	}
	( *directory_entry )->entry_type = LIBFSFAT_DIRECTORY_ENTRY_TYPE_TERMINATOR;

	return( 1 );

on_error:
	if( *directory_entry != NULL )
	{
		memory_free(
		 *directory_entry );

		*directory_entry = NULL;
	}
	return( -1 );
}

/* Frees a directory entry
 * Returns 1 if successful or -1 on error
 */
int libfsfat_directory_entry_free(
     libfsfat_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_directory_entry_free";
	int result            = 1;

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( *directory_entry != NULL )
	{
		if( ( *directory_entry )->name_entries_array != NULL )
		{
			if( libcdata_array_free(
			     &( ( *directory_entry )->name_entries_array ),
			     NULL,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free name entries array.",
				 function );

				result = -1;
			}
		}
		if( ( *directory_entry )->name != NULL )
		{
			memory_free(
			 ( *directory_entry )->name );
		}
		memory_free(
		 *directory_entry );

		*directory_entry = NULL;
	}
	return( result );
}

/* Clones a directory entry
 * Returns 1 if successful or -1 on error
 */
int libfsfat_directory_entry_clone(
     libfsfat_directory_entry_t **destination_directory_entry,
     libfsfat_directory_entry_t *source_directory_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_directory_entry_clone";

	if( destination_directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( *destination_directory_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination directory entry value already set.",
		 function );

		return( -1 );
	}
	if( source_directory_entry == NULL )
	{
		*destination_directory_entry = source_directory_entry;

		return( 1 );
	}
	if( source_directory_entry->name == NULL )
	{
		if( libfsfat_directory_entry_get_name(
		     source_directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine name of source directory entry.",
			 function );

			return( -1 );
		}
	}
	*destination_directory_entry = memory_allocate_structure(
	                                libfsfat_directory_entry_t );

	if( *destination_directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination directory entry.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_directory_entry,
	     source_directory_entry,
	     sizeof( libfsfat_directory_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy directory entry.",
		 function );

		( *destination_directory_entry )->name = NULL;

		goto on_error;
	}
	( *destination_directory_entry )->name               = NULL;
	( *destination_directory_entry )->name_entries_array = NULL;

	if( source_directory_entry->name != NULL )
	{
		( *destination_directory_entry )->name = (uint8_t *) memory_allocate(
		                                                      sizeof( uint8_t ) * source_directory_entry->name_size );

		if( ( *destination_directory_entry )->name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create name.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_directory_entry )->name,
		     source_directory_entry->name,
		     source_directory_entry->name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy name.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( *destination_directory_entry != NULL )
	{
		if( ( *destination_directory_entry )->name != NULL )
		{
			memory_free(
			 ( *destination_directory_entry )->name );
		}
		memory_free(
		 *destination_directory_entry );

		*destination_directory_entry = NULL;
	}
	return( -1 );
}

/* Reads a directory entry
 * Returns 1 if successful, 0 if empty or -1 on error
 */
int libfsfat_directory_entry_read_data(
     libfsfat_directory_entry_t *directory_entry,
     const uint8_t *data,
     size_t data_size,
     uint8_t file_system_format,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_directory_entry_read_data";

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit  = 0;
	uint32_t value_32bit  = 0;
	uint16_t value_16bit  = 0;
#endif

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
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
	if( data_size != sizeof( fsfat_directory_entry_t ) )
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
		 "%s: directory entry data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsfat_directory_entry_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( file_system_format == LIBFSFAT_FILE_SYSTEM_FORMAT_EXFAT )
	{
		switch( data[ 0 ] )
		{
			case 0:
				directory_entry->entry_type = LIBFSFAT_DIRECTORY_ENTRY_TYPE_TERMINATOR;
				break;

			case 0x81:
				directory_entry->entry_type = LIBFSFAT_DIRECTORY_ENTRY_TYPE_EXFAT_ALLOCATION_BITMAP;
				break;

			case 0x82:
				directory_entry->entry_type = LIBFSFAT_DIRECTORY_ENTRY_TYPE_EXFAT_UPCASE_TABLE;
				break;

			case 0x83:
				directory_entry->entry_type = LIBFSFAT_DIRECTORY_ENTRY_TYPE_EXFAT_VOLUME_LABEL;
				break;

			case 0x85:
				directory_entry->entry_type = LIBFSFAT_DIRECTORY_ENTRY_TYPE_EXFAT_FILE_ENTRY;
				break;

			case 0xc0:
				directory_entry->entry_type = LIBFSFAT_DIRECTORY_ENTRY_TYPE_EXFAT_DATA_STREAM;
				break;

			case 0xc1:
				directory_entry->entry_type = LIBFSFAT_DIRECTORY_ENTRY_TYPE_EXFAT_FILE_ENTRY_NAME;
				break;

			default:
				directory_entry->entry_type = LIBFSFAT_DIRECTORY_ENTRY_TYPE_EXFAT_UNKNOWN;
				break;
		}
	}
	else if( memory_compare(
	          data,
	          libfsfat_directory_entry_empty,
	          sizeof( fsfat_directory_entry_t ) ) == 0 )
	{
		directory_entry->entry_type = LIBFSFAT_DIRECTORY_ENTRY_TYPE_TERMINATOR;
	}
	else
	{
		directory_entry->entry_type = LIBFSFAT_DIRECTORY_ENTRY_TYPE_SHORT_NAME;

		if( ( ( data[ 0 ] >= 0x01 )
		  &&  ( data[ 0 ] <= 0x13 ) )
		 || ( ( data[ 0 ] >= 0x41 )
		  &&  ( data[ 0 ] <= 0x54 ) ) )
		{
			if( ( data[ 11 ] == 0x0f )
			 && ( data[ 12 ] == 0x00 )
			 && ( data[ 26 ] == 0x00 )
			 && ( data[ 27 ] == 0x00 ) )
			{
				directory_entry->entry_type = LIBFSFAT_DIRECTORY_ENTRY_TYPE_VFAT_LONG_NAME;
			}
		}
		else if( data[ 0 ] == 0xe5 )
		{
			directory_entry->entry_type = LIBFSFAT_DIRECTORY_ENTRY_TYPE_UNALLOCATED;
		}
	}
	if( directory_entry->entry_type == LIBFSFAT_DIRECTORY_ENTRY_TYPE_TERMINATOR )
	{
		return( 0 );
	}
	else if( directory_entry->entry_type == LIBFSFAT_DIRECTORY_ENTRY_TYPE_SHORT_NAME )
	{
		if( memory_copy(
		     directory_entry->name_data,
		     ( (fsfat_directory_entry_t *) data )->name,
		     8 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy name.",
			 function );

			return( -1 );
		}
		if( memory_copy(
		     &( directory_entry->name_data[ 8 ] ),
		     ( (fsfat_directory_entry_t *) data )->extension,
		     3 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy extension.",
			 function );

			return( -1 );
		}
		directory_entry->file_attribute_flags = ( (fsfat_directory_entry_t *) data )->file_attribute_flags;

		directory_entry->creation_time_fraction = ( (fsfat_directory_entry_t *) data )->creation_time_fraction;

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsfat_directory_entry_t *) data )->creation_date,
		 directory_entry->creation_date );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsfat_directory_entry_t *) data )->creation_time,
		 directory_entry->creation_time );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsfat_directory_entry_t *) data )->access_date,
		 directory_entry->access_date );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsfat_directory_entry_t *) data )->modification_date,
		 directory_entry->modification_date );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsfat_directory_entry_t *) data )->modification_time,
		 directory_entry->modification_time );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsfat_directory_entry_t *) data )->data_start_cluster,
		 directory_entry->data_start_cluster );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsfat_directory_entry_t *) data )->data_size,
		 directory_entry->data_size );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: name\t\t\t\t: %c%c%c%c%c%c%c%c\n",
			 function,
			 ( (fsfat_directory_entry_t *) data )->name[ 0 ],
			 ( (fsfat_directory_entry_t *) data )->name[ 1 ],
			 ( (fsfat_directory_entry_t *) data )->name[ 2 ],
			 ( (fsfat_directory_entry_t *) data )->name[ 3 ],
			 ( (fsfat_directory_entry_t *) data )->name[ 4 ],
			 ( (fsfat_directory_entry_t *) data )->name[ 5 ],
			 ( (fsfat_directory_entry_t *) data )->name[ 6 ],
			 ( (fsfat_directory_entry_t *) data )->name[ 7 ] );

			libcnotify_printf(
			 "%s: extension\t\t\t\t: %c%c%c\n",
			 function,
			 ( (fsfat_directory_entry_t *) data )->extension[ 0 ],
			 ( (fsfat_directory_entry_t *) data )->extension[ 1 ],
			 ( (fsfat_directory_entry_t *) data )->extension[ 2 ] );

			libcnotify_printf(
			 "%s: file attribute flags\t\t: 0x%02" PRIx16 "\n",
			 function,
			 directory_entry->file_attribute_flags );
			libfsfat_debug_print_file_attribute_flags(
			 directory_entry->file_attribute_flags );
			libcnotify_printf(
			 "\n" );

			libcnotify_printf(
			 "%s: unknown1\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (fsfat_directory_entry_t *) data )->unknown1 );

			if( libfsfat_debug_print_fat_date_time_value(
			     function,
			     "creation time\t\t\t",
			     directory_entry->creation_date,
			     directory_entry->creation_time,
			     directory_entry->creation_time_fraction,
			     0,
			     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print FAT date time value.",
				 function );

				return( -1 );
			}
			if( libfsfat_debug_print_fat_date_time_value(
			     function,
			     "access time\t\t\t\t",
			     directory_entry->access_date,
			     0,
			     0,
			     0,
			     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print FAT date time value.",
				 function );

				return( -1 );
			}
			byte_stream_copy_to_uint16_little_endian(
			 ( (fsfat_directory_entry_t *) data )->unknown3,
			 value_16bit );
			libcnotify_printf(
			 "%s: unknown3\t\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 value_16bit );

			if( libfsfat_debug_print_fat_date_time_value(
			     function,
			     "modification time\t\t\t",
			     directory_entry->modification_date,
			     directory_entry->modification_time,
			     0,
			     0,
			     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print FAT date time value.",
				 function );

				return( -1 );
			}
			libcnotify_printf(
			 "%s: data start cluster\t\t\t: %" PRIu32 "\n",
			 function,
			 directory_entry->data_start_cluster );

			libcnotify_printf(
			 "%s: data size\t\t\t\t: %" PRIu64 "\n",
			 function,
			 directory_entry->data_size );

			libcnotify_printf(
			 "\n" );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	}
	else if( directory_entry->entry_type == LIBFSFAT_DIRECTORY_ENTRY_TYPE_VFAT_LONG_NAME )
	{
		directory_entry->vfat_sequence_number = ( (fsfat_directory_entry_vfat_t *) data )->sequence_number;

		if( memory_copy(
		     directory_entry->name_data,
		     ( (fsfat_directory_entry_vfat_t *) data )->first_name_segment,
		     10 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy first name segment.",
			 function );

			return( -1 );
		}
		if( memory_copy(
		     &( directory_entry->name_data[ 10 ] ),
		     ( (fsfat_directory_entry_vfat_t *) data )->second_name_segment,
		     12 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy second name segment.",
			 function );

			return( -1 );
		}
		if( memory_copy(
		     &( directory_entry->name_data[ 10 + 12 ] ),
		     ( (fsfat_directory_entry_vfat_t *) data )->third_name_segment,
		     4 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy third name segment.",
			 function );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: VFAT sequence number\t\t: 0x%02" PRIx8 " (%" PRIu8 ")\n",
			 function,
			 ( (fsfat_directory_entry_vfat_t *) data )->sequence_number,
			 ( (fsfat_directory_entry_vfat_t *) data )->sequence_number & 0x1f );

			libcnotify_printf(
			 "%s: first name segment:\n",
			 function );
			libcnotify_print_data(
			 ( (fsfat_directory_entry_vfat_t *) data )->first_name_segment,
			 10,
			 0 );

			libcnotify_printf(
			 "%s: unknown1\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (fsfat_directory_entry_vfat_t *) data )->unknown1 );

			libcnotify_printf(
			 "%s: unknown2\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (fsfat_directory_entry_vfat_t *) data )->unknown2 );

			libcnotify_printf(
			 "%s: name checksum\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (fsfat_directory_entry_vfat_t *) data )->name_checksum );

			libcnotify_printf(
			 "%s: second name segment:\n",
			 function );
			libcnotify_print_data(
			 ( (fsfat_directory_entry_vfat_t *) data )->second_name_segment,
			 12,
			 0 );

			byte_stream_copy_to_uint16_little_endian(
			 ( (fsfat_directory_entry_vfat_t *) data )->unknown3,
			 value_16bit );
			libcnotify_printf(
			 "%s: unknown3\t\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 value_16bit );

			libcnotify_printf(
			 "%s: third name segment:\n",
			 function );
			libcnotify_print_data(
			 ( (fsfat_directory_entry_vfat_t *) data )->third_name_segment,
			 4,
			 0 );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	}
	else if( directory_entry->entry_type == LIBFSFAT_DIRECTORY_ENTRY_TYPE_EXFAT_ALLOCATION_BITMAP )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: entry type\t\t\t\t: 0x%02" PRIx8 " (%s)\n",
			 function,
			 ( (fsfat_directory_entry_exfat_allocation_bitmap_t *) data )->entry_type,
			 libfsfat_debug_print_exfat_directory_entry_type(
			  ( (fsfat_directory_entry_exfat_allocation_bitmap_t *) data )->entry_type ) );

			libcnotify_printf(
			 "%s: bitmap flags\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (fsfat_directory_entry_exfat_allocation_bitmap_t *) data )->bitmap_flags );

			libcnotify_printf(
			 "%s: unknown1:\n",
			 function );
			libcnotify_print_data(
			 ( (fsfat_directory_entry_exfat_allocation_bitmap_t *) data )->unknown1,
			 18,
			 0 );

			byte_stream_copy_to_uint32_little_endian(
			 ( (fsfat_directory_entry_exfat_allocation_bitmap_t *) data )->data_start_cluster,
			 value_32bit );
			libcnotify_printf(
			 "%s: data start cluster\t\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (fsfat_directory_entry_exfat_allocation_bitmap_t *) data )->data_size,
			 value_64bit );
			libcnotify_printf(
			 "%s: data size\t\t\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			libcnotify_printf(
			 "\n" );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	}
	else if( directory_entry->entry_type == LIBFSFAT_DIRECTORY_ENTRY_TYPE_EXFAT_DATA_STREAM )
	{
		directory_entry->name_size = ( (fsfat_directory_entry_exfat_volume_label_t *) data )->name_size;

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsfat_directory_entry_exfat_data_stream_t *) data )->valid_data_size,
		 directory_entry->valid_data_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsfat_directory_entry_exfat_data_stream_t *) data )->data_start_cluster,
		 directory_entry->data_start_cluster );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsfat_directory_entry_exfat_data_stream_t *) data )->data_size,
		 directory_entry->data_size );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: entry type\t\t\t\t: 0x%02" PRIx8 " (%s)\n",
			 function,
			 ( (fsfat_directory_entry_exfat_allocation_bitmap_t *) data )->entry_type,
			 libfsfat_debug_print_exfat_directory_entry_type(
			  ( (fsfat_directory_entry_exfat_allocation_bitmap_t *) data )->entry_type ) );

			libcnotify_printf(
			 "%s: unknown1\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (fsfat_directory_entry_exfat_data_stream_t *) data )->unknown1 );

			libcnotify_printf(
			 "%s: unknown2\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (fsfat_directory_entry_exfat_data_stream_t *) data )->unknown2 );

			libcnotify_printf(
			 "%s: name size\t\t\t\t: %" PRIu16 " (%" PRIu8 " characters)\n",
			 function,
			 (uint16_t) ( (fsfat_directory_entry_exfat_data_stream_t *) data )->name_size * 2,
			 ( (fsfat_directory_entry_exfat_data_stream_t *) data )->name_size );

			byte_stream_copy_to_uint16_little_endian(
			 ( (fsfat_directory_entry_exfat_data_stream_t *) data )->name_hash,
			 value_16bit );
			libcnotify_printf(
			 "%s: name hash\t\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 value_16bit );

			byte_stream_copy_to_uint16_little_endian(
			 ( (fsfat_directory_entry_exfat_data_stream_t *) data )->unknown3,
			 value_16bit );
			libcnotify_printf(
			 "%s: unknown3\t\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 value_16bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (fsfat_directory_entry_exfat_data_stream_t *) data )->unknown4,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown4\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "%s: valid data size\t\t\t: %" PRIu64 "\n",
			 function,
			 directory_entry->valid_data_size );

			libcnotify_printf(
			 "%s: data start cluster\t\t\t: %" PRIu32 "\n",
			 function,
			 directory_entry->data_start_cluster );

			libcnotify_printf(
			 "%s: data size\t\t\t\t: %" PRIu64 "\n",
			 function,
			 directory_entry->data_size );

			libcnotify_printf(
			 "\n" );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		directory_entry->name_size *= 2;
	}
	else if( directory_entry->entry_type == LIBFSFAT_DIRECTORY_ENTRY_TYPE_EXFAT_FILE_ENTRY )
	{
		byte_stream_copy_to_uint16_little_endian(
		 ( (fsfat_directory_entry_exfat_file_entry_t *) data )->file_attribute_flags,
		 directory_entry->file_attribute_flags );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsfat_directory_entry_exfat_file_entry_t *) data )->creation_date,
		 directory_entry->creation_date );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsfat_directory_entry_exfat_file_entry_t *) data )->creation_time,
		 directory_entry->creation_time );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsfat_directory_entry_exfat_file_entry_t *) data )->modification_date,
		 directory_entry->modification_date );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsfat_directory_entry_exfat_file_entry_t *) data )->modification_time,
		 directory_entry->modification_time );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsfat_directory_entry_exfat_file_entry_t *) data )->access_date,
		 directory_entry->access_date );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsfat_directory_entry_exfat_file_entry_t *) data )->access_time,
		 directory_entry->access_time );

		directory_entry->creation_time_fraction = ( (fsfat_directory_entry_exfat_file_entry_t *) data )->creation_time_fraction;

		directory_entry->modification_time_fraction = ( (fsfat_directory_entry_exfat_file_entry_t *) data )->modification_time_fraction;

		directory_entry->creation_time_utc_offset = ( (fsfat_directory_entry_exfat_file_entry_t *) data )->creation_time_utc_offset;

		directory_entry->access_time_utc_offset = ( (fsfat_directory_entry_exfat_file_entry_t *) data )->access_time_utc_offset;

		directory_entry->modification_time_utc_offset = ( (fsfat_directory_entry_exfat_file_entry_t *) data )->modification_time_utc_offset;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: entry type\t\t\t\t: 0x%02" PRIx8 " (%s)\n",
			 function,
			 ( (fsfat_directory_entry_exfat_allocation_bitmap_t *) data )->entry_type,
			 libfsfat_debug_print_exfat_directory_entry_type(
			  ( (fsfat_directory_entry_exfat_allocation_bitmap_t *) data )->entry_type ) );

			libcnotify_printf(
			 "%s: unknown1\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (fsfat_directory_entry_exfat_file_entry_t *) data )->unknown1 );

			byte_stream_copy_to_uint16_little_endian(
			 ( (fsfat_directory_entry_exfat_file_entry_t *) data )->unknown2,
			 value_16bit );
			libcnotify_printf(
			 "%s: unknown2\t\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 value_16bit );

			libcnotify_printf(
			 "%s: file attribute flags\t\t: 0x%02" PRIx16 "\n",
			 function,
			 directory_entry->file_attribute_flags );
			libfsfat_debug_print_file_attribute_flags(
			 directory_entry->file_attribute_flags );
			libcnotify_printf(
			 "\n" );

			if( libfsfat_debug_print_fat_date_time_value(
			     function,
			     "creation time\t\t\t",
			     directory_entry->creation_date,
			     directory_entry->creation_time,
			     directory_entry->creation_time_fraction,
			     directory_entry->creation_time_utc_offset,
			     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print FAT date time value.",
				 function );

				return( -1 );
			}
			if( libfsfat_debug_print_fat_date_time_value(
			     function,
			     "modification time\t\t\t",
			     directory_entry->modification_date,
			     directory_entry->modification_time,
			     directory_entry->modification_time_fraction,
			     directory_entry->modification_time_utc_offset,
			     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print FAT date time value.",
				 function );

				return( -1 );
			}
			if( libfsfat_debug_print_fat_date_time_value(
			     function,
			     "access time\t\t\t\t",
			     directory_entry->access_date,
			     directory_entry->access_time,
			     0,
			     directory_entry->access_time_utc_offset,
			     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print FAT date time value.",
				 function );

				return( -1 );
			}
			libcnotify_printf(
			 "%s: unknown4:\n",
			 function );
			libcnotify_print_data(
			 ( (fsfat_directory_entry_exfat_file_entry_t *) data )->unknown4,
			 7,
			 0 );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	}
	else if( directory_entry->entry_type == LIBFSFAT_DIRECTORY_ENTRY_TYPE_EXFAT_FILE_ENTRY_NAME )
	{
		if( memory_copy(
		     directory_entry->name_data,
		     ( (fsfat_directory_entry_exfat_file_entry_name_t *) data )->name,
		     30 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy exFAT file entry name.",
			 function );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: entry type\t\t\t\t: 0x%02" PRIx8 " (%s)\n",
			 function,
			 ( (fsfat_directory_entry_exfat_allocation_bitmap_t *) data )->entry_type,
			 libfsfat_debug_print_exfat_directory_entry_type(
			  ( (fsfat_directory_entry_exfat_allocation_bitmap_t *) data )->entry_type ) );

			libcnotify_printf(
			 "%s: unknown1\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (fsfat_directory_entry_exfat_file_entry_name_t *) data )->unknown1 );

			libcnotify_printf(
			 "%s: name data:\n",
			 function );
			libcnotify_print_data(
			 ( (fsfat_directory_entry_exfat_file_entry_name_t *) data )->name,
			 30,
			 0 );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	}
	else if( directory_entry->entry_type == LIBFSFAT_DIRECTORY_ENTRY_TYPE_EXFAT_VOLUME_LABEL )
	{
		if( memory_copy(
		     directory_entry->name_data,
		     ( (fsfat_directory_entry_exfat_volume_label_t *) data )->name,
		     22 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy exFAT volume label name.",
			 function );

			return( -1 );
		}
		directory_entry->name_size = ( (fsfat_directory_entry_exfat_volume_label_t *) data )->name_size;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: entry type\t\t\t\t: 0x%02" PRIx8 " (%s)\n",
			 function,
			 ( (fsfat_directory_entry_exfat_allocation_bitmap_t *) data )->entry_type,
			 libfsfat_debug_print_exfat_directory_entry_type(
			  ( (fsfat_directory_entry_exfat_allocation_bitmap_t *) data )->entry_type ) );

			libcnotify_printf(
			 "%s: name size\t\t\t\t: %" PRIu16 " (%" PRIu8 " characters)\n",
			 function,
			 (uint16_t) ( (fsfat_directory_entry_exfat_volume_label_t *) data )->name_size * 2,
			 ( (fsfat_directory_entry_exfat_volume_label_t *) data )->name_size );

			libcnotify_printf(
			 "%s: name data:\n",
			 function );
			libcnotify_print_data(
			 ( (fsfat_directory_entry_exfat_volume_label_t *) data )->name,
			 22,
			 0 );

			libcnotify_printf(
			 "%s: unknown1:\n",
			 function );
			libcnotify_print_data(
			 ( (fsfat_directory_entry_exfat_volume_label_t *) data )->unknown1,
			 8,
			 0 );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		if( directory_entry->name_size > 11 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid exFAT volume label name size value out of bounds.",
			 function );

			return( -1 );
		}
		directory_entry->name_size *= 2;
	}
	return( 1 );
}

/* Reads a directory entry
 * Returns 1 if successful, 0 if empty or -1 on error
 */
int libfsfat_directory_entry_read_file_io_handle(
     libfsfat_directory_entry_t *directory_entry,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     uint8_t file_system_format,
     libcerror_error_t **error )
{
	uint8_t directory_entry_data[ sizeof( fsfat_directory_entry_t ) ];

	static char *function = "libfsfat_directory_entry_read_file_io_handle";
	ssize_t read_count    = 0;
	int result            = 0;

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading directory entry at offset: %" PRIi64 " (0x%08" PRIx64 ").\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              directory_entry_data,
	              sizeof( fsfat_directory_entry_t ),
	              file_offset,
	              error );

	if( read_count != (ssize_t) sizeof( fsfat_directory_entry_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read directory entry data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	result = libfsfat_directory_entry_read_data(
	          directory_entry,
	          directory_entry_data,
	          sizeof( fsfat_directory_entry_t ),
	          file_system_format,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read directory entry.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the identifier
 * The identifier is the offset of the short name directory entry
 * Returns 1 if successful or -1 on error
 */
int libfsfat_directory_entry_get_identifier(
     libfsfat_directory_entry_t *directory_entry,
     uint64_t *identifier,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_directory_entry_get_identifier";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	*identifier = directory_entry->identifier;

	return( 1 );
}

/* Retrieves the access date and time
 * The timestamp is an unsigned 64-bit integer containing the 10 milli seconds intervals since January 1, 1980
 * Returns 1 if successful or -1 on error
 */
int libfsfat_directory_entry_get_access_time(
     libfsfat_directory_entry_t *directory_entry,
     uint64_t *fat_timestamp,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_directory_entry_get_access_time";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( libfsfat_date_time_get_timestamp(
	     directory_entry->access_date,
	     directory_entry->access_time,
	     0,
	     directory_entry->access_time_utc_offset,
	     fat_timestamp,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve access timestamp.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the creation date and time
 * The timestamp is an unsigned 64-bit integer containing the 10 milli seconds intervals since January 1, 1980
 * Returns 1 if successful or -1 on error
 */
int libfsfat_directory_entry_get_creation_time(
     libfsfat_directory_entry_t *directory_entry,
     uint64_t *fat_timestamp,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_directory_entry_get_creation_time";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( libfsfat_date_time_get_timestamp(
	     directory_entry->creation_date,
	     directory_entry->creation_time,
	     directory_entry->creation_time_fraction,
	     directory_entry->creation_time_utc_offset,
	     fat_timestamp,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve creation timestamp.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the modification date and time
 * The timestamp is an unsigned 64-bit integer containing the 10 milli seconds intervals since January 1, 1980
 * Returns 1 if successful or -1 on error
 */
int libfsfat_directory_entry_get_modification_time(
     libfsfat_directory_entry_t *directory_entry,
     uint64_t *fat_timestamp,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_directory_entry_get_modification_time";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( libfsfat_date_time_get_timestamp(
	     directory_entry->modification_date,
	     directory_entry->modification_time,
	     directory_entry->modification_time_fraction,
	     directory_entry->modification_time_utc_offset,
	     fat_timestamp,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve modification timestamp.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the file attribute flags
 * Returns 1 if successful or -1 on error
 */
int libfsfat_directory_entry_get_file_attribute_flags(
     libfsfat_directory_entry_t *directory_entry,
     uint16_t *file_attribute_flags,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_directory_entry_get_file_attribute_flags";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( file_attribute_flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file attribute flags.",
		 function );

		return( -1 );
	}
	*file_attribute_flags = directory_entry->file_attribute_flags;

	return( 1 );
}

/* Determines the name
 * Returns 1 if successful or -1 on error
 */
int libfsfat_directory_entry_get_name(
     libfsfat_directory_entry_t *directory_entry,
     libcerror_error_t **error )
{
	static char *function   = "libfsfat_directory_entry_get_name";
	size_t name_data_offset = 0;
	size_t name_offset      = 0;
	size_t name_size        = 0;

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( directory_entry->name != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid directory entry - name value already set.",
		 function );

		return( -1 );
	}
	if( directory_entry->name_entries_array != NULL )
	{
		if( libfsfat_directory_entry_get_name_from_exfat_file_name_entries(
		     directory_entry,
		     directory_entry->name_entries_array,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine name from exFAT file name entries.",
			 function );

			goto on_error;
		}
	}
	else if( directory_entry->entry_type == LIBFSFAT_DIRECTORY_ENTRY_TYPE_SHORT_NAME )
	{
		name_size = 8 + 3 + 1;

		if( ( directory_entry->file_attribute_flags & LIBFSFAT_FILE_ATTRIBUTE_FLAG_VOLUME_LABEL ) == 0 )
		{
			name_size += 1;
		}
		directory_entry->name = (uint8_t *) memory_allocate(
		                                     sizeof( uint8_t ) * name_size );

		if( directory_entry->name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create name.",
			 function );

			goto on_error;
		}
		directory_entry->name_size = name_size;

		name_offset = 0;

		for( name_data_offset = 0;
		     name_data_offset < 8;
		     name_data_offset++ )
		{
			if( directory_entry->name_data[ name_data_offset ] == ' ' )
			{
				break;
			}
			directory_entry->name[ name_offset++ ] = directory_entry->name_data[ name_data_offset ];
		}
		for( name_data_offset = 8;
		     name_data_offset < 8 + 3;
		     name_data_offset++ )
		{
			if( directory_entry->name_data[ name_data_offset ] == ' ' )
			{
				break;
			}
			if( ( name_data_offset == 8 )
			 && ( ( directory_entry->file_attribute_flags & LIBFSFAT_FILE_ATTRIBUTE_FLAG_VOLUME_LABEL ) == 0 ) )
			{
				directory_entry->name[ name_offset++ ] = '.';
			}
			directory_entry->name[ name_offset++ ] = directory_entry->name_data[ name_data_offset ];
		}
		directory_entry->name[ name_offset++ ] = 0;
		directory_entry->name_size             = name_offset;
		directory_entry->is_unicode            = 0;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: name data:\n",
			 function );
			libcnotify_print_data(
			 directory_entry->name,
			 directory_entry->name_size,
			 0 );
		}
#endif
	}
	else if( directory_entry->entry_type == LIBFSFAT_DIRECTORY_ENTRY_TYPE_EXFAT_VOLUME_LABEL )
	{
		name_offset = directory_entry->name_size;
		name_size   = directory_entry->name_size + 2;

		directory_entry->name = (uint8_t *) memory_allocate(
		                                     sizeof( uint8_t ) * name_size );

		if( directory_entry->name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create name.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     directory_entry->name,
		     directory_entry->name_data,
		     directory_entry->name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to volume label name: %d.",
			 function );

			goto on_error;
		}
		directory_entry->name[ name_offset ]     = 0;
		directory_entry->name[ name_offset + 1 ] = 0;
		directory_entry->name_size               = name_size;
		directory_entry->is_unicode              = 1;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: name data:\n",
			 function );
			libcnotify_print_data(
			 directory_entry->name,
			 directory_entry->name_size,
			 0 );
		}
#endif
	}
	return( 1 );

on_error:
	if( directory_entry->name != NULL )
	{
		memory_free(
		 directory_entry->name );

		directory_entry->name = NULL;
	}
	directory_entry->name_size = 0;

	return( -1 );
}

/* Determines the name from exFAT file name entries
 * Returns 1 if successful or -1 on error
 */
int libfsfat_directory_entry_get_name_from_exfat_file_name_entries(
     libfsfat_directory_entry_t *directory_entry,
     libcdata_array_t *name_entries_array,
     libcerror_error_t **error )
{
	libfsfat_directory_entry_t *name_directory_entry = NULL;
	static char *function                            = "libfsfat_directory_entry_get_name_from_exfat_file_name_entries";
	size_t name_offset                               = 0;
	size_t name_size                                 = 0;
	int entry_index                                  = 0;
	int number_of_entries                            = 0;

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( directory_entry->name != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid directory entry - name value already set.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     name_entries_array,
	     &number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of name entries.",
		 function );

		goto on_error;
	}
	if( ( number_of_entries == 0 )
	 || ( number_of_entries > 9 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of name entries value out of bounds.",
		 function );

		goto on_error;
	}
	name_size = ( (size_t) 30 * number_of_entries ) + 2;

	directory_entry->name = (uint8_t *) memory_allocate(
	                                     sizeof( uint8_t ) * name_size );

	if( directory_entry->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create name.",
		 function );

		goto on_error;
	}
	directory_entry->name_size = name_size;

	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     name_entries_array,
		     entry_index,
		     (intptr_t **) &name_directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve directory entry: %d from name entries array.",
			 function,
			 entry_index );

			goto on_error;
		}
		if( name_directory_entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid name directory entry: %d.",
			 function,
			 entry_index );

			goto on_error;
		}
		if( memory_copy(
		     &( directory_entry->name[ name_offset ] ),
		     name_directory_entry->name_data,
		     30 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy name segment: %d.",
			 function );

			goto on_error;
		}
		name_offset += 30;
	}
	directory_entry->name[ name_offset++ ] = 0;
	directory_entry->name[ name_offset++ ] = 0;

	for( name_offset = 0;
	     ( name_offset + 1 ) < directory_entry->name_size;
	     name_offset += 2 )
	{
		if( ( directory_entry->name[ name_offset ] == 0 )
		 && ( directory_entry->name[ name_offset + 1 ] == 0 ) )
		{
			name_offset += 2;
			break;
		}
	}
	directory_entry->name_size  = name_offset;
	directory_entry->is_unicode = 1;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: name data:\n",
		 function );
		libcnotify_print_data(
		 directory_entry->name,
		 directory_entry->name_size,
		 0 );
	}
#endif
	return( 1 );

on_error:
	if( directory_entry->name != NULL )
	{
		memory_free(
		 directory_entry->name );

		directory_entry->name = NULL;
	}
	directory_entry->name_size = 0;

	return( -1 );
}

/* Determines the name from VFAT long file name entries
 * Returns 1 if successful or -1 on error
 */
int libfsfat_directory_entry_get_name_from_vfat_long_file_name_entries(
     libfsfat_directory_entry_t *directory_entry,
     libcdata_array_t *name_entries_array,
     libcerror_error_t **error )
{
	libfsfat_directory_entry_t *name_directory_entry = NULL;
	static char *function                            = "libfsfat_directory_entry_get_name_from_vfat_long_file_name_entries";
	size_t name_offset                               = 0;
	size_t name_size                                 = 0;
	int entry_index                                  = 0;
	int number_of_entries                            = 0;

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( directory_entry->name != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid directory entry - name value already set.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     name_entries_array,
	     &number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of long file name entries.",
		 function );

		goto on_error;
	}
	if( ( number_of_entries == 0 )
	 || ( number_of_entries > 10 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of long file name entries value out of bounds.",
		 function );

		goto on_error;
	}
	name_size = ( (size_t) ( 10 + 12 + 4 ) * number_of_entries ) + 2;

	directory_entry->name = (uint8_t *) memory_allocate(
	                                     sizeof( uint8_t ) * name_size );

	if( directory_entry->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create name.",
		 function );

		goto on_error;
	}
	directory_entry->name_size = name_size;

	for( entry_index = number_of_entries - 1;
	     entry_index >= 0;
	     entry_index-- )
	{
		if( libcdata_array_get_entry_by_index(
		     name_entries_array,
		     entry_index,
		     (intptr_t **) &name_directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve directory entry: %d from long file name entries array.",
			 function,
			 entry_index );

			goto on_error;
		}
		if( name_directory_entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid long file name directory entry: %d.",
			 function,
			 entry_index );

			goto on_error;
		}
		if( memory_copy(
		     &( directory_entry->name[ name_offset ] ),
		     name_directory_entry->name_data,
		     10 + 12 + 4 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy long file name segment: %d.",
			 function );

			goto on_error;
		}
		name_offset += 10 + 12 + 4;
	}
	directory_entry->name[ name_offset++ ] = 0;
	directory_entry->name[ name_offset++ ] = 0;

/* TODO determine if name is UCS-2 or ASCII */

	for( name_offset = 0;
	     ( name_offset + 1 ) < directory_entry->name_size;
	     name_offset += 2 )
	{
		if( ( directory_entry->name[ name_offset ] == 0 )
		 && ( directory_entry->name[ name_offset + 1 ] == 0 ) )
		{
			name_offset += 2;
			break;
		}
	}
	directory_entry->name_size  = name_offset;
	directory_entry->is_unicode = 1;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: name data:\n",
		 function );
		libcnotify_print_data(
		 directory_entry->name,
		 directory_entry->name_size,
		 0 );
	}
#endif
	return( 1 );

on_error:
	if( directory_entry->name != NULL )
	{
		memory_free(
		 directory_entry->name );

		directory_entry->name = NULL;
	}
	directory_entry->name_size = 0;

	return( -1 );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsfat_directory_entry_get_utf8_name_size(
     libfsfat_directory_entry_t *directory_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_directory_entry_get_utf8_name_size";
	int result            = 0;

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( directory_entry->name == NULL )
	{
		if( libfsfat_directory_entry_get_name(
		     directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine name.",
			 function );

			return( -1 );
		}
	}
	if( directory_entry->is_unicode != 0 )
	{
		result = libuna_utf8_string_size_from_utf16_stream(
		          directory_entry->name,
		          directory_entry->name_size,
		          LIBUNA_ENDIAN_LITTLE,
		          utf8_string_size,
		          error );
	}
	else
	{
/* TODO add codepage support */
		result = libuna_utf8_string_size_from_byte_stream(
		          directory_entry->name,
		          directory_entry->name_size,
		          LIBUNA_CODEPAGE_ASCII,
		          utf8_string_size,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsfat_directory_entry_get_utf8_name(
     libfsfat_directory_entry_t *directory_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_directory_entry_get_utf8_name";
	int result            = 0;

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( directory_entry->name == NULL )
	{
		if( libfsfat_directory_entry_get_name(
		     directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine name.",
			 function );

			return( -1 );
		}
	}
	if( directory_entry->is_unicode != 0 )
	{
		result = libuna_utf8_string_copy_from_utf16_stream(
		          utf8_string,
		          utf8_string_size,
		          directory_entry->name,
		          directory_entry->name_size,
		          LIBUNA_ENDIAN_LITTLE,
		          error );
	}
	else
	{
/* TODO add codepage support */
		result = libuna_utf8_string_copy_from_byte_stream(
		          utf8_string,
		          utf8_string_size,
		          directory_entry->name,
		          directory_entry->name_size,
		          LIBUNA_CODEPAGE_ASCII,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Compares an UTF-8 string with the name of the directory entry
 * Returns LIBUNA_COMPARE_LESS, LIBUNA_COMPARE_EQUAL, LIBUNA_COMPARE_GREATER if successful or -1 on error
 */
int libfsfat_directory_entry_compare_with_utf8_string(
     libfsfat_directory_entry_t *directory_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_directory_entry_compare_with_utf8_string";
	int result            = 0;

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( directory_entry->name == NULL )
	{
		if( libfsfat_directory_entry_get_name(
		     directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine name.",
			 function );

			return( -1 );
		}
	}
/* TODO add compare of both long and short name */
/* TODO add codepage support */
	result = libfsfat_name_compare_with_utf8_string(
	          directory_entry->name,
	          directory_entry->name_size,
	          directory_entry->is_unicode,
	          utf8_string,
	          utf8_string_length,
	          1,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to compare UTF-8 string with directory entry name.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsfat_directory_entry_get_utf16_name_size(
     libfsfat_directory_entry_t *directory_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_directory_entry_get_utf16_name_size";
	int result            = 0;

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( directory_entry->name == NULL )
	{
		if( libfsfat_directory_entry_get_name(
		     directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine name.",
			 function );

			return( -1 );
		}
	}
	if( directory_entry->is_unicode != 0 )
	{
		result = libuna_utf16_string_size_from_utf16_stream(
		          directory_entry->name,
		          directory_entry->name_size,
		          LIBUNA_ENDIAN_LITTLE,
		          utf16_string_size,
		          error );
	}
	else
	{
/* TODO add codepage support */
		result = libuna_utf16_string_size_from_byte_stream(
		          directory_entry->name,
		          directory_entry->name_size,
		          LIBUNA_CODEPAGE_ASCII,
		          utf16_string_size,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsfat_directory_entry_get_utf16_name(
     libfsfat_directory_entry_t *directory_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_directory_entry_get_utf16_name";
	int result            = 0;

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( directory_entry->name == NULL )
	{
		if( libfsfat_directory_entry_get_name(
		     directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine name.",
			 function );

			return( -1 );
		}
	}
	if( directory_entry->is_unicode != 0 )
	{
		result = libuna_utf16_string_copy_from_utf16_stream(
		          utf16_string,
		          utf16_string_size,
		          directory_entry->name,
		          directory_entry->name_size,
		          LIBUNA_ENDIAN_LITTLE,
		          error );
	}
	else
	{
/* TODO add codepage support */
		result = libuna_utf16_string_copy_from_byte_stream(
		          utf16_string,
		          utf16_string_size,
		          directory_entry->name,
		          directory_entry->name_size,
		          LIBUNA_CODEPAGE_ASCII,
		          error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Compares an UTF-16 string with the name of the directory entry
 * Returns LIBUNA_COMPARE_LESS, LIBUNA_COMPARE_EQUAL, LIBUNA_COMPARE_GREATER if successful or -1 on error
 */
int libfsfat_directory_entry_compare_with_utf16_string(
     libfsfat_directory_entry_t *directory_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_directory_entry_compare_with_utf16_string";
	int result            = 0;

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( directory_entry->name == NULL )
	{
		if( libfsfat_directory_entry_get_name(
		     directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine name.",
			 function );

			return( -1 );
		}
	}
/* TODO add compare of both long and short name */
/* TODO add codepage support */
	result = libfsfat_name_compare_with_utf16_string(
	          directory_entry->name,
	          directory_entry->name_size,
	          directory_entry->is_unicode,
	          utf16_string,
	          utf16_string_length,
	          1,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to compare UTF-16 string with directory entry name.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the data start cluster
 * Returns 1 if successful or -1 on error
 */
int libfsfat_directory_entry_get_data_start_cluster(
     libfsfat_directory_entry_t *directory_entry,
     uint32_t *data_start_cluster,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_directory_entry_get_data_start_cluster";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( data_start_cluster == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data start cluster.",
		 function );

		return( -1 );
	}
	*data_start_cluster = directory_entry->data_start_cluster;

	return( 1 );
}

/* Retrieves the data size
 * Returns 1 if successful or -1 on error
 */
int libfsfat_directory_entry_get_data_size(
     libfsfat_directory_entry_t *directory_entry,
     uint64_t *data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_directory_entry_get_data_size";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data size.",
		 function );

		return( -1 );
	}
	*data_size = directory_entry->data_size;

	return( 1 );
}

/* Retrieves the valid data size
 * Returns 1 if successful or -1 on error
 */
int libfsfat_directory_entry_get_valid_data_size(
     libfsfat_directory_entry_t *directory_entry,
     uint64_t *valid_data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_directory_entry_get_valid_data_size";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( valid_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid valid data size.",
		 function );

		return( -1 );
	}
	*valid_data_size = directory_entry->valid_data_size;

	return( 1 );
}

