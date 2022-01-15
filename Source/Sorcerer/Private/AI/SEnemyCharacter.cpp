


#include "AI/SEnemyCharacter.h"

#include "Components/CapsuleComponent.h"
#include "UI/SActorWidgetComponent.h"

ASEnemyCharacter::ASEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthBarWidget = CreateDefaultSubobject<USActorWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(RootComponent);
	HealthBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
}
