

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "SBTDecorator_Chance.generated.h"

/**
 * Decorator node that randomly success with a given probability 
 */
UCLASS()
class SORCERER_API USBTDecorator_Chance : public UBTDecorator
{
	GENERATED_BODY()
public:
	USBTDecorator_Chance();
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
	uint8 Probability;
};
