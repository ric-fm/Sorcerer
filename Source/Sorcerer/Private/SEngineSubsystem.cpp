


#include "SEngineSubsystem.h"

#include "AbilitySystemGlobals.h"

void USEngineSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// This is necessary to use TargetData in Gameplay Abilities, otherwise	clients will get errors and disconnected from the server
	UAbilitySystemGlobals::Get().InitGlobalData();
}
