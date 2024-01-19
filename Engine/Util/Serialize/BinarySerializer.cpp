#include "BinarySerializer.h"

#include "../../define_Macro.h"

BinarySerializer::BinarySerializer()
	: m_data()
	, m_ReadOffset()
	, m_WriteOffset()
{
}

BinarySerializer::~BinarySerializer()
{
}

bool BinarySerializer::SaveFile(std::filesystem::path const& _savePath)
{
	auto openMode = std::ios::binary;
	std::ofstream openFile(_savePath, openMode);

	return SaveFile(openFile);
}


bool BinarySerializer::LoadFile(std::filesystem::path const& _loadPath)
{
	auto openMode = std::ios::binary;
	std::ifstream openFile(_loadPath, openMode);

	return LoadFile(openFile);
}


bool BinarySerializer::SaveFile(std::ofstream& _saveFile)
{
	if (false == _saveFile.is_open())
	{
		ERROR_MESSAGE("파일 열기 실패");
		return false;
	}
	else if (0 == (std::ios::binary & _saveFile.flags()))
	{
		ERROR_MESSAGE("Binary 모드로 열린 파일이 아닙니다.");
		return false;
	}

	_saveFile.write(reinterpret_cast<char*>(m_data.data()), m_data.size());

	return true;
}

bool BinarySerializer::LoadFile(std::ifstream& _loadFile)
{
	if (false == _loadFile.is_open())
	{
		ERROR_MESSAGE("파일 열기 실패");
		return false;
	}
	else if (0 == (std::ios::binary & _loadFile.flags()))
	{
		ERROR_MESSAGE("Binary 모드로 열린 파일이 아닙니다.");
		return false;
	}

	//원래 위치 받아둔다
	std::streampos origPos = _loadFile.tellg();

	//파일 사이즈 확인 후 resize
	_loadFile.seekg(0, std::ios::end);
	std::streampos fileSize = _loadFile.tellg() - origPos;

	size_t origDataSize = m_data.size();
	m_data.resize(origDataSize + fileSize);

	//원래 위치로 복귀
	_loadFile.seekg(origPos);

	char* readPos = reinterpret_cast<char*>(m_data.data() + origDataSize);
	_loadFile.read(readPos, fileSize);

	return true;
}

void BinarySerializer::Write(const unsigned char* _pSrc, size_t _size)
{
	if (nullptr == _pSrc || 0 == _size)
	{
		return;
	}

	size_t reqSize = m_WriteOffset + _size;
	if (m_data.size() < reqSize)
	{
		if (m_data.capacity() < reqSize)
		{
			m_data.reserve(reqSize * 2);
		}

		m_data.resize(reqSize);
	}

	unsigned char* dest = m_data.data() + m_WriteOffset;
	memcpy_s(dest, m_data.size() - m_WriteOffset, _pSrc, _size);
	m_WriteOffset = reqSize;
}

size_t BinarySerializer::Read(unsigned char* _pDest, size_t _size)
{
	if (nullptr == _pDest || 0 == _size)
	{
		return 0;
	}
	//이미 끝까지 읽은 경우
	else if (m_data.size() <= m_ReadOffset)
	{
		return 0;
	}
	//읽고자 하는 바이트 수가 최대 사이즈를 넘어설 경우 최대 사이즈까지만 읽음
	else if (m_data.size() < m_ReadOffset + _size)
	{
		_size = m_data.size() - m_ReadOffset;
	}

	unsigned char* src = m_data.data() + m_ReadOffset;
	memcpy_s(_pDest, _size, src, _size);
	m_ReadOffset += _size;

	return _size;
}
