
#include "Component/InteractionComponent.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

}

bool UInteractionComponent::TryInteract(APlayerController* InstigatorController)
{
	OnTryInteract.Broadcast();
	
	if (bCanInteract)
	{
		OnInteractSuccess.Broadcast(InstigatorController);
		return true;
	}
	else
	{
		OnInteractFailed.Broadcast(InstigatorController);
		return false;
	}
}


FICMetaData UInteractionComponent::GetMetaData() const
{
	return FICMetaData(bCanInteract,ActorName,ActionName);
}
