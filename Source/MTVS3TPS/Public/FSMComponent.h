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

	UFUNCTION(BlueprintCallable)
	void OnMyTakeDamage(int32 damage = 1);


	// 태어날 때 애니메이션을 가져오고싶다.
	// 나의 상태가 변경될 때 애니메이션의 상태도 동일하게 바꾸고싶다.
	UPROPERTY()
	class UEnemyAnimInstance* Anim;

	// 애니메이션에서 발생한 알림을 받고싶다.
	void OnMyAttackStart();
	void OnMyAttackEnd();
	void OnMyHit();

	void OnMyDamageEnd();

	// 애니메이션 재생이 끝나면 아래로 내려가게 하고싶다.
	bool bDieDone;
	
	FORCEINLINE void OnMyDieEnd() { bDieDone = true; }


	// 체력이 갱신될때 UI도 갱신하고싶다.
	UPROPERTY()
	class UEnemyHPWidget* HPWidget;

	// 네비게이션을 이용해서 길찾기를 하고싶다.
	UPROPERTY()
	class AAIController* EnemyAI;

	FVector PatrolPoint;
	float PatrolPointRadius = 500;

	// 내위치에서 반경 5미터
	bool SetPatrolPoint(FVector origin, float radius, FVector& dest);


};
