

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileBase.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

// Base Class for Sorcerer Projectiles
UCLASS(Abstract)
class SORCERER_API ASProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	virtual void PostInitializeComponents() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sorcerer|Components")
	USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sorcerer|Components")
	UParticleSystemComponent* ParticleSystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sorcerer|Components")
	UProjectileMovementComponent* ProjectileMovementComponent;

	// Max Range of the projectile. If the actor does not hit anything, it will be destroyed
	// MaxRange <= 0 for infinite range (not recommended)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sorcerer|Projectile")
	float MaxRange;

	// FX Spawned on Projectile explosion
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sorcerer|Projectile")
	UParticleSystem* ExplosionFX;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void Explode();
};
