
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "SCharacterBase.generated.h"

struct FOnAttributeChangeData;
struct FGameplayTagContainer;
class UGameplayEffect;
class USGameplayAbility;
class USAttributeSet;
class USAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttributeChangedDelegate, ASCharacterBase*, Owner, float, NewValue);

/**
 * Base Class for Sorcerer Characters to support Gameplay Ability System
 */
UCLASS()
class SORCERER_API ASCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	friend class USAttributeSet;

public:
	ASCharacterBase();

protected:
	virtual void BeginPlay() override;
	
public:
	// Implements IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	virtual void PossessedBy(AController* NewController) override;

	
	// Get the current character level that is passed to the Ability System 
	UFUNCTION(BlueprintCallable, Category = "Sorcerer|Attributes")
	int32 GetCharacterLevel() const;

	/// Get the current character health 
	UFUNCTION(BlueprintCallable, Category = "Sorcerer|Attributes")
	float GetHealth() const;

	// Get the character max health 
	UFUNCTION(BlueprintCallable, Category = "Sorcerer|Attributes")
	float GetMaxHealth() const;
	
	// Helper to check if player is alive (Health > 0)
	UFUNCTION(BlueprintCallable, Category = "Sorcerer|Attributes")
	virtual bool IsAlive() const;

	UPROPERTY(BlueprintAssignable, Category = "Sorcerer|Attributes")
	FAttributeChangedDelegate OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Sorcerer|Attributes")
	FAttributeChangedDelegate OnMaxHealthChanged;

protected:
	UPROPERTY(BlueprintReadOnly)
	USAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	USAttributeSet* AttributeSet;

	// Abilities for this Character granted on startup.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sorcerer|Abilities")
	TArray<TSubclassOf<USGameplayAbility>> StartupAbilities;

	// Effects applied to this Character on startup.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sorcerer|Abilities")
	TArray<TSubclassOf<UGameplayEffect>> StartupEffects;

	// Is Startup Gameplay Abilities granted
	bool bAbilitiesGranted;
	
	// Is Startup Gameplay Effects applied
	bool bEffectsApplied;

	// Grant startup abilities (run on the Server, replicated on the owning client)
	virtual void AddStartupAbilities();

	// Revoke startup abilities (run on the Server, replicated on the owning client)
	virtual void RemoveStartupAbilities();

	// Apply startup effects (run on the Server, replicated on the owning client)
	virtual void AddStartupEffects();

	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);

};
