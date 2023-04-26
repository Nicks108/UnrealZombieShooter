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

protected:
	virtual void BeginPlay() override;

public:
	AA_PlayerCharacterBase();
	virtual ~AA_PlayerCharacterBase() override;
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;
	
	UPROPERTY(EditAnywhere)
	float HorizontalRotationSpeed=0.1f;
	UPROPERTY(EditAnywhere)
	float VerticalRotationSpeed=1.0f;
	UPROPERTY(EditAnywhere)
	float HorizontalMovmentSpeed=1.0f;
	UPROPERTY(EditAnywhere)
	float VerticalMovmentSpeed=1.0f;
	
	void MoveForward(float AxisValue);
	void LookUp(float AxisValue);
	void LookRight(float AxisValue);
	void MoveRight(float AxisValue);
};
