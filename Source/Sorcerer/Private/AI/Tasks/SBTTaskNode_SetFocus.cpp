


#include "AI/Tasks/SBTTaskNode_SetFocus.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

USBTTaskNode_SetFocus::USBTTaskNode_SetFocus()
{
	Target.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(USBTTaskNode_SetFocus, Target), AActor::StaticClass());
}

EBTNodeResult::Type USBTTaskNode_SetFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if(ensure(BlackboardComponent))
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(Target.SelectedKeyName));
		if(TargetActor)
		{
			OwnerComp.GetAIOwner()->SetFocus(TargetActor);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
