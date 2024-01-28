// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CouchCoopGameMode.generated.h"

UCLASS(minimalapi)
class ACouchCoopGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACouchCoopGameMode();

	virtual void BeginPlay() override;

	void CreateAllUsers() const;
};



