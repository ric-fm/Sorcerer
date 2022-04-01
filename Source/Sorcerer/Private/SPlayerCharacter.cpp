#include "SPlayerCharacter.h"

#include "SPlayerState.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Abilities/SAbilitySystemComponent.h"
#include "Abilities/SAttributeSet.h"
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

void ASPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!GetPlayerState() || GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	ASPlayerState* PS = Cast<ASPlayerState>(GetPlayerState());
	if(PS)
	{
		AbilitySystemComponent = PS->AbilitySystemComponent;
		AttributeSet = PS->AttributeSet;

		AbilitySystemComponent->InitAbilityActorInfo(PS, this);
		AddStartupAbilities();
		AddStartupEffects();
		SetupListeners();
	}
}

void ASPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ASPlayerState* PS = Cast<ASPlayerState>(GetPlayerState());
	if(PS)
	{
		AbilitySystemComponent = PS->AbilitySystemComponent;
		AttributeSet = PS->AttributeSet;

		AbilitySystemComponent->InitAbilityActorInfo(PS, this);

		// Bind player input to the AbilitySystemComponent. Also called in SetupPlayerInputComponent because of a potential race condition.
		BindAbilitiesInput();
	}
}

FVector ASPlayerCharacter::GetPawnViewLocation() const
{
	return CameraComponent->GetComponentLocation();
}

FVector ASPlayerCharacter::GetAimingDirectionFrom(const FVector FromLocation) const
{
	FHitResult HitResult;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	
	FCollisionShape Shape;
	Shape.SetSphere(25.0f);

	FVector StartLocation = GetPawnViewLocation();
	FVector EndLocation = StartLocation + GetControlRotation().Vector() * 5000.0f;
	
	bool bHit = GetWorld()->SweepSingleByObjectType(HitResult, StartLocation, EndLocation, FQuat::Identity, ObjectQueryParams, Shape, QueryParams);
	if(bHit)
	{
		EndLocation = HitResult.ImpactPoint;
	}
	
	FVector Direction = EndLocation - FromLocation;
	Direction.Normalize();

	return Direction;
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
	if (bAbilitiesInputBound || !GetPlayerState() || !AbilitySystemComponent.IsValid() || !IsValid(InputComponent))
	{
		return;
	}
	
	const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "ESAbilityInputID",
	                                       static_cast<int32>(ESAbilityInputID::Confirm),
	                                       static_cast<int32>(ESAbilityInputID::Cancel));

	AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);

	bAbilitiesInputBound = true;
}
