// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCharacterBase.h"
#include "SPlayerCharacter.generated.h"

/**
 * Class for Main Character controlled by the User
 */
UCLASS()
class SORCERER_API ASPlayerCharacter : public ASCharacterBase
{
	GENERATED_BODY()
	
public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	
	void LookUp(float Value);
	void Turn(float Value);
};
