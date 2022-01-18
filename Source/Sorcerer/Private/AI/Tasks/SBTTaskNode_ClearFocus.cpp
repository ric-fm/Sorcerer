


#include "AI/Tasks/SBTTaskNode_ClearFocus.h"

#include "AIController.h"

EBTNodeResult::Type USBTTaskNode_ClearFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetAIOwner()->ClearFocus(EAIFocusPriority::Gameplay);
	return EBTNodeResult::Succeeded;
}
