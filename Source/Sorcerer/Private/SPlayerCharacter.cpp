#include "SPlayerCharacter.h"

#include "Abilities/GameplayAbilityTypes.h"
#include "Abilities/SAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Sorcerer/Sorcerer.h"

ASPlayerCharacter::ASPlayerCharacter()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void ASPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASPlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ASPlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &ASPlayerCharacter::Turn);

	// Bind player input to the AbilitySystemComponent. Also called in OnRep_PlayerState because of a potential race condition.
	BindAbilitiesInput();
}

void ASPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Bind player input to the AbilitySystemComponent. Also called in SetupPlayerInputComponent because of a potential race condition.
	BindAbilitiesInput();
}

void ASPlayerCharacter::MoveForward(float Value)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = ControlRotation.Roll = 0.0f;

	AddMovementInput(ControlRotation.Vector(), Value);
}

void ASPlayerCharacter::MoveRight(float Value)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = ControlRotation.Roll = 0.0f;

	AddMovementInput(FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::Y), Value);
}

void ASPlayerCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ASPlayerCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ASPlayerCharacter::BindAbilitiesInput()
{
	if (!bAbilitiesInputBound && AbilitySystemComponent && IsValid(InputComponent))
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "ESAbilityInputID",
		                                       static_cast<int32>(ESAbilityInputID::Confirm),
		                                       static_cast<int32>(ESAbilityInputID::Cancel));

		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);

		bAbilitiesInputBound = true;
	}
}
