#pragma once

namespace core
{
	template <typename T, typename Size = unsigned char>
	class EnumFlags
	{
	public:
		explicit EnumFlags(void);
		EnumFlags(T e);
		EnumFlags(const EnumFlags<T, Size>& f);

		bool operator==(T e) const;
		bool operator==(const EnumFlags<T, Size>& f) const;
		bool operator==(bool b) const;
		bool operator!=(T e) const;
		bool operator!=(const EnumFlags<T, Size>& f) const;

		EnumFlags<T, Size>& operator=(const EnumFlags<T, Size>& f);
		EnumFlags<T, Size>& operator=(T e);

		EnumFlags<T, Size>& operator|=(T e);
		EnumFlags<T, Size>& operator|=(const EnumFlags<T, Size>& f);
		EnumFlags<T, Size>	operator|(T e) const;
		EnumFlags<T, Size>	operator|(const EnumFlags<T, Size>& f) const;

		EnumFlags<T, Size>& operator&=(T e);
		EnumFlags<T, Size>& operator&=(const EnumFlags<T, Size>& f);
		EnumFlags<T, Size>	operator&(T e) const;
		EnumFlags<T, Size>	operator&(const EnumFlags<T, Size>& f) const;

		EnumFlags<T, Size>& operator^=(T e);
		EnumFlags<T, Size>& operator^=(const EnumFlags<T, Size>& f);
		EnumFlags<T, Size>	operator^(T e) const;
		EnumFlags<T, Size>	operator^(const EnumFlags<T, Size>& f) const;

		EnumFlags<T, Size> operator~(void) const;

		operator bool(void) const;
		operator unsigned char(void) const;
		operator unsigned short(void) const;
		operator unsigned long(void) const;

		void				clear(T e);
		void				raise(T e);
		bool				isSet(T e) const;
		EnumFlags<T, Size>& setAll(T e);
		T					Get() const { return static_cast<T>(_bits); }

	public:
		friend EnumFlags<T, Size> operator&(T a, EnumFlags<T, Size>& b)
		{
			EnumFlags<T, Size> out{};
			out._bits = a & b._bits;
			return out;
		}

	private:
		Size _bits;
	};

	template <typename T, typename Size>
	EnumFlags<T, Size>::EnumFlags(void) { _bits = 0; }

	template <typename T, typename Size>
	EnumFlags<T, Size>::EnumFlags(T e) { _bits = static_cast<Size>(e); }

	template <typename T, typename Size>
	EnumFlags<T, Size>::EnumFlags(const EnumFlags<T, Size>& other) { _bits = other._bits; }

	template <typename T, typename Size>
	bool EnumFlags<T, Size>::operator==(T e) const { return _bits == static_cast<Size>(e); }

	template <typename T, typename Size>
	bool EnumFlags<T, Size>::operator==(const EnumFlags<T, Size>& other) const { return _bits == other._bits; }

	template <typename T, typename Size>
	bool EnumFlags<T, Size>::operator==(bool b) const { return bool(*this) == b; }

	template <typename T, typename Size>
	bool EnumFlags<T, Size>::operator!=(T e) const { return _bits != static_cast<Size>(e); }

	template <typename T, typename Size>
	bool EnumFlags<T, Size>::operator!=(const EnumFlags<T, Size>& other) const { return _bits != other._bits; }

	template <typename T, typename Size>
	EnumFlags<T, Size>& EnumFlags<T, Size>::operator=(T e)
	{
		_bits = static_cast<Size>(e);
		return *this;
	}

	template <typename T, typename Size>
	EnumFlags<T, Size>& EnumFlags<T, Size>::operator=(const EnumFlags<T, Size>& f)
	{
		_bits = f._bits;
		return *this;
	}

	template <typename T, typename Size>
	EnumFlags<T, Size>& EnumFlags<T, Size>::operator|=(T e)
	{
		_bits |= static_cast<Size>(e);
		return *this;
	}

	template <typename T, typename Size>
	EnumFlags<T, Size>& EnumFlags<T, Size>::operator|=(const EnumFlags<T, Size>& f)
	{
		_bits |= f._bits;
		return *this;
	}

	template <typename T, typename Size>
	EnumFlags<T, Size> EnumFlags<T, Size>::operator|(T e) const
	{
		EnumFlags<T, Size> out(*this);
		out |= e;
		return out;
	}

	template <typename T, typename Size>
	EnumFlags<T, Size> EnumFlags<T, Size>::operator|(const EnumFlags<T, Size>& f) const
	{
		EnumFlags<T, Size> out(*this);
		out |= f;
		return out;
	}

	template <typename T, typename Size>
	EnumFlags<T, Size>& EnumFlags<T, Size>::operator&=(T e)
	{
		_bits &= static_cast<Size>(e);
		return *this;
	}

	template <typename T, typename Size>
	EnumFlags<T, Size>& EnumFlags<T, Size>::operator&=(const EnumFlags<T, Size>& f)
	{
		_bits &= f._bits;
		return *this;
	}

	template <typename T, typename Size>
	EnumFlags<T, Size> EnumFlags<T, Size>::operator&(T e) const
	{
		EnumFlags<T, Size> out = EnumFlags<T, Size>{ *this };
		out._bits &= static_cast<Size>(e);
		return EnumFlags<T, Size>{out};
	}

	template <typename T, typename Size>
	EnumFlags<T, Size> EnumFlags<T, Size>::operator&(const EnumFlags<T, Size>& f) const
	{
		EnumFlags<T, Size> out = *this;
		out._bits &= f._bits;
		return out;
	}

	template <typename T, typename Size>
	EnumFlags<T, Size>& EnumFlags<T, Size>::operator^=(T e)
	{
		_bits ^= static_cast<Size>(e);
		return *this;
	}

	template <typename T, typename Size>
	EnumFlags<T, Size>& EnumFlags<T, Size>::operator^=(const EnumFlags<T, Size>& f)
	{
		_bits ^= f._bits;
		return *this;
	}

	template <typename T, typename Size>
	EnumFlags<T, Size> EnumFlags<T, Size>::operator^(T e) const
	{
		EnumFlags<T, Size> out = *this;
		out._bits ^= static_cast<Size>(e);
		return out;
	}

	template <typename T, typename Size>
	EnumFlags<T, Size> EnumFlags<T, Size>::operator^(const EnumFlags<T, Size>& f) const
	{
		EnumFlags<T, Size> out = *this;
		out._bits ^= f._bits;
		return out;
	}

	template <typename T, typename Size>
	EnumFlags<T, Size> EnumFlags<T, Size>::operator~(void) const
	{
		EnumFlags<T, Size> out{};
		out._bits = static_cast<Size>(~_bits);
		return EnumFlags<T, Size>{out};
	}

	template <typename T, typename Size>
	EnumFlags<T, Size>::operator bool(void) const { return _bits ? true : false; }

	template <typename T, typename Size>
	EnumFlags<T, Size>::operator unsigned char(void) const { return static_cast<unsigned char>(_bits); }

	template <typename T, typename Size>
	EnumFlags<T, Size>::operator unsigned short(void) const { return static_cast<unsigned short>(_bits); }

	template <typename T, typename Size>
	EnumFlags<T, Size>::operator unsigned long(void) const { return static_cast<unsigned long>(_bits); }

	template <typename T, typename Size>
	void EnumFlags<T, Size>::clear(T e) { _bits &= ~static_cast<Size>(e); }

	template <typename T, typename Size>
	void EnumFlags<T, Size>::raise(T e) { _bits |= static_cast<Size>(e); }

	template <typename T, typename Size>
	bool EnumFlags<T, Size>::isSet(T e) const { return (_bits & static_cast<Size>(e)) == static_cast<Size>(e); }

	template <typename T, typename Size>
	EnumFlags<T, Size>& EnumFlags<T, Size>::setAll(T e)
	{
		_bits = static_cast<Size>(e);
		return *this;
	}
}
