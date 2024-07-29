// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FSMComponent.generated.h"

//  열거형, scoped enum
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	IDLE,
	MOVE = 100,
	ATTACK,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MTVS3TPS_API UFSMComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFSMComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	EEnemyState State = EEnemyState::IDLE;

	void TickIdle(const float& DeltaTime);
	void TickMove(const float& DeltaTime);
	void TickAttack(const float& DeltaTime);
};
