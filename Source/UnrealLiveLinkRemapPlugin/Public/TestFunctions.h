// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TestFunctions.generated.h"

/**
 * 
 */
UCLASS()
class UNREALLIVELINKREMAPPLUGIN_API UTestFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:	

	UFUNCTION(BlueprintCallable, Category = "RadicalTest")
	static void testConnectionAndData();
};
