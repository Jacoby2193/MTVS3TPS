// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FSMComponent.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MTVS3TPS_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// 나의 오너 폰(AEnemy)을 기억하고싶다.
	virtual void NativeInitializeAnimation() override;
	
	UPROPERTY()
	class AEnemy* Enemy;

	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* EnemyMontage;
	
	// 상태, 공격여부
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EEnemyState EnemyState;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bAttack;

	UFUNCTION()
	void AnimNotify_AttackStart();
	
	UFUNCTION()
	void AnimNotify_AttackEnd();
	
	UFUNCTION()
	void AnimNotify_Hit();

	UFUNCTION()
	void AnimNotify_DamageEnd();

	UFUNCTION()
	void AnimNotify_DieEnd();
};
