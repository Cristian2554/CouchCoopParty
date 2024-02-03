// Fill out your copyright notice in the Description page of Project Settings.


#include "CouchCoopPlayerState.h"
#include "Components/TBAbilitySystemComponent.h"

ACouchCoopPlayerState::ACouchCoopPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UTBAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	NetUpdateFrequency = 100.0f;
}

UAbilitySystemComponent* ACouchCoopPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
