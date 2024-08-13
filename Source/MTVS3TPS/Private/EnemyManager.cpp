// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "EngineUtils.h"
#include "Engine/StaticMeshActor.h"
#include "Enemy.h"

// Sets default values
AEnemyManager::AEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
	
	// Spawn목록을 만들고싶다.
	FName tag = TEXT("EnemySpawn");
	for ( TActorIterator<AActor> It(GetWorld(), AStaticMeshActor::StaticClass()); It; ++It )
	{
		AActor* Actor = *It;
		if ( IsValid(Actor) && Actor->ActorHasTag(tag) )
		{
			SpawnList.Add(Actor->GetTransform());
		}
	}

	// 적을 생성하는 행위를 시작
	FTimerHandle handle;
	float randTime = FMath::RandRange(MinTime , MaxTime);
	GetWorld()->GetTimerManager().SetTimer(handle, this, &AEnemyManager::MakeEnemy, randTime , false);

}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	bMakeEnemy = false;
}

void AEnemyManager::MakeEnemy()
{
	if (bMakeEnemy)
	{
		// 적을 랜덤위치에 생성
		int32 randIndex = FMath::RandRange(0 , SpawnList.Num() - 1);
		FTransform t= SpawnList[randIndex];
		GetWorld()->SpawnActor<AEnemy>(EnemyFactory, t);

		FTimerHandle handle;
		float randTime = FMath::RandRange(MinTime , MaxTime);
		GetWorld()->GetTimerManager().SetTimer(handle , this , &AEnemyManager::MakeEnemy , randTime , false);
	}
}

