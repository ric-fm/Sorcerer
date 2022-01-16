
#pragma once

#include "CoreMinimal.h"

SORCERER_API DECLARE_LOG_CATEGORY_EXTERN(LogSorcerer, Log, All);

UENUM(BlueprintType)
enum class ESAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	Jump,
	Ability1,
	Ability2,
	Ability3
};

UENUM(BlueprintType)
enum class ESHitDirection : uint8
{
	None,
	Front,
	Back,
	Left,
	Right
};
