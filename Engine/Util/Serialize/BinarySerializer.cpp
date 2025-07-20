
#include "BinarySerializer.h"

#include <Engine/define_Macro.h>


namespace core
{
	BinarySerializer::BinarySerializer()
		: m_data()
		, m_read_offset()
		, m_write_offset()
	{
	}

	BinarySerializer::~BinarySerializer()
	{
	}


	eResult BinarySerializer::save_file(std::ostream& _file) const
	{
		_file.write(reinterpret_cast<const char*>(m_data.data()), m_data.size());
		return eResult::Success;
	}

	eResult BinarySerializer::load_file(std::istream& _file)
	{
		//원래 위치 받아둔다
		std::streampos origPos = _file.tellg();

		//파일 사이즈 확인 후 resize
		_file.seekg(0, std::ios::end);
		std::streampos fileSize = _file.tellg() - origPos;
		 
		size_t origDataSize = m_data.size();
		m_data.resize(origDataSize + fileSize);

		//원래 위치로 복귀
		_file.seekg(origPos);

		char* readPos = reinterpret_cast<char*>(m_data.data() + origDataSize);
		_file.read(readPos, fileSize);

		return eResult::Success;
	}

	void BinarySerializer::write(const unsigned char* _pSrc, size_t _size)
	{
		if (nullptr == _pSrc || 0 == _size)
		{
			return;
		}

		size_t reqSize = m_write_offset + _size;
		if (m_data.size() < reqSize)
		{
			if (m_data.capacity() < reqSize)
			{
				m_data.reserve(reqSize * 2);
			}

			m_data.resize(reqSize);
		}

		unsigned char* dest = m_data.data() + m_write_offset;
		memcpy_s(dest, m_data.size() - m_write_offset, _pSrc, _size);
		m_write_offset = reqSize;
	}

	size_t BinarySerializer::read(unsigned char* _pDest, size_t _size) const
	{
		if (nullptr == _pDest || 0 == _size)
		{
			return 0;
		}
		//이미 끝까지 읽은 경우
		else if (m_data.size() <= m_read_offset)
		{
			return 0;
		}
		//읽고자 하는 바이트 수가 최대 사이즈를 넘어설 경우 최대 사이즈까지만 읽음
		else if (m_data.size() < m_read_offset + _size)
		{
			_size = m_data.size() - m_read_offset;
		}

		const unsigned char* src = m_data.data() + m_read_offset;
		memcpy_s(_pDest, _size, src, _size);
		m_read_offset += _size;

		return _size;
	}

	std::ostream& operator<<(std::ostream& _os, const BinarySerializer& _ser)
	{
		if (eResult_fail(_ser.save_file(_os)))
		{
			ERROR_MESSAGE("저장 실패.");
		}
		return _os;
	}

	std::istream& operator>>(std::istream& _is, BinarySerializer& _ser)
	{
		if (eResult_fail(_ser.load_file(_is)))
		{
			ERROR_MESSAGE("불러오기 실패.");
		}
		return _is;
	}

}
