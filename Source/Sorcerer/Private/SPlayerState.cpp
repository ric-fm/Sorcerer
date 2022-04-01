


#include "SPlayerState.h"

#include "Abilities/SAbilitySystemComponent.h"
#include "Abilities/SAttributeSet.h"

ASPlayerState::ASPlayerState()
{
	NetUpdateFrequency = 100.0f;
	
	AbilitySystemComponent = CreateDefaultSubobject<USAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	AttributeSet = CreateDefaultSubobject<USAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* ASPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
