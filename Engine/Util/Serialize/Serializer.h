#pragma once

#include "../type_traits_Ex.h"

#include <filesystem>
#include <iostream>
#include <cstdint>
#include <vector>


class Serializer
{
protected:
	Serializer() {};
	virtual ~Serializer() {};
public:
	virtual bool SaveFile(std::filesystem::path const& _fullPath) = 0;
	virtual bool LoadFile(std::filesystem::path const& _fullPath) = 0;
};


//다중상속 용도
template <typename T> requires std::is_base_of_v<Serializer, T>
class Serializable
{
protected:
	Serializable() {};
	virtual ~Serializable() {};

public:
	inline bool SaveFile(std::filesystem::path const& _fullPath);
	inline bool LoadFile(std::filesystem::path const& _fullPath);

	virtual bool Serialize(T& _ser) = 0;
	virtual bool DeSerialize(T& _ser) = 0;
};


template<typename T> requires std::is_base_of_v<Serializer, T>
inline bool Serializable<T>::SaveFile(std::filesystem::path const& _fullPath)
{
	T ser{};

	bool result = Serialize(&ser);
	if (eResultFail(result))
	{
		return false;
	}

	return ser.SaveFile(_fullPath);
}

template<typename T> requires std::is_base_of_v<Serializer, T>
inline bool Serializable<T>::LoadFile(std::filesystem::path const& _fullPath)
{
	T ser{};
	bool result = ser.LoadFile(_fullPath);

	if (eResultFail(result))
	{
		return false;
	}

	return ser.DeSerialize();
}



