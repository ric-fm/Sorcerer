

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "SActorWidgetComponent.generated.h"

/**
 * Extension of the WidgetComponent to make it easy to have Owning Actor context to the Widget
 * Automatically calls SetOwningActor on the Widget
 */
UCLASS()
class SORCERER_API USActorWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	USActorWidgetComponent();

	virtual void InitWidget() override;
};
