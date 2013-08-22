
#include "stdafx.h"
#include "file.h"

CFile::CFile(const std::wstring &filename, mode modes)
{
	unsigned int len = filename.length();
	if (len > 0)
	{
		m_filename = new wchar_t[len+1];
		wmemcpy(m_filename, filename.c_str(), len);
		m_filename[len] = L'\0';
	}
	m_modes = modes;
	m_in = NULL;
	m_out = NULL;
}

CFile::~CFile()
{
	if (m_filename)
		delete []m_filename;
	if (m_in)
	{
		m_in->close();
		delete m_in;
	}
	if (m_out)
	{
		m_out->close();
		delete m_out;
	}
}

bool CFile::open()
{
	if (!m_filename) return false;
	if ((m_modes&CFile::READ) != 0 && (m_modes&CFile::WRITE) != 0)
		return false;
	if ((m_modes&CFile::READ) != 0)
	{
		m_in = new std::ifstream();
		m_in->open(m_filename, std::ios::in|std::ios::binary);
		if (m_in->fail())
		{
			m_in->close();
			delete m_in;
			m_in = NULL;
			return false;
		}
	}
	else if ((m_modes&CFile::WRITE) != 0)
	{
		m_out = new std::ofstream();
		m_out->open(m_filename, std::ios::out|std::ios::binary);
		if (m_out->fail())
		{
			m_out->close();
			delete m_out;
			m_out = NULL;
			return false;
		}
	}

	return true;
}

long long CFile::length() const
{
	if (m_in)
	{
		m_in->seekg(0, std::ios::end);
		return m_in->tellg();
	}
	return -1;
}

bool CFile::seek(std::ios::off_type offset, std::ios::seekdir dir) const
{
	if (m_in)
	{
		m_in->seekg(offset, dir);
		return true;
	}
	return false;
}

bool CFile::read(char *dst, unsigned int length) const
{
	if (m_in)
	{
		m_in->read(dst, length);
		return true;
	}
	return false;
}

bool CFile::readAt(char *dst, unsigned int length, unsigned int begin) const
{
	if (m_in)
	{
		m_in->seekg(begin, std::ios::beg);
		m_in->read(dst, length);
		return true;
	}
	return false;
}

long long CFile::getPosition() const
{
	if (m_in)
	{
		return m_in->tellg();
	}
	return -1;
}

bool CFile::write(const char *src, unsigned int length)
{
	if (m_out)
	{
		m_out->write(src, length);
		return true;
	}
	return false;
}

bool CFile::close()
{
	if (m_in)
	{
		m_in->close();
		delete m_in;
		m_in = NULL;
		return true;
	}
	if (m_out)
	{
		m_out->flush();
		m_out->close();
		delete m_out;
		m_out = NULL;
		return true;
	}
	return false;
}