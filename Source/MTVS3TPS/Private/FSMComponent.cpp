// Fill out your copyright notice in the Description page of Project Settings.


#include "FSMComponent.h"
#include "TPSPlayer.h"
#include "Enemy.h"

// Sets default values for this component's properties
UFSMComponent::UFSMComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFSMComponent::BeginPlay()
{
	Super::BeginPlay();

	// 태어날 때 오너를 기억하고싶다.
	Me = Cast<AEnemy>(GetOwner());

}



void UFSMComponent::TickComponent(float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime , TickType , ThisTickFunction);

	FString myState = UEnum::GetValueAsString(State);
	DrawDebugString(GetWorld() , GetOwner()->GetActorLocation() , myState , nullptr , FColor::Yellow , 0 , true , 1);

	switch ( State )
	{
	case EEnemyState::IDLE:		TickIdle(DeltaTime);		break;
	case EEnemyState::MOVE:		TickMove(DeltaTime);		break;
	case EEnemyState::ATTACK:	TickAttack(DeltaTime);		break;
	case EEnemyState::DAMAGE:	TickDamage(DeltaTime);		break;
	case EEnemyState::DIE:		TickDie(DeltaTime);			break;
	}
}

void UFSMComponent::TickIdle(const float& DeltaTime)
{
	// TO DO:
	// 목적지(주인공)를 찾고싶다.
	Target = Cast<ATPSPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	// 조건
	// 만약 목적지를 찾았다면
	if ( Target ) {
		// 이동상태로 전이하고싶다.
		State = EEnemyState::MOVE;
	}
}

void UFSMComponent::TickMove(const float& DeltaTime)
{
	// 목적지를 향해서 이동하고싶다.
	FVector dir = Target->GetActorLocation() - Me->GetActorLocation();
	float dist = dir.Size();
	Me->AddMovementInput(dir.GetSafeNormal());

	// 조건
	// 만약 목적지와의 거리가 공격 가능거리라면
	if ( dist < AttackDistance )
	{
		// 공격상태로 전이하고싶다.
		State = EEnemyState::ATTACK;
	}
}

void UFSMComponent::TickAttack(const float& DeltaTime)
{
	// 시간이 흐르다가
	CurrentTime += DeltaTime;
	// 만약 현재시간이 AttackWaitTime을 초과한다면
	if ( CurrentTime > AttackWaitTime )
	{
		CurrentTime = 0;
		// 만약 목적지와의 거리가 공격가능거리라면
		float dist = Me->GetDistanceTo(Target);
		if ( dist < AttackDistance ) {
			// 플레이어에게 데미지를 입히고싶다.
			UE_LOG(LogTemp , Warning , TEXT("Enemy -> Player에게 Damage"));
		}
		// 그렇지 않다면 
		else {
			// 이동상태로 전이하고싶다.
			State = EEnemyState::MOVE;
		}
	}

}

void UFSMComponent::TickDamage(const float& DeltaTime)
{
	// 시간이 흐르다가
	// 1초가 지나면 
	// 이동상태로 전이하고싶다.
	State = EEnemyState::MOVE;
}

void UFSMComponent::TickDie(const float& DeltaTime)
{
	// 아래로 이동하고싶다.
	
	// 시간이 흐르다가
	// 1초가 지나면
	// 파괴되고싶다.
	Me->Destroy();
}

void UFSMComponent::OnMyTakeDamage(int32 damage)
{
	// 총에 맞으면 체력을 1 감소시키고싶다.
	HP -= damage;
	// 만약 체력이 0보다 크다면 
	if (HP > 0)
	{
		// 데미지상태로 전이하고싶다.
		State = EEnemyState::DAMAGE;
	}
	// 그렇지 않다면
	else
	{
		// 죽음상태로 전이하고싶다.
		State = EEnemyState::DIE;
	}

}

