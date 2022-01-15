

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SProjectile_Explosive.generated.h"

/**
 * Projectile that apply effect in a radius
 */
UCLASS()
class SORCERER_API ASProjectile_Explosive : public ASProjectileBase
{
	GENERATED_BODY()
	
public:
	ASProjectile_Explosive();
	
	virtual void OnHitWorld_Implementation(const FHitResult& Hit) override;
	virtual void OnHitCharacter_Implementation(const FHitResult& Hit) override;

protected:
	// Radius of the explosion. Effects will applied to Characters in this radius
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn), Category = "Sorcerer|Projectile")
	float ExplosionRadius;

	void ApplyRadiusEffects();
};
