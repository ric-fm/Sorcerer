
#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Sorcerer/Sorcerer.h"
#include "SGameplayAbility.generated.h"

/**
 * Base Class for Sorcerer Gameplay Abilities
 */
UCLASS()
class SORCERER_API USGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	// Abilities with this set will automatically activate when the input is pressed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sorcerer|Ability")
	ESAbilityInputID AbilityInputID = ESAbilityInputID::None;
	
};
