

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "SBTDecorator_CheckAbilityByTag.generated.h"

/**
 * Check if Owner has and Ability with a given tag an if is currently activatable 
 */
UCLASS()
class SORCERER_API USBTDecorator_CheckAbilityByTag : public UBTDecorator
{
	GENERATED_BODY()
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	// The Tags of the Ability to check if can be activated
	UPROPERTY(EditAnywhere, Meta = (Categories = "Ability"))
	FGameplayTagContainer AbilityTags;
};
