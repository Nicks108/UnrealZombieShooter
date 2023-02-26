// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"

#include <string>

#include "EngineUtils.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/LevelStreamingKismet.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogLevelGen);




struct DirectoryVisitor : public IPlatformFile::FDirectoryVisitor
{
	//This function is called for every file or directory it finds.
	bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
	{
		// did we find a Directory or a file?
		if(bIsDirectory)
		{
			UE_LOG(LogLevelGen, Warning, TEXT("FilePaths: Directory found: %s"),FilenameOrDirectory);
		}
		else
		{
			UE_LOG(LogLevelGen, Warning, TEXT("FilePaths: File Found: %s"),FilenameOrDirectory);
		}
		return true;
	}
};




// Sets default values
ALevelGenerator::ALevelGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALevelGenerator::BeginPlay()
{
	Super::BeginPlay();

	FString ContentDir = FPaths::ProjectContentDir();
	UE_LOG(LogLevelGen, Warning, TEXT("contentDir: %s"), *ContentDir);

	ContentDir.Append(TEXT("Levels"));
	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();
	if(FileManager.DirectoryExists(*ContentDir))
	{
		UE_LOG(LogLevelGen, Warning, TEXT("FilePaths: Directory Exists %s"), *ContentDir);
	}
	else
	{
		UE_LOG(LogLevelGen, Warning, TEXT("FilePaths: Directory Does not exist %s"), *ContentDir);
	}

	//Now we need to create a DirectoryVisitor 
	DirectoryVisitor Visitor;
	// The ItterateDirectory takes two arguments The directory and the Visitor we created above.
	if(FileManager.IterateDirectory(*ContentDir,Visitor))
	{
		UE_LOG(LogLevelGen, Warning, TEXT("FilePaths: Directory's or files found"));
	}
	else
	{
		UE_LOG(LogLevelGen, Warning, TEXT("FilePaths: Directory did not exist or visitor returned false"));
	}
	TArray<FString> FoundFiles;
	FileManager.FindFiles(FoundFiles ,*ContentDir,*FString());
	
	UE_LOG(LogLevelGen, Warning, TEXT("Found %i Files"), FoundFiles.Num());



	FAssetRegistryModule& AssetRegisteryModule =
		FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	


	TArray<FString> PathsToScan;
	
	PathsToScan.Add(TEXT("/Game/Levels"));
	IAssetRegistry& AssetRegistry = AssetRegisteryModule.Get();
	AssetRegistry.ScanPathsSynchronous(PathsToScan);

	TArray<FAssetData> AssetData;
	const UClass* Class = ULevel::StaticClass();
	AssetRegisteryModule.Get().GetAssetsByPath(FName("/Game/Levels"), AssetData);
	
	
	for (FAssetData	Asset : AssetData)
	{
		UE_LOG(LogLevelGen, Warning, TEXT("Asset name %s "), *Asset.AssetName.ToString());
		UE_LOG(LogLevelGen, Warning, TEXT("Asset Class %s"), *Asset.AssetClassPath.ToString());

		ULevel* AssetAsULevel= Cast<ULevel>(Asset.GetAsset());
		
		LevelChunckArray.Add(Asset.PackageName);

		FLatentActionInfo LatentInfo;
		//UGameplayStatics::LoadStreamLevel(this, Asset.AssetName, true, false, LatentInfo);
		int i=0;
	}

	int i=0;
	for (FAssetData	Asset : AssetData)
	{
		FStaticConstructObjectParameters params(ULevelStreamingDynamic::StaticClass());
		params.Outer = GetWorld();
		params.Name = NAME_None;
		params.SetFlags = RF_NoFlags;
		params.InternalSetFlags = EInternalObjectFlags::None;
		
		ULevelStreamingDynamic* StreamingLevel = static_cast<ULevelStreamingDynamic*>( StaticConstructObject_Internal(params));

		// Associate a package name.
		StreamingLevel->SetWorldAssetByPackageName(Asset.AssetName);
		if (GetWorld()->IsPlayInEditor())
		{
			FWorldContext WorldContext = GEngine->GetWorldContextFromWorldChecked(GetWorld());
			StreamingLevel->RenameForPIE(WorldContext.PIEInstance);
		}

		StreamingLevel->LevelColor = FColor::MakeRandomColor();
		StreamingLevel->SetShouldBeLoaded(true);
		StreamingLevel->SetShouldBeVisible(true);
		StreamingLevel->bShouldBlockOnLoad = false;
		StreamingLevel->bInitiallyLoaded = true;
		StreamingLevel->bInitiallyVisible = true;

		FTransform A{
			FRotator{},                 // Rotation
			FVector{1.0f, 2.0f, 3.0f},  // Translation
			FVector{2.0f, 2.0f, 2.0f}   // Scale
		};

		StreamingLevel->LevelTransform = A; // where to put it
		
		StreamingLevel->PackageNameToLoad =Asset.PackageName; // PackageName containing level to load

		UE_LOG(LogLevelGen, Warning, TEXT("trying to loadlevel %s"), *StreamingLevel->PackageNameToLoad.ToString());

		
		FString PackageFileName;
		if( !FPackageName::DoesPackageExist( StreamingLevel->PackageNameToLoad.ToString(), &PackageFileName ) )
		{
			UE_LOG(LogLevelGen, Error, TEXT("trying to load invalid level %s"), *StreamingLevel->PackageNameToLoad.ToString());
			return ;
		}

		StreamingLevel->PackageNameToLoad = FName(*FPackageName::FilenameToLongPackageName(PackageFileName));

		// Add the new level to world.
		GetWorld()->AddStreamingLevel(StreamingLevel );
	}


	
	
}

// Called every frame
void ALevelGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


