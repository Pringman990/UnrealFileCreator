#pragma once
#include <string>
#include <fstream>
#include <iostream>

extern std::string GProjectName;

bool CreateCustomClassFileH(std::ofstream& outFile, const std::string& aFilename, const std::string& aClassName, const std::string& aBaseClass)
{
    if (!outFile) {
        std::cerr << "Error: Could not create the file, ofstream null\n";
        return false;
    }

    // Write the content
    outFile << "#pragma once\n";
    outFile << "\n";
    outFile << "#include \"CoreMinimal.h\"\n";
    outFile << "#include \"" << aFilename << ".generated.h\"\n";
    outFile << "\n";
    outFile << "UCLASS(BlueprintType, Blueprintable)\n";
    outFile << "class " << GProjectName << "_API " << aClassName << " : public " << aBaseClass << "\n";
    outFile << "{\n";
    outFile << "\tGENERATED_BODY()\n";
    outFile << "public:\n";
    outFile << "\n";
    outFile << "public:\n";
    outFile << "\n";
    outFile << "private:\n";
    outFile << "\n";
    outFile << "};\n";

    return true;
}

bool CreateCustomClassFileCPP(std::ofstream& outFile, const std::string& aFilename, const std::string& aClassName)
{
    if (!outFile) {
        std::cerr << "Error: Could not create the file, ofstream null\n";
        return false;
    }

    // Write the content
    outFile << "#include \"" << aFilename << ".h\"\n";
    outFile << "\n";
    
    outFile << aClassName << "::" << aClassName << "()\n";
    outFile << "{\n";
    outFile << "\t// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.\n";
    outFile << "\tPrimaryActorTick.bCanEverTick = true;\n";
    outFile << "}\n";

    outFile << "void " << aClassName << "::BeginPlay()\n";
    outFile << "{\n";
    outFile << "\tSuper::BeginPlay();\n";
    outFile << "}\n";

    outFile << "void " << aClassName << "::Tick(float DeltaTime)\n";
    outFile << "{\n";
    outFile << "\tSuper::Tick(DeltaTime);\n";
    outFile << "}\n";

    return true;
}

bool CreateAActorFileString(std::ofstream& outHFile, std::ofstream& outCppFile, const std::string& aFilename, const std::string& aHeaderIncludePath)
{
    if (!outHFile) {
        std::cerr << "\033[31mError: Could not create the file, ofstream null\033[37m\n";
        return false;
    }
    if (!outCppFile) {
        std::cerr << "\033[31mError: Could not create the file, ofstream null\033[37m\n";
        return false;
    }

    std::string className = "A" + aFilename;

    // Header
    outHFile << "#pragma once\n";
    outHFile << "\n";
    outHFile << "#include \"CoreMinimal.h\"\n";
    outHFile << "#include \"GameFramework/Actor.h\"\n";
    outHFile << "#include \"" << aFilename << ".generated.h\"\n";
    outHFile << "\n";
    outHFile << "UCLASS(BlueprintType, Blueprintable)\n";
    outHFile << "class " << GProjectName << "_API " << className << " : public " << "AActor" << "\n";
    outHFile << "{\n";
    outHFile << "\tGENERATED_BODY()\n";
    outHFile << "public:\n";
    outHFile << "\t" << className << "();\n";
    outHFile << "\tvirtual void BeginPlay() override;\n";
    outHFile << "\tvirtual void Tick(float DeltaTime) override;\n";
    outHFile << "\n";
    outHFile << "public:\n";
    outHFile << "\n";
    outHFile << "private:\n";
    outHFile << "\n";
    outHFile << "};\n";


    // CPP
    outCppFile << "#include \"" << aHeaderIncludePath << "\"\n";
    outCppFile << "\n";

    outCppFile << className << "::" << className << "()\n";
    outCppFile << "{\n";
    outCppFile << "\t// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.\n";
    outCppFile << "\tPrimaryActorTick.bCanEverTick = true;\n";
    outCppFile << "}\n";
    outHFile << "\n";
  
    outCppFile << "void " << className << "::BeginPlay()\n";
    outCppFile << "{\n";
    outCppFile << "\tSuper::BeginPlay();\n";
    outCppFile << "}\n";
    outHFile << "\n";
    
    outCppFile << "void " << className << "::Tick(float DeltaTime)\n";
    outCppFile << "{\n";
    outCppFile << "\tSuper::Tick(DeltaTime);\n";
    outCppFile << "}\n";

    return true;
}

bool CreateActorComponentFileString(std::ofstream& outHFile, std::ofstream& outCppFile, const std::string& aFilename, const std::string& aHeaderIncludePath)
{
    if (!outHFile) {
        std::cerr << "\033[31mError: Could not create the file, ofstream null\033[37m\n";
        return false;
    }
    if (!outCppFile) {
        std::cerr << "\033[31mError: Could not create the file, ofstream null\033[37m\n";
        return false;
    }

    std::string className = "U" + aFilename;

    // Header
    outHFile << "#pragma once\n";
    outHFile << "\n";
    outHFile << "#include \"CoreMinimal.h\"\n";
    outHFile << "#include \"Components/ActorComponent.h\"\n";
    outHFile << "#include \"" << aFilename << ".generated.h\"\n";
    outHFile << "\n";
    outHFile << "UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))\n";
    outHFile << "class " << GProjectName << "_API " << className << " : public " << "UActorComponent" << "\n";
    outHFile << "{\n";
    outHFile << "\tGENERATED_BODY()\n";
    outHFile << "public:\n";
    outHFile << "\t" << className << "();\n";
    outHFile << "\tvirtual void BeginPlay() override;\n";
    outHFile << "\tvirtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;\n";
    outHFile << "\n";
    outHFile << "public:\n";
    outHFile << "\n";
    outHFile << "private:\n";
    outHFile << "\n";
    outHFile << "};\n";


    // CPP
    outCppFile << "#include \"" << aHeaderIncludePath << "\"\n";
    outCppFile << "\n";

    outCppFile << className << "::" << className << "()\n";
    outCppFile << "{\n";
    outCppFile << "\t// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.\n";
    outCppFile << "\tPrimaryComponentTick.bCanEverTick = true;\n";
    outCppFile << "}\n";
    outHFile << "\n";

    outCppFile << "void " << className << "::BeginPlay()\n";
    outCppFile << "{\n";
    outCppFile << "\tSuper::BeginPlay();\n";
    outCppFile << "}\n";
    outHFile << "\n";

    outCppFile << "void " << className << "::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)\n";
    outCppFile << "{\n";
    outCppFile << "\tSuper::TickComponent(DeltaTime, TickType, ThisTickFunction);\n";
    outCppFile << "}\n";

    return true;
}

bool CreateSceneComponentFileString(std::ofstream& outHFile, std::ofstream& outCppFile, const std::string& aFilename, const std::string& aHeaderIncludePath)
{
    if (!outHFile) {
        std::cerr << "\033[31mError: Could not create the file, ofstream null\033[37m\n";
        return false;
    }
    if (!outCppFile) {
        std::cerr << "\033[31mError: Could not create the file, ofstream null\033[37m\n";
        return false;
    }

    std::string className = "U" + aFilename;

    // Header
    outHFile << "#pragma once\n";
    outHFile << "\n";
    outHFile << "#include \"CoreMinimal.h\"\n";
    outHFile << "#include \"Components/SceneComponent.h\"\n";
    outHFile << "#include \"" << aFilename << ".generated.h\"\n";
    outHFile << "\n";
    outHFile << "UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))\n";
    outHFile << "class " << GProjectName << "_API " << className << " : public " << "USceneComponent" << "\n";
    outHFile << "{\n";
    outHFile << "\tGENERATED_BODY()\n";
    outHFile << "public:\n";
    outHFile << "\t" << className << "();\n";
    outHFile << "\tvirtual void BeginPlay() override;\n";
    outHFile << "\tvirtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;\n";
    outHFile << "\n";
    outHFile << "public:\n";
    outHFile << "\n";
    outHFile << "private:\n";
    outHFile << "\n";
    outHFile << "};\n";


    // CPP
    outCppFile << "#include \"" << aHeaderIncludePath << "\"\n";
    outCppFile << "\n";

    outCppFile << className << "::" << className << "()\n";
    outCppFile << "{\n";
    outCppFile << "\t// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.\n";
    outCppFile << "\tPrimaryComponentTick.bCanEverTick = true;\n";
    outCppFile << "}\n";
    outHFile << "\n";

    outCppFile << "void " << className << "::BeginPlay()\n";
    outCppFile << "{\n";
    outCppFile << "\tSuper::BeginPlay();\n";
    outCppFile << "}\n";
    outHFile << "\n";

    outCppFile << "void " << className << "::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)\n";
    outCppFile << "{\n";
    outCppFile << "\tSuper::TickComponent(DeltaTime, TickType, ThisTickFunction);\n";
    outCppFile << "}\n";

    return true;
}

bool CreateInterfaceFileString(std::ofstream& outHFile, std::ofstream& outCppFile, const std::string& aFilename, const std::string& aHeaderIncludePath)
{
    if (!outHFile) {
        std::cerr << "\033[31mError: Could not create the file, ofstream null\033[37m\n";
        return false;
    }
    if (!outCppFile) {
        std::cerr << "\033[31mError: Could not create the file, ofstream null\033[37m\n";
        return false;
    }

    std::string className = "U" + aFilename;
    std::string interfaceName = "I" + aFilename;

    // Header
    outHFile << "#pragma once\n";
    outHFile << "\n";
    outHFile << "#include \"CoreMinimal.h\"\n";
    outHFile << "#include \"UObject/Interface.h\"\n";
    outHFile << "#include \"" << aFilename << ".generated.h\"\n";
    outHFile << "\n";
    
    outHFile << "UINTERFACE(MinimalAPI, BlueprintType, Blueprintable)\n";
    outHFile << "class " << className << " : public " << "UInterface" << "\n";
    outHFile << "{\n";
    outHFile << "\tGENERATED_BODY()\n";
    outHFile << "};\n";

    outHFile << "class " << GProjectName << "_API " << interfaceName << "\n";
    outHFile << "{\n";
    outHFile << "\tGENERATED_BODY()\n";
    outHFile << "public:\n";
    outHFile << "\n";
    outHFile << "private:\n";
    outHFile << "\n";
    outHFile << "};\n";

    // CPP
    outCppFile << "#include \"" << aHeaderIncludePath << "\"\n";

    return true;
}

bool CreateStructFileString(std::ofstream& outHFile, std::ofstream& outCppFile, const std::string& aFilename, const std::string& aHeaderIncludePath)
{
    if (!outHFile) {
        std::cerr << "\033[31mError: Could not create the file, ofstream null\033[37m\n";
        return false;
    }
    if (!outCppFile) {
        std::cerr << "\033[31mError: Could not create the file, ofstream null\033[37m\n";
        return false;
    }

    std::string structName = "F" + aFilename;

    // Header
    outHFile << "#pragma once\n";
    outHFile << "\n";
    outHFile << "#include \"CoreMinimal.h\"\n";
    outHFile << "#include \"" << aFilename << ".generated.h\"\n";
    outHFile << "\n";

    outHFile << "USTRUCT(BlueprintType, Blueprintable)\n";
    outHFile << "struct " << structName << "\n";
    outHFile << "{\n";
    outHFile << "\tGENERATED_BODY()\n"; 
    outHFile << "\n";
    outHFile << "};\n";

    // CPP
    outCppFile << "#include \"" << aHeaderIncludePath << "\"\n";

    return true;
}