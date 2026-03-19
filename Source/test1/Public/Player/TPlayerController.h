
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TPlayerController.generated.h"

class UTChatWidget;

UCLASS()
class TEST1_API ATPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
public:
	ATPlayerController();
	
	void SetChatMessageString(const FString& InChatMessageString);
	void PrintChatMessageString(const FString& InChatMessageString);
	
	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessageString(const FString& InChatMessageString);
	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessageString(const FString& InChatMessageString);
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	FString ChatMessageString;
	
	// 위젯 클래스
	UPROPERTY(EditDefaultsOnly, Category=UI)
	TSubclassOf<UTChatWidget> ChatWidgetClass;
	UPROPERTY()
	TObjectPtr<UTChatWidget> CachedChatWidget;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> NotificationTextWidgetClass;
	UPROPERTY()
	TObjectPtr<UUserWidget> NotificationTextWidgetInstance;
	
public:
	UPROPERTY(Replicated)
	FText NotificationText;
};
