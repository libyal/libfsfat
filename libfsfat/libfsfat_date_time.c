/*
 * Date time functions
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
#include <types.h>

#include "libfsfat_libcerror.h"

/* Determines the FAT timestamp of FAT date and time values
 * The timestamp is an unsigned 64-bit integer containing the 10 milli seconds intervals since January 1, 1980
 * Returns 1 if successful or -1 on error
 */
int libfsfat_date_time_get_timestamp(
     uint16_t fat_date,
     uint16_t fat_time,
     uint16_t fat_time_fraction,
     uint16_t fat_time_utc_offset,
     uint64_t *fat_timestamp,
     libcerror_error_t **error )
{
	static char *function       = "libfsfat_date_time_get_timestamp";
	uint64_t safe_fat_timestamp = 0;
	uint16_t year               = 0;
	uint8_t day_of_month        = 0;
	uint8_t hours               = 0;
	uint8_t month               = 0;
	uint8_t minutes             = 0;
	uint8_t seconds             = 0;

	if( fat_timestamp == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid FAT timestamp.",
		 function );

		return( -1 );
	}
	/* The year value is stored in bits 9 - 15 of the date (7 bits)
	 * A year value of 0 represents 1980
	 */
	year = (uint16_t) ( 1980 + ( ( fat_date >> 9 ) & 0x7f ) );

	/* The month value is stored in bits 5 - 8 of the date (4 bits)
	 * A month value of 1 represents January
	 */
	month = (uint8_t) ( ( fat_date >> 5 ) & 0x0f );

	/* The day value is stored in bits 0 - 4 of the date (5 bits)
	 */
	day_of_month = (uint8_t) ( fat_date & 0x1f );

	/* The hours value is stored in bits 11 - 15 of the time (5 bits)
	 */
	hours = (uint8_t) ( ( fat_time >> 11 ) & 0x1f );

	/* The minutes value is stored in bits 5 - 10 of the time (6 bits)
	 */
	minutes = (uint8_t) ( ( fat_time >> 5 ) & 0x3f );

	/* The seconds value is stored in bits 0 - 4 of the time (5 bits)
	 * The seconds are stored as 2 second intervals
	 */
	seconds = (uint8_t) ( fat_time & 0x1f ) * 2;

	safe_fat_timestamp = day_of_month;

	while( month > 0 )
	{
		/* February (2)
		 */
		if( month == 2 )
		{
			if( ( ( ( year % 4 ) == 0 )
			  &&  ( ( year % 100 ) != 0 ) )
			 || ( ( year % 400 ) == 0 ) )
			{
				safe_fat_timestamp += 29;
			}
			else
			{
				safe_fat_timestamp += 28;
			}
		}
		/* April (4), June (6), September (9), November (11)
		 */
		else if( ( month == 4 )
		      || ( month == 6 )
		      || ( month == 9 )
		      || ( month == 11 ) )
		{
			safe_fat_timestamp += 30;
		}
		/* January (1), March (3), May (5), July (7), August (8), October (10), December (12)
		 */
		else if( ( month == 1 )
		      || ( month == 3 )
		      || ( month == 5 )
		      || ( month == 7 )
		      || ( month == 8 )
		      || ( month == 10 )
		      || ( month == 12 ) )
		{
			safe_fat_timestamp += 31;
		}
		month--;
	}
	while( year > 1980 )
	{
		if( ( ( ( year % 4 ) == 0 )
		  &&  ( ( year % 100 ) != 0 ) )
		 || ( ( year % 400 ) == 0 ) )
		{
			safe_fat_timestamp += 366;
		}
		else
		{
			safe_fat_timestamp += 365;
		}
		year--;
	}
	safe_fat_timestamp *= 24;
	safe_fat_timestamp += hours;
	safe_fat_timestamp *= 60;
	safe_fat_timestamp += minutes;

	if( ( fat_time_utc_offset & 0x80 ) != 0 )
	{
		fat_time_utc_offset &= 0x7f;

		if( fat_time_utc_offset < 0x40 )
		{
			safe_fat_timestamp += (uint64_t) fat_time_utc_offset * 15;
		}
		else
		{
			safe_fat_timestamp -= (uint64_t) ( 0x80 - fat_time_utc_offset ) * 15;
		}
	}
	safe_fat_timestamp *= 60;
	safe_fat_timestamp += seconds;
	safe_fat_timestamp *= 100;
	safe_fat_timestamp += fat_time_fraction;

	*fat_timestamp = safe_fat_timestamp;

	return( 1 );
}

