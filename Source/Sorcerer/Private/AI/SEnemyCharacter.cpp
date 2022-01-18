#include "AI/SEnemyCharacter.h"

#include "BrainComponent.h"
#include "AI/SAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/SActorWidgetComponent.h"

ASEnemyCharacter::ASEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthBarWidget = CreateDefaultSubobject<USActorWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(RootComponent);
	HealthBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));

	AIControllerClass = ASAIController::StaticClass();
}

void ASEnemyCharacter::HealthChanged(const FOnAttributeChangeData& Data)
{
	Super::HealthChanged(Data);

	if (!IsAlive())
	{
		HealthBarWidget->SetVisibility(false, true);

		AAIController* AIC = Cast<AAIController>(GetController());
		if(AIC)
		{
			AIC->GetBrainComponent()->StopLogic("Killed");
		}
		
		SetLifeSpan(10.0f);
	}
}

void ASEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

}

void ASEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (BehaviorTree)
	{
		AAIController* AIC = Cast<AAIController>(GetController());

		AIC->RunBehaviorTree(BehaviorTree);

		//@TODO At the moment I set the target to the first player
		UBlackboardComponent* BlackboardComponent = AIC->GetBlackboardComponent();
		if(BlackboardComponent)
		{
			ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
			BlackboardComponent->SetValueAsObject("TargetActor", Character);
		}
	}
}
