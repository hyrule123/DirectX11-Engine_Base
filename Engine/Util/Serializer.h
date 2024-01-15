#pragma once


#include "type_traits_Ex.h"
#include "../CommonType.h"

#include <filesystem>
#include <iostream>
#include <cstdint>
#include <vector>
#include <concepts>
#include <string>


class Serializable
{
public:
	virtual bool Write() = 0;
	virtual bool Read() = 0;
};

template <typename T>
struct Remove_Const_Reference
{
	using type = std::remove_const_t<std::remove_reference_t<T>>;
};
template <typename T>
using Remove_Const_Reference_t = Remove_Const_Reference<T>::type;

template <typename T>
concept AllowedTypes = 
//* 연산자가 있을경우(포인터일경우) 저장 불가
!requires (T a) { *a; } &&
//string이 아니여야 함
!(std::is_same_v<Remove_Const_Reference_t<T>, std::string> ||
	std::is_same_v<Remove_Const_Reference_t<T>, std::wstring>);

template <typename T>
concept StringTypes =
std::is_same_v<T, std::string> ||
std::is_same_v<T, std::wstring>;

template <typename T>
concept PointerTypes = requires(T a)
{
	a->GetStrKey();
};



class BinarySerializer
{
public:
	BinarySerializer();
	~BinarySerializer();

	bool Save(std::filesystem::path const& _savePath);
	bool Save(std::ofstream& _saveFile);

	bool Load(std::filesystem::path const& _loadPath);
	bool Load(std::ifstream& _loadFile);

	void Write(const unsigned char* _pSrc, size_t _size);
	size_t Read(unsigned char* _pDest, size_t _size);

	template <AllowedTypes T>
	void operator<<(const T& _data) 
	{ 
		Write(reinterpret_cast<const unsigned char*>(&_data), sizeof(T)); 
	};

	template <StringTypes T>
	void operator<<(const T& _data);
	template <AllowedTypes T>
	void operator<<(const std::vector<T>& _data);

	template <AllowedTypes T>
	void operator>>(T& _data);
	template <StringTypes T>
	void operator>>(T& _data);

	template <AllowedTypes T>
	void operator>>(std::vector<T>& _data);


	size_t GetDataSize() const { return m_data.size(); }
	void ReserveDataSize(size_t _size) { m_data.reserve(_size); }

	size_t GetReadOffset() const { return m_ReadOffset; }
	inline size_t SetReadOffset(size_t _offset);

	size_t GetWriteOffset() const { return m_WriteOffset; }
	inline size_t SetWriteOffsert(size_t _offset);

	void Clear() { m_data.clear(); m_ReadOffset = (size_t)0; m_WriteOffset = (size_t)0; }
private:
	std::vector<unsigned char> m_data;
	size_t m_ReadOffset;
	size_t m_WriteOffset;
};


template<StringTypes T>
inline void BinarySerializer::operator<<(const T& _data)
{
	const unsigned char* src = reinterpret_cast<const unsigned char*>(_data.data());
	size_t BytesToRead = sizeof(T::value_type) * _data.size();

	(*this) << BytesToRead;
	Write(src, BytesToRead);
}

template<AllowedTypes T>
inline void BinarySerializer::operator<<(const std::vector<T>& _data)
{
	(*this) << _data.size();
	for (size_t i = 0; i < _data.size(); ++i)
	{
		(*this) << _data[i];
	}
}



template<AllowedTypes T>
inline void BinarySerializer::operator>>(T& _data)
{
	Read(reinterpret_cast<unsigned char*>(&_data), sizeof(T));
}

template<StringTypes T>
inline void BinarySerializer::operator>>(T& _data)
{
	size_t BytesToRead{};
	(*this) >> BytesToRead;
	_data.clear();
	_data.resize(BytesToRead);

	unsigned char* dest = reinterpret_cast<unsigned char*>(_data.data());
	Read(dest, BytesToRead);
}

template<AllowedTypes T>
inline void BinarySerializer::operator>>(std::vector<T>& _data)
{
	size_t size = 0;
	(*this) >> size;
	_data.resize(size);
	for (size_t i = 0; i < _data.size(); ++i)
	{
		(*this) >> _data[i];
	}
}


inline size_t BinarySerializer::SetReadOffset(size_t _offset)
{
	size_t ret = _offset;

	if (m_data.size() <= ret)
	{
		ret = m_data.size();
	}
	m_ReadOffset = ret;

	return ret;
}

inline size_t BinarySerializer::SetWriteOffsert(size_t _offset)
{
	size_t ret = _offset;

	if (m_data.size() <= ret)
	{
		ret = m_data.size();
	}
	m_WriteOffset = ret;

	return ret;
}

//#include "../json-cpp/json-forwards.h"
//class JsonSerializer
//{
//
//
//
//};
