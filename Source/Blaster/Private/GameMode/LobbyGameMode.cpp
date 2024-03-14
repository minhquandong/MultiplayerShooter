// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LobbyGameMode.h"

#include "GameFramework/GameStateBase.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// Get how many player is connecting to the lobby
	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();			// PlayerArray containt the Player State of each player who join the game
	if (NumberOfPlayers == 2)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			bUseSeamlessTravel = true;
			World->ServerTravel(FString("/Game/Levels/Level?listen"));
		}
	}
}
