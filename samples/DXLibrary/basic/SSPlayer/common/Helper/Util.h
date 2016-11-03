#pragma once
#include <string>
#include <vector>
#include <stdarg.h>

/**
 * 定数
 */

#define __SSPI__	(3.14159265358979323846f)
#define __SS2PI__	(__SSPI__ * 2)

#define SS_SAFE_DELETE(p)			do { if(p) { delete (p); (p) = 0; } } while(0)
#define SS_SAFE_DELETE_ARRAY(p)		do { if(p) { delete[] (p); (p) = 0; } } while(0)

#ifdef _DEBUG
	#define SSLOG(...)       do {} while (0)
	#define SS_ASSERT(cond)    assert(cond)
	#define SS_ASSERT2(cond, msg) SS_ASSERT(cond)
	#define SSLOGERROR(format,...)  do {} while (0)
#else
	#define SSLOG(...)       do {} while (0)
	#define SS_ASSERT(cond)
	#define SS_ASSERT2(cond, msg) ((void)(cond))
	#define SSLOGERROR(format,...)  do {} while (0)
#endif



namespace ss{

	
// printf 形式のフォーマット
#ifndef va_copy
#    define va_copy(dest, src) ((dest) = (src))
#endif
static std::string Format(const char* format, ...){

	static std::vector<char> tmp(1000);

	va_list args, source;
	va_start(args, format);
	va_copy(source, args);

	while (1)
	{
		va_copy(args, source);
		//Windows
		if (_vsnprintf_s(&tmp[0], tmp.size(), _TRUNCATE, format, args) == -1)
		{
			tmp.resize(tmp.size() * 2);
		}
		else
		{
			break;
		}
	}
	tmp.push_back('\0');
	std::string ret = &(tmp[0]);
	va_end(args);
	return ret;
}



//配列のサイズを返す
template<class TYPE, size_t N>
size_t lengthof(const TYPE(&ar)[N]){
	return N;
}

//[minVal:maxVal]の範囲にする
template<class T>
T clamp(T val, T minVal, T maxVal){
	assert(minVal <= maxVal);
	return std::min(std::max(val, minVal), maxVal);
}

//[minVal:maxVal)の範囲でループさせる
template<class T>
T wrap(T val, T minVal, T maxVal){
	assert(minVal < maxVal);
	int n = (val - minVal) % (maxVal - minVal);
	return (n >= 0) ? (n + minVal) : (n + maxVal);
}

template<class T>
T fwrap(T val, T minVal, T maxVal){
	assert(minVal < maxVal);
	double n = fmod(val - minVal, maxVal - minVal);
	return (n >= 0) ? (n + minVal) : (n + maxVal);
}


} //namespace sss
