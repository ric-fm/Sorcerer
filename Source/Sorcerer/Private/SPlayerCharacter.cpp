
#include "SPlayerCharacter.h"

void ASPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASPlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ASPlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &ASPlayerCharacter::Turn);
}

void ASPlayerCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void ASPlayerCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void ASPlayerCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ASPlayerCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}
