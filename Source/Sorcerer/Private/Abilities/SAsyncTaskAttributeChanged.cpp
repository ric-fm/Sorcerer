


#include "Abilities/SAsyncTaskAttributeChanged.h"


USAsyncTaskAttributeChanged* USAsyncTaskAttributeChanged::ListenForAttributeChange(
	UAbilitySystemComponent* AbilitySystemComponent, FGameplayAttribute Attribute)
{
	USAsyncTaskAttributeChanged* WaitForAttributeChangedTask = NewObject<USAsyncTaskAttributeChanged>();
	WaitForAttributeChangedTask->AbilitySystemComponent = AbilitySystemComponent;
	WaitForAttributeChangedTask->AttributeToListenFor = Attribute;

	if(!IsValid(AbilitySystemComponent) || !Attribute.IsValid())
	{
		WaitForAttributeChangedTask->RemoveFromRoot();
		return nullptr;
	}

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(WaitForAttributeChangedTask, &USAsyncTaskAttributeChanged::AttributeChanged);

	return WaitForAttributeChangedTask;
}

USAsyncTaskAttributeChanged* USAsyncTaskAttributeChanged::ListenForAttributesChange(
	UAbilitySystemComponent* AbilitySystemComponent, TArray<FGameplayAttribute> Attributes)
{
	USAsyncTaskAttributeChanged* WaitForAttributeChangedTask = NewObject<USAsyncTaskAttributeChanged>();
	WaitForAttributeChangedTask->AbilitySystemComponent = AbilitySystemComponent;
	WaitForAttributeChangedTask->AttributesToListenFor = Attributes;

	if(!IsValid(AbilitySystemComponent) || Attributes.Num() == 0)
	{
		WaitForAttributeChangedTask->RemoveFromRoot();
		return nullptr;
	}

	for(FGameplayAttribute Attribute : Attributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(WaitForAttributeChangedTask, &USAsyncTaskAttributeChanged::AttributeChanged);
	}

	return WaitForAttributeChangedTask;
}

void USAsyncTaskAttributeChanged::EndTask()
{
	if(IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeToListenFor).RemoveAll(this);

		for(FGameplayAttribute Attribute : AttributesToListenFor)
		{
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).RemoveAll(this);
		}
	}

	SetReadyToDestroy();
	MarkPendingKill();
}

void USAsyncTaskAttributeChanged::AttributeChanged(const FOnAttributeChangeData& Data)
{
	OnAttributeChanged.Broadcast(Data.Attribute, Data.NewValue, Data.OldValue);
}
