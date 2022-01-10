

#include "SCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "Abilities/SAbilitySystemComponent.h"
#include "Abilities/SAttributeSet.h"
#include "Abilities/SGameplayAbility.h"

ASCharacterBase::ASCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<USAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<USAttributeSet>(TEXT("AttributeSet"));

	bAbilitiesGranted = false;
	bEffectsApplied = false;
}

void ASCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

UAbilitySystemComponent* ASCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ASCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if(AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		AddStartupAbilities();
		AddStartupEffects();

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this,&ASCharacterBase::HealthChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute()).AddUObject(this,&ASCharacterBase::MaxHealthChanged);
	}
}

bool ASCharacterBase::IsAlive() const
{
	return GetHealth() > 0.0f;
}

void ASCharacterBase::AddStartupAbilities()
{
	// Grant abilities (only on server)
	if(bAbilitiesGranted || GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	for(TSubclassOf<USGameplayAbility>& Ability : StartupAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, GetCharacterLevel(), INDEX_NONE, this));
	}

	bAbilitiesGranted = true;
}

void ASCharacterBase::RemoveStartupAbilities()
{
	// Revoke abilities (only on server)
	if(!bAbilitiesGranted || GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	// Get all active startup abilities
	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for(const FGameplayAbilitySpec& AbilitySpec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if((AbilitySpec.SourceObject == this) && StartupAbilities.Contains(AbilitySpec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add(AbilitySpec.Handle);
		}
	}

	// Second pass to remove abilities
	for(int32 i = 0; i < AbilitiesToRemove.Num(); ++i)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}

	bAbilitiesGranted = false;
}

void ASCharacterBase::AddStartupEffects()
{
	// Apply effects (only on server)
	if(bEffectsApplied || GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	for(TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle GameplayEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContextHandle);
		if(GameplayEffectSpecHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*GameplayEffectSpecHandle.Data.Get(), AbilitySystemComponent);
		}
	}

	bEffectsApplied = true;
}

void ASCharacterBase::HealthChanged(const FOnAttributeChangeData& Data)
{
	OnHealthChanged.Broadcast(this, Data.NewValue);
}

void ASCharacterBase::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	OnMaxHealthChanged.Broadcast(this, Data.NewValue);
}

int32 ASCharacterBase::GetCharacterLevel() const
{
	return 1;
}

float ASCharacterBase::GetHealth() const
{
	if(AttributeSet == nullptr)
	{
		return 1.0f;
	}
	return AttributeSet->GetHealth();
}

float ASCharacterBase::GetMaxHealth() const
{
	if(AttributeSet == nullptr)
	{
		return 1.0f;
	}
	return AttributeSet->GetMaxHealth();
}

