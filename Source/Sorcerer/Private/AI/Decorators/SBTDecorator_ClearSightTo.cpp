


#include "AI/Decorators/SBTDecorator_ClearSightTo.h"

#include "AIController.h"
#include "CollisionDebugDrawingPublic.h"
#include "BehaviorTree/BlackboardComponent.h"

USBTDecorator_ClearSightTo::USBTDecorator_ClearSightTo()
	: TraceRadius(30.0f)
{
	Target.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(USBTDecorator_ClearSightTo, Target), AActor::StaticClass());
}

bool USBTDecorator_ClearSightTo::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if(ensure(BlackboardComponent))
	{
		AActor* SourceActor = OwnerComp.GetAIOwner()->GetPawn();
		AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(Target.SelectedKeyName));

		FHitResult HitResult;
		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
		FCollisionShape CollisionShape;
		CollisionShape.SetSphere(TraceRadius);
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(SourceActor);
		
		bool bHit = GetWorld()->SweepSingleByObjectType(HitResult, SourceActor->GetActorLocation(), TargetActor->GetActorLocation(), FQuat::Identity, ObjectQueryParams, CollisionShape, QueryParams);

		return bHit && HitResult.Actor == TargetActor;
	}
	return false;
}
