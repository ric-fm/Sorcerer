


#include "AI/Decorators/SBTDecorator_CheckAbilityByTag.h"

#include "AIController.h"
#include "SCharacterBase.h"

bool USBTDecorator_CheckAbilityByTag::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
                                                                 uint8* NodeMemory) const
{
	ASCharacterBase* Character = Cast<ASCharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if(Character)
	{
		return Character->CanActivateAbilityByTag(AbilityTags);
	}
	return false;
}
