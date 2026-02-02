// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"

#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"

ATank::ATank()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(CapsuleComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}

	SetPlayerEnabled(false);
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerController)
	{
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

		RotateTurret(HitResult.ImpactPoint);
	}
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATank::MoveInput);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ATank::TurnInput);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ATank::Fire);
	}
}

void ATank::MoveInput(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();

	UWorld* World = GetWorld();
	if (World)
	{
		FVector DeltaLocation = FVector(0.0f, 0.0f, 0.0f);
		DeltaLocation.X = Speed * InputValue * UGameplayStatics::GetWorldDeltaSeconds(World);
		AddActorLocalOffset(DeltaLocation, true);
	}
}

void ATank::TurnInput(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();

	UWorld* World = GetWorld();
	if (World)
	{
		FRotator DeltaRotation = FRotator(0.0f, 0.0f, 0.0f);
		DeltaRotation.Yaw = TurnRate * InputValue * World->GetDeltaSeconds();
		AddActorLocalRotation(DeltaRotation, true);
	}
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();

	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	SetPlayerEnabled(false);

	bIsAlive = false;
}

void ATank::SetPlayerEnabled(bool bEnabled)
{
	if (PlayerController)
	{
		if (bEnabled)
		{
			EnableInput(PlayerController);
		}
		else
		{
			DisableInput(PlayerController);
		}

		PlayerController->bShowMouseCursor = bEnabled;
	}
}
