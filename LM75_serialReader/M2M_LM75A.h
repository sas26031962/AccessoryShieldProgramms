//---------------------------------------------------------------------------------------------
//
// Library for the LM75A temperature sensor by NXP and Texas Instruments.
//
// Copyright 2016-2017, M2M Solutions AB
// Written by Jonny Bergdahl, 2016-11-18
//
// Licensed under the MIT license, see the LICENSE.txt file.
//
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef M2M_LM75_h
#define M2M_LM75_h
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

//#define LM75A_DEFAULT_ADDRESS		0x48		// Address is configured with pins A0-A2
#define LM75A_DEFAULT_ADDRESS		0x4f		// Address is configured with pins A0-A2
#define LM75A_REGISTER_TEMP			0			// Temperature register (read-only)
#define LM75A_REGISTER_CONFIG		1			// Configuration register
#define LM75A_REGISTER_THYST		2			// Hysterisis register
#define LM75A_REGISTER_TOS			3			// OS register
#define LM75A_REGISTER_PRODID		7			// Product ID register - Only valid for Texas Instruments

#define LM75_CONF_OS_COMP_INT		1			// OS operation mode selection
#define LM75_CONF_OS_POL			2			// OS polarity selection
#define LM75_CONF_OS_F_QUE			3			// OS fault queue programming

#define LM75A_INVALID_TEMPERATURE	-1000.0f	// Just an arbritary value outside of the sensor limits

enum FaultQueueValue : uint8_t
{
	NUMBER_OF_FAULTS_1 = 0,
	NUMBER_OF_FAULTS_2 = 0b01000,
	NUMBER_OF_FAULTS_4 = 0b10000,
	NUMBER_OF_FAULTS_6 = 0b11000
};

enum OsPolarity : uint8_t
{
	OS_POLARITY_ACTIVELOW = 0,
	OS_POLARITY_ACTIVEHIGH = 0b100
};

enum DeviceMode : uint8_t
{
	DEVICE_MODE_COMPARATOR = 0,
	DEVICE_MODE_INTERRUPT = 0b10
};

class M2M_LM75A 
{
	// Private variables
	uint8_t _i2cAddress;

	// Private functions
	uint8_t read8bitRegister(const uint8_t reg);
	bool read16bitRegister(uint8_t register, uint16_t& response);
	bool write16bitRegister(const uint8_t reg, const uint16_t value);
	bool write8bitRegister(const uint8_t reg, const uint8_t value);

public:
	// Constructors
    M2M_LM75A();
    M2M_LM75A(uint8_t address);

	// Startup/Teardown
	void begin();
	void end();
	
	// Power management
	void shutdown();
	void wakeup();
	bool isShutdown();

	// Temperature functions
    float getTemperature();
	float getTemperatureInFarenheit();

	// Configuration functions
	float getHysterisisTemperature();	
	FaultQueueValue getFaultQueueValue();
	float getOSTripTemperature();
	OsPolarity getOsPolarity();
	DeviceMode getDeviceMode();
	void setHysterisisTemperature(const float temperature);
	void setOsTripTemperature(const float temperature);
	void setFaultQueueValue(const FaultQueueValue value);
	void setOsPolarity(const OsPolarity polarity);
	void setDeviceMode(const DeviceMode deviceMode);	

	// Other
	bool isConnected();
	uint8_t getConfig();
	float getProdId();
};

//---------------------------------------------------------------------------------------------
//
// Library for the LM75A temperature sensor by NXP and Texas Instruments.
//
// Copyright 2016-2017, M2M Solutions AB
// Written by Jonny Bergdahl, 2016-11-18
//
// Licensed under the MIT license, see the LICENSE.txt file.
//
////////////////////////////////////////////////////////////////////////////////////////////////
#include <Wire.h>
#include "M2M_LM75A.h"

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Constructor
//
M2M_LM75A::M2M_LM75A()
{
	_i2cAddress = LM75A_DEFAULT_ADDRESS;
}

M2M_LM75A::M2M_LM75A(uint8_t i2cAddress)
{
	_i2cAddress = i2cAddress;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Startup/Teardown
//
void M2M_LM75A::begin()
{
	Wire.begin();
}
void M2M_LM75A::end()
{
	//Wire.end();
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Power management
//
void M2M_LM75A::shutdown()
{
	uint8_t config = read8bitRegister(LM75A_REGISTER_CONFIG);
	write8bitRegister(LM75A_REGISTER_CONFIG, (config & 0b11111110) | 0b1);
}

void M2M_LM75A::wakeup()
{
	uint8_t config = read8bitRegister(LM75A_REGISTER_CONFIG);
	write8bitRegister(LM75A_REGISTER_CONFIG, config & 0b11111110);
}

bool M2M_LM75A::isShutdown()
{
	return (read8bitRegister(LM75A_REGISTER_CONFIG) & 0b1) == 1;	
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Temperature functions
//
float M2M_LM75A::getTemperature()
{
	uint16_t result;
	if (!read16bitRegister(LM75A_REGISTER_TEMP, result))
	{
		return LM75A_INVALID_TEMPERATURE;
	}
	return (float)result / 256.0f;
}

float M2M_LM75A::getTemperatureInFarenheit()
{
	return getTemperature() * 1.8f + 32.0f;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Configuration functions
//
float M2M_LM75A::getHysterisisTemperature()
{
	uint16_t result;
	if (!read16bitRegister(LM75A_REGISTER_THYST, result))
	{
		return LM75A_INVALID_TEMPERATURE;
	}
	return (float)result / 256.0f;
}

FaultQueueValue M2M_LM75A::getFaultQueueValue()
{
	return (FaultQueueValue)(read8bitRegister(LM75A_REGISTER_CONFIG) & 0b00011000);
}

float M2M_LM75A::getOSTripTemperature()
{
	uint16_t result;
	if (!read16bitRegister(LM75A_REGISTER_TOS, result))
	{
		return LM75A_INVALID_TEMPERATURE;
	}
	return (float)result / 256.0f;
}

OsPolarity M2M_LM75A::getOsPolarity()
{
	return (OsPolarity)(read8bitRegister(LM75A_REGISTER_CONFIG) & 0b100);
}

DeviceMode M2M_LM75A::getDeviceMode()
{
	return (DeviceMode)(read8bitRegister(LM75A_REGISTER_CONFIG) & 0b010);
}

void M2M_LM75A::setHysterisisTemperature(float temperature)
{
	write16bitRegister(LM75A_REGISTER_THYST, temperature * 256);
}

void M2M_LM75A::setOsTripTemperature(float temperature)
{
	write16bitRegister(LM75A_REGISTER_TOS, temperature * 256);
}

void M2M_LM75A::setFaultQueueValue(FaultQueueValue value)
{
	uint8_t config = read8bitRegister(LM75A_REGISTER_CONFIG);
	write8bitRegister(LM75A_REGISTER_CONFIG, (config & 0b11100111) | value);
}

void M2M_LM75A::setOsPolarity(OsPolarity polarity)
{
	uint8_t config = read8bitRegister(LM75A_REGISTER_CONFIG);
	write8bitRegister(LM75A_REGISTER_CONFIG, (config & 0b11111011) | polarity);
}

void M2M_LM75A::setDeviceMode(DeviceMode deviceMode)
{
	uint8_t config = read8bitRegister(LM75A_REGISTER_CONFIG);
	write8bitRegister(LM75A_REGISTER_CONFIG, (config & 0b11111101) | deviceMode);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Other
//

bool M2M_LM75A::isConnected()
{
	uint8_t oldValue = read8bitRegister(LM75A_REGISTER_CONFIG);
	write8bitRegister(LM75A_REGISTER_CONFIG, 0x0f);
	uint8_t newValue = read8bitRegister(LM75A_REGISTER_CONFIG);
	write8bitRegister(LM75A_REGISTER_CONFIG, oldValue);
	return newValue == 0x0f;
}

uint8_t M2M_LM75A::getConfig()
{
	return read8bitRegister(LM75A_REGISTER_CONFIG);
}

float M2M_LM75A::getProdId()
{
	uint8_t value = read8bitRegister(LM75A_REGISTER_PRODID);
	return (float)(value >> 4) + (value & 0x0F) / 10.0f;
} 

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Private functions
//

uint8_t M2M_LM75A::read8bitRegister(const uint8_t reg)
{
	uint8_t result;

	Wire.beginTransmission(_i2cAddress);
	Wire.write(reg);
	Wire.endTransmission();
	Wire.requestFrom(_i2cAddress, (uint8_t)1);
	result = Wire.read();
	return result;
}

bool M2M_LM75A::read16bitRegister(const uint8_t reg, uint16_t& response)
{
	uint8_t result;

	Wire.beginTransmission(_i2cAddress);
	Wire.write(reg);
	result = Wire.endTransmission();
	// result is 0-4 
	if (result != 0)
	{
		return false;
	}

	result = Wire.requestFrom(_i2cAddress, (uint8_t)2);
	if (result != 2)
	{
		return false;
	}
	uint8_t part1 = Wire.read();
	uint8_t part2 = Wire.read();
	
	//response = (Wire.read() << 8) | Wire.read();
	uint16_t temp = part1 << 8 | part2;
	response = part1 << 8 | part2;
	return true;
}

bool M2M_LM75A::write16bitRegister(const uint8_t reg, const uint16_t value)
{
	Wire.beginTransmission(_i2cAddress);
	Wire.write(reg);
	Wire.write((byte)(value >> 8));
	Wire.write((byte)value);
	return Wire.endTransmission() == 0;
}

bool M2M_LM75A::write8bitRegister(const uint8_t reg, const uint8_t value)
{
	Wire.beginTransmission(_i2cAddress);
	Wire.write(reg);
	Wire.write(value);
	return Wire.endTransmission() == 0;
}


#endif
