#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>

#define F_CPU 4000000UL		
#include <util/delay.h>
#include "hx711.h"
	
void HX711_init(uint8_t gain)
{
	PD_SCK_SET_OUTPUT;
	DOUT_SET_INPUT;

	HX711_set_gain(gain);
}

bool HX711_is_ready()
{
	return (DOUT_INPUT & (1 << DOUT_PIN)) == 0;
}

void HX711_set_gain(uint8_t gain)
{
	switch (gain)
	{
		case 128:		// channel A, gain factor 128
		GAIN = 1;
		break;
		case 64:		// channel A, gain factor 64
		GAIN = 3;
		break;
		case 32:		// channel B, gain factor 32
		GAIN = 2;
		break;
	}

	PD_SCK_SET_LOW;
	HX711_read();
}

int32_t HX711_read()
{
	// wait for the chip to become ready
	while (!HX711_is_ready());

	uint32_t value = 0;
	uint8_t data[3] = { 0 };
	uint8_t filler = 0x00;

	// pulse the clock pin 24 times to read the data
	data[2] = shiftIn();
	data[1] = shiftIn();
	data[0] = shiftIn();

	// set the channel and the gain factor for the next reading using the clock pin
	for (uint8_t i = 0; i < GAIN; i++)
	{
		PD_SCK_SET_HIGH;
		PD_SCK_SET_LOW;
	}

	// Datasheet indicates the value is returned as a two's complement value
	// Flip all the bits

	// Replicate the most significant bit to pad out a 32-bit signed integer
	if ( data[2] & 0x80 )
	{
		filler = 0xFF;
	} else if ((0x7F == data[2]) && (0xFF == data[1]) && (0xFF == data[0]))
	{
		filler = 0xFF;
	} else
	{
		filler = 0x00;
	}

	// Construct a 32-bit signed integer
	value = ( (uint32_t)(filler) << 24
	| (uint32_t)(data[2]) << 16
	| (uint32_t)(data[1]) << 8
	| (uint32_t)(data[0]) );

	// ... and add 1
	return (int32_t)(++value);
}

int32_t HX711_read_average(uint8_t times)
{
	int32_t sum = 0;
	for (uint8_t i = 0; i < times; i++)
	{
		sum += HX711_read();
		// TODO: See if yield will work | yield();
	}
	return sum / times;
}

double HX711_get_value(uint8_t times)
{
	return HX711_read_average(times) - OFFSET;
}

float HX711_get_units(uint8_t times)
{
	return HX711_get_value(times) / SCALE;
}

void HX711_tare(uint8_t times)
{
	double sum = HX711_read_average(times);
	HX711_set_offset(sum);
}

void HX711_set_scale(float scale)
{
	SCALE = scale;
}

float HX711_get_scale()
{
	return SCALE;
}

void HX711_set_offset(int32_t offset)
{
	OFFSET = offset;
}

int32_t HX711_get_offset()
{
	return OFFSET;
}

void HX711_power_down()
{
	PD_SCK_SET_LOW;
	PD_SCK_SET_HIGH;
	_delay_us(70);
}

void HX711_power_up()
{
	PD_SCK_SET_LOW;
}

uint8_t shiftIn(void)
{
	uint8_t value = 0;

	for (uint8_t i = 0; i < 8; ++i)
	{
		PD_SCK_SET_HIGH;
		value |= DOUT_READ << (7 - i);
		PD_SCK_SET_LOW;
		
		PD_SCK_SET_HIGH;
		value >>= 1;
		if (DOUT_READ)
		{
			value |= 0x80;
		}
		PD_SCK_SET_LOW;
	}
	return value;
}