// Fill out your copyright notice in the Description page of Project Settings.


#include "RadicalActorBase.h"

// Sets default values
ARadicalActorBase::ARadicalActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARadicalActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARadicalActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

