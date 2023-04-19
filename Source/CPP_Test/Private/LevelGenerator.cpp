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

	// FString ContentDir = FPaths::ProjectContentDir();
	// UE_LOG(LogLevelGen, Warning, TEXT("contentDir: %s"), *ContentDir);
	//
	// ContentDir.Append(TEXT("Levels"));
	// IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();
	// if(FileManager.DirectoryExists(*ContentDir))
	// {
	// 	UE_LOG(LogLevelGen, Warning, TEXT("FilePaths: Directory Exists %s"), *ContentDir);
	// }
	// else
	// {
	// 	UE_LOG(LogLevelGen, Warning, TEXT("FilePaths: Directory Does not exist %s"), *ContentDir);
	// }
	//
	// //Now we need to create a DirectoryVisitor 
	// DirectoryVisitor Visitor;
	// // The ItterateDirectory takes two arguments The directory and the Visitor we created above.
	// if(FileManager.IterateDirectory(*ContentDir,Visitor))
	// {
	// 	UE_LOG(LogLevelGen, Warning, TEXT("FilePaths: Directory's or files found"));
	// }
	// else
	// {
	// 	UE_LOG(LogLevelGen, Warning, TEXT("FilePaths: Directory did not exist or visitor returned false"));
	// }
	// TArray<FString> FoundFiles;
	// FileManager.FindFiles(FoundFiles ,*ContentDir,*FString());
	//
	// UE_LOG(LogLevelGen, Warning, TEXT("Found %i Files"), FoundFiles.Num());



	FAssetRegistryModule& AssetRegisteryModule =
		FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	


	TArray<FString> PathsToScan;
	
	// PathsToScan.Add(TEXT("/Game/Levels"));
	// IAssetRegistry& AssetRegistry = AssetRegisteryModule.Get();
	// AssetRegistry.ScanPathsSynchronous(PathsToScan);
	const UClass* Class = ULevel::StaticClass();


	enum CITY_ZONE_TYPES
	{
		COMMERCIAL_LARGE,
		COMMERCIAL,
		RESIDENTIAL,
		NUM_ZONE_TYPES
		
	};
	TArray<FAssetData> AssetData[NUM_ZONE_TYPES]; 
	
	AssetRegisteryModule.Get().GetAssetsByPath(FName("/Game/Levels/commercial"), AssetData[COMMERCIAL]);

	TArray<FAssetData> AssetDataCommercial_Large;
	AssetRegisteryModule.Get().GetAssetsByPath(FName("/Game/Levels/commercial_Large"), AssetData[COMMERCIAL_LARGE]);

	TArray<FAssetData> AssetDataResidential;
	AssetRegisteryModule.Get().GetAssetsByPath(FName("/Game/Levels/residential"), AssetData[RESIDENTIAL]);
	
	
	
	
	for (FAssetData	Asset : AssetData[COMMERCIAL])
	{
		UE_LOG(LogLevelGen, Warning, TEXT("Asset name %s "), *Asset.AssetName.ToString());
		UE_LOG(LogLevelGen, Warning, TEXT("Asset Class %s"), *Asset.AssetClassPath.ToString());
		Asset.
		// ULevel* AssetAsULevel= Cast<ULevel>(Asset.GetAsset());
		//
		// LevelChunckArray.Add(Asset.PackageName);
		//
		// FLatentActionInfo LatentInfo;
		// int i=0;
		UE_LOG(LogLevelGen, Warning, TEXT("--------------------------") );
	}

	
	
	int** CityZoneGrid = new int*[CityGridSegments];
	for(int i=0; i< CityGridSegments;i++)
	{
		CityZoneGrid[i] = new int[CityGridSegments];
	}

	
	float MinValue=INTMAX_MAX;
	float MaxValue= 0 ;
	for (int y=0; y< CityGridSegments; y ++)
	{
		for (int x =0; x < CityGridSegments; x++)
		{
			float p = FMath::PerlinNoise2D(FVector2D(freq_x * (x) + x_offset, freq_y *y + y_offset ));
			float p1= p + 1.0;
			float Pd2 = p1/2.0;
			int f = Pd2*NUM_ZONE_TYPES;
			CityZoneGrid[x][y]= f;
			//UE_LOG(LogLevelGen, Warning, TEXT("zone Perlin  %f, %f, %f, %i "), p, p1,Pd2, f );

			if(f < MinValue)
				MinValue = f;
			if(f > MaxValue)
				MaxValue = f;
		}
		
	}

	// for (int y=0; y< CityGridSegments; y ++)
	// {
	// 	for (int x =0; x < CityGridSegments; x++)
	// 	{
	// 		UE_LOG(LogLevelGen, Warning, TEXT("zone Perlin Range %f"), CityZoneGrid[x][y]);
	// 		CityZoneGrid[x][y] = ConvertRange( MinValue, MaxValue, 0 , NUM_ZONE_TYPES-1, CityZoneGrid[x][y]);
	// 		UE_LOG(LogLevelGen, Warning, TEXT("zone Perlin New Range %f"), CityZoneGrid[x][y]);
	// 	}
	// }
	

	

for(int y=0;y <CityGridSegments;y++)
{
	for (int x =0; x < CityGridSegments; x++)
	{
			//get randome zone
		int RandomeZoneType = CityZoneGrid[x][y];

		//get randome tile in zone
		FAssetData	Asset = AssetData[RandomeZoneType][FMath::RandRange(0,AssetData[RandomeZoneType].Num()-1)];
		double RandomeRot = 90 * FMath::RandRange(0, 4);
		FStaticConstructObjectParameters params(ULevelStreamingDynamic::StaticClass());
		params.Outer = GetWorld();
		params.Name = FName(Asset.AssetName);
		params.SetFlags = RF_Public;
		params.InternalSetFlags = EInternalObjectFlags::None;
		
		ULevelStreamingDynamic* StreamingLevel = static_cast<ULevelStreamingDynamic*>( StaticConstructObject_Internal(params));

		// Associate a package name.
		StreamingLevel->SetWorldAssetByPackageName(Asset.PackageName);
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
			FRotator{0,RandomeRot,0},                 // Rotation
			FVector{x*20000.0f, y*20000.0f,0.0f},  // Translation
			FVector{1.0f, 1.0f, 1.0f}   // Scale
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

		// Add the new level to world.
		bool Sucess;
		ULevelStreamingDynamic* inst = ULevelStreamingDynamic::LoadLevelInstance(GetWorld(), PackageFileName, A.GetLocation(),  A.Rotator(), Sucess);
		//GetWorld()->AddStreamingLevel(StreamingLevel);
		GetWorld()->AddStreamingLevel(inst);
		//GetWorld()->UpdateLevelStreaming();
		//FLatentActionInfo info;
		//UGameplayStatics::LoadStreamLevel(GetWorld(),FName(StreamingLevel->GetName()), true, false, info);
		//GetWorld()->UpdateLevelStreaming();
		FLatentActionInfo info2;
		UGameplayStatics::LoadStreamLevel(GetWorld(),FName(inst->GetName()), true, false, info2);
		GetWorld()->UpdateLevelStreaming();
	}
}
	
	
}

// Called every frame
void ALevelGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


float ALevelGenerator::ConvertRange(float OldMin, float OldMax, float NewMin, float NewMax, float OldValue)
{
	float OldRange = (OldMax - OldMin);  
	float NewRange = (NewMax - NewMin) ; 
	float NewValue = (((OldValue - OldMin) * NewRange) / OldRange) + NewMin;
	return NewValue;
}


