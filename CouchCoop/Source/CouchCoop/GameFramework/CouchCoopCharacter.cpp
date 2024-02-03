// Copyright Epic Games, Inc. All Rights Reserved.

#include "CouchCoopCharacter.h"
#include "CouchCoopPlayerState.h"
#include "Engine/LocalPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/TBAbilitySystemComponent.h"
#include "Components/TBAbilityInputBindingComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ACouchCoopCharacter

ACouchCoopCharacter::ACouchCoopCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	AbilityInputBindingComponent = CreateDefaultSubobject<UTBAbilityInputBindingComponent>(TEXT("Input Binding Component"));
}

void ACouchCoopCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void ACouchCoopCharacter::PossessedBy(AController* newController)
{
	Super::PossessedBy(newController);
	
	LocalInitialization();
}

void ACouchCoopCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	LocalInitialization();
}

void ACouchCoopCharacter::LocalInitialization()
{
	//Add Input Mapping Context
	if (APlayerController* playerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Initialize Ability System Component
	if(ACouchCoopPlayerState* playerState = GetPlayerState<ACouchCoopPlayerState>())
	{
		AbilitySystemComponent = Cast<UTBAbilitySystemComponent>(playerState->GetAbilitySystemComponent());
		playerState->GetAbilitySystemComponent()->InitAbilityActorInfo(playerState, this);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ACouchCoopCharacter::SetupPlayerInputComponent(UInputComponent* playerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(playerInputComponent)) {
		// Moving
		enhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACouchCoopCharacter::Move);

		// Looking
		enhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACouchCoopCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ACouchCoopCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ACouchCoopCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

UAbilitySystemComponent* ACouchCoopCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

UTBAbilityInputBindingComponent* ACouchCoopCharacter::GetInputBindingComponent() const
{
	return AbilityInputBindingComponent;
}