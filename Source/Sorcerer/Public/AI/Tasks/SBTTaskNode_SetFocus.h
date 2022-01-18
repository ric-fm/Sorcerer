

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SBTTaskNode_SetFocus.generated.h"

/**
 * Task to set the AIController focus, so the character look to the focused actor
 */
UCLASS()
class SORCERER_API USBTTaskNode_SetFocus : public UBTTaskNode
{
	GENERATED_BODY()
public:
	USBTTaskNode_SetFocus();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Target;
};
