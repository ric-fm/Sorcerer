

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileBase.generated.h"

class UGameplayEffect;
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
	UFUNCTION(BlueprintNativeEvent)
	void OnHitWorld(const FHitResult& Hit);

	UFUNCTION(BlueprintNativeEvent)
	void OnHitCharacter(const FHitResult& Hit);
	
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

	// Gameplay Effects applied to the Hit Actor
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sorcerer|Projectile")
	TArray<TSubclassOf<UGameplayEffect>> EffectClasses;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void Explode();

	void ApplyEffects(const FHitResult& Hit);
};
