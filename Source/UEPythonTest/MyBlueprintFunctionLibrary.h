// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UEPYTHONTEST_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static FString Translate(const FString& Text, const FString& ToLanguage);

	UFUNCTION(BlueprintCallable)
	static FString Detect(const FString& Text);

	UFUNCTION(BlueprintCallable)
	static FString GetPronunciation(const FString& Text, const FString& SourceLanguage);

	UFUNCTION(BlueprintCallable)
	static TArray<FString> GetLanguages();
};
