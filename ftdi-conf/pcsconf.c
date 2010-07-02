/* pcsconf: Configure an FT232RL to behave as a PC-SRIC adapter.
   Copyright (C) 2010 Robert Spanton
   <rspanton@zepler.net>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.*/
#include <stdio.h>
#include <ftdi.h>
#include <stdint.h>

int main( int argc, char** argv )
{
	int ret;
	uint8_t eeprom[128];
	struct ftdi_context fc;
	struct ftdi_eeprom eeset;
	char* PRODUCT = "USB to SRIC adapter";
	char* MANUFACTURER = "Student Robotics";
	/* 500mA please. (max current is 2 * value mA) */
	int POWER = 250;

	if( ftdi_init( &fc ) < 0 ) {
		fprintf( stderr, "Failed to init ftdi context\n" );
		return 1;
	}

	ret = ftdi_usb_open( &fc, 0x0403, 0x6001 );
	if( ret < 0 ) {
		fprintf( stderr, "Unable to open ftdi device: %d (%s)\n", ret, ftdi_get_error_string(&fc));
		return 1;
	}

	ret = ftdi_read_eeprom( &fc, (unsigned char*)&eeprom );
	if( ret < 0 ) {
		fprintf( stderr, "Unable to read ftdi eeprom: %d (%s)\n", ret, ftdi_get_error_string(&fc));
		return 1;
	}

	if( ftdi_eeprom_decode( &eeset, eeprom, 128 ) < 0 ) {
		fprintf( stderr, "Failed to decode FTDI EEPROM :(\n" );
		return 1;
	}

	if( strcmp( PRODUCT, eeset.product ) != 0
	    || strcmp( MANUFACTURER, eeset.manufacturer ) != 0
	    || eeset.max_power != POWER )
		printf( "INFO: Device not currently configured for PC-SRIC.\n" );
	else
		printf( "INFO: Device already configured for PC-SRIC (Reconfiguring anyway).\n" );

	/* We have to tell it how large the EEPROM is :-O */
	eeset.size = 128;

	eeset.product = PRODUCT;
	eeset.manufacturer = MANUFACTURER;
	eeset.max_power = POWER;

	ret = ftdi_eeprom_build( &eeset, eeprom );
	if( ret < 0 ) {
		fprintf( stderr, "Unable to build eeprom: %d (%s)\n", ret, ftdi_get_error_string(&fc) );
		return 1;
	}

	ret = ftdi_write_eeprom( &fc, eeprom );
	if( ret < 0 ) {
		fprintf( stderr, "Unable to write eeprom: %d (%s)\n", ret, ftdi_get_error_string(&fc) );
		return 1;
	}

	ret = ftdi_usb_close( &fc );
	if( ret < 0 ) {
		fprintf( stderr, "Unable to close ftdi device: %d (%s)\n", ret, ftdi_get_error_string(&fc) );
	}

	ftdi_deinit( &fc );
	return 0;
}
