#pragma once


#include "type_traits_Ex.h"
#include "../CommonType.h"

#include <filesystem>
#include <iostream>
#include <cstdint>
#include <vector>
#include <concepts>
#include <string>



template <typename T>
struct Remove_Const_Reference
{
	using type = std::remove_const_t<std::remove_reference_t<T>>;
};
template <typename T>
using Remove_Const_Reference_t = Remove_Const_Reference<T>::type;

template <typename T>
concept PointerTypes = requires(T a) { *a; };

template <typename T>
concept AllowedTypes = 
//* 연산자가 있을경우(포인터일경우) 저장 불가
!PointerTypes<T> &&
//string이 아니여야 함
!(std::is_same_v<Remove_Const_Reference_t<T>, std::string> ||
	std::is_same_v<Remove_Const_Reference_t<T>, std::wstring>);

template <typename T>
concept StringTypes =
std::is_same_v<T, std::string> ||
std::is_same_v<T, std::wstring>;


class Serializer
{
protected:
	Serializer() {};
	virtual ~Serializer() {};
public:
	virtual bool SaveFile(std::filesystem::path const& _fullPath) = 0;
	virtual bool LoadFile(std::filesystem::path const& _fullPath) = 0;
};

class Serializer_Binary : public Serializer
{
public:
	Serializer_Binary();
	virtual ~Serializer_Binary();

	virtual bool SaveFile(std::filesystem::path const& _fullPath) final;
	virtual bool LoadFile(std::filesystem::path const& _fullPath) final;

	bool SaveFile(std::ofstream& _saveFile);
	bool LoadFile(std::ifstream& _loadFile);

	void Write(const unsigned char* _pSrc, size_t _size);
	size_t Read(unsigned char* _pDest, size_t _size);

	template <AllowedTypes T>
	inline void operator <<(const T& _data);
	template <StringTypes T>
	inline void operator <<(const T& _data);
	template <AllowedTypes T>
	inline void operator <<(const std::vector<T>& _data);


	template <AllowedTypes T>
	inline void operator >>(T& _data);
	template <StringTypes T>
	inline void operator >>(T& _data);
	template <AllowedTypes T>
	inline void operator >>(std::vector<T>& _data);


	size_t GetDataSize() const { return m_data.size(); }
	void ReserveDataSize(size_t _size) { m_data.reserve(_size); }

	size_t GetReadOffset() const { return m_ReadOffset; }
	inline size_t SetReadOffset(size_t _offset);

	size_t GetWriteOffset() const { return m_WriteOffset; }
	inline size_t SetWriteOffset(size_t _offset);

	void Clear() { m_data.clear(); m_ReadOffset = (size_t)0; m_WriteOffset = (size_t)0; }
private:
	std::vector<unsigned char> m_data;
	size_t m_ReadOffset;
	size_t m_WriteOffset;
};



template<AllowedTypes T >
inline void Serializer_Binary::operator<<(const T& _data)
{
	Write(reinterpret_cast<const unsigned char*>(&_data), sizeof(T));
};

template<StringTypes T>
inline void Serializer_Binary::operator<<(const T& _data)
{
	const unsigned char* src = reinterpret_cast<const unsigned char*>(_data.data());
	size_t BytesToRead = sizeof(T::value_type) * _data.size();

	(*this) << BytesToRead;
	Write(src, BytesToRead);
}

template<AllowedTypes T>
inline void Serializer_Binary::operator<<(const std::vector<T>& _data)
{
	(*this) << _data.size();
	for (size_t i = 0; i < _data.size(); ++i)
	{
		(*this) << _data[i];
	}
}



template<AllowedTypes T>
inline void Serializer_Binary::operator>>(T& _data)
{
	Read(reinterpret_cast<unsigned char*>(&_data), sizeof(T));
}

template<StringTypes T>
inline void Serializer_Binary::operator>>(T& _data)
{
	size_t BytesToRead{};
	(*this) >> BytesToRead;
	_data.clear();
	_data.resize(BytesToRead);

	unsigned char* dest = reinterpret_cast<unsigned char*>(_data.data());
	Read(dest, BytesToRead);
}

template<AllowedTypes T>
inline void Serializer_Binary::operator>>(std::vector<T>& _data)
{
	size_t size = 0;
	(*this) >> size;
	_data.resize(size);
	for (size_t i = 0; i < _data.size(); ++i)
	{
		(*this) >> _data[i];
	}
}


inline size_t Serializer_Binary::SetReadOffset(size_t _offset)
{
	size_t ret = _offset;

	if (m_data.size() <= ret)
	{
		ret = m_data.size();
	}
	m_ReadOffset = ret;

	return ret;
}

inline size_t Serializer_Binary::SetWriteOffset(size_t _offset)
{
	size_t ret = _offset;

	if (m_data.size() <= ret)
	{
		ret = m_data.size();
	}
	m_WriteOffset = ret;

	return ret;
}


class Serializer_Json : public Serializer
{
public:
	Serializer_Json() {};
	virtual ~Serializer_Json() {};

	virtual bool SaveFile(std::filesystem::path const& _fullPath) override { return false; }
	virtual bool LoadFile(std::filesystem::path const& _fullPath) override { return false; }
};


template <typename T> requires std::is_base_of_v<Serializer, T>
class Serializable
{
protected:
	Serializable() {};
	virtual ~Serializable() {};

public:
	inline bool SaveFile(std::filesystem::path const& _fullPath);
	inline bool LoadFile(std::filesystem::path const& _fullPath);

	//필요한 Seri
	virtual bool Serialize(T* _ser) = 0;
	virtual bool DeSerialize(T* _ser) = 0;
};


template<typename T>
inline bool Serializable<T>::SaveFile(std::filesystem::path const& _fullPath)
{
	T ser{};

	bool result = Serialize(&ser);
	if (false == result)
	{
		return false;
	}

	return ser.SaveFile(_fullPath);
}

template<typename T>
inline bool Serializable<T>::LoadFile(std::filesystem::path const& _fullPath)
{
	T ser{};
	bool result = ser.LoadFile(_fullPath);
	
	if (false == result)
	{
		return false;
	}
	
	return ser.DeSerialize();
}
