// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

UCLASS()
class MTVS3TPS_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 일정 시간마다 적을 태어나게 하고싶다.
	// 시간은 랜덤으로 정하고싶다.
	// 타이머를 이용하자.
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly)
	float MinTime = 1.0f;
	UPROPERTY(EditDefaultsOnly)
	float MaxTime = 2.0f;
	// 타이머를 이용해서 계속 적을 만들것인지에 대한 여부
	bool bMakeEnemy = true;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AEnemy> EnemyFactory;

	void MakeEnemy();

	TArray<FTransform> SpawnList;

};
