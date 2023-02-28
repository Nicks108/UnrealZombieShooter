// Fill out your copyright notice in the Description page of Project Settings.


#include "NewLevelGen.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "Engine/LevelStreamingDynamic.h"

// Sets default values
ANewLevelGen::ANewLevelGen()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANewLevelGen::BeginPlay()
{
	Super::BeginPlay();


	FAssetRegistryModule& AssetRegisteryModule =
		FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FString> PathsToScan;
	
	PathsToScan.Add(TEXT("/Game/Levels"));
	IAssetRegistry& AssetRegistry = AssetRegisteryModule.Get();
	AssetRegistry.ScanPathsSynchronous(PathsToScan);

	TArray<FAssetData> AssetData;
	const UClass* Class = ULevel::StaticClass();
	AssetRegisteryModule.Get().GetAssetsByPath(FName("/Game/Levels"), AssetData);
	

	ULevelStreaming* StreamingLevel = nullptr;
	UClass* CurrentStreamingLevelClass = ULevelStreamingDynamic::StaticClass();
	TSubclassOf<ULevelStreaming> LevelStreamingClass = CurrentStreamingLevelClass;
	StreamingLevel = NewObject<ULevelStreaming>(GetWorld(), LevelStreamingClass, NAME_None, RF_NoFlags, NULL);
	// Associate a package name.
	StreamingLevel->SetWorldAssetByPackageName(AssetData[0].PackageName);
	StreamingLevel->SetWorldAssetByPackageName(AssetData[0].AssetName);
	if (GetWorld()->IsPlayInEditor())
	{
		FWorldContext WorldContext = GEngine->GetWorldContextFromWorldChecked(GetWorld());
		StreamingLevel->RenameForPIE(WorldContext.PIEInstance);
	}
	StreamingLevel->PackageNameToLoad =AssetData[0].PackageName;

	FTransform LevelTransform{
		FRotator{},                 // Rotation
		FVector{1.0f, 2.0f, 3.0f},  // Translation
		FVector{2.0f, 2.0f, 2.0f}   // Scale
	};
	StreamingLevel->LevelTransform = LevelTransform;

	// Seed the level's draw color.
	StreamingLevel->LevelColor = FLinearColor::MakeRandomColor();
	GetWorld()->AddStreamingLevel(StreamingLevel);


	// Refresh just the newly created level.
	TArray<ULevelStreaming*> LevelsForRefresh;
	LevelsForRefresh.Add(StreamingLevel);
	GetWorld()->RefreshStreamingLevels(LevelsForRefresh);
	
}

// Called every frame
void ANewLevelGen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

