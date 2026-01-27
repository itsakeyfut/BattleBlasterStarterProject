// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameInstance.h"

void UBattleBlasterGameInstance::ChangeLevel(int32 Index)
{
	if (Index > 0 && Index <= LastLevelIndex)
	{
		CurrentLevelIndex = Index;

		FString LevelName = FString::Printf(TEXT("Lvel_%d"), Index);
	}
}
