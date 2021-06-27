// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBlueprintFunctionLibrary.h"
#include "UEPythonTest.h"
#include "IPythonScriptPlugin.h"

FString UMyBlueprintFunctionLibrary::Translate(const FString& Text, const FString& ToLanguage)
{
	auto& Python = *IPythonScriptPlugin::Get();
	
	FPythonCommandEx PythonCommand;
	PythonCommand.ExecutionMode = EPythonCommandExecutionMode::ExecuteFile;
	PythonCommand.Command = FString::Printf(
		TEXT("from googletrans import Translator\ntranslator = Translator()\ntranslated = translator.translate('%s', dest='%s')"),
		*Text, *ToLanguage
	);
	if (!Python.ExecPythonCommandEx(PythonCommand))
	{
		UE_LOG(LogUEPythonTest, Error, TEXT("%s"), *PythonCommand.CommandResult);
		return PythonCommand.CommandResult;
	}
	
	PythonCommand.ExecutionMode = EPythonCommandExecutionMode::EvaluateStatement;
	PythonCommand.Command = TEXT("'' + translated.text");
	if (!Python.ExecPythonCommandEx(PythonCommand))
	{
		UE_LOG(LogUEPythonTest, Error, TEXT("%s"), *PythonCommand.CommandResult);
		return PythonCommand.CommandResult;
	}

	return PythonCommand.CommandResult.Mid(1, PythonCommand.CommandResult.Len() - 2);
}

FString UMyBlueprintFunctionLibrary::Detect(const FString& Text)
{
	auto& Python = *IPythonScriptPlugin::Get();
	
	FPythonCommandEx PythonCommand;
	PythonCommand.ExecutionMode = EPythonCommandExecutionMode::ExecuteFile;
	PythonCommand.Command = FString::Printf(
		TEXT("from googletrans import Translator\ntranslator = Translator()\ndetected = translator.detect('%s')"),
		*Text
	);
	if (!Python.ExecPythonCommandEx(PythonCommand))
	{
		UE_LOG(LogUEPythonTest, Error, TEXT("%s"), *PythonCommand.CommandResult);
		return PythonCommand.CommandResult;
	}
	
	PythonCommand.ExecutionMode = EPythonCommandExecutionMode::EvaluateStatement;
	PythonCommand.Command = TEXT("'' + detected.lang");
	if (!Python.ExecPythonCommandEx(PythonCommand))
	{
		UE_LOG(LogUEPythonTest, Error, TEXT("%s"), *PythonCommand.CommandResult);
		return PythonCommand.CommandResult;
	}

	return PythonCommand.CommandResult.Mid(1, PythonCommand.CommandResult.Len() - 2);
}

FString UMyBlueprintFunctionLibrary::GetPronunciation(const FString& Text, const FString& SourceLanguage)
{
	auto& Python = *IPythonScriptPlugin::Get();
	
	FPythonCommandEx PythonCommand;
	PythonCommand.ExecutionMode = EPythonCommandExecutionMode::ExecuteFile;
	PythonCommand.Command = FString::Printf(
		TEXT("from googletrans import Translator\ntranslator = Translator()\npronunciation = translator.translate('%s', src='%s', dest='%s')"),
		*Text, *SourceLanguage, *SourceLanguage
	);
	if (!Python.ExecPythonCommandEx(PythonCommand))
	{
		UE_LOG(LogUEPythonTest, Error, TEXT("%s"), *PythonCommand.CommandResult);
		return PythonCommand.CommandResult;
	}
	
	PythonCommand.ExecutionMode = EPythonCommandExecutionMode::EvaluateStatement;
	PythonCommand.Command = TEXT("'' + pronunciation.pronunciation");
	if (!Python.ExecPythonCommandEx(PythonCommand))
	{
		UE_LOG(LogUEPythonTest, Error, TEXT("%s"), *PythonCommand.CommandResult);
		return PythonCommand.CommandResult;
	}

	return PythonCommand.CommandResult.Mid(1, PythonCommand.CommandResult.Len() - 2);
}

TArray<FString> UMyBlueprintFunctionLibrary::GetLanguages()
{
	auto& Python = *IPythonScriptPlugin::Get();
	
	FPythonCommandEx PythonCommand;
	PythonCommand.ExecutionMode = EPythonCommandExecutionMode::ExecuteFile;
	PythonCommand.Command = TEXT("import googletrans");
	if (!Python.ExecPythonCommandEx(PythonCommand))
	{
		UE_LOG(LogUEPythonTest, Error, TEXT("%s"), *PythonCommand.CommandResult);
		return { PythonCommand.CommandResult };
	}
	
	PythonCommand.ExecutionMode = EPythonCommandExecutionMode::EvaluateStatement;
	PythonCommand.Command = TEXT("googletrans.LANGUAGES");
	if (!Python.ExecPythonCommandEx(PythonCommand))
	{
		UE_LOG(LogUEPythonTest, Error, TEXT("%s"), *PythonCommand.CommandResult);
		return { PythonCommand.CommandResult };
	}

	FString LanguagesString = PythonCommand.CommandResult.Mid(1, PythonCommand.CommandResult.Len() - 2);
	LanguagesString.ReplaceInline(TEXT("'"), TEXT(""));
	LanguagesString.ReplaceInline(TEXT(" "), TEXT(""));

	TArray<FString> LanguageCodeAndNames;
	LanguagesString.ParseIntoArray(LanguageCodeAndNames, TEXT(","));

	TArray<FString> LanguageCodes;
	for (const auto& LanguageCodeAndName : LanguageCodeAndNames)
	{
		FString LanguageCode;
		FString LanguageName;
		if (LanguageCodeAndName.Split(TEXT(":"), &LanguageCode, &LanguageName))
		{
			LanguageCodes.Add(LanguageCode);
		}
	}
	
	return LanguageCodes;
}
