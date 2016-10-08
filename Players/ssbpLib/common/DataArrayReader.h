#pragma once
#include "SS5PlayerData.h"
#include "SS5PlayerTypes.h"

namespace ss{

/**
 * DataArrayReader
 * 与えられたバッファを読み進める
 */
class DataArrayReader{
public:
	DataArrayReader(const ss_u16* dataPtr)
		: _dataPtr(dataPtr)
	{}

	ss_u16 readU16() { return *_dataPtr++; }
	ss_s16 readS16() { return static_cast<ss_s16>(*_dataPtr++); }

	unsigned int readU32(){
		unsigned int l = readU16();
		unsigned int u = readU16();
		return static_cast<unsigned int>((u << 16) | l);
	}

	int readS32(){
		return static_cast<int>(readU32());
	}

	float readFloat(){
		union {
			float			f;
			unsigned int	i;
		} c;
		c.i = readU32();
		return c.f;
	}

	void readColor(SSColor4B& color){
		unsigned int raw = readU32();
		color.a = static_cast<unsigned char>(raw >> 24);
		color.r = static_cast<unsigned char>(raw >> 16);
		color.g = static_cast<unsigned char>(raw >> 8);
		color.b = static_cast<unsigned char>(raw);
	}

	ss_offset readOffset(){
		return static_cast<ss_offset>(readS32());
	}

private:
	const ss_u16*	_dataPtr;
};


} //namespace ss
