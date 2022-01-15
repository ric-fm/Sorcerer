


#include "SProjectile_Explosive.h"


ASProjectile_Explosive::ASProjectile_Explosive()
{
	ExplosionRadius = 500.0f;
}

void ASProjectile_Explosive::OnHitWorld_Implementation(const FHitResult& Hit)
{
	ApplyRadiusEffects();
	
	Explode();
}

void ASProjectile_Explosive::OnHitCharacter_Implementation(const FHitResult& Hit)
{
	if(Hit.Actor != GetInstigator())
	{
		ApplyRadiusEffects();
	
		Explode();
	}
}

void ASProjectile_Explosive::ApplyRadiusEffects()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	
	FCollisionShape Shape;
	Shape.SetSphere(ExplosionRadius);
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetInstigator());
	
	TArray<FHitResult> HitResults;
	bool bHit = GetWorld()->SweepMultiByObjectType(HitResults, GetActorLocation(), GetActorLocation(), FQuat::Identity, ObjectQueryParams, Shape, QueryParams);
	if(bHit)
	{
		for(const FHitResult& HitResult : HitResults)
		{
			ApplyEffects(HitResult);
		}
	}
}
