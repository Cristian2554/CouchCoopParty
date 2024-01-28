// Copyright Epic Games, Inc. All Rights Reserved.

#include "CouchCoopGameMode.h"
#include "CouchCoopCharacter.h"
#include "GameFramework/InputDeviceLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ACouchCoopGameMode::ACouchCoopGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ACouchCoopGameMode::BeginPlay()
{
	Super::BeginPlay();

	CreateAllUsers();
}

void ACouchCoopGameMode::CreateAllUsers() const
{
	TArray<FPlatformUserId> outUsers;
	UInputDeviceLibrary::GetAllActiveUsers(outUsers);

	for(const FPlatformUserId& user : outUsers)
	{
		UGameplayStatics::CreatePlayerFromPlatformUser(GetWorld(), user, true);
		
	}
}
