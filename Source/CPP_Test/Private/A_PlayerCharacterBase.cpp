// Fill out your copyright notice in the Description page of Project Settings.


#include "A_PlayerCharacterBase.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"


AA_PlayerCharacterBase::AA_PlayerCharacterBase()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRING ARM"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	


}

 void AA_PlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}


void AA_PlayerCharacterBase::Move(float Value)
{
	

	double DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);

	FVector DeltaLocation(Value, 0, 0);
	DeltaLocation *= (Speed * DeltaTime);
	this->AddActorLocalOffset(DeltaLocation);
}


void AA_PlayerCharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}


void AA_PlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AA_PlayerCharacterBase::Move);
}
