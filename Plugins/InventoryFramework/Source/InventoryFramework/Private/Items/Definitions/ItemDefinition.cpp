

#include "Items/Definitions/ItemDefinition.h"
#include "Items/Instances/ItemInstance.h"



UItemDefinition::UItemDefinition()
{
	InstanceClass = UItemInstance::StaticClass();
}

void UItemDefinition::OnItemAcquired(AActor* Buyer) const
{
	for (UItemFragment* Fragment : Fragments)
	{
		if (Fragment)
		{
			Fragment->OnItemAcquired(Buyer);
		}
	}
}