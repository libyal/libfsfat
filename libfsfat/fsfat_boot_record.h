/*
 * Master Boot Record (MBR) boot record definitions
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

#if !defined( _FSFAT_BOOT_RECORD_H )
#define _FSFAT_BOOT_RECORD_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsfat_boot_record_fat12 fsfat_boot_record_fat12_t;

struct fsfat_boot_record_fat12
{
	/* The boot entry point
	 * Consists of 3 bytes
	 */
	uint8_t boot_entry_point[ 3 ];

	/* The OEM name
	 * Consists of 8 bytes
	 */
	uint8_t oem_name[ 8 ];

	/* The number of bytes per sector
	 * Consists of 2 bytes
	 */
	uint8_t bytes_per_sector[ 2 ];

	/* The number of sectors per cluster block
	 * Consists of 1 byte
	 */
	uint8_t sectors_per_cluster_block;

	/* The number of reserved sectors
	 * Consists of 2 bytes
	 */
	uint8_t number_of_reserved_sectors[ 2 ];

	/* The number of (file) allocation tables
	 * Consists of 1 byte
	 */
	uint8_t number_of_allocation_tables;

	/* The number of root directory entries
	 * Consists of 2 bytes
	 */
	uint8_t number_of_root_directory_entries[ 2 ];

	/* The total number of sectors 16-bit
	 * Consists of 2 bytes
	 */
	uint8_t total_number_of_sectors_16bit[ 2 ];

	/* The media descriptor
	 * Consists of 1 byte
	 */
	uint8_t media_descriptor;

	/* The (file) allocation table size 16-bit
	 * Consists of 2 bytes
	 */
	uint8_t allocation_table_size_16bit[ 2 ];

	/* The number of sectors per track
	 * Consists of 2 bytes
	 */
	uint8_t sectors_per_track[ 2 ];

	/* The number of heads
	 * Consists of 2 bytes
	 */
	uint8_t number_of_heads[ 2 ];

	/* The number of hidden sectors
	 * Consists of 4 bytes
	 */
	uint8_t number_of_hidden_sectors[ 4 ];

	/* The total number of sectors 32-bit
	 * Consists of 4 bytes
	 */
	uint8_t total_number_of_sectors_32bit[ 4 ];

	/* The drive number
	 * Consists of 1 byte
	 */
	uint8_t drive_number;

	/* Unknown
	 * Consists of 1 byte
	 */
	uint8_t unknown2;

	/* The extended boot signature
	 * Consists of 1 byte
	 */
	uint8_t extended_boot_signature;

	/* The volume serial number
	 * Consists of 4 bytes
	 */
	uint8_t volume_serial_number[ 4 ];

	/* The volume label
	 * Consists of 11 bytes
	 */
	uint8_t volume_label[ 11 ];

	/* The file system hint
	 * Consists of 8 bytes
	 */
	uint8_t file_system_hint[ 8 ];

	/* The bootcode
	 * Consists of 448 bytes
	 */
	uint8_t bootcode[ 448 ];

	/* The sector signature
	 * Consists of 2 bytes
	 * Contains: 0x55 0xaa
	 */
	uint8_t sector_signature[ 2 ];
};

typedef struct fsfat_boot_record_fat32 fsfat_boot_record_fat32_t;

struct fsfat_boot_record_fat32
{
	/* The boot entry point
	 * Consists of 3 bytes
	 */
	uint8_t boot_entry_point[ 3 ];

	/* The OEM name
	 * Consists of 8 bytes
	 */
	uint8_t oem_name[ 8 ];

	/* The number of bytes per sector
	 * Consists of 2 bytes
	 */
	uint8_t bytes_per_sector[ 2 ];

	/* The number of sectors per cluster block
	 * Consists of 1 byte
	 */
	uint8_t sectors_per_cluster_block;

	/* The number of reserved sectors
	 * Consists of 2 bytes
	 */
	uint8_t number_of_reserved_sectors[ 2 ];

	/* The number of (file) allocation tables
	 * Consists of 1 byte
	 */
	uint8_t number_of_allocation_tables;

	/* The number of root directory entries
	 * Consists of 2 bytes
	 */
	uint8_t number_of_root_directory_entries[ 2 ];

	/* The total number of sectors 16-bit
	 * Consists of 2 bytes
	 */
	uint8_t total_number_of_sectors_16bit[ 2 ];

	/* The media descriptor
	 * Consists of 1 byte
	 */
	uint8_t media_descriptor;

	/* The (file) allocation table size 16-bit
	 * Consists of 2 bytes
	 */
	uint8_t allocation_table_size_16bit[ 2 ];

	/* The number of sectors per track
	 * Consists of 2 bytes
	 */
	uint8_t sectors_per_track[ 2 ];

	/* The number of heads
	 * Consists of 2 bytes
	 */
	uint8_t number_of_heads[ 2 ];

	/* The number of hidden sectors
	 * Consists of 4 bytes
	 */
	uint8_t number_of_hidden_sectors[ 4 ];

	/* The total number of sectors 32-bit
	 * Consists of 4 bytes
	 */
	uint8_t total_number_of_sectors_32bit[ 4 ];

	/* The (file) allocation table size 32-bit
	 * Consists of 4 bytes
	 */
	uint8_t allocation_table_size_32bit[ 4 ];

	/* The extended flags
	 * Consists of 2 bytes
	 */
	uint8_t extended_flags[ 2 ];

	/* The (format) revision minor number
	 * Consists of 1 byte
	 */
	uint8_t revision_minor_number;

	/* The (format) revision major number
	 * Consists of 1 byte
	 */
	uint8_t revision_major_number;

	/* The root directory cluster
	 * Consists of 4 bytes
	 */
	uint8_t root_directory_cluster[ 4 ];

	/* The file system information (FSINFO) sector number
	 * Consists of 2 bytes
	 */
	uint8_t fsinfo_sector_number[ 2 ];

	/* The boot sector number
	 * Consists of 2 bytes
	 */
	uint8_t boot_sector_number[ 2 ];

	/* Unknown
	 * Consists of 12 bytes
	 */
	uint8_t unknown1[ 12 ];

	/* The drive number
	 * Consists of 1 byte
	 */
	uint8_t drive_number;

	/* Unknown
	 * Consists of 1 byte
	 */
	uint8_t unknown2;

	/* The extended boot signature
	 * Consists of 1 byte
	 */
	uint8_t extended_boot_signature;

	/* The volume serial number
	 * Consists of 4 bytes
	 */
	uint8_t volume_serial_number[ 4 ];

	/* The volume label
	 * Consists of 11 bytes
	 */
	uint8_t volume_label[ 11 ];

	/* The file system hint
	 * Consists of 8 bytes
	 */
	uint8_t file_system_hint[ 8 ];

	/* The bootcode
	 * Consists of 420 bytes
	 */
	uint8_t bootcode[ 420 ];

	/* The sector signature
	 * Consists of 2 bytes
	 * Contains: 0x55 0xaa
	 */
	uint8_t sector_signature[ 2 ];
};

typedef struct fsfat_boot_record_exfat fsfat_boot_record_exfat_t;

struct fsfat_boot_record_exfat
{
	/* The boot entry point
	 * Consists of 3 bytes
	 */
	uint8_t boot_entry_point[ 3 ];

	/* The OEM name
	 * Consists of 8 bytes
	 */
	uint8_t oem_name[ 8 ];

	/* Unknown
	 * Consists of 53 bytes
	 */
	uint8_t unknown1[ 53 ];

	/* The partition offset
	 * Consists of 8 bytes
	 */
	uint8_t partition_offset[ 8 ];

	/* The total number of sectors 64-bit
	 * Consists of 8 bytes
	 */
	uint8_t total_number_of_sectors_64bit[ 8 ];

	/* The (file) allocation table start sector
	 * Consists of 4 bytes
	 */
	uint8_t allocation_table_offset[ 4 ];

	/* The (file) allocation table size 32-bit
	 * Consists of 4 bytes
	 */
	uint8_t allocation_table_size_32bit[ 4 ];

	/* The first (data) cluster offset
	 * Consists of 4 bytes
	 */
	uint8_t first_cluster_offset[ 4 ];

	/* The total number of clusters
	 * Consists of 4 bytes
	 */
	uint8_t total_number_of_clusters[ 4 ];

	/* The root directory cluster
	 * Consists of 4 bytes
	 */
	uint8_t root_directory_cluster[ 4 ];

	/* The volume serial number
	 * Consists of 4 bytes
	 */
	uint8_t volume_serial_number[ 4 ];

	/* The (format) revision minor number
	 * Consists of 1 byte
	 */
	uint8_t revision_minor_number;

	/* The (format) revision major number
	 * Consists of 1 byte
	 */
	uint8_t revision_major_number;

	/* The volume flags
	 * Consists of 2 bytes
	 */
	uint8_t volume_flags[ 2 ];

	/* The number of bytes per sector
	 * Consists of 1 byte
	 */
	uint8_t bytes_per_sector;

	/* The number of sectors per cluster block
	 * Consists of 1 byte
	 */
	uint8_t sectors_per_cluster_block;

	/* The number of (file) allocation tables
	 * Consists of 1 byte
	 */
	uint8_t number_of_allocation_tables;

	/* The drive number
	 * Consists of 1 byte
	 */
	uint8_t drive_number;

	/* Unknown
	 * Consists of 1 byte
	 */
	uint8_t unknown4;

	/* Unknown
	 * Consists of 7 bytes
	 */
	uint8_t unknown5[ 7 ];

	/* The bootcode
	 * Consists of 390 bytes
	 */
	uint8_t bootcode[ 390 ];

	/* The sector signature
	 * Consists of 2 bytes
	 * Contains: 0x55 0xaa
	 */
	uint8_t sector_signature[ 2 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSFAT_BOOT_RECORD_H ) */

