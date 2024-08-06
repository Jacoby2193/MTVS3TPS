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
};
