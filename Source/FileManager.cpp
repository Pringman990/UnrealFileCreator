#include "FileManager.h"

#include <iostream>
#include <filesystem>
#include <unordered_map>

namespace fs = std::filesystem;
extern std::string GProjectName;

FileManager::FileManager()
{
}

FileManager::~FileManager()
{
}

void FileManager::LoadFiles()
{
#ifdef _DEBUG
	std::string path = "../../FileTypes";
#else
	std::string path = "FileTypes/";
#endif // _DEBUG


	if (!fs::exists(path))
	{
		std::cout << "\033[31mFileTypes folder did not exist, please add it again\033[37m\n";
		return;
	}

	for (auto entry : fs::directory_iterator(path))
	{
		if (entry.is_directory())
			continue;

		std::string filename = entry.path().filename().replace_extension().string();

		if (LoadedFiles.find(filename) != LoadedFiles.end())
		{
			std::cout << "\033[31mTrying to load already loaded file\033[37m\n";
			std::cout << "\033[31mSkipping second file: " << filename << "\033[37m\n";
			continue;
		}

		std::ifstream ifs(entry.path());
		std::stringstream buffer;
		buffer << ifs.rdbuf();
		LoadedFiles[filename] = buffer.str();
	}
}

bool FileManager::CreateFile(
	const std::string& pathHFile, 
	const std::string& pathCppFile, 
	const std::string& aFileType,
	const std::string& aFilename, 
	const std::string& aHeaderIncludePath
)
{
	auto it = LoadedFiles.find(aFileType);
	if (it == LoadedFiles.end())
	{
		std::cout << "\033[31mFile did not exist: " << aFileType << "\033[37m\n";
		return false;
	}

	std::string fileData = it->second;

	size_t headerStart = fileData.find("<HEADER>");
	size_t cppStart = fileData.find("<CPP>");

	if (headerStart == std::string::npos || cppStart == std::string::npos)
	{
		std::cerr << "\033[31mError: Missing <HEADER> or <CPP> section in template file\033[37m\n";
		return false;
	}

	std::string headerContent = fileData.substr(headerStart + 8, cppStart - (headerStart + 8));
	std::string cppContent = fileData.substr(cppStart + 5);

	std::unordered_map<std::string, std::string> replacements = {
		{"CLASSNAME", aFilename},
		{"PROJECTNAME", GProjectName},
		{"HEADERFILEPATH", aHeaderIncludePath}
	};

	for (const auto& [tag, value] : replacements)
	{
		std::string placeholder = "<" + tag + ">";
		
		size_t pos = 0;
		while ((pos = headerContent.find(placeholder, pos)) != std::string::npos)
		{
			headerContent.replace(pos, placeholder.length(), value);
			pos += value.length();
		}

		pos = 0;
		while ((pos = cppContent.find(placeholder, pos)) != std::string::npos)
		{
			cppContent.replace(pos, placeholder.length(), value);
			pos += value.length();
		}
	}

	std::ofstream ofH(pathHFile);
	std::ofstream ofCPP(pathCppFile);

	ofH << headerContent;
	ofCPP << cppContent;

	ofH.close();
	ofCPP.close();

	std::cout << "\033[32mFiles generated successfully: " << aFilename << ".h and " << aFilename << ".cpp\033[37m\n";

	return true;
}

std::vector<FileType> FileManager::GetFileTypes()
{
	std::vector<FileType> types;
	for (const auto& [type, data] : LoadedFiles)
	{
		types.push_back(type);
	}
	return types;
}
