
#include "Player/TPlayerController.h"

#include "EngineUtils.h"
#include "Blueprint/UserWidget.h"
#include "TChatWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Game/TGameModeBase.h"
#include "Game/TGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/TPlayerState.h"

void ATPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false)
	{
		return;
	}
	
	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);

	if (IsValid(ChatWidgetClass) == true)
	{
		CachedChatWidget = CreateWidget<UTChatWidget>(this, ChatWidgetClass);
		if (IsValid(CachedChatWidget) == true)
		{
			CachedChatWidget->AddToViewport();
		}
	}
	
	if (IsValid(NotificationTextWidgetClass) == true)
	{
		NotificationTextWidgetInstance = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance) == true)
		{
			NotificationTextWidgetInstance->AddToViewport();
		}
	}
}

ATPlayerController::ATPlayerController()
{
	bReplicates = true;
}

void ATPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;

	if (IsLocalController() == true)
	{
		ATPlayerState* TPS = GetPlayerState<ATPlayerState>();
		
		if (IsValid(TPS) == true)
		{
			FString CombinedMessageString = TPS->GetPlayerInfoString() + TEXT(": ") + InChatMessageString;

			ServerRPCPrintChatMessageString(CombinedMessageString);
		}
	}
}

void ATPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	UKismetSystemLibrary::PrintString(this, InChatMessageString, true, true, FLinearColor::Green, 5.0f);
	
}

void ATPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, NotificationText);
}

void ATPlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}

void ATPlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM) == true)
	{
		ATGameModeBase* TGM = Cast<ATGameModeBase>(GM);
		if (IsValid(TGM) == true)
		{
			TGM->PrintChatMessageString(this, InChatMessageString);
		}
	}
}

