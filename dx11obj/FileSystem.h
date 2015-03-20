#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdio.h>
#include <string>

using namespace std;

// get directory name
class Path {
public:
	static std::wstring GetDirectoryName(const wstring& path) {
		int lastPathSep = path.find_last_of(L'\\');
		
		if (lastPathSep > -1) {
			return path.substr(0, lastPathSep);
		}

		return L"";
	}
};

// thanks for being a pain C++, char to wide char string
class Tools {
public:
	static void CharToWideCharString(wchar_t *dest, char *source) {
		int length = strlen(source);
		
		for (int i = 0; i <= length; i++) {
			dest[i] = (wchar_t)source[i];
		}
	}
};

// read all text from the file
class File {
public:
	static std::string ReadAllText(const wstring& path) {
		FILE* f = _wfopen(path.c_str(), L"rb");
		
		if (f == NULL) {
			throw exception("File not found");
		}

		fseek(f, 0, SEEK_END);
		int size = ftell(f);
		rewind(f);

		char* data = new char[size + 1];
		fread(data, 1, size, f);
		fclose(f);
		data[size] = 0;

		std::string result(data);
		delete[] data;

		return result;
	}
};

#endif