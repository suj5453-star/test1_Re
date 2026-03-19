
#include "game/TGameModeBase.h"

#include "EngineUtils.h"
#include "game/TGameStateBase.h"
#include "Player/TPlayerController.h"
#include "Player/TPlayerState.h"

ATGameModeBase::ATGameModeBase()
{
	GameStateClass = ATGameStateBase::StaticClass();
	PlayerStateClass = ATPlayerState::StaticClass();
	PlayerControllerClass = ATPlayerController::StaticClass();
}

void ATGameModeBase::StartPlay()
{
	Super::StartPlay();
	SecretNumberString = GenerateSecretNumber();
}

void ATGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	
	ATPlayerController* TPlayerController = Cast<ATPlayerController>(NewPlayer);
	
	if (IsValid(TPlayerController) == true)
	{
		TPlayerController->NotificationText = FText::FromString(TEXT("서버에 입장하였습니다."));
		
		AllPlayerControllers.Add(TPlayerController);

		ATPlayerState* TPS = TPlayerController->GetPlayerState<ATPlayerState>();
		if (IsValid(TPS) == true)
		{
			TPS->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		
			ATGameStateBase* TGameStateBase =  GetGameState<ATGameStateBase>();
			if (IsValid(TGameStateBase) == true)
			{
				TGameStateBase->MulticastRPCBroadcastLoginMessage(TPS->PlayerNameString);
			}
		}
	}
}

FString ATGameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}

	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });
	
	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}

	return Result;
}

bool ATGameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	bool bCanPlay = false;
	
	// 반복 x -> while(false), 중복없이 0이 아닌 3개 숫자
	do {
		if (InNumberString.Len() != 3)
		{
			break;
		}

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}
			
			UniqueDigits.Add(C);
		}

		if (bIsUnique == false)
		{
			break;
		}
		
		if (UniqueDigits.Num() != 3)
		{
			break;
		}
		
		bCanPlay = true;
		
	} while (false);	

	return bCanPlay;
}

FString ATGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	// 자릿수까지 맞으면 strike, 숫자는 맞고 자릿수는 틀리면 ball
	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else 
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;				
			}
		}
	}

	// 하나도 못맞추면 OUT
	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS, %dB"), StrikeCount, BallCount);
}

void ATGameModeBase::PrintChatMessageString(
	ATPlayerController* InChattingPlayerController,
	const FString& InChatMessageString)
{
	int Index = InChatMessageString.Len() - 3;
	FString GuessNumberString = InChatMessageString.RightChop(Index);
	
	if (IsGuessNumberString(GuessNumberString) == true)
	{
		FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString);
		IncreaseGuessCount(InChattingPlayerController);
		
		for (TActorIterator<ATPlayerController> It(GetWorld()); It; ++It)
		{
			ATPlayerController* TPlayerController = *It;
			if (IsValid(TPlayerController) == true)
			{
				FString CombinedMessageString = InChatMessageString + TEXT(" -> ") + JudgeResultString;
				TPlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);
			}
		}
		int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
		JudgGame(InChattingPlayerController, StrikeCount);
	}
	else
	{
		for (TActorIterator<ATPlayerController> It(GetWorld()); It; ++It)
		{
			ATPlayerController* TPlayerController = *It;
			if (IsValid(TPlayerController) == true)
			{
				TPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
			}
		}
	}
}

void ATGameModeBase::IncreaseGuessCount(ATPlayerController* InChattingPlayerController)
{
	ATPlayerState* CXPS = InChattingPlayerController->GetPlayerState<ATPlayerState>();
	if (IsValid(CXPS) == true)
	{
		CXPS->CurrentGuessCount++;
	}
}

void ATGameModeBase::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();

	for (const auto& TPlayerController : AllPlayerControllers)
	{
		ATPlayerState* TPS = TPlayerController->GetPlayerState<ATPlayerState>();
		if (IsValid(TPS) == true)
		{
			TPS->CurrentGuessCount = 0;
		}
	}
}

void ATGameModeBase::JudgGame(ATPlayerController* InChattingPlayerController, int InStrikeCount)
{
	if (3 == InStrikeCount)
	{
		ATPlayerState* TPS = InChattingPlayerController->GetPlayerState<ATPlayerState>();
		for (const auto& TPlayerController : AllPlayerControllers)
		{
			if (IsValid(TPS) == true)
			{
				FString CombinedMessageString = TPS->PlayerNameString + TEXT(" 이 게임에서 승리하셨습니다.");
				TPlayerController->NotificationText = FText::FromString(CombinedMessageString);
			}
		}
		ResetGame();
	}
	else
	{
		bool bIsDraw = true;
		for (const auto& TPlayerController : AllPlayerControllers)
		{
			ATPlayerState* TPS = TPlayerController->GetPlayerState<ATPlayerState>();
			if (IsValid(TPS) == true)
			{
				if (TPS->CurrentGuessCount < TPS->MaxGuessCount)
				{
					bIsDraw = false;
					break;
				}
			}
		}

		if (true == bIsDraw)
		{
			for (const auto& TPlayerController : AllPlayerControllers)
			{
				TPlayerController->NotificationText = FText::FromString(TEXT("무승부"));
			}	
			ResetGame();
		}
	}
}
