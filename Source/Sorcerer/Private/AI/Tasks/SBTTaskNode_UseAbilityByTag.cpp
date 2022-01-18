


#include "AI/Tasks/SBTTaskNode_UseAbilityByTag.h"

#include "AIController.h"
#include "SCharacterBase.h"

USBTTaskNode_UseAbilityByTag::USBTTaskNode_UseAbilityByTag()
{
	// Set bCreateNodeInstance to true to handle latent task ending correctly.
	// If not, if there are multiple enemies on the world, the last one spawned receive
	// OnAbilityEnded events off all characters
	bCreateNodeInstance = true;
}

EBTNodeResult::Type USBTTaskNode_UseAbilityByTag::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ASCharacterBase* Character = Cast<ASCharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if(Character)
	{
		UAbilitySystemComponent* AbilitySystemComponent = Character->GetAbilitySystemComponent();
		if(AbilitySystemComponent)
		{			
			BTComp = &OwnerComp;

			TArray<FGameplayAbilitySpec*> Abilities;
			AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTags, Abilities);


			if(Abilities.Num() > 0)
			{
				AbilitySpecHandle = Abilities[0]->Handle;
				if(AbilitySpecHandle.IsValid())
				{
					AbilityEndedDelegate = AbilitySystemComponent->OnAbilityEnded.AddUObject(this, &USBTTaskNode_UseAbilityByTag::OnAbilityEnded);
					bool bActivated = AbilitySystemComponent->TryActivateAbility(AbilitySpecHandle);
					if(bActivated)
					{
						return EBTNodeResult::InProgress;
					}
					else
					{
						return EBTNodeResult::Failed;
					}
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}

EBTNodeResult::Type USBTTaskNode_UseAbilityByTag::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ASCharacterBase* Character = Cast<ASCharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if(Character)
	{
		UAbilitySystemComponent* AbilitySystemComponent = Character->GetAbilitySystemComponent();
		if(AbilitySystemComponent)
		{
			if(AbilitySpecHandle.IsValid())
			{
				AbilitySystemComponent->CancelAbilityHandle(AbilitySpecHandle);
			}
		}
	}
	return EBTNodeResult::Aborted;
}

void USBTTaskNode_UseAbilityByTag::OnAbilityEnded(const FAbilityEndedData& Data)
{
	ASCharacterBase* Character = Cast<ASCharacterBase>(BTComp->GetAIOwner()->GetPawn());
	if(Data.AbilitySpecHandle == AbilitySpecHandle)
	{
		if(Character)
		{
			Character->GetAbilitySystemComponent()->OnAbilityEnded.Remove(AbilityEndedDelegate);
		}
		FinishLatentTask(*BTComp, (Data.bWasCancelled ? EBTNodeResult::Failed : EBTNodeResult::Succeeded));
	}
}
