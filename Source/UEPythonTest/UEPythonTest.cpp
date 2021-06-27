// Copyright Epic Games, Inc. All Rights Reserved.

#include "UEPythonTest.h"
#include "Modules/ModuleManager.h"
#include "IPythonScriptPlugin.h"

DEFINE_LOG_CATEGORY(LogUEPythonTest);

class FUEPythonTestModule : public IModuleInterface
{
public:
	// IModuleInterface interface.
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// End of IModuleInterface interface.
};

void FUEPythonTestModule::StartupModule()
{
	const FString PythonInstall = FPaths::ConvertRelativePathToFull(FPaths::Combine(
		FPaths::ProjectContentDir(), TEXT("Python")
	));

	if (IFileManager::Get().DirectoryExists(*FPaths::Combine(PythonInstall, TEXT("googletrans"))))
	{
		return;
	}
	
	const FString PythonRoot = FPaths::ConvertRelativePathToFull(FPaths::Combine(
		FPaths::EngineDir(), TEXT("Binaries"), TEXT("ThirdParty"), TEXT("Python3"), TEXT("Win64")
	));

	const FString Parameter = FString::Printf(
		TEXT("-m pip install --target=\"%s\" googletrans==3.1.0a0"), *PythonInstall
	);
	
	void* WritePipe = nullptr;
	void* ReadPipe= nullptr;
	
	FProcHandle Handle = FPlatformProcess::CreateProc(
		*FPaths::Combine(PythonRoot, TEXT("python.exe")), *Parameter,
		false, true, true,
		nullptr, 0, nullptr, WritePipe, ReadPipe
	);

	while(FPlatformProcess::IsProcRunning(Handle))
	{
		const FString Output = FPlatformProcess::ReadPipe(ReadPipe);
		if (!Output.IsEmpty())
		{
			UE_LOG(LogUEPythonTest, Log, TEXT("%s"), *Output);
		}
	}

	int32 ReturnCode = -1;
	FPlatformProcess::GetProcReturnCode(Handle, &ReturnCode);
	UE_LOG(LogUEPythonTest, Log, TEXT("googletrans install = %d"), (ReturnCode == 0));

	auto& Python = *IPythonScriptPlugin::Get();

	FPythonCommandEx PythonCommand;
	PythonCommand.Command = TEXT(
		"import site\nfrom importlib import reload\nreload(site)"
	);
	
	const bool bResult = Python.ExecPythonCommandEx(PythonCommand);

	UE_LOG(LogUEPythonTest, Log, TEXT("reload site = %d"), bResult);
}

void FUEPythonTestModule::ShutdownModule()
{
	
}

IMPLEMENT_PRIMARY_GAME_MODULE(FUEPythonTestModule, UEPythonTest, "UEPythonTest");
