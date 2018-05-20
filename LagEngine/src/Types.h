#pragma once

namespace Lag
{
	//Integers
	//1 byte
	typedef unsigned char byte;
	typedef unsigned char uint8;
	typedef char int8;

	//2 bytes
	typedef unsigned short int uint16;
	typedef short int int16;

	//4 bytes
	typedef unsigned int uint32;
	typedef int int32;

	//8 bytes
	typedef unsigned long long int uint64;
	typedef long long int int64;

	//Floating point
	typedef float f32;
	typedef double f64;


	//Max values
	constexpr uint8 MAX_UINT8 = 255;
	constexpr uint8 MAX_INT8 = 127;

	constexpr uint16 MAX_UINT16 = 65535;
	constexpr uint16 MAX_INT16 = 32767;

	constexpr uint32 MAX_UINT32 = 4294967295;
	constexpr uint32 MAX_INT32 = 2147483647;

	constexpr uint64 MAX_UINT64 = 18446744073709551615;
	constexpr uint64 MAX_INT64 = 9223372036854775807;
}