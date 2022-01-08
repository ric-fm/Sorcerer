
#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "SAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * Custom AttributeSet Class
 */
UCLASS()
class SORCERER_API USAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	USAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifeTimeProps) const override;
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// Current Health, when 0 we expect owner to die. Capped by MaxHealth
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Sorcerer|Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(USAttributeSet, Health)

	// MaxHealth is its own attribute, since GameplayEffects may modify it
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Sorcerer|Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(USAttributeSet, MaxHealth)

	// Armor reduces the amount of damage done by attackers
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Sorcerer|Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(USAttributeSet, Armor)

	/**
	 * Damage is a meta attribute used by the DamageExecution to calculate final damage, which then turns into -Health
	 * Temporary value that only exist on the Server.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Sorcerer|Attributes")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(USAttributeSet, Damage)

protected:

	/** Helper function to proportionally adjust the value of an attribute when it's associated max attribute changes. */
	void AdjustAttributeForMaxChange(const FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_Armor(const FGameplayAttributeData& OldArmor);

};
