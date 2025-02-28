#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <windows.h>
#include <shobjidl.h>
#include <algorithm>
#include <cctype>
#include <thread>

#include "FileManager.h"

#undef CreateFile
#define UNREALENGINEPATH "UnrealFileCreatorMeta.txt"

namespace fs = std::filesystem;
std::string SelectDirectory();
void CreateFile();
void GetUnrealEngineDirectory();
void GenerateVisualStudioFiles();
std::string GetBatchCommand();

std::string GProjectName;
std::string GProjectNameNonCaps;
std::string GUnrealBatchCommand;
std::string GUnrealEngineFilePath;
std::string GUnrealVersionSelector;
FileManager GFileManager;
char* GUserName;

std::thread GLoadScreenThread;
std::atomic<bool> keepPrinting(false);

bool CheckUnrealEngineFilePath()
{
	if (!fs::exists(UNREALENGINEPATH))
	{
		return false;
	}

	std::ifstream ifs(UNREALENGINEPATH);
	if (!ifs.is_open())
	{
		ifs.close();
		return false;
	}

	std::stringstream buffer;
	buffer << ifs.rdbuf();
	ifs.close();

	std::string metaData = buffer.str();

	size_t UnrealEnginePathOffset = metaData.find("UnrealEnginePath: ");
	if (UnrealEnginePathOffset == std::string::npos)
		return false;

	std::string substr = metaData.substr(UnrealEnginePathOffset + 18);
	GUnrealEngineFilePath = substr;
	std::cout << "\033[32mUnreal engine file path found: " << GUnrealEngineFilePath << "\033[37m\n";
	return true;
}

bool CheckUnrealVersionSelectorPath()
{
	if (!fs::exists(UNREALENGINEPATH))
	{
		return false;
	}

	std::ifstream ifs(UNREALENGINEPATH);
	if (!ifs.is_open())
	{
		ifs.close();
		return false;
	}

	std::stringstream buffer;
	buffer << ifs.rdbuf();
	ifs.close();

	std::string metaData = buffer.str();

	size_t UnrealVersionSelectorPathOffset = metaData.find("UnrealVersionSelectorPath: ");
	if (UnrealVersionSelectorPathOffset == std::string::npos)
		return false;

	std::string substr = metaData.substr(UnrealVersionSelectorPathOffset + 27);
	GUnrealVersionSelector = substr;
	std::cout << "\033[32mUnreal version selector path found: " << GUnrealVersionSelector << "\033[37m\n";
	return true;
}

void PressEnterToContinue()
{
	system("pause");
}

void EnterUnrealEngineFilePath()
{
	while (true)
	{
		std::cout << "Please enter the Unreal engine file path\n";
		std::cout << "Usualy found at: Project Files/Epic Games/UE_X.XX\n";
		PressEnterToContinue();

		GUnrealEngineFilePath = SelectDirectory();
		if (!GUnrealEngineFilePath.empty())
		{
			break;
		}
		std::cout << "\033[31mOperation aborted or invalid directory selected.\033[37m\n";
	}
}

void EnterUnrealVersionSelectorPath()
{
	while (true)
	{
		std::cout << "Please enter the Unreal version selector path\n";
		std::cout << "Usualy found at: C:/Program Files (x86)/Epic Games/Launcher/Engine/Binaries/Win64/\n";
		PressEnterToContinue();

		GUnrealVersionSelector = SelectDirectory();
		if (!GUnrealVersionSelector.empty())
		{
			GUnrealVersionSelector += "/UnrealVersionSelector.exe";
			break;
		}
		std::cout << "\033[31mOperation aborted or invalid directory selected.\033[37m\n";
	}
}

void ReadConfig(std::string& selectorPath, std::string& enginePath)
{
	std::ifstream file(UNREALENGINEPATH);
	if (!file)
	{
		std::cerr << "\033[31mConfig file not found. A new one will be created.\033[37m\n";
		return;
	}

	std::string line;
	while (std::getline(file, line))
	{
		size_t pos = line.find(": ");
		if (pos != std::string::npos)
		{
			std::string key = line.substr(0, pos);
			std::string value = line.substr(pos + 2);

			if (key == "UnrealVersionSelectorPath")
			{
				selectorPath = value;
			}
			else if (key == "UnrealEnginePath")
			{
				enginePath = value;
			}
		}
	}
	file.close();
}

// Function to write config file
void WriteConfig(const std::string& selectorPath, const std::string& enginePath)
{
	std::ofstream file(UNREALENGINEPATH);
	if (!file)
	{
		std::cerr << "\033[31mFailed to open config file for writing!\033[37m\n";
		return;
	}

	file << "UnrealVersionSelectorPath: " << selectorPath << "\n";
	file << "UnrealEnginePath: " << enginePath << "\n";

	file.close();
}

void LoadScreen()
{
	while (true)
	{
		if (keepPrinting)
		{
			std::cout << ".";
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	}
}

int main()
{
	GLoadScreenThread = std::thread(LoadScreen);

	GFileManager.LoadFiles();

	char* username = nullptr;
	size_t len = 0;

	// Use _dupenv_s to safely get the USERNAME environment variable
	if (_dupenv_s(&username, &len, "USERNAME") == 0 && username != nullptr) 
	{
		GUserName = username;
	}
	else {
		std::cerr << "Failed to get username from environment variable." << std::endl;
	}

	bool foundProjectPath = false;
	for (auto entry : fs::directory_iterator("."))
	{
		if (entry.is_directory())
			continue;

		if (entry.path().extension() == ".uproject")
		{
			GProjectName = entry.path().filename().string();
			size_t offset = GProjectName.find(".");
			GProjectName = GProjectName.erase(offset);
			GProjectNameNonCaps = GProjectName;
			std::transform(GProjectName.begin(), GProjectName.end(), GProjectName.begin(), [](unsigned char c) { return std::toupper(c); });
			foundProjectPath = true;
			break;
		}
	}

	if (!foundProjectPath)
	{
		std::cout << "\033[31mProject name not found\033[37m\n";
		std::cout << "Please enter the project name manually, name can be found on the .uproject file: \n";

		std::string projectName;
		std::getline(std::cin, projectName);
		GProjectName = projectName;
		GProjectNameNonCaps = GProjectName;
		std::transform(GProjectName.begin(), GProjectName.end(), GProjectName.begin(), [](unsigned char c) { return std::toupper(c); });
		std::cout << "\033[32mProject name registered: " << GProjectName << "\033[37m\n";
	}

	std::string selectorPath, enginePath;
	ReadConfig(selectorPath, enginePath);

	bool selectorExist = fs::exists(selectorPath);
	bool engineExist = fs::exists(enginePath);

	if (!selectorExist)
	{
		EnterUnrealVersionSelectorPath();
	}
	else
	{
		GUnrealVersionSelector = selectorPath;
	}

	if (!engineExist)
	{
		EnterUnrealEngineFilePath();
	}
	else
	{
		GUnrealEngineFilePath = enginePath;
	}

	if (!selectorExist || !engineExist)
	{
		WriteConfig(GUnrealVersionSelector, GUnrealEngineFilePath);
	}

	std::cout << "\033[32mUnreal engine file path: " << GUnrealEngineFilePath << "\033[37m\n";
	std::cout << "\033[32mUnreal version selector path: " << GUnrealVersionSelector << "\033[37m\n";

	while (true)
	{
		std::cout << std::string("What would you like to do ") + GUserName + "?\n";
		std::cout << std::left;
		std::cout << std::setw(28) << "file" << "Start setup for file creation\n";
		std::cout << std::setw(28) << "unreal-engine" << "To change Unreal Engine path\n";
		std::cout << std::setw(28) << "unreal-version" << "To change Unreal Version Selector path\n";
		std::cout << std::setw(28) << "paths" << "Check current metadata paths\n";
		std::cout << std::setw(28) << "project-name" << "Check current project name\n";
		std::cout << std::setw(28) << "project-generate" << "Generates visual studio files\n";
		std::cout << std::setw(28) << "project-batch-command" << "Shows visual studio generation command\n";
		std::cout << std::setw(28) << "clear" << "Clear console\n";
		std::cout << std::setw(28) << "abort" << "Cancel any action\n";
		std::cout << std::setw(28) << "exit" << "Closes console\n";

		std::string command;
		std::getline(std::cin, command);

		if (command == "file")
		{
			CreateFile();
			continue;
		}
		else if (command == "unreal-engine")
		{
			EnterUnrealEngineFilePath();
			WriteConfig(GUnrealVersionSelector, GUnrealEngineFilePath);
			std::cout << "\033[32mUnreal engine file path: " << GUnrealEngineFilePath << "\033[37m\n";
			continue;
		}
		else if (command == "unreal-version")
		{
			EnterUnrealVersionSelectorPath();
			WriteConfig(GUnrealVersionSelector, GUnrealEngineFilePath);
			std::cout << "\033[32mUnreal version selector path: " << GUnrealVersionSelector << "\033[37m\n";
			continue;
		}
		else if (command == "paths")
		{
			std::cout << "\033[32mUnreal engine file path: " << GUnrealEngineFilePath << "\033[37m\n";
			std::cout << "\033[32mUnreal version selector path: " << GUnrealVersionSelector << "\033[37m\n";
			continue;
		}
		else if (command == "project-name")
		{
			std::cout << "\033[32mProject name: " << GProjectName << "\033[37m\n";
			continue;
		}
		else if (command == "project-generate")
		{
			GenerateVisualStudioFiles();
			continue;
		}
		else if (command == "project-batch-command")
		{
			std::cout << "\033[32mBatch command: " << GetBatchCommand() << "\033[37m\n";
			continue;
		}
		else if (command == "abort")
		{
			std::cout << "\033[33mActions already aborted\033[37m\n";
			continue;
		}
		else if (command == "exit")
		{
			break;
		}
		else if (command == "clear")
		{
			system("cls");
			continue;
		}
		else
		{
			std::cout << "\033[33mUnknown command, enter again\033[37m\n";
		}
	}

	GLoadScreenThread.join();
	return 0;
}

std::string SelectDirectory() {
	HRESULT hr;
	std::string directoryPath;

	// Initialize COM Library
	hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (FAILED(hr)) {
		std::cerr << "\033[31mFailed to initialize COM library.\033[37m\n";
		return "";
	}

	IFileDialog* pFileDialog;

	// Create File Open Dialog
	hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_IFileDialog, reinterpret_cast<void**>(&pFileDialog));
	if (SUCCEEDED(hr)) {
		DWORD dwOptions;
		pFileDialog->GetOptions(&dwOptions);
		pFileDialog->SetOptions(dwOptions | FOS_PICKFOLDERS); // Set to folder selection mode

		// Show the dialog
		hr = pFileDialog->Show(NULL);
		if (SUCCEEDED(hr)) {
			IShellItem* pItem;
			hr = pFileDialog->GetResult(&pItem);
			if (SUCCEEDED(hr)) {
				PWSTR pszFilePath;
				hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

				// Convert wide string (WCHAR*) to std::string
				if (SUCCEEDED(hr)) {
					size_t requiredSize = 0;
					wcstombs_s(&requiredSize, nullptr, 0, pszFilePath, 0); // Get required size

					char* path = new char[requiredSize];
					wcstombs_s(nullptr, path, requiredSize, pszFilePath, _TRUNCATE);

					directoryPath = path;
					delete[] path;

					CoTaskMemFree(pszFilePath);
				}
				pItem->Release();
			}
		}
		pFileDialog->Release();
	}

	// Uninitialize COM
	CoUninitialize();

	return directoryPath;
}

void GetUnrealEngineDirectory()
{
	std::string unrealEnginePath = SelectDirectory();
	std::string command = unrealEnginePath + "/Rebuild.bat \"" + "." + "\"";
	GUnrealBatchCommand = command;
}

std::string GetBatchCommand()
{
	std::string fullprojectPath = fs::current_path().string();
	fullprojectPath += "/" + GProjectNameNonCaps;
	fullprojectPath += ".uproject";
	return std::string("cmd /C \"\"" + GUnrealVersionSelector + "\"" + " /projectfiles " + "\"" + fullprojectPath + "\"");
}

void GenerateVisualStudioFiles()
{
	int result = system(GetBatchCommand().c_str());

	if (result == 0) {
		std::cout << "\033[32mProject files regenerated successfully.\033[37m\n";
	}
	else {
		std::cerr << "\033[31mError: Failed to regenerate project files.\033[37m\n";
	}
}

void GeneratePublicPrivatePaths(const std::string& originalPath, fs::path& outHeaderPath, fs::path& outCPPPath) {
	fs::path path(originalPath);

	// Iterate through path components
	fs::path resultPath;
	bool replaced = false;

	for (const auto& part : path) {
		std::string partStr = part.string();

		if (!replaced && (partStr == "Public" || partStr == "Private")) {
			// Replace "public" with "private" and vice versa
			outHeaderPath = resultPath / "Public";
			outCPPPath = resultPath / "Private";
			replaced = true; // Ensure only one replacement occurs
		}
		else {
			outHeaderPath /= part;
			outCPPPath /= part;
		}

		resultPath /= part;
	}

	if (!replaced)
		std::cout << "\033[31mNeither 'public' nor 'private' found in the path.\033[37m\n";

	// Check if the counterpart directory exists, and create it if missing
	if (!fs::exists(outHeaderPath)) {
		std::cout << "Creating missing directory: " << outHeaderPath << "\n";
		fs::create_directories(outHeaderPath);
	}

	if (!fs::exists(outCPPPath)) {
		std::cout << "Creating missing directory: " << outCPPPath << "\n";
		fs::create_directories(outCPPPath);
	}
}

void CreateFile()
{
	std::string directory, filename;

	directory = SelectDirectory();

	if (directory.empty()) {
		std::cout << "\033[31mOperation aborted or invalid directory selected.\033[37m\n";
		return;
	}

	fs::path headerPath;
	fs::path cppPath;
	GeneratePublicPrivatePaths(directory, headerPath, cppPath);
	std::cout << "Header File will be created at: " << headerPath.string() << "\n";
	std::cout << "CPP File will be created at: " << cppPath.string() << "\n";

	int32_t classTypeIndex = -1;
	std::vector<FileType> types = GFileManager.GetFileTypes();
	while (true)
	{
		std::cout << "Enter the class type (numeric) (or type 'abort' to cancel): \n";

		for (int32_t i = 0; i < types.size(); i++)
		{
			std::cout << std::to_string(i) << ". " << types[i] << "\n";
		}

		std::string classType;
		std::getline(std::cin, classType);

		char* end;
		classTypeIndex = std::strtol(classType.c_str(), &end, 10);

		if (classType == "abort")
		{
			std::cout << "\033[33mOperation aborted.\033[37m\n";
			return;
		}

		if (*end == '\0' && classTypeIndex < types.size())
		{
			break;
		}
		else
		{
			std::cout << "\033[33mFile type not supported try again\033[37m\n";
			continue;
		}
	}

#ifdef _DEBUG
	std::string sourcePath = "../../TestSource";
#else
	std::string sourcePath = "Source/";
#endif // _DEBUG

	if (!fs::exists(sourcePath))
	{
		std::cout << "\033[31mSource folder did not exist\033[37m\n";
		return;
	}

	if (!fs::exists(GUnrealEngineFilePath))
	{
		std::cout << "\033[31mUnreal engine folder did not exist\033[37m\n";
		return;
	}

	//TODO: add animation to file lookup loading
	while (true)
	{
		std::cout << "Enter the filename (or type 'abort' to cancel): ";
		std::getline(std::cin, filename);

		if (filename == "abort")
		{
			std::cout << "\033[33mOperation aborted.\033[37m\n";
			return;
		}

		std::cout << "Checking for identical filename in source... \n";
		keepPrinting = true;


		bool foundIdenticalName = false;
		for (const auto& entry : fs::recursive_directory_iterator(sourcePath))
		{
			std::string entryName = entry.path().filename().replace_extension().string();
			if (filename == entryName)
			{
				std::cout << "\033[33m\nFilename already exist try again.\033[37m\n";
				foundIdenticalName = true;
				keepPrinting = false;
				break;
			}
		}

		std::cout << "\nChecking for identical filename in unreal engine source... \n";
		if (!foundIdenticalName)
		{
			for (const auto& entry : fs::recursive_directory_iterator(GUnrealEngineFilePath))
			{
				std::string entryName = entry.path().filename().replace_extension().string();
				if (filename == entryName)
				{
					std::cout << "\033[33m\nFilename already exist try again.\033[37m\n";
					foundIdenticalName = true;
					keepPrinting = false;
					break;
				}
			}
		}

		if (!foundIdenticalName)
		{
			keepPrinting = false;
			break;
		}
	}
	std::cout << "\033[32m\nFilename accepted\033[37m\n";
	keepPrinting = false;

	headerPath = fs::path(headerPath) / (filename + ".h");
	cppPath = fs::path(cppPath) / (filename + ".cpp");

	fs::path headerIncludePath;
	bool startAdding = false;
	for (const auto& part : headerPath) {
		std::string partStr = part.string();

		if (!startAdding && (partStr == "Public" || partStr == "Private")) {
			startAdding = true;
			continue;
		}

		if (startAdding)
		{
			headerIncludePath /= part;
			if (partStr == (filename + ".h"))
			{
				break;
			}
		}
	}

	if (classTypeIndex != -1)
	{
		const std::string& fileType = types[classTypeIndex];
		if (!GFileManager.CreateFile(headerPath.string(), cppPath.string(), fileType, filename, headerIncludePath.string()))
		{
			std::cout << "\033[31mSomething went wrong, aborting...\033[37m\n";
			return;
		}
	}

	std::cout << "\033[32mFile was created\033[37m\n";

	while (true)
	{
		std::cout << "Would you like to generate visual studio project files? (y,n)";
		std::string generate;
		std::getline(std::cin, generate);

		if (generate == "n")
		{
			return;
		}
		else if (generate == "y")
		{
			GenerateVisualStudioFiles();
			return;
		}
		else
		{
			std::cout << "\033[33mUnknown command, try again\033[37m\n";
			continue;
		}
	}
}
