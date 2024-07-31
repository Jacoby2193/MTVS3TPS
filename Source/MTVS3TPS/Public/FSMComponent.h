// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FSMComponent.generated.h"

//  열거형, scoped enum
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	IDLE UMETA(DisplayName = "대기") ,
	MOVE  UMETA(DisplayName = "이동") ,
	ATTACK  UMETA(DisplayName = "공격") ,
	DAMAGE UMETA(DisplayName = "데미지") ,
	DIE UMETA(DisplayName = "죽음") ,
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
	void TickDamage(const float& DeltaTime);
	void TickDie(const float& DeltaTime);

	void SetState(EEnemyState NextState);


	UPROPERTY()
	class ATPSPlayer* Target;

	// 공격 가능거리
	float AttackDistance = 200;

	UPROPERTY()
	class AEnemy* Me;

	float CurrentTime;
	float AttackWaitTime = 1;


	// 총에 맞으면 체력을 1 감소시키고싶다.
	// 만약 체력이 0보다 크다면 데미지상태로 전이하고싶다.
	// 그렇지 않다면 죽음상태로 전이하고싶다.

	int32 MaxHP = 2;
	int32 HP = MaxHP;

	void OnMyTakeDamage(int32 damage = 1);

};
