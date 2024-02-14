#pragma once
#include "../../define_Enum.h"
#include "../type_traits_Ex.h"

namespace ehw
{
	//수동으로 처리해줘야 하는 타입들을 여기 추가할것.
	template <typename T>
	concept BinaryManualHandleTypes =
		type_traits_Ex::StringLike<T> ||
		type_traits_Ex::WStringLike<T>;

	
	template <typename T>
	concept BinaryDefaultTypes =
		!type_traits_Ex::PointerTypes<T> &&
		!BinaryManualHandleTypes<T>;
		
	//만약 특정 타입에 대해 특수화가 필요하다면
	//수동으로 처리해줘야 하는 타입을 BinaryManualHandleTypes에 추가하고
	//아래 컨셉의 템플릿 함수를 특수화해주면 된다.
	template <typename T>
	concept NotBinaryDefaultTypes = 
		!type_traits_Ex::PointerTypes<T> &&
		BinaryManualHandleTypes<T>;


	class BinarySerializer
	{
	public:
		BinarySerializer();
		~BinarySerializer();

		eResult SaveFile(std::ostream& _os) const;
		eResult LoadFile(std::istream& _is);

		void Write(const unsigned char* _pSrc, size_t _size);
		size_t Read(unsigned char* _pDest, size_t _size) const;

		template <BinaryDefaultTypes T>
		inline void operator <<(const T& _data);

		template <BinaryDefaultTypes T>
		inline void operator <<(const std::vector<T>& _data);

		//template <typename CharType>
		inline void operator <<(const std::string_view _data);
		
		template <BinaryDefaultTypes T>
		inline void operator >>(T& _data) const;

		template <BinaryDefaultTypes T>
		inline void operator >>(std::vector<T>& _data) const;

		template <typename CharType>
		inline void operator >>(std::basic_string<CharType>& _data) const;


		size_t GetDataSize() const { return m_data.size(); }
		void ReserveDataSize(size_t _size) { m_data.reserve(_size); }

		size_t GetReadOffset() const { return m_readOffset; }
		inline size_t SetReadOffset(size_t _offset);

		size_t GetWriteOffset() const { return m_writeOffset; }
		inline size_t SetWriteOffset(size_t _offset);

		void Clear() { m_data.clear(); m_readOffset = (size_t)0; m_writeOffset = (size_t)0; }
	private:
		std::vector<unsigned char> m_data;
		size_t m_writeOffset;
		mutable size_t m_readOffset;
	};


	template<BinaryDefaultTypes T>
	inline void BinarySerializer::operator<<(const T& _data)
	{
		Write(reinterpret_cast<const unsigned char*>(&_data), sizeof(T));
	}

	inline void BinarySerializer::operator<<(const std::string_view _data)
	{
		size_t strByteSize = _data.size();
		Write(reinterpret_cast<const unsigned char*>(&strByteSize), sizeof(strByteSize));
		Write(reinterpret_cast<const unsigned char*>(_data.data()), strByteSize);
	}

	//template<typename CharType>
	//inline void BinarySerializer::operator<<(const std::basic_string_view<CharType> _data)
	//{
	//	size_t strByteSize = _data.size() * sizeof(CharType);
	//	Write(&strByteSize, sizeof(strByteSize));
	//	Write(reinterpret_cast<const unsigned char*>(_data.data()), strByteSize);
	//}
	

	template<BinaryDefaultTypes T>
	inline void BinarySerializer::operator<<(const std::vector<T>& _data)
	{
		(*this) << _data.size();
		for (size_t i = 0; i < _data.size(); ++i)
		{
			(*this) << _data[i];
		}
	}

	template<BinaryDefaultTypes T>
	inline void BinarySerializer::operator>>(T& _data) const
	{
		Read(reinterpret_cast<unsigned char*>(&_data), sizeof(T));
	}


	template<BinaryDefaultTypes T>
	inline void BinarySerializer::operator>>(std::vector<T>& _data) const
	{
		size_t size = 0;
		(*this) >> size;
		_data.resize(size);
		for (size_t i = 0; i < _data.size(); ++i)
		{
			(*this) >> _data[i];
		}
	}

	template<typename CharType>
	inline void BinarySerializer::operator>>(std::basic_string<CharType>& _data) const
	{
		size_t bytesToRead{};
		Read(&bytesToRead, sizeof(size_t));
		_data.clear();
		
		_data.resize(bytesToRead / sizeof(CharType));

		Read(reinterpret_cast<unsigned char*>(_data.data()), bytesToRead);
	}


	inline size_t BinarySerializer::SetReadOffset(size_t _offset)
	{
		size_t ret = _offset;

		if (m_data.size() <= ret)
		{
			ret = m_data.size();
		}
		m_readOffset = ret;

		return ret;
	}

	inline size_t BinarySerializer::SetWriteOffset(size_t _offset)
	{
		size_t ret = _offset;

		if (m_data.size() <= ret)
		{
			ret = m_data.size();
		}
		m_writeOffset = ret;

		return ret;
	}


	//Global operator
	std::ostream& operator <<(std::ostream& _os, const BinarySerializer& _ser);
	std::istream& operator >>(std::istream& _is, BinarySerializer& _ser);
}
