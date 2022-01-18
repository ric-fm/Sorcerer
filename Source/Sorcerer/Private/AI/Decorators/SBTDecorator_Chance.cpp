


#include "AI/Decorators/SBTDecorator_Chance.h"

USBTDecorator_Chance::USBTDecorator_Chance()
	: Probability(50)
{
}

bool USBTDecorator_Chance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if(Probability == 0)
		return false;

	if(Probability == 100)
		return true;
	
	int RandValue = FMath::RandRange(0,99);

	 return RandValue < Probability;
}
