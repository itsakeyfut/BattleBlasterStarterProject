// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"

void ATower::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Tank && IsInFireRange())
	{
		RotateTurret(Tank->GetActorLocation());
	}
}

void ATower::CheckFireCondition()
{
	if (Tank && Tank->IsPlayerAlive() && IsInFireRange())
	{
		Fire();
	}
}

bool ATower::IsInFireRange()
{
	bool bResult = false;
	if (Tank)
	{
		float DistanceToTank = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
		bResult = (DistanceToTank <= FireRange);
	}
	return bResult;
}

void ATower::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}