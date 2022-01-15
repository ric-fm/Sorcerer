// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCharacterBase.h"
#include "SPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
/**
 * Class for Main Character controlled by the User
 */
UCLASS()
class SORCERER_API ASPlayerCharacter : public ASCharacterBase
{
	GENERATED_BODY()
	
public:
	ASPlayerCharacter();
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void OnRep_PlayerState() override;
	virtual FVector GetPawnViewLocation() const override;

	// Get the direction from a given location to the Player Aiming Point
	UFUNCTION(BlueprintCallable, Category = "Sorcerer")
	FVector GetAimingDirectionFrom(const FVector FromLocation) const;
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* CameraComponent;

	// Is Input Bound to Abilities
	bool bAbilitiesInputBound;

	// Bind Input to Abilities (using enum ESAbilityInputID)
	void BindAbilitiesInput();

	
	void MoveForward(float Value);
	void MoveRight(float Value);
	
	void LookUp(float Value);
	void Turn(float Value);
};
