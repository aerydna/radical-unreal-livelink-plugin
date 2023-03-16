// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameFramework/SaveGame.h"
#include "MyDeveloperSettings.generated.h"

/**
 * 
 */

UCLASS(config = RadicalSettings, defaultconfig, meta = (DisplayName = "Radical Live"))
class URadicalSettings : public UDeveloperSettings {
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Radical Live")
    static const FString GetWebsocketEndpoint() { return GetMutableDefault<URadicalSettings>()->_WebSocket; }

private:
    // UDeveloperSettings implementations
    FName GetCategoryName() const override {
        static const FName categoryName(TEXT("Radical Plugin Settings"));
        return categoryName;
    }

    UPROPERTY(EditAnywhere, config, Category = "Radical Live", meta = (DisplayName = "Websocket Endpoint", AllowPrivateAccess = true))
        FString _WebSocket{ L"https://radicalmotion.com/wp-json/radapi/v4/user/liveauth" };
};
