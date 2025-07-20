#pragma once
#include <Engine/define_Enum.h>
#include <Engine/Util/type_traits_Ex.h>

//전방선언 후 CPP에서 include 후 사용할것.

namespace core
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

		eResult save_file(std::ostream& _os) const;
		eResult load_file(std::istream& _is);

		void write(const unsigned char* _pSrc, size_t _size);
		size_t read(unsigned char* _pDest, size_t _size) const;

		//Global operator
		friend std::ostream& operator <<(std::ostream& _os, const BinarySerializer& _ser);
		friend std::istream& operator >>(std::istream& _is, BinarySerializer& _ser);

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

		size_t get_data_size() const { return m_data.size(); }
		void reserve_data_size(size_t _size) { m_data.reserve(_size); }

		size_t get_read_offset() const { return m_read_offset; }
		inline size_t set_read_offset(size_t _offset);

		size_t get_write_offset() const { return m_write_offset; }
		inline size_t set_write_offset(size_t _offset);

		void clear() { m_data.clear(); m_read_offset = (size_t)0; m_write_offset = (size_t)0; }
	private:
		std::vector<unsigned char> m_data;
		size_t m_write_offset;
		mutable size_t m_read_offset;
	};


	template<BinaryDefaultTypes T>
	inline void BinarySerializer::operator<<(const T& _data)
	{
		write(reinterpret_cast<const unsigned char*>(&_data), sizeof(T));
	}

	inline void BinarySerializer::operator<<(const std::string_view _data)
	{
		size_t strByteSize = _data.size();
		write(reinterpret_cast<const unsigned char*>(&strByteSize), sizeof(strByteSize));
		write(reinterpret_cast<const unsigned char*>(_data.data()), strByteSize);
	}

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
		read(reinterpret_cast<unsigned char*>(&_data), sizeof(T));
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
		read(reinterpret_cast<unsigned char*>(&bytesToRead), sizeof(size_t));
		_data.clear();
		
		_data.resize(bytesToRead / sizeof(CharType));

		read(reinterpret_cast<unsigned char*>(_data.data()), bytesToRead);
	}


	inline size_t BinarySerializer::set_read_offset(size_t _offset)
	{
		size_t ret = _offset;

		if (m_data.size() <= ret)
		{
			ret = m_data.size();
		}
		m_read_offset = ret;

		return ret;
	}

	inline size_t BinarySerializer::set_write_offset(size_t _offset)
	{
		size_t ret = _offset;

		if (m_data.size() <= ret)
		{
			ret = m_data.size();
		}
		m_write_offset = ret;

		return ret;
	}



}
