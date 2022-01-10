

#include "Abilities/SDamageExecutionCalculation.h"

#include "Abilities/SAttributeSet.h"

// Declare the attributes to capture and define how we want to capture them from the Source and Target.
struct SDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);

	SDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(USAttributeSet, Damage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USAttributeSet, Armor, Target, false);
	}
};

static const SDamageStatics& DamageStatics()
{
	static SDamageStatics DamageStatics;
	return DamageStatics;
}

USDamageExecutionCalculation::USDamageExecutionCalculation()
{
	RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
}

void USDamageExecutionCalculation::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	// Get relevant info

	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// Get Target Armor
	float Armor = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, Armor);
	Armor = FMath::Max<float>(Armor, 0.0f);

	// Get Source Damage
	float DamageBase = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef, EvaluationParameters, DamageBase);

	// Apply final damage
	float FinalDamage = DamageBase * (100.0f / (100.0f + Armor));

	if(FinalDamage > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier((FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Additive, FinalDamage)));
	}
}
