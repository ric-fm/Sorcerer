

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SBTTaskNode_ClearFocus.generated.h"

/**
 * Task to clear the AIController focus
 */
UCLASS()
class SORCERER_API USBTTaskNode_ClearFocus : public UBTTaskNode
{
	GENERATED_BODY()
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
