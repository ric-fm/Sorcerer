


#include "UI/SActorWidgetComponent.h"

#include "UI/SActorWidget.h"
#include "Sorcerer/Sorcerer.h"

USActorWidgetComponent::USActorWidgetComponent()
{
	// Set common defaults when using Widgets on Actors
	SetWidgetSpace(EWidgetSpace::Screen);
	SetDrawAtDesiredSize(true);
	SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

void USActorWidgetComponent::InitWidget()
{
	Super::InitWidget();
	
#if !UE_BUILD_SHIPPING
	if (GetWidget() != nullptr && !GetWidget()->IsA(USActorWidget::StaticClass()))
	{
		// Suggest deriving from actor widget (not strictly required, but you lose the benefit of auto-assigning the Actor this way)
		UE_LOG(LogSorcerer, Warning, TEXT("WidgetClass for %s does not derive from SActorWidget"), *GetNameSafe(GetOwner()));
	}
#endif

	USActorWidget* WidgetInstance = Cast<USActorWidget>(GetWidget());
	if(WidgetInstance)
	{
		WidgetInstance->SetOwningActor(GetOwner());
	}
}
