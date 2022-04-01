


#include "SProjectileBase.h"

#include "SCharacterBase.h"
#include "Abilities/SAbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASProjectileBase::ASProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereComponent->SetCollisionProfileName("Projectile");
	RootComponent = SphereComponent;

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	ParticleSystemComponent->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->InitialSpeed = 5000.0f;
	ProjectileMovementComponent->MaxSpeed = 5000.0f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	MaxRange = 10000.0f;

	bReplicates = true;
}

// Called when the game starts or when spawned
void ASProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		// Set LifeSpan based on MaxRange
		if(MaxRange > 0.0f && ProjectileMovementComponent->InitialSpeed > 0.0f)
		{
			SetLifeSpan(MaxRange / ProjectileMovementComponent->InitialSpeed);
		}
	}
}

void ASProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComponent->OnComponentHit.AddDynamic(this, &ASProjectileBase::OnHit);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASProjectileBase::OnBeginOverlap);
}

void ASProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	OnHitWorld(Hit);
}

void ASProjectileBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnHitCharacter(SweepResult);
}

void ASProjectileBase::OnHitWorld_Implementation(const FHitResult& Hit)
{
	Explode();
}

void ASProjectileBase::OnHitCharacter_Implementation(const FHitResult& Hit)
{
	if(Hit.Actor != GetInstigator())
	{
		if(HasAuthority())
		{
			ApplyEffects(Hit);
		}
		
		Explode();
	}
}

void ASProjectileBase::Explode()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionFX, GetActorLocation());
	if(HasAuthority())
	{
		Destroy();
	}
}

void ASProjectileBase::ApplyEffects(const FHitResult& Hit)
{
	if(EffectClasses.Num() > 0)
	{
		ASCharacterBase* Character = Cast<ASCharacterBase>(Hit.Actor.Get());
		if(Character)
		{
			USAbilitySystemComponent* AbilitySystemComponent = Cast<USAbilitySystemComponent>(Character->GetAbilitySystemComponent());

			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddInstigator(GetInstigator(), GetInstigator());
			EffectContext.AddSourceObject(this);
			EffectContext.AddHitResult(Hit);

			for(TSubclassOf<UGameplayEffect> EffectClass : EffectClasses)
			{
				if(IsValid(EffectClass))
				{
					FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EffectClass, Character->GetCharacterLevel(),EffectContext);
					if(EffectSpecHandle.IsValid())
					{
						AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
					}
				}
			}
		}
	}
}
