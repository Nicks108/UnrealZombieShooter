// Fill out your copyright notice in the Description page of Project Settings.


#include "A_PlayerCharacterBase.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AA_PlayerCharacterBase::AA_PlayerCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRING ARM"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}


AA_PlayerCharacterBase::~AA_PlayerCharacterBase()
{
	Camera->DestroyComponent();
	SpringArm->DestroyComponent();
}


// Called when the game starts or when spawned
void AA_PlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AA_PlayerCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AA_PlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"),this,&AA_PlayerCharacterBase::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"),this,&AA_PlayerCharacterBase::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"),this,&AA_PlayerCharacterBase::LookRight);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"),this,&AA_PlayerCharacterBase::MoveRight);

}


void AA_PlayerCharacterBase::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector()* AxisValue * this->VerticalMovmentSpeed);
}


void AA_PlayerCharacterBase::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue* VerticalRotationSpeed);
}


void AA_PlayerCharacterBase::LookRight(float AxisValue)
{
	AddControllerYawInput(AxisValue * HorizontalRotationSpeed);
}


void AA_PlayerCharacterBase::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue * HorizontalMovmentSpeed);
}
