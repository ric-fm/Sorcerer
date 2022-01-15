

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SActorWidget.generated.h"

/**
 * Base class for UMG Widgets that belong to a single Actor in the World via SActorWidgetComponent
 */
UCLASS(Abstract)
class SORCERER_API USActorWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Set the owning actor so widgets have access to
	UFUNCTION(BlueprintCallable, Category = "Sorcerer|UI")
	void SetOwningActor(AActor* NewOwner);

	// Get the owning actor
	AActor* GetOwningActor() const;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Socerer|UI")
	AActor* OwningActor;
};
