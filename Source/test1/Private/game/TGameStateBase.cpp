
#include "game/TGameStateBase.h"

#include "Kismet/GameplayStatics.h"
#include "Player/TPlayerController.h"
#include "Net/UnrealNetwork.h"

void ATGameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (HasAuthority() == false)
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (IsValid(PC) == true)
		{
			ATPlayerController* TPC = Cast<ATPlayerController>(PC);
			if (IsValid(TPC) == true)
			{
				FString NotificationString = InNameString + TEXT(" 이 게임에 참가하셨습니다.");
				TPC->PrintChatMessageString(NotificationString);
			}
		}
	}
}
