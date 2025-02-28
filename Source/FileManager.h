#pragma once
#include <map>
#include <string>
#include <fstream>
#include <vector>

using FileType = std::string;
using FileData = std::string;


class FileManager
{
public:
	FileManager();
	~FileManager();

	void LoadFiles();

#undef CreateFile
	bool CreateFile(const std::string& pathHFile,
		const std::string& pathCppFile, 
		const std::string& aFileType,
		const std::string& aFilename,
		const std::string& aHeaderIncludePath
	);

	std::vector<FileType> GetFileTypes();
private:
private:
	std::map<FileType, FileData> LoadedFiles;
};