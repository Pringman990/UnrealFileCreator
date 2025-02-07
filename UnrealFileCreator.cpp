#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <windows.h>
#include <shobjidl.h>
#include <algorithm>
#include <cctype>

#include "FileTypeCreation.h"

#undef CreateFile

namespace fs = std::filesystem;
void CreateFile();
void GetUnrealEngineDirectory();
void GenerateVisualStudioFiles();

std::string GProjectName;
std::string GUnrealBatchCommand;

int main()
{
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
			std::transform(GProjectName.begin(), GProjectName.end(), GProjectName.begin(), [](unsigned char c) { return std::toupper(c); });
			foundProjectPath = true;
			break;
		}
	}
	if (!foundProjectPath)
	{
		std::cout << "\033[31mProject name not found\033[37m\n";
	}

	//std::cout << "Please select the unreal engine BatchFiles folder\n";
	//std::cout << "BatchFiles usualy exist under: C:/Program Files/Epic Games/UE_X.XX/Engine/Build/BatchFiles/ \n";
	//GetUnrealEngineDirectory();
	//GenerateVisualStudioFiles();

	while (true)
	{
		std::cout << "What would you like to do?\n";
		std::cout << "file		Start setup for file creation\n";
		std::cout << "abort		Cancel any action\n";

		std::string command;
		std::getline(std::cin, command);

		if (command == "file")
		{
			CreateFile();
		}
		else if (command == "abort")
		{
			return 0;
		}
		else
		{
			std::cout << "\033[33mUnknown command, enter again\033[37m\n";
		}
	}
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

void GenerateVisualStudioFiles()
{
	int result = std::system(GUnrealBatchCommand.c_str());

	if (result == 0) {
		std::cout << "Project files regenerated successfully.\n";
	}
	else {
		std::cerr << "Error: Failed to regenerate project files.\n";
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

	std::string classType;
	while (true)
	{
		std::cout << "Enter the class type (numeric) (or type 'abort' to cancel): \n";
		std::cout << "1. Actor \n";
		std::cout << "2. ActorComponent \n";
		std::cout << "3. SceneComponent \n";
		std::cout << "4. Interface \n";
		std::cout << "5. Struct \n";

		std::getline(std::cin, classType);

		if (classType == "abort")
		{
			std::cout << "\033[33mOperation aborted.\033[37m\n";
			return;
		}
		if (classType == "1" || classType == "2" || classType == "3" || classType == "4" || classType == "5")
		{
			break;
		}
		else
		{
			std::cout << "\033[33mFile type not supported try again\033[37m\n";
			continue;
		}
	}

	// Get the filename from the user
	std::cout << "Enter the filename (or type 'abort' to cancel): ";
	std::getline(std::cin, filename);

	if (filename == "abort")
	{
		std::cout << "\033[33mOperation aborted.\033[37m\n";
		return;
	}

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

	std::ofstream ofH(headerPath);
	std::ofstream ofCPP(cppPath);

	if (classType == "1")
	{
		if (!CreateAActorFileString(ofH, ofCPP, filename, headerIncludePath.string()))
		{
			std::cout << "\033[31mSomething went wrong, aborting...\033[37m\n";
			ofH.close();
			ofCPP.close();
			return;
		}
	}
	else if (classType == "2")
	{
		if (!CreateActorComponentFileString(ofH, ofCPP, filename, headerIncludePath.string()))
		{
			std::cout << "\033[31mSomething went wrong, aborting...\033[37m \n";
			ofH.close();
			ofCPP.close();
			return;
		}
	}
	else if (classType == "3")
	{
		if (!CreateSceneComponentFileString(ofH, ofCPP, filename, headerIncludePath.string()))
		{
			std::cout << "\033[31mSomething went wrong, aborting...\033[37m \n";
			ofH.close();
			ofCPP.close();
			return;
		}
	}
	else if (classType == "4")
	{
		if (!CreateInterfaceFileString(ofH, ofCPP, filename, headerIncludePath.string()))
		{
			std::cout << "\033[31mSomething went wrong, aborting...\033[37m \n";
			ofH.close();
			ofCPP.close();
			return;
		}
	}
	else if (classType == "5")
	{
		if (!CreateStructFileString(ofH, ofCPP, filename, headerIncludePath.string()))
		{
			std::cout << "\033[31mSomething went wrong, aborting...\033[37m \n";
			ofH.close();
			ofCPP.close();
			return;
		}
	}

	ofH.close();
	ofCPP.close();

	std::cout << "\033[32mFile was created\033[37m\n";
}
