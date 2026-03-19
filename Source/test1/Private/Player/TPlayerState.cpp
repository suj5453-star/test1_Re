
#include "Player/TPlayerState.h"

#include "Net/UnrealNetwork.h"
#include "Player/TPlayerController.h"

ATPlayerState::ATPlayerState() 
	: PlayerNameString(TEXT("None")),
	  CurrentGuessCount(0),
	  MaxGuessCount(3)
{
	bReplicates = true;
}

FString ATPlayerState::GetPlayerInfoString()
{
	FString PlayerInfoString 
	= PlayerNameString 
	+ TEXT("(")
	+ FString::FromInt(CurrentGuessCount) 
	+ TEXT("/") 
	+ FString::FromInt(MaxGuessCount) 
	+ TEXT(")");
	
	return PlayerInfoString;
}

void ATPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, PlayerNameString);
	DOREPLIFETIME(ThisClass, CurrentGuessCount);
	DOREPLIFETIME(ThisClass, MaxGuessCount);
}

void ATPlayerState::BeginPlay()
{
	Super::BeginPlay();
}
