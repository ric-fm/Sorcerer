

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "SBTDecorator_ClearSightTo.generated.h"

/**
 * Decorator node to check if the Character has a clear sight to the given target.
 * It performs a Sphere Trace by Object Types (ECC_WorldStatic, ECC_WorldDynamic, ECC_Pawn).
 */
UCLASS()
class SORCERER_API USBTDecorator_ClearSightTo : public UBTDecorator
{
	GENERATED_BODY()
public:
	USBTDecorator_ClearSightTo();
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Target;

	UPROPERTY(EditAnywhere)
	float TraceRadius;
};
