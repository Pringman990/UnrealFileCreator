----------------------------------------------------------------------------
-- the dirs table is a listing of absolute paths, since we generate projects
-- and files it makes a lot of sense to make them absolute to avoid problems
-- outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

dirs = {}
dirs["root"] 				= os.realpath("../../")

--Roots
dirs["Binaries"]			= os.realpath(dirs.root .. "Binaries/")
dirs["Intermediate"]		= os.realpath(dirs.root .. "Intermediate/")
dirs["Source"] 				= os.realpath(dirs.root .. "Source/")

--Binaries
dirs["Binaries_Win64"]		= os.realpath(dirs.Binaries .. "Win64/")

--Intermediates
dirs["Inter_Temp"]			= os.realpath(dirs.Intermediate .. "Temp/")
dirs["Inter_ProjectFiles"]	= os.realpath(dirs.Intermediate .. "ProjectFiles/")
dirs["Inter_Lib"]	        = os.realpath(dirs.Intermediate .. "Lib/")

UCE_TARGET_NAME_DEBUG = "%{prj.name}_%{cfg.buildcfg}"
UCE_TARGET_NAME_RETAIL = "%{prj.name}"
UCE_TARGET_DIR = dirs.Inter_Lib .. "%{cfg.buildcfg}"

UCE_OBJ_DIR = dirs.Inter_Temp .. "%{prj.name}/%{cfg.buildcfg}"
UCE_VCXPROJ_DIR = dirs.Inter_ProjectFiles

UCE_EXECUTABLE_DIR = dirs.Binaries_Win64

UCE_BINARIES_DIR = dirs.Binaries
UCE_SOURCE_DIR = dirs.Source