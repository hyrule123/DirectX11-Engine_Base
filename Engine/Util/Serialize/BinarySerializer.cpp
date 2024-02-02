#include "BinarySerializer.h"

#include "../../define_Macro.h"


namespace ehw
{
	BinarySerializer::BinarySerializer()
		: m_data()
		, m_readOffset()
		, m_writeOffset()
	{
	}

	BinarySerializer::~BinarySerializer()
	{
	}

	eResult BinarySerializer::SaveFile(std::filesystem::path const& _savePath)
	{
		auto openMode = std::ios::binary;
		std::ofstream openFile(_savePath, openMode);

		return SaveFile(openFile);
	}


	eResult BinarySerializer::LoadFile(std::filesystem::path const& _loadPath)
	{
		auto openMode = std::ios::binary;
		std::ifstream openFile(_loadPath, openMode);

		return LoadFile(openFile);
	}


	eResult BinarySerializer::SaveFile(std::ofstream& _file)
	{
		if (false == _file.is_open())
		{
			ERROR_MESSAGE("파일 열기 실패");
			return eResult::Fail_Open;
		}
		else if (0 == (std::ios::binary & _file.flags()))
		{
			ERROR_MESSAGE("Binary 모드로 열린 파일이 아닙니다.");
			return eResult::Fail_InValid;
		}

		_file.write(reinterpret_cast<char*>(m_data.data()), m_data.size());

		return eResult::Success;
	}

	eResult BinarySerializer::LoadFile(std::ifstream& _file)
	{
		if (false == _file.is_open())
		{
			ERROR_MESSAGE("파일 열기 실패");
			return eResult::Fail_Open;
		}
		else if (0 == (std::ios::binary & _file.flags()))
		{
			ERROR_MESSAGE("Binary 모드로 열린 파일이 아닙니다.");
			return eResult::Fail_InValid;
		}

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

	void BinarySerializer::Write(const unsigned char* _pSrc, size_t _size)
	{
		if (nullptr == _pSrc || 0 == _size)
		{
			return;
		}

		size_t reqSize = m_writeOffset + _size;
		if (m_data.size() < reqSize)
		{
			if (m_data.capacity() < reqSize)
			{
				m_data.reserve(reqSize * 2);
			}

			m_data.resize(reqSize);
		}

		unsigned char* dest = m_data.data() + m_writeOffset;
		memcpy_s(dest, m_data.size() - m_writeOffset, _pSrc, _size);
		m_writeOffset = reqSize;
	}

	size_t BinarySerializer::Read(unsigned char* _pDest, size_t _size) const
	{
		if (nullptr == _pDest || 0 == _size)
		{
			return 0;
		}
		//이미 끝까지 읽은 경우
		else if (m_data.size() <= m_readOffset)
		{
			return 0;
		}
		//읽고자 하는 바이트 수가 최대 사이즈를 넘어설 경우 최대 사이즈까지만 읽음
		else if (m_data.size() < m_readOffset + _size)
		{
			_size = m_data.size() - m_readOffset;
		}

		unsigned char* src = m_data.data() + m_readOffset;
		memcpy_s(_pDest, _size, src, _size);
		m_readOffset += _size;

		return _size;
	}

}
