print("Including Source/")

include "../Scripts/Premake/common.lua"

solution "UnrealFileCreator"
    location "../"
    startproject "UnrealFileCreator"
    
    configurations {
        "Debug",
        "Retail"
    }

	platforms{ 
		"Win64"
	}

	filter("platforms:Win64")
		architecture "x64"

	 filter ("configurations:Debug")
		runtime "Debug"
		symbols "Full"
		optimize "Off"
		defines {"_DEBUG"}

	filter ("configurations:Retail")
		runtime "Release"
		symbols "Off"
		optimize "Full"
		defines {"NDEBUG"}

	filter ("configurations:Retail", "action:vs*")
		buildoptions {"/wd4189"}

	filter "system:windows"
		symbols "On"		
		systemversion "latest"
		defines {"_Win32", "_Win64"}

	flags {"FatalWarnings", "MultiProcessorCompile"}
    warnings "Extra"
    externalanglebrackets "On"

    filter {}

project "UnrealFileCreator"
	language "C++"
	cppdialect "C++20"
	kind "ConsoleApp"
	
    targetdir (UCE_EXECUTABLE_DIR)

    objdir(UCE_OBJ_DIR)
    location (UCE_VCXPROJ_DIR)

	files {
		"**.h",
		"**.hpp",
		"**.cpp",
		"**.c"
	}

	includedirs {
		dirs.Source
	}
	filter ("configurations:Debug")
		targetname(UCE_TARGET_NAME_DEBUG)
			filter ("configurations:Retail")
		targetname(UCE_TARGET_NAME_RETAIL)

filter {}