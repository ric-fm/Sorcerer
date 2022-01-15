


#include "UI/SActorWidget.h"

void USActorWidget::SetOwningActor(AActor* NewOwner)
{
	OwningActor = NewOwner;
}

AActor* USActorWidget::GetOwningActor() const
{
	return OwningActor;
}
