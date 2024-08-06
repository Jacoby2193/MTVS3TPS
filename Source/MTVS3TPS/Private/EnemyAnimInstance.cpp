// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "Enemy.h"

void UEnemyAnimInstance::AnimNotify_AttackStart()
{
	AEnemy* enemy = Cast<AEnemy>(TryGetPawnOwner());
	if ( enemy )
	{
		enemy->FSMComp->OnMyAttackStart();
	}
}

void UEnemyAnimInstance::AnimNotify_AttackEnd()
{
	// 목적지와의 공격거리를 측정해서 처리해야한다.
	AEnemy* enemy = Cast<AEnemy>(TryGetPawnOwner());
	if ( enemy )
	{
		enemy->FSMComp->OnMyAttackEnd();
	}
}

void UEnemyAnimInstance::AnimNotify_Hit()
{
	AEnemy* enemy = Cast<AEnemy>(TryGetPawnOwner());
	if ( enemy )
	{
		enemy->FSMComp->OnMyHit();
	}
}
