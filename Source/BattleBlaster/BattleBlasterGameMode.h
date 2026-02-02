// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "Tank.h"
#include"ScreenMessage.h"

#include "BattleBlasterGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API ABattleBlasterGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	void ActorDied(AActor* DeadActor);

protected:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UScreenMessage> ScreenMessageClass;

	UPROPERTY(EditAnywhere, Category = "Game Rules")
	float GameOverDelay = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Game Rules")
	int32 CountdownDelay = 3;

private:
	UScreenMessage* ScreenMessageWidget;

	int32 CountdownSeconds;

	FTimerHandle CountdownTimerHandle;

	bool IsVictory = false;

	ATank* Tank;

	int32 TowerCount;

	void OnGameOverTimeout();
	void OnCountdownTimerTimeout();
};
