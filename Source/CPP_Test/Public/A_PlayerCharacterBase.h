// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "A_CharacterBase.h"
#include "A_PlayerCharacterBase.generated.h"

/**
 * 
 */
UCLASS()
class CPP_TEST_API AA_PlayerCharacterBase : public AA_CharacterBase
{
	GENERATED_BODY()

public:
	AA_PlayerCharacterBase();

protected:
	virtual  void BeginPlay() override;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	double Speed = 1;


	void Move(float Value);

public:
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
