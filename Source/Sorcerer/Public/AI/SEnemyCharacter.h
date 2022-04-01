

#pragma once

#include "CoreMinimal.h"
#include "SCharacterBase.h"
#include "SEnemyCharacter.generated.h"

class UBehaviorTree;
class USActorWidgetComponent;

/**
 * Base class for Sorcerer Enemy Characters 
 */
UCLASS()
class SORCERER_API ASEnemyCharacter : public ASCharacterBase
{
	GENERATED_BODY()

public:
	ASEnemyCharacter();

protected:
	UPROPERTY()
	USAbilitySystemComponent* HardRefAbilitySystemComponent;

	UPROPERTY()
	USAttributeSet* HardRefAttributeSet;
	
protected:
	virtual void HealthChanged(const FOnAttributeChangeData& Data) override;
public:
	virtual void PossessedBy(AController* NewController) override;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sorcerer|AI")
	UBehaviorTree* BehaviorTree;
	
	// Health 3D World Widget
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sorcerer|Components")
	USActorWidgetComponent* HealthBarWidget;
	
	
};
