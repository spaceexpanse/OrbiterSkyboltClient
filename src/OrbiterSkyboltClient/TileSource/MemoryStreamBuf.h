#pragma once

#include <streambuf>

// From https://stackoverflow.com/questions/7781898/get-an-istream-from-a-char
struct MemoryStreamBuf : std::streambuf
{
	MemoryStreamBuf(char* buffer, std::size_t sizeBytes)
	{
		setg(buffer, buffer, buffer + sizeBytes);
	}
};