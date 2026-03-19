#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TGameStateBase.generated.h"


UCLASS()
class TEST1_API ATGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadcastLoginMessage(const FString& InNameString = FString(TEXT("XXXXXXX")));
	
	
};
