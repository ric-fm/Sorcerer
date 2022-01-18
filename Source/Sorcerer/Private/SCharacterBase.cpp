

#include "SCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "Abilities/SAbilitySystemComponent.h"
#include "Abilities/SAttributeSet.h"
#include "Abilities/SGameplayAbility.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ASCharacterBase::ASCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<USAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<USAttributeSet>(TEXT("AttributeSet"));

	bAbilitiesGranted = false;
	bEffectsApplied = false;

	GetMesh()->SetCollisionProfileName("NoCollision");
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
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetDamageAttribute()).AddUObject(this,&ASCharacterBase::DamageChanged);
	}
}

bool ASCharacterBase::IsAlive() const
{
	return GetHealth() > 0.0f;
}

void ASCharacterBase::SetRagdollEnabled(bool bEnabled)
{
	if(bEnabled)
	{
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetCollisionProfileName("Ragdoll");

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCharacterMovement()->DisableMovement();
	}
	else
	{
		GetMesh()->SetAllBodiesSimulatePhysics(false);
		GetMesh()->SetCollisionProfileName("NoCollision");

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	}
}

bool ASCharacterBase::CanActivateAbilityByTag(FGameplayTagContainer AbilityTags)
{
	if(AbilitySystemComponent)
	{
		TArray<FGameplayAbilitySpec*> Abilities;
		AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTags, Abilities);

		if(Abilities.Num() > 0)
		{
			FGameplayAbilitySpec* AbilitySpec = Abilities[0];
			FGameplayAbilityActorInfo* ActorInfo = AbilitySystemComponent->AbilityActorInfo.Get();
			FGameplayAbilitySpecHandle AbilitySpecHandle = AbilitySpec->Handle;
			
			return AbilitySpec->Ability->CanActivateAbility(AbilitySpecHandle, ActorInfo);
		}
	}
	return false;
}

bool ASCharacterBase::GetRemainingTimeForGameplayEffectByTag(FGameplayTagContainer CooldownTags, float& TimeRemaining,
                                                 float& CooldownDuration)
{
	if (AbilitySystemComponent && CooldownTags.Num() > 0)
	{
		TimeRemaining = 0.f;
		CooldownDuration = 0.f;

		FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTags);
		TArray< TPair<float, float> > DurationAndTimeRemaining = AbilitySystemComponent->GetActiveEffectsTimeRemainingAndDuration(Query);
		if (DurationAndTimeRemaining.Num() > 0)
		{
			int32 BestIdx = 0;
			float LongestTime = DurationAndTimeRemaining[0].Key;
			for (int32 Idx = 1; Idx < DurationAndTimeRemaining.Num(); ++Idx)
			{
				if (DurationAndTimeRemaining[Idx].Key > LongestTime)
				{
					LongestTime = DurationAndTimeRemaining[Idx].Key;
					BestIdx = Idx;
				}
			}

			TimeRemaining = DurationAndTimeRemaining[BestIdx].Key;
			CooldownDuration = DurationAndTimeRemaining[BestIdx].Value;

			return true;
		}
	}

	return false;
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
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, GetCharacterLevel(), static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID), this));
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

void ASCharacterBase::DamageChanged(const FOnAttributeChangeData& Data)
{
	if(Data.GEModData)
	{
		const FHitResult* HitResult = Data.GEModData->EffectSpec.GetContext().GetHitResult();
		if(HitResult)
		{
			ESHitDirection HitDirection = GetHitDirection(HitResult->Location);
			OnHit.Broadcast(HitDirection);
		}
	}
}

ESHitDirection ASCharacterBase::GetHitDirection(const FVector& ImpactPoint)
{
	const FVector& ActorLocation = GetActorLocation();
	// PointPlaneDist is super cheap - 1 vector subtraction, 1 dot product.
	float DistanceToFrontBackPlane = FVector::PointPlaneDist(ImpactPoint, ActorLocation, GetActorRightVector());
	float DistanceToRightLeftPlane = FVector::PointPlaneDist(ImpactPoint, ActorLocation, GetActorForwardVector());


	if (FMath::Abs(DistanceToFrontBackPlane) <= FMath::Abs(DistanceToRightLeftPlane))
	{
		// Determine if Front or Back

		// Can see if it's left or right of Left/Right plane which would determine Front or Back
		if (DistanceToRightLeftPlane >= 0)
		{
			return ESHitDirection::Front;
		}
		else
		{
			return ESHitDirection::Back;
		}
	}
	else
	{
		// Determine if Right or Left

		if (DistanceToFrontBackPlane >= 0)
		{
			return ESHitDirection::Right;
		}
		else
		{
			return ESHitDirection::Left;
		}
	}
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

