#pragma once

#include <string>

namespace DZB {

	class FileUtils
	{
	public:
		static std::string read_file(const char* filepath)
		{
			//open as read only text file
			FILE* file = fopen(filepath, "rt");

			//seek to end of file
			fseek(file, 0, SEEK_END);

			//get the number of characters read
			unsigned long length = ftell(file);

			//char* variable to hold file data
			char* data = new char[length + 1];

			//set last value to termination char 0
			memset(data, 0, length + 1);

			//go back to start of file
			fseek(file, 0, SEEK_SET);

			//read in data
			fread(data, 1, length, file);

			//close file
			fclose(file);

			//copy data into a string
			std::string result(data);

			//delete data
			delete[] data;

			//return result
			return result;
		}

	};


}