// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
DECLARE_LOG_CATEGORY_EXTERN(LogLevelGen, Log, All);

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelGenerator.generated.h"

UCLASS()
class CPP_TEST_API ALevelGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	UPROPERTY(EditAnywhere, Category = "Chunks")
	TArray<FName> LevelChunckArray;
	
	UPROPERTY(EditAnywhere, Category = "Chunks")
	int CityGridSegments;

	UPROPERTY(EditAnywhere, Category = "Chunks")
	float freq_x=0.2;
	UPROPERTY(EditAnywhere, Category = "Chunks")
	float freq_y =0.2;
	UPROPERTY(EditAnywhere, Category = "Chunks")
	float x_offset =0;
	UPROPERTY(EditAnywhere, Category = "Chunks")
	float y_offset =0;

	float ConvertRange(float OldMin, float OldMax, float NewMin, float NewMax, float OldValue);
	
};
