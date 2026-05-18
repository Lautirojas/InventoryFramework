
#include "Items/Definitions/ItemDefinition.h"

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