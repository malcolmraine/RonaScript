/*****************************************************************************
* File: FileInfo.cpp
* Description:
* Author: Malcolm Hall
* Date: 5/16/22
* Version: 1
*
******************************************************************************/

#include "FileInfo.h"

#include <utility>

int FileInfo::CONTEXTUAL_BLOCK_TAB_LVL = 2;

/*****************************************************************************/
FileInfo::FileInfo(const std::string& filepath)
{
	_filepath = std::make_shared<std::string>(filepath);
}

/*****************************************************************************/
FileInfo::FileInfo(const FileInfo& other)
{
	SetFilePath(other.GetFilePath());
	SetCharNum(other.GetCharNum());
	SetLineNum(other.GetLineNum());
	_is_original = false;
}

/*****************************************************************************/
FileInfo::~FileInfo() = default;

/*****************************************************************************/
void FileInfo::SetFilePath(const std::string& filepath)
{
	std::shared_ptr<std::string> newpath = std::make_shared<std::string>(filepath);
	_filepath.swap(newpath);
}

/*****************************************************************************/
std::string FileInfo::GetFilePath() const
{
	return *_filepath;
}

/*****************************************************************************/
void FileInfo::SetLineNum(FileSize linenum)
{
	_linenum = linenum;
}

/*****************************************************************************/
FileSize FileInfo::GetLineNum() const
{
	return _linenum;
}

/*****************************************************************************/
void FileInfo::IncrementLineNum()
{
	++_linenum;
}

/*****************************************************************************/
void FileInfo::SetCharNum(FileSize charnum)
{
	_charnum = charnum;
}

/*****************************************************************************/
FileSize FileInfo::GetCharNum() const
{
	return _charnum;
}

/*****************************************************************************/
void FileInfo::IncrementCharNum()
{
	++_charnum;
	++_char_cnt;
}

/*****************************************************************************/
std::string FileInfo::GetContextualBlock(bool formatted)
{
	std::string block =
		GetContextualBlockTabStr() + GetLineAt(_previous_line_start, true, false);
	std::string line;

	if (std::getline(_file_obj, line))
	{
		if (formatted)
		{
			block += GetFormattedLine(line);
		}
		else
		{
			block += line + "\n";
		}

		if (std::getline(_file_obj, line))
		{
			if (formatted)
			{
				block += GetContextualBlockTabStr() + line + "\n";
			}
			else
			{
				block += line + "\n";
			}
		}
	}
	return block;
}

/*****************************************************************************/
std::string FileInfo::GetLine()
{
	return GetLineAt(_current_line_start, false);
}

/*****************************************************************************/
std::string FileInfo::ToString() const
{
	return "File: " + GetFilePath() + "  " + std::to_string(_linenum) + ":"
		+ std::to_string(_charnum);
}

/*****************************************************************************/
bool FileInfo::IsOriginal() const
{
	return _is_original;
}

/*****************************************************************************/
std::string FileInfo::GetLineAt(FileSize line_start, bool keep_open, bool formatted)
{
	if (!_file_obj.is_open())
	{
		_file_obj.open(GetFilePath(), std::ios::in);
	}
	_file_obj.seekg(line_start);
	std::string line;
	std::getline(_file_obj, line);

	if (!keep_open)
	{
		_file_obj.close();
	}

	if (formatted)
	{
		return GetFormattedLine(line) + "\n";
	}
	else
	{
		return line + "\n";
	}
}

/*****************************************************************************/
std::string FileInfo::GetContextualBlockTabStr()
{
	std::string s;

	for (int i = 0; i < FileInfo::CONTEXTUAL_BLOCK_TAB_LVL; ++i)
	{
		s += "\t";
	}

	return s;
}

/*****************************************************************************/
std::string FileInfo::GetFormattedLine(const std::string& line)
{
	return _line_prefix + GetContextualBlockTabStr() + line + "\n";
}
