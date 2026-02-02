// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Tower.h"
#include "BattleBlasterGameInstance.h"

void ABattleBlasterGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Towers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), Towers);
	TowerCount = Towers.Num();

	UE_LOG(LogTemp, Display, TEXT("Number of towers: %d"), TowerCount);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		Tank = Cast<ATank>(PlayerPawn);
		if (!Tank)
		{
			UE_LOG(LogTemp, Display, TEXT("GameMode: Failed to find the tank actor!"));
		}
	}

	for (AActor* TowerActor : Towers)
	{
		if (TowerActor)
		{
			ATower* Tower = Cast<ATower>(TowerActor);
			if (Tower && Tank)
			{
				Tower->SetTank(Tank);
				UE_LOG(LogTemp, Display, TEXT("%s setting the tank variables!"), *Tower->GetActorNameOrLabel());
			}
		}
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		ScreenMessageWidget = CreateWidget<UScreenMessage>(PlayerController, ScreenMessageClass);
		if (ScreenMessageWidget)
		{
			ScreenMessageWidget->AddToPlayerScreen();
			ScreenMessageWidget->SetMessageText("Get Ready!");
		}
	}


	CountdownSeconds = CountdownDelay;
	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ABattleBlasterGameMode::OnCountdownTimerTimeout, 1.0f, true);
}

void ABattleBlasterGameMode::OnCountdownTimerTimeout()
{
	CountdownSeconds--;
	if (CountdownSeconds > 0)
	{
		if (ScreenMessageWidget)
		{
			ScreenMessageWidget->SetMessageText(FString::FromInt(CountdownSeconds));
		}
	}
	else if (CountdownSeconds == 0)
	{
		if (ScreenMessageWidget)
		{
			ScreenMessageWidget->SetMessageText("Go!");
		}
		if (Tank)
		{
			Tank->SetPlayerEnabled(true);
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
		if (ScreenMessageWidget)
		{
			ScreenMessageWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ABattleBlasterGameMode::ActorDied(AActor* DeadActor)
{
	bool bIsGameOver = false;

	if (DeadActor == Tank)
	{
		Tank->HandleDestruction();
		bIsGameOver = true;
	}
	else
	{
		ATower* DeadTower = Cast<ATower>(DeadActor);
		if (DeadTower)
		{
			DeadTower->HandleDestruction();

			TowerCount--;
			if (TowerCount == 0)
			{
				bIsGameOver = true;
				bIsVictory = true;
			}
		}
	}

	if (bIsGameOver)
	{
		if (ScreenMessageWidget)
		{
			FString GameOverString = bIsVictory ? "Victory!" : "Defeat!";
			ScreenMessageWidget->SetMessageText(GameOverString);
			ScreenMessageWidget->SetVisibility(ESlateVisibility::Visible);
		}

		FTimerHandle GameOverTimerHandle;
		GetWorldTimerManager().SetTimer(GameOverTimerHandle, this, &ABattleBlasterGameMode::OnGameOverTimeout, GameOverDelay, false);
	}
}

void ABattleBlasterGameMode::OnGameOverTimeout()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UBattleBlasterGameInstance* BattleBlasterGameInstance = Cast<UBattleBlasterGameInstance>(GameInstance);
		if (BattleBlasterGameInstance)
		{
			if (bIsVictory)
			{
				// Load the next level
				BattleBlasterGameInstance->LoadNextLevel();
			}
			else
			{
				// Reload the current level
				BattleBlasterGameInstance->RestartCurrentLevel();
			}
		}
	}
}
