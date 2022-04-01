

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

class USAttributeSet;
class USAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class SORCERER_API ASPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ASPlayerState();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;


	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Sorcerer|Abilities")
	USAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Sorcerer|Abilities")
	USAttributeSet* AttributeSet;
	
};
