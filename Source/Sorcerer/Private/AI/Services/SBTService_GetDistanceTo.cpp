


#include "AI/Services/SBTService_GetDistanceTo.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

USBTService_GetDistanceTo::USBTService_GetDistanceTo()
{
	Target.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(USBTService_GetDistanceTo, Target), AActor::StaticClass());

	Distance.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(USBTService_GetDistanceTo, Distance));
}

void USBTService_GetDistanceTo::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	float DistanceToTarget = INFINITY;

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if(ensure(BlackboardComponent))
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(Target.SelectedKeyName));
		if(TargetActor)
		{
			DistanceToTarget = FVector::Dist( OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), TargetActor->GetActorLocation());
		}
	}
	BlackboardComponent->SetValueAsFloat(Distance.SelectedKeyName, DistanceToTarget);
}
