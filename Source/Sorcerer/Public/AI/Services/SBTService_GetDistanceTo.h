

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SBTService_GetDistanceTo.generated.h"

/**
 * Service to calculate distance from Behavior Tree Owner to a given target
 */
UCLASS()
class SORCERER_API USBTService_GetDistanceTo : public UBTService
{
	GENERATED_BODY()

public:
	USBTService_GetDistanceTo();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Target;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Distance;
};
