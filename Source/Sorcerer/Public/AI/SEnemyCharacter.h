

#pragma once

#include "CoreMinimal.h"
#include "SCharacterBase.h"
#include "SEnemyCharacter.generated.h"

class USActorWidgetComponent;

/**
 * Base class for Sorcerer Enemy Characters 
 */
UCLASS()
class SORCERER_API ASEnemyCharacter : public ASCharacterBase
{
	GENERATED_BODY()

public:
	ASEnemyCharacter();
	
protected:
	virtual void HealthChanged(const FOnAttributeChangeData& Data) override;

	// Health 3D World Widget
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sorcerer|Components")
	USActorWidgetComponent* HealthBarWidget;
	
	
};
