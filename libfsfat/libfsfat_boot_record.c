/*
 * The boot record functions
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

#include "libfsfat_boot_record.h"
#include "libfsfat_definitions.h"
#include "libfsfat_libbfio.h"
#include "libfsfat_libcerror.h"
#include "libfsfat_libcnotify.h"

#include "fsfat_boot_record.h"

/* Creates a boot record
 * Make sure the value boot_record is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsfat_boot_record_initialize(
     libfsfat_boot_record_t **boot_record,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_boot_record_initialize";

	if( boot_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid boot record.",
		 function );

		return( -1 );
	}
	if( *boot_record != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid boot record value already set.",
		 function );

		return( -1 );
	}
	*boot_record = memory_allocate_structure(
	                libfsfat_boot_record_t );

	if( *boot_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create boot record.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *boot_record,
	     0,
	     sizeof( libfsfat_boot_record_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear boot record.",
		 function );

		memory_free(
		 *boot_record );

		*boot_record = NULL;

		return( -1 );
	}
	return( 1 );

on_error:
	if( *boot_record != NULL )
	{
		memory_free(
		 *boot_record );

		*boot_record = NULL;
	}
	return( -1 );
}

/* Frees a boot record
 * Returns 1 if successful or -1 on error
 */
int libfsfat_boot_record_free(
     libfsfat_boot_record_t **boot_record,
     libcerror_error_t **error )
{
	static char *function = "libfsfat_boot_record_free";

	if( boot_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid boot record.",
		 function );

		return( -1 );
	}
	if( *boot_record != NULL )
	{
		memory_free(
		 *boot_record );

		*boot_record = NULL;
	}
	return( 1 );
}

/* Reads a boot record
 * Returns 1 if successful or -1 on error
 */
int libfsfat_boot_record_read_data(
     libfsfat_boot_record_t *boot_record,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function                     = "libfsfat_boot_record_read_data";
	uint64_t total_number_of_clusters         = 0;
	uint64_t total_number_of_sectors          = 0;
	uint32_t allocation_table_size            = 0;
	uint32_t allocation_table_size_32bit      = 0;
	uint32_t total_number_of_sectors_32bit    = 0;
	uint16_t allocation_table_size_16bit      = 0;
	uint16_t number_of_reserved_sectors       = 0;
	uint16_t number_of_root_directory_entries = 0;
	uint16_t total_number_of_sectors_16bit    = 0;
	uint8_t number_of_allocation_tables       = 0;
	uint8_t sectors_per_cluster_block         = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	const char *fshint                        = NULL;
	uint64_t value_64bit                      = 0;
	uint32_t value_32bit                      = 0;
	uint16_t value_16bit                      = 0;
#endif

	if( boot_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid boot record.",
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
	if( data_size != sizeof( fsfat_boot_record_fat12_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: boot record data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsfat_boot_record_fat12_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( ( ( (fsfat_boot_record_fat12_t *) data )->sector_signature[ 0 ] != 0x55 )
	 || ( ( (fsfat_boot_record_fat12_t *) data )->sector_signature[ 1 ] != 0xaa ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid sector signature: 0x%02" PRIx8 " 0x%02" PRIx8 ".",
		 function,
		 ( (fsfat_boot_record_fat12_t *) data )->sector_signature[ 0 ],
		 ( (fsfat_boot_record_fat12_t *) data )->sector_signature[ 1 ] );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: boot entry point:\n",
		 function );
		libcnotify_print_data(
		 data,
		 3,
		 0 );

		libcnotify_printf(
		 "%s: OEM name\t\t\t\t: %c%c%c%c%c%c%c%c\n",
		 function,
		 ( (fsfat_boot_record_fat12_t *) data )->oem_name[ 0 ],
		 ( (fsfat_boot_record_fat12_t *) data )->oem_name[ 1 ],
		 ( (fsfat_boot_record_fat12_t *) data )->oem_name[ 2 ],
		 ( (fsfat_boot_record_fat12_t *) data )->oem_name[ 3 ],
		 ( (fsfat_boot_record_fat12_t *) data )->oem_name[ 4 ],
		 ( (fsfat_boot_record_fat12_t *) data )->oem_name[ 5 ],
		 ( (fsfat_boot_record_fat12_t *) data )->oem_name[ 6 ],
		 ( (fsfat_boot_record_fat12_t *) data )->oem_name[ 7 ] );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( ( ( (fsfat_boot_record_fat12_t *) data )->oem_name[ 0 ] == 'E' )
	 && ( ( (fsfat_boot_record_fat12_t *) data )->oem_name[ 1 ] == 'X' )
	 && ( ( (fsfat_boot_record_fat12_t *) data )->oem_name[ 2 ] == 'F' )
	 && ( ( (fsfat_boot_record_fat12_t *) data )->oem_name[ 3 ] == 'A' )
	 && ( ( (fsfat_boot_record_fat12_t *) data )->oem_name[ 4 ] == 'T' )
	 && ( ( (fsfat_boot_record_fat12_t *) data )->oem_name[ 5 ] == ' ' )
	 && ( ( (fsfat_boot_record_fat12_t *) data )->oem_name[ 6 ] == ' ' )
	 && ( ( (fsfat_boot_record_fat12_t *) data )->oem_name[ 7 ] == ' ' ) )
	{
		boot_record->file_system_format = LIBFSFAT_FILE_SYSTEM_FORMAT_EXFAT;
	}
	if( boot_record->file_system_format == LIBFSFAT_FILE_SYSTEM_FORMAT_EXFAT )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsfat_boot_record_exfat_t *) data )->total_number_of_sectors_64bit,
		 total_number_of_sectors );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsfat_boot_record_exfat_t *) data )->allocation_table_offset,
		 boot_record->allocation_table_offset );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsfat_boot_record_exfat_t *) data )->allocation_table_size_32bit,
		 boot_record->allocation_table_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsfat_boot_record_exfat_t *) data )->first_cluster_offset,
		 boot_record->first_cluster_offset );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsfat_boot_record_exfat_t *) data )->total_number_of_clusters,
		 boot_record->total_number_of_clusters );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsfat_boot_record_exfat_t *) data )->root_directory_cluster,
		 boot_record->root_directory_cluster );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsfat_boot_record_exfat_t *) data )->volume_serial_number,
		 boot_record->volume_serial_number );

		boot_record->bytes_per_sector = ( (fsfat_boot_record_exfat_t *) data )->bytes_per_sector;

		sectors_per_cluster_block = ( (fsfat_boot_record_exfat_t *) data )->sectors_per_cluster_block;

		number_of_allocation_tables = ( (fsfat_boot_record_exfat_t *) data )->number_of_allocation_tables;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unknown1:\n",
			 function );
			libcnotify_print_data(
			 ( (fsfat_boot_record_exfat_t *) data )->unknown1,
			 53,
			 0 );

			byte_stream_copy_to_uint64_little_endian(
			 ( (fsfat_boot_record_exfat_t *) data )->partition_offset,
			 value_64bit );
			libcnotify_printf(
			 "%s: partition offset\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );

			libcnotify_printf(
			 "%s: total number of sectors (64-bit)\t: %" PRIu64 "\n",
			 function,
			 total_number_of_sectors );

			libcnotify_printf(
			 "%s: allocation table offset\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 boot_record->allocation_table_offset );

			libcnotify_printf(
			 "%s: allocation table size (32-bit)\t\t: %" PRIu32 "\n",
			 function,
			 boot_record->allocation_table_size );

			libcnotify_printf(
			 "%s: first cluster offset\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 boot_record->first_cluster_offset );

			libcnotify_printf(
			 "%s: total number of clusters\t\t: %" PRIu32 "\n",
			 function,
			 boot_record->total_number_of_clusters );

			libcnotify_printf(
			 "%s: root directory cluster\t\t\t: %" PRIu32 "\n",
			 function,
			 boot_record->root_directory_cluster );

			libcnotify_printf(
			 "%s: volume serial number\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 boot_record->volume_serial_number );

			libcnotify_printf(
			 "%s: revision minor number\t\t\t: %" PRIu8 "\n",
			 function,
			 ( (fsfat_boot_record_exfat_t *) data )->revision_minor_number );

			libcnotify_printf(
			 "%s: revision major number\t\t\t: %" PRIu8 "\n",
			 function,
			 ( (fsfat_boot_record_exfat_t *) data )->revision_major_number );

			byte_stream_copy_to_uint16_little_endian(
			 ( (fsfat_boot_record_exfat_t *) data )->volume_flags,
			 value_16bit );
			libcnotify_printf(
			 "%s: volume flags\t\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 value_16bit );

			libcnotify_printf(
			 "%s: bytes per sector\t\t\t: %" PRIu64 " (2^%" PRIu8 ")\n",
			 function,
			 (uint64_t) 1 << ( (fsfat_boot_record_exfat_t *) data )->bytes_per_sector,
			 ( (fsfat_boot_record_exfat_t *) data )->bytes_per_sector );

			libcnotify_printf(
			 "%s: sectors per cluster block\t\t: %" PRIu64 " (2^%" PRIu8 ")\n",
			 function,
			 (uint64_t) 1 << ( (fsfat_boot_record_exfat_t *) data )->sectors_per_cluster_block,
			 ( (fsfat_boot_record_exfat_t *) data )->sectors_per_cluster_block );

			libcnotify_printf(
			 "%s: number of allocation tables\t\t: %" PRIu8 "\n",
			 function,
			 number_of_allocation_tables );

			libcnotify_printf(
			 "%s: drive number\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (fsfat_boot_record_exfat_t *) data )->drive_number );

			libcnotify_printf(
			 "%s: unknown4\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (fsfat_boot_record_exfat_t *) data )->unknown4 );

			libcnotify_printf(
			 "%s: unknown5:\n",
			 function );
			libcnotify_print_data(
			 ( (fsfat_boot_record_exfat_t *) data )->unknown5,
			 7,
			 0 );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	}
	else
	{
		byte_stream_copy_to_uint16_little_endian(
		 ( (fsfat_boot_record_fat12_t *) data )->bytes_per_sector,
		 boot_record->bytes_per_sector );

		sectors_per_cluster_block = ( (fsfat_boot_record_fat12_t *) data )->sectors_per_cluster_block;

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsfat_boot_record_fat12_t *) data )->number_of_reserved_sectors,
		 number_of_reserved_sectors );

		number_of_allocation_tables = ( (fsfat_boot_record_fat12_t *) data )->number_of_allocation_tables;

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsfat_boot_record_fat12_t *) data )->number_of_root_directory_entries,
		 number_of_root_directory_entries );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsfat_boot_record_fat12_t *) data )->total_number_of_sectors_16bit,
		 total_number_of_sectors_16bit );

		total_number_of_sectors = (uint64_t) total_number_of_sectors_16bit;

		byte_stream_copy_to_uint16_little_endian(
		 ( (fsfat_boot_record_fat12_t *) data )->allocation_table_size_16bit,
		 allocation_table_size_16bit );

		allocation_table_size = (uint32_t) allocation_table_size_16bit;

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsfat_boot_record_fat12_t *) data )->total_number_of_sectors_32bit,
		 total_number_of_sectors_32bit );

		if( total_number_of_sectors_32bit != 0 )
		{
			total_number_of_sectors = (uint64_t) total_number_of_sectors_32bit;
		}
		if( ( number_of_root_directory_entries == 0 )
		 && ( total_number_of_sectors_16bit == 0 )
		 && ( allocation_table_size_16bit == 0 ) )
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (fsfat_boot_record_fat32_t *) data )->allocation_table_size_32bit,
			 allocation_table_size_32bit );

			allocation_table_size = allocation_table_size_32bit;

			byte_stream_copy_to_uint32_little_endian(
			 ( (fsfat_boot_record_fat32_t *) data )->root_directory_cluster,
			 boot_record->root_directory_cluster );

			if( ( (fsfat_boot_record_fat32_t *) data )->extended_boot_signature == 0x29 )
			{
				byte_stream_copy_to_uint32_little_endian(
				 ( (fsfat_boot_record_fat32_t *) data )->volume_serial_number,
				 boot_record->volume_serial_number );
			}
		}
		else
		{
			if( ( (fsfat_boot_record_fat12_t *) data )->extended_boot_signature == 0x29 )
			{
				byte_stream_copy_to_uint32_little_endian(
				 ( (fsfat_boot_record_fat12_t *) data )->volume_serial_number,
				 boot_record->volume_serial_number );
			}
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: bytes per sector\t\t\t: %" PRIu16 "\n",
			 function,
			 boot_record->bytes_per_sector );

			libcnotify_printf(
			 "%s: sectors per cluster block\t\t: %" PRIu8 "\n",
			 function,
			 sectors_per_cluster_block );

			libcnotify_printf(
			 "%s: number of reserved sectors\t\t: %" PRIu16 "\n",
			 function,
			 number_of_reserved_sectors );

			libcnotify_printf(
			 "%s: number of allocation tables\t\t: %" PRIu8 "\n",
			 function,
			 number_of_allocation_tables );

			libcnotify_printf(
			 "%s: number of root directory entries\t: %" PRIu16 "\n",
			 function,
			 number_of_root_directory_entries );

			libcnotify_printf(
			 "%s: total number of sectors (16-bit)\t: %" PRIu16 "\n",
			 function,
			 total_number_of_sectors_16bit );

			libcnotify_printf(
			 "%s: media descriptor\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (fsfat_boot_record_fat12_t *) data )->media_descriptor );

			libcnotify_printf(
			 "%s: allocation table size (16-bit)\t\t: %" PRIu16 "\n",
			 function,
			 allocation_table_size_16bit );

			byte_stream_copy_to_uint16_little_endian(
			 ( (fsfat_boot_record_fat12_t *) data )->sectors_per_track,
			 value_16bit );
			libcnotify_printf(
			 "%s: sectors per track\t\t\t: %" PRIu16 "\n",
			 function,
			 value_16bit );

			byte_stream_copy_to_uint16_little_endian(
			 ( (fsfat_boot_record_fat12_t *) data )->number_of_heads,
			 value_16bit );
			libcnotify_printf(
			 "%s: number of heads\t\t\t\t: %" PRIu16 "\n",
			 function,
			 value_16bit );

			byte_stream_copy_to_uint32_little_endian(
			 ( (fsfat_boot_record_fat12_t *) data )->number_of_hidden_sectors,
			 value_32bit );
			libcnotify_printf(
			 "%s: number of hidden sectors\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "%s: total number of sectors (32-bit)\t: %" PRIu32 "\n",
			 function,
			 total_number_of_sectors_32bit );

			if( ( number_of_root_directory_entries == 0 )
			 && ( total_number_of_sectors_16bit == 0 )
			 && ( allocation_table_size_16bit == 0 ) )
			{
				libcnotify_printf(
				 "%s: allocation table size (32-bit)\t\t: %" PRIu32 "\n",
				 function,
				 allocation_table_size_32bit );

				byte_stream_copy_to_uint16_little_endian(
				 ( (fsfat_boot_record_fat32_t *) data )->extended_flags,
				 value_16bit );
				libcnotify_printf(
				 "%s: extended flags\t\t\t\t: 0x%04" PRIx16 "\n",
				 function,
				 value_16bit );

				libcnotify_printf(
				 "%s: revision minor number\t\t\t: %" PRIu8 "\n",
				 function,
				 ( (fsfat_boot_record_fat32_t *) data )->revision_minor_number );

				libcnotify_printf(
				 "%s: revision major number\t\t\t: %" PRIu8 "\n",
				 function,
				 ( (fsfat_boot_record_fat32_t *) data )->revision_major_number );

				libcnotify_printf(
				 "%s: root directory cluster\t\t\t: %" PRIu32 "\n",
				 function,
				 boot_record->root_directory_cluster );

				byte_stream_copy_to_uint16_little_endian(
				 ( (fsfat_boot_record_fat32_t *) data )->fsinfo_sector_number,
				 value_16bit );
				libcnotify_printf(
				 "%s: FSINFO sector number\t\t\t: %" PRIu16 "\n",
				 function,
				 value_16bit );

				byte_stream_copy_to_uint16_little_endian(
				 ( (fsfat_boot_record_fat32_t *) data )->boot_sector_number,
				 value_16bit );
				libcnotify_printf(
				 "%s: boot sector number\t\t\t: %" PRIu16 "\n",
				 function,
				 value_16bit );

				libcnotify_printf(
				 "%s: unknown1\n",
				 function );
				libcnotify_print_data(
				 ( (fsfat_boot_record_fat32_t *) data )->unknown1,
				 12,
				 0 );

				libcnotify_printf(
				 "%s: drive number\t\t\t\t: 0x%02" PRIx8 "\n",
				 function,
				 ( (fsfat_boot_record_fat32_t *) data )->drive_number );

				libcnotify_printf(
				 "%s: unknown2\t\t\t\t: 0x%02" PRIx8 "\n",
				 function,
				 ( (fsfat_boot_record_fat32_t *) data )->unknown2 );

				libcnotify_printf(
				 "%s: extended boot signature\t\t\t: 0x%02" PRIx8 "\n",
				 function,
				 ( (fsfat_boot_record_fat32_t *) data )->extended_boot_signature );

				if( ( (fsfat_boot_record_fat32_t *) data )->extended_boot_signature == 0x29 )
				{
					libcnotify_printf(
					 "%s: volume serial number\t\t\t: 0x%08" PRIx32 "\n",
					 function,
					 boot_record->volume_serial_number );

					libcnotify_printf(
					 "%s: volume label\t\t\t\t: %c%c%c%c%c%c%c%c%c%c%c\n",
					 function,
					 ( (fsfat_boot_record_fat32_t *) data )->volume_label[ 0 ],
					 ( (fsfat_boot_record_fat32_t *) data )->volume_label[ 1 ],
					 ( (fsfat_boot_record_fat32_t *) data )->volume_label[ 2 ],
					 ( (fsfat_boot_record_fat32_t *) data )->volume_label[ 3 ],
					 ( (fsfat_boot_record_fat32_t *) data )->volume_label[ 4 ],
					 ( (fsfat_boot_record_fat32_t *) data )->volume_label[ 5 ],
					 ( (fsfat_boot_record_fat32_t *) data )->volume_label[ 6 ],
					 ( (fsfat_boot_record_fat32_t *) data )->volume_label[ 7 ],
					 ( (fsfat_boot_record_fat32_t *) data )->volume_label[ 8 ],
					 ( (fsfat_boot_record_fat32_t *) data )->volume_label[ 9 ],
					 ( (fsfat_boot_record_fat32_t *) data )->volume_label[ 10 ] );

					libcnotify_printf(
					 "%s: file system hint\t\t\t: %c%c%c%c%c%c%c%c\n",
					 function,
					 ( (fsfat_boot_record_fat32_t *) data )->file_system_hint[ 0 ],
					 ( (fsfat_boot_record_fat32_t *) data )->file_system_hint[ 1 ],
					 ( (fsfat_boot_record_fat32_t *) data )->file_system_hint[ 2 ],
					 ( (fsfat_boot_record_fat32_t *) data )->file_system_hint[ 3 ],
					 ( (fsfat_boot_record_fat32_t *) data )->file_system_hint[ 4 ],
					 ( (fsfat_boot_record_fat32_t *) data )->file_system_hint[ 5 ],
					 ( (fsfat_boot_record_fat32_t *) data )->file_system_hint[ 6 ],
					 ( (fsfat_boot_record_fat32_t *) data )->file_system_hint[ 7 ] );
				}
				libcnotify_printf(
				 "%s: bootcode\n",
				 function );
				libcnotify_print_data(
				 ( (fsfat_boot_record_fat32_t *) data )->bootcode,
				 420,
				 0 );
			}
			else
			{
				libcnotify_printf(
				 "%s: drive number\t\t\t\t: 0x%02" PRIx8 "\n",
				 function,
				 ( (fsfat_boot_record_fat12_t *) data )->drive_number );

				libcnotify_printf(
				 "%s: unknown2\t\t\t\t: 0x%02" PRIx8 "\n",
				 function,
				 ( (fsfat_boot_record_fat12_t *) data )->unknown2 );

				libcnotify_printf(
				 "%s: extended boot signature\t\t\t: 0x%02" PRIx8 "\n",
				 function,
				 ( (fsfat_boot_record_fat12_t *) data )->extended_boot_signature );

				if( ( (fsfat_boot_record_fat12_t *) data )->extended_boot_signature == 0x29 )
				{
					libcnotify_printf(
					 "%s: volume serial number\t\t\t: 0x%08" PRIx32 "\n",
					 function,
					 boot_record->volume_serial_number );

					libcnotify_printf(
					 "%s: volume label\t\t\t\t: %c%c%c%c%c%c%c%c%c%c%c\n",
					 function,
					 ( (fsfat_boot_record_fat12_t *) data )->volume_label[ 0 ],
					 ( (fsfat_boot_record_fat12_t *) data )->volume_label[ 1 ],
					 ( (fsfat_boot_record_fat12_t *) data )->volume_label[ 2 ],
					 ( (fsfat_boot_record_fat12_t *) data )->volume_label[ 3 ],
					 ( (fsfat_boot_record_fat12_t *) data )->volume_label[ 4 ],
					 ( (fsfat_boot_record_fat12_t *) data )->volume_label[ 5 ],
					 ( (fsfat_boot_record_fat12_t *) data )->volume_label[ 6 ],
					 ( (fsfat_boot_record_fat12_t *) data )->volume_label[ 7 ],
					 ( (fsfat_boot_record_fat12_t *) data )->volume_label[ 8 ],
					 ( (fsfat_boot_record_fat12_t *) data )->volume_label[ 9 ],
					 ( (fsfat_boot_record_fat12_t *) data )->volume_label[ 10 ] );

					libcnotify_printf(
					 "%s: file system hint\t\t\t: %c%c%c%c%c%c%c%c\n",
					 function,
					 ( (fsfat_boot_record_fat12_t *) data )->file_system_hint[ 0 ],
					 ( (fsfat_boot_record_fat12_t *) data )->file_system_hint[ 1 ],
					 ( (fsfat_boot_record_fat12_t *) data )->file_system_hint[ 2 ],
					 ( (fsfat_boot_record_fat12_t *) data )->file_system_hint[ 3 ],
					 ( (fsfat_boot_record_fat12_t *) data )->file_system_hint[ 4 ],
					 ( (fsfat_boot_record_fat12_t *) data )->file_system_hint[ 5 ],
					 ( (fsfat_boot_record_fat12_t *) data )->file_system_hint[ 6 ],
					 ( (fsfat_boot_record_fat12_t *) data )->file_system_hint[ 7 ] );
				}
				libcnotify_printf(
				 "%s: bootcode\n",
				 function );
				libcnotify_print_data(
				 ( (fsfat_boot_record_fat12_t *) data )->bootcode,
				 448,
				 0 );
			}
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	}

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: sector signature\t\t\t: 0x%02" PRIx8 " 0x%02" PRIx8 "\n",
		 function,
		 ( (fsfat_boot_record_fat12_t *) data )->sector_signature[ 0 ],
		 ( (fsfat_boot_record_fat12_t *) data )->sector_signature[ 1 ] );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( boot_record->file_system_format == LIBFSFAT_FILE_SYSTEM_FORMAT_EXFAT )
	{
		if( ( boot_record->bytes_per_sector < 9 )
		 || ( boot_record->bytes_per_sector > 12 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid bytes per sector value out of bounds.",
			 function );

			return( -1 );
		}
		boot_record->bytes_per_sector = 1 << boot_record->bytes_per_sector;

		if( sectors_per_cluster_block > 25 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid sectors per cluster block value out of bounds.",
			 function );

			return( -1 );
		}
/* TODO check bounds */
		boot_record->cluster_block_size = ( (size_t) 1 << sectors_per_cluster_block ) * boot_record->bytes_per_sector;

		boot_record->allocation_table_offset *= boot_record->bytes_per_sector;
		boot_record->allocation_table_size   *= boot_record->bytes_per_sector;
		boot_record->first_cluster_offset    *= boot_record->bytes_per_sector;
	}
	else
	{
		if( ( boot_record->bytes_per_sector != 512 )
		 && ( boot_record->bytes_per_sector != 1024 )
		 && ( boot_record->bytes_per_sector != 2048 )
		 && ( boot_record->bytes_per_sector != 4096 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid bytes per sector value out of bounds.",
			 function );

			return( -1 );
		}
		if( ( sectors_per_cluster_block != 1 )
		 && ( sectors_per_cluster_block != 2 )
		 && ( sectors_per_cluster_block != 4 )
		 && ( sectors_per_cluster_block != 8 )
		 && ( sectors_per_cluster_block != 16 )
		 && ( sectors_per_cluster_block != 32 )
		 && ( sectors_per_cluster_block != 64 )
		 && ( sectors_per_cluster_block != 128 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid sectors per cluster block value out of bounds.",
			 function );

			return( -1 );
		}
		boot_record->cluster_block_size = (size_t) sectors_per_cluster_block * boot_record->bytes_per_sector;

		total_number_of_clusters  = total_number_of_sectors;
		total_number_of_clusters -= number_of_reserved_sectors;
		total_number_of_clusters -= number_of_allocation_tables * allocation_table_size;
		total_number_of_clusters /= sectors_per_cluster_block;

		if( total_number_of_clusters < 4085 )
		{
			boot_record->file_system_format = LIBFSFAT_FILE_SYSTEM_FORMAT_FAT12;
		}
		else if( total_number_of_clusters < 65525 )
		{
			boot_record->file_system_format = LIBFSFAT_FILE_SYSTEM_FORMAT_FAT16;
		}
		else
		{
			boot_record->file_system_format = LIBFSFAT_FILE_SYSTEM_FORMAT_FAT32;
		}
		boot_record->total_number_of_clusters = (uint32_t) total_number_of_clusters;
		boot_record->allocation_table_offset  = (off64_t) number_of_reserved_sectors * boot_record->bytes_per_sector;
		boot_record->allocation_table_size    = (size64_t) allocation_table_size * boot_record->bytes_per_sector;
		boot_record->first_cluster_offset     = (off64_t) number_of_reserved_sectors + ( (off64_t) number_of_allocation_tables * allocation_table_size );
		boot_record->first_cluster_offset    *= boot_record->bytes_per_sector;

		if( ( number_of_root_directory_entries != 0 )
		 || ( total_number_of_sectors_16bit != 0 )
		 || ( allocation_table_size_16bit != 0 ) )
		{
			boot_record->root_directory_offset = boot_record->first_cluster_offset;
			boot_record->root_directory_size   = (size64_t) number_of_root_directory_entries * 32;

			boot_record->first_cluster_offset += boot_record->root_directory_size;
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: cluster block size\t\t\t: %" PRIzd "\n",
		 function,
		 boot_record->cluster_block_size );

		switch( boot_record->file_system_format )
		{
			case LIBFSFAT_FILE_SYSTEM_FORMAT_EXFAT:
				fshint = "exFAT";
				break;

			case LIBFSFAT_FILE_SYSTEM_FORMAT_FAT12:
				fshint = "FAT-12";
				break;

			case LIBFSFAT_FILE_SYSTEM_FORMAT_FAT16:
				fshint = "FAT-16";
				break;

			case LIBFSFAT_FILE_SYSTEM_FORMAT_FAT32:
				fshint = "FAT-32";
				break;

			default:
				fshint = "UNKNOWN";
				break;
		}
		libcnotify_printf(
		 "%s: total number of clusters\t\t: %" PRIu32 " (%s)\n",
		 function,
		 boot_record->total_number_of_clusters,
		 fshint );

		libcnotify_printf(
		 "%s: allocation table offset\t\t\t: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 boot_record->allocation_table_offset,
		 boot_record->allocation_table_offset );

		libcnotify_printf(
		 "%s: allocation table size\t\t\t: %" PRIu64 "\n",
		 function,
		 boot_record->allocation_table_size );

		libcnotify_printf(
		 "%s: root directory offset\t\t\t: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 boot_record->root_directory_offset,
		 boot_record->root_directory_offset );

		libcnotify_printf(
		 "%s: root directory size\t\t\t: %" PRIu64 "\n",
		 function,
		 boot_record->root_directory_size );

		libcnotify_printf(
		 "%s: root directory cluster\t\t\t: %" PRIu32 "\n",
		 function,
		 boot_record->root_directory_cluster );

		libcnotify_printf(
		 "%s: first cluster offset\t\t\t: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 boot_record->first_cluster_offset,
		 boot_record->first_cluster_offset );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

/* Reads a boot record
 * Returns 1 if successful or -1 on error
 */
int libfsfat_boot_record_read_file_io_handle(
     libfsfat_boot_record_t *boot_record,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	uint8_t boot_record_data[ sizeof( fsfat_boot_record_fat12_t ) ];

	static char *function = "libfsfat_boot_record_read_file_io_handle";
	ssize_t read_count    = 0;

	if( boot_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid boot record.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading boot record at offset: %" PRIi64 " (0x%08" PRIx64 ").\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              boot_record_data,
	              sizeof( fsfat_boot_record_fat12_t ),
	              file_offset,
	              error );

	if( read_count != (ssize_t) sizeof( fsfat_boot_record_fat12_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read boot record data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	if( libfsfat_boot_record_read_data(
	     boot_record,
	     boot_record_data,
	     sizeof( fsfat_boot_record_fat12_t ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read boot record.",
		 function );

		return( -1 );
	}
	return( 1 );
}

