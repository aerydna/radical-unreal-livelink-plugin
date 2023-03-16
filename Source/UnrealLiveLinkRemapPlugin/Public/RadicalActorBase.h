// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RadicalEditor.h"
#include "RadicalActorBase.generated.h"

UCLASS()
class UNREALLIVELINKREMAPPLUGIN_API ARadicalActorBase : public AActor
{
	GENERATED_BODY()
	
	UFUNCTION()
	TArray<FString> GetPlayerList() const {

		auto radSubSystem = GEditor->GetEditorSubsystem<URadicalEditor>();
		if (!radSubSystem)
			return TArray<FString>();
		return radSubSystem->getAvailablePlayers();
	}
public:	
	// Sets default values for this actor's properties
	ARadicalActorBase();

	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (GetOptions = "GetPlayerList"))
	FString PlayerID;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
