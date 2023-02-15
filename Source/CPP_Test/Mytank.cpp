// Fill out your copyright notice in the Description page of Project Settings.


#include "Mytank.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

AMytank::AMytank()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRING ARM"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	


}


void AMytank::BeginPlay()
{
	Super::BeginPlay();
}


void AMytank::Move(float Value)
{
	

	double DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);

	FVector DeltaLocation(Value, 0, 0);
	DeltaLocation *= (Speed * DeltaTime);
	this->AddActorLocalOffset(DeltaLocation);
}


void AMytank::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}


void AMytank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMytank::Move);
}


