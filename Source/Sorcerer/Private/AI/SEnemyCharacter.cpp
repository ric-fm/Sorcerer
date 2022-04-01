#include "AI/SEnemyCharacter.h"

#include "BrainComponent.h"
#include "Abilities/SAbilitySystemComponent.h"
#include "Abilities/SAttributeSet.h"
#include "AI/SAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/SActorWidgetComponent.h"

ASEnemyCharacter::ASEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	HardRefAbilitySystemComponent = CreateDefaultSubobject<USAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	HardRefAbilitySystemComponent->SetIsReplicated(true);
	HardRefAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	AbilitySystemComponent = HardRefAbilitySystemComponent;
	
	HardRefAttributeSet = CreateDefaultSubobject<USAttributeSet>(TEXT("AttributeSet"));
	AttributeSet = HardRefAttributeSet;

	HealthBarWidget = CreateDefaultSubobject<USActorWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(RootComponent);
	HealthBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));

	AutoPossessAI = EAutoPossessAI::PlacedInWorld;
	AIControllerClass = ASAIController::StaticClass();
}

void ASEnemyCharacter::HealthChanged(const FOnAttributeChangeData& Data)
{
	Super::HealthChanged(Data);

	if (!IsAlive())
	{
		HealthBarWidget->SetVisibility(false, true);

		AAIController* AIC = Cast<AAIController>(GetController());
		if(AIC && AIC->GetBrainComponent())
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

	if(AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		AddStartupAbilities();
		AddStartupEffects();
		SetupListeners();
	}

	if(!HasAuthority())
	{
		return;
	}
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
