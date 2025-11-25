// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SweetDreamsCharacter.h"
#include "Core/SweetDreamsBPLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetStringLibrary.h"

ASweetDreamsCharacter::ASweetDreamsCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; 	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); 
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->JumpZVelocity = 800.f;
	GetCharacterMovement()->AirControl = 1.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->SetFieldOfView(80.0f);
	Camera->bUsePawnControlRotation = false;

	GetCapsuleComponent()->SetCanEverAffectNavigation(true);
}

void ASweetDreamsCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASweetDreamsCharacter, bIsRunning);
}

void ASweetDreamsCharacter::BeginPlay()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
	}
	Super::BeginPlay();
}

void ASweetDreamsCharacter::MoveForward(float Value)
{
	if (IsValid(Controller) && Value != 0.0f && bCanMove)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ASweetDreamsCharacter::MoveRight(float Value)
{
	if (IsValid(Controller) && Value != 0.0f && bCanMove)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ASweetDreamsCharacter::Run()
{
	if (HasAuthority())
	{
		Run_Internal();
	}
	else 
	{
		ServerRun(true);
	}
}

void ASweetDreamsCharacter::StopRunning()
{
	if (HasAuthority())
	{
		StopRunning_Internal();
	}
	else
	{
		ServerRun(false);
	}
}

void ASweetDreamsCharacter::ServerRun_Implementation(bool bRunning)
{
	if (bRunning)
	{
		Run_Internal();
	}
	else 
	{
		StopRunning_Internal();
	}
}

void ASweetDreamsCharacter::Run_Internal()
{
	if (!bCanMove || !bCanRun) return;
	if (MaxRunTime > 0.f && (CurrentRunTime >= MaxRunTime)) return;
	bIsRunning = true;
	OnRep_Run();
}

void ASweetDreamsCharacter::StopRunning_Internal()
{
	bIsRunning = false;
	OnRep_Run();
}

void ASweetDreamsCharacter::OnRep_Run()
{
	if (!GetCharacterMovement()) return;
	GetCharacterMovement()->MaxWalkSpeed = bIsRunning ? RunSpeed : WalkSpeed;
	GetCharacterMovement()->bNetworkUpdateReceived = true;
}

void ASweetDreamsCharacter::CameraVertical(float Value, float Sensitivity)
{
	if (!bCanMoveCamera) return;
	if (Sensitivity < 0.f) Sensitivity = 1.f;
	AddControllerPitchInput(Value * Sensitivity);
}

void ASweetDreamsCharacter::CameraHorizontal(float Value, float Sensitivity)
{
	if (!bCanMoveCamera) return;
	if (Sensitivity < 0.f) Sensitivity = 1.f;
	AddControllerYawInput(Value * Sensitivity);
}


