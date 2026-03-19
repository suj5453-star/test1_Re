
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Player/TPlayerController.h"
#include "TGameModeBase.generated.h"

class ATPlayerController;

UCLASS()
class TEST1_API ATGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATGameModeBase();

protected:
	virtual void StartPlay() override;
	FString SecretNumberString;
	TArray<TObjectPtr<ATPlayerController>> AllPlayerControllers;
	

public:
	virtual void OnPostLogin(AController* NewPlayer) override;
	FString GenerateSecretNumber();
	bool IsGuessNumberString(const FString& InNumberString);
	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);
	void PrintChatMessageString(ATPlayerController* InChattingPlayerController, const FString& InChatMessageString);
	void IncreaseGuessCount(ATPlayerController* InChattingPlayerController);
	void ResetGame();
	void JudgGame(ATPlayerController* InChattingPlayerController, int InStrikeCount);
		
};