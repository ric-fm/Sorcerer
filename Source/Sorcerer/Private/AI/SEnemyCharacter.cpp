


#include "AI/SEnemyCharacter.h"

#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/SActorWidgetComponent.h"

ASEnemyCharacter::ASEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthBarWidget = CreateDefaultSubobject<USActorWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(RootComponent);
	HealthBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
}

void ASEnemyCharacter::HealthChanged(const FOnAttributeChangeData& Data)
{
	Super::HealthChanged(Data);

	if(!IsAlive())
	{
		HealthBarWidget->SetVisibility(false, true);
		
		SetLifeSpan(10.0f);
	}
}
