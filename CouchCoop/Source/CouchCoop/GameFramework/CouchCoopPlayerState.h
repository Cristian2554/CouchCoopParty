// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "CouchCoopPlayerState.generated.h"

class UTBAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class COUCHCOOP_API ACouchCoopPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

	ACouchCoopPlayerState();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AbilitySystem, meta = (AllowPrivateAccess = "true"))
	UTBAbilitySystemComponent* AbilitySystemComponent;

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
};
