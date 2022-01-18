

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AbilitySystemComponent.h"
#include "SBTTaskNode_UseAbilityByTag.generated.h"

/**
 * Task to use Gameplay Ability. It will be finished on Ability end
 */
UCLASS()
class SORCERER_API USBTTaskNode_UseAbilityByTag : public UBTTaskNode
{
	GENERATED_BODY()
public:
	USBTTaskNode_UseAbilityByTag();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// The Tags of the Ability to play 
	UPROPERTY(EditAnywhere, Meta = (Categories = "Ability"))
	FGameplayTagContainer AbilityTags;

	// Reference to Ability Spec Handle to cancel ability on abort
	FGameplayAbilitySpecHandle AbilitySpecHandle;

	//Reference to AbilitySystemComponent.OnAbilityEnded to finish the task correctly
	FDelegateHandle AbilityEndedDelegate;

	// Reference to BehaviorTreeComponent to call FinishLatentAction on Ability ended.
	UPROPERTY()
	UBehaviorTreeComponent* BTComp;

	UFUNCTION()
	void OnAbilityEnded(const FAbilityEndedData& Data);
};
