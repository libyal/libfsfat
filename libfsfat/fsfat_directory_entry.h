/*
 * Master Boot Record (MBR) directory entry definitions
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

#if !defined( _FSFAT_DIRECTORY_ENTRY_H )
#define _FSFAT_DIRECTORY_ENTRY_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsfat_directory_entry fsfat_directory_entry_t;

struct fsfat_directory_entry
{
	/* The (short) name
	 * Consists of 8 bytes
	 */
	uint8_t name[ 8 ];

	/* The (short) extension
	 * Consists of 3 bytes
	 */
	uint8_t extension[ 3 ];

	/* The file attribute flags
	 * Consists of 1 byte
	 */
	uint8_t file_attribute_flags;

	/* Unknown
	 * Consists of 1 byte
	 */
	uint8_t unknown1;

	/* The creation time fraction of seconds
	 * Consists of 1 byte
	 */
	uint8_t creation_time_fraction;

	/* The creation time
	 * Consists of 2 bytes
	 */
	uint8_t creation_time[ 2 ];

	/* The creation date
	 * Consists of 2 bytes
	 */
	uint8_t creation_date[ 2 ];

	/* The (last) access date
	 * Consists of 2 bytes
	 */
	uint8_t access_date[ 2 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown3[ 2 ];

	/* The (last) modification time
	 * Consists of 2 bytes
	 */
	uint8_t modification_time[ 2 ];

	/* The (last) modification date
	 * Consists of 2 bytes
	 */
	uint8_t modification_date[ 2 ];

	/* The data start cluster
	 * Consists of 2 bytes
	 */
	uint8_t data_start_cluster[ 2 ];

	/* The data size
	 * Consists of 4 bytes
	 */
	uint8_t data_size[ 4 ];
};

typedef struct fsfat_directory_entry_vfat fsfat_directory_entry_vfat_t;

struct fsfat_directory_entry_vfat
{
	/* The sequence number
	 * Consists of 1 byte
	 */
	uint8_t sequence_number;

	/* The first name segment
	 * Consists of 10 bytes
	 */
	uint8_t first_name_segment[ 10 ];

	/* Unknown
	 * Consists of 1 byte
	 */
	uint8_t unknown1;

	/* Unknown
	 * Consists of 1 byte
	 */
	uint8_t unknown2;

	/* The name checksum
	 * Consists of 1 byte
	 */
	uint8_t name_checksum;

	/* The second name segment
	 * Consists of 12 bytes
	 */
	uint8_t second_name_segment[ 12 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown3[ 2 ];

	/* The third name segment
	 * Consists of 4 bytes
	 */
	uint8_t third_name_segment[ 4 ];
};

typedef struct fsfat_directory_entry_exfat_allocation_bitmap fsfat_directory_entry_exfat_allocation_bitmap_t;

struct fsfat_directory_entry_exfat_allocation_bitmap
{
	/* The entry type
	 * Consists of 1 byte
	 */
	uint8_t entry_type;

	/* The bitmap flags
	 * Consists of 1 byte
	 */
	uint8_t bitmap_flags;

	/* Unknown (reserved)
	 * Consists of 18 bytes
	 */
	uint8_t unknown1[ 18 ];

	/* The data start cluster
	 * Consists of 4 bytes
	 */
	uint8_t data_start_cluster[ 4 ];

	/* The data size
	 * Consists of 8 bytes
	 */
	uint8_t data_size[ 8 ];
};

typedef struct fsfat_directory_entry_exfat_data_stream fsfat_directory_entry_exfat_data_stream_t;

struct fsfat_directory_entry_exfat_data_stream
{
	/* The entry type
	 * Consists of 1 byte
	 */
	uint8_t entry_type;

	/* Unknown
	 * Consists of 1 byte
	 */
	uint8_t unknown1;

	/* Unknown
	 * Consists of 1 byte
	 */
	uint8_t unknown2;

	/* Name size
	 * Consists of 1 byte
	 */
	uint8_t name_size;

	/* Name hash
	 * Consists of 2 bytes
	 */
	uint8_t name_hash[ 2 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown3[ 2 ];

	/* The valid data size
	 * Consists of 8 bytes
	 */
	uint8_t valid_data_size[ 8 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown4[ 4 ];

	/* The data start cluster
	 * Consists of 4 bytes
	 */
	uint8_t data_start_cluster[ 4 ];

	/* The data size
	 * Consists of 8 bytes
	 */
	uint8_t data_size[ 8 ];
};

typedef struct fsfat_directory_entry_exfat_file_entry fsfat_directory_entry_exfat_file_entry_t;

struct fsfat_directory_entry_exfat_file_entry
{
	/* The entry type
	 * Consists of 1 byte
	 */
	uint8_t entry_type;

	/* Unknown
	 * Consists of 1 byte
	 */
	uint8_t unknown1;

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown2[ 2 ];

	/* The file attribute flags
	 * Consists of 2 bytes
	 */
	uint8_t file_attribute_flags[ 2 ];

	/* Unknown (reserved)
	 * Consists of 2 bytes
	 */
	uint8_t unknown3[ 2 ];

	/* The creation time
	 * Consists of 2 bytes
	 */
	uint8_t creation_time[ 2 ];

	/* The creation date
	 * Consists of 2 bytes
	 */
	uint8_t creation_date[ 2 ];

	/* The (last) modification time
	 * Consists of 2 bytes
	 */
	uint8_t modification_time[ 2 ];

	/* The (last) modification date
	 * Consists of 2 bytes
	 */
	uint8_t modification_date[ 2 ];

	/* The (last) access time
	 * Consists of 2 bytes
	 */
	uint8_t access_time[ 2 ];

	/* The (last) access date
	 * Consists of 2 bytes
	 */
	uint8_t access_date[ 2 ];

	/* The creation time fraction of seconds
	 * Consists of 1 byte
	 */
	uint8_t creation_time_fraction;

	/* The modification time fraction of seconds
	 * Consists of 1 byte
	 */
	uint8_t modification_time_fraction;

	/* The creation time UTC offset
	 * Consists of 1 byte
	 */
	uint8_t creation_time_utc_offset;

	/* The modification time UTC offset
	 * Consists of 1 byte
	 */
	uint8_t modification_time_utc_offset;

	/* The (last) access time UTC offset
	 * Consists of 1 byte
	 */
	uint8_t access_time_utc_offset;

	/* Unknown (reserved)
	 * Consists of 7 bytes
	 */
	uint8_t unknown4[ 7 ];
};

typedef struct fsfat_directory_entry_exfat_file_entry_name fsfat_directory_entry_exfat_file_entry_name_t;

struct fsfat_directory_entry_exfat_file_entry_name
{
	/* The entry type
	 * Consists of 1 byte
	 */
	uint8_t entry_type;

	/* Unknown
	 * Consists of 1 byte
	 */
	uint8_t unknown1;

	/* The name
	 * Consists of 30 bytes
	 */
	uint8_t name[ 30 ];
};

typedef struct fsfat_directory_entry_exfat_volume_label fsfat_directory_entry_exfat_volume_label_t;

struct fsfat_directory_entry_exfat_volume_label
{
	/* The entry type
	 * Consists of 1 byte
	 */
	uint8_t entry_type;

	/* The name size
	 * Consists of 1 byte
	 */
	uint8_t name_size;

	/* The name
	 * Consists of 22 bytes
	 */
	uint8_t name[ 22 ];

	/* Unknown (reserved)
	 * Consists of 8 bytes
	 */
	uint8_t unknown1[ 8 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSFAT_DIRECTORY_ENTRY_H ) */

