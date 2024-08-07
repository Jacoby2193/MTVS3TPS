// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "Enemy.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Enemy = Cast<AEnemy>(TryGetPawnOwner());
}

void UEnemyAnimInstance::AnimNotify_AttackStart()
{
	if ( Enemy && Enemy->FSMComp )
	{
		Enemy->FSMComp->OnMyAttackStart();
	}
}

void UEnemyAnimInstance::AnimNotify_AttackEnd()
{
	// 목적지와의 공격거리를 측정해서 처리해야한다.
	if ( Enemy && Enemy->FSMComp )
	{
		Enemy->FSMComp->OnMyAttackEnd();
	}
}

void UEnemyAnimInstance::AnimNotify_Hit()
{
	if ( Enemy && Enemy->FSMComp )
	{
		Enemy->FSMComp->OnMyHit();
	}
}

void UEnemyAnimInstance::AnimNotify_DamageEnd()
{
	// 몽타주 재생멈춰야한다.
	Montage_Stop(0.1f, EnemyMontage);
	if ( Enemy && Enemy->FSMComp )
	{
		Enemy->FSMComp->OnMyDamageEnd();
	}
}

void UEnemyAnimInstance::AnimNotify_DieEnd()
{
	// 넘어지는 애니메이션이 끝났어요~
	if ( Enemy && Enemy->FSMComp )
	{
		Enemy->FSMComp->OnMyDieEnd();
	}
}
