// Fill out your copyright notice in the Description page of Project Settings.


#include "FSMComponent.h"
#include "TPSPlayer.h"
#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "EnemyAnimInstance.h"
#include "EnemyHPWidget.h"
#include "Components/WidgetComponent.h"
#include "AIController.h"

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

	// 태어날 때 애니메이션을 가져오고싶다.
	Anim = Cast<UEnemyAnimInstance>(Me->GetMesh()->GetAnimInstance());

	HPWidget = Cast<UEnemyHPWidget>(Me->HPComp->GetWidget());
	// 체력UI를 Full로 채우고싶다.
	HPWidget->SetHPBar(HP , MaxHP);

	EnemyAI = Cast<AAIController>(Me->GetController());
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
		SetState(EEnemyState::MOVE);
	}
}

void UFSMComponent::TickMove(const float& DeltaTime)
{
	// 목적지를 향해서 이동하고싶다.
	FVector dir = Target->GetActorLocation() - Me->GetActorLocation();
	float dist = dir.Size();
	//Me->AddMovementInput(dir.GetSafeNormal());

	FVector destinataion = Target->GetActorLocation();
	EPathFollowingRequestResult::Type result = EnemyAI->MoveToLocation(destinataion);

	//if ( result== )

	// 조건
	// 만약 목적지와의 거리가 공격 가능거리라면
	if ( dist < AttackDistance )
	{
		// 공격상태로 전이하고싶다.
		SetState(EEnemyState::ATTACK);
	}
}

void UFSMComponent::TickAttack(const float& DeltaTime)
{
	//// 시간이 흐르다가
	//CurrentTime += DeltaTime;
	//// 만약 현재시간이 AttackWaitTime을 초과한다면
	//if ( CurrentTime > AttackWaitTime )
	//{
	//	CurrentTime = 0;

	//	// 애니메이션의 공격 동작을 실행하고싶다.
	//	// 만약 목적지와의 거리가 공격가능거리라면
	//	float dist = Me->GetDistanceTo(Target);
	//	if ( dist < AttackDistance ) {
	//		// 플레이어에게 데미지를 입히고싶다.
	//		UE_LOG(LogTemp , Warning , TEXT("Enemy -> Player에게 Damage"));
	//	}
	//	// 그렇지 않다면 
	//	else {
	//		// 이동상태로 전이하고싶다.
	//		SetState(EEnemyState::MOVE);
	//	}
	//}

}

void UFSMComponent::TickDamage(const float& DeltaTime)
{
	//// 시간이 흐르다가
	//CurrentTime += DeltaTime;
	//// 1초가 지나면 
	//if ( CurrentTime > 1 )
	//{
	//	// 이동상태로 전이하고싶다.
	//	SetState(EEnemyState::MOVE);
	//}
}

void UFSMComponent::TickDie(const float& DeltaTime)
{
	// 애니메이션 재생이 끝나면 아래로 내려가게 하고싶다.
	if ( false == bDieDone )
	{
		return;
	}
	// 아래로 이동하고싶다.
	float speed = 200;
	FVector p = Me->GetActorLocation();
	FVector velocity = FVector::DownVector * speed;
	Me->SetActorLocation(p + velocity * DeltaTime);


	CurrentTime += DeltaTime;
	// 1초가 지나면 
	if ( CurrentTime > 3 )
	{
		// 파괴되고싶다.
		Me->Destroy();
	}
}

void UFSMComponent::SetState(EEnemyState NextState)
{
	EEnemyState prevState = State;
	State = NextState;

	// 애니메이션의 상태도 동기화 하고싶다.
	Anim->EnemyState = NextState;

	CurrentTime = 0;




	// 상태가 바뀔때 무엇인가 초기화 하고싶다면 여기서 하세요.
	switch ( State )
	{
	case EEnemyState::IDLE:
		break;
	case EEnemyState::MOVE:
		break;
	case EEnemyState::ATTACK:
		break;
	case EEnemyState::DAMAGE:
		break;
	case EEnemyState::DIE:
		break;
	default:
		break;
	}
}

void UFSMComponent::OnMyTakeDamage(int32 damage)
{
	// 총에 맞으면 체력을 1 감소시키고싶다.
	HP = FMath::Max(HP - damage, 0);
	HPWidget->SetHPBar(HP , MaxHP);

	// 만약 체력이 0보다 크다면 
	if ( HP > 0 )
	{
		// 데미지상태로 전이하고싶다.
		SetState(EEnemyState::DAMAGE);
		//Anim->EnemyMontage
		int randValue = FMath::RandRange(0 , 1);
		FString sectionName = FString::Printf(TEXT("Damage%d") , randValue);
		Me->PlayAnimMontage(Anim->EnemyMontage , 1 , FName(*sectionName));
	}
	// 그렇지 않다면
	else
	{
		// 죽음상태로 전이하고싶다.
		SetState(EEnemyState::DIE);
		Me->PlayAnimMontage(Anim->EnemyMontage , 1 , TEXT("Die"));
		// 캡슐컴포넌트의 충돌설정을 NoCollision으로 하고싶다.
		Me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

}

void UFSMComponent::OnMyAttackStart()
{
	Anim->bAttack = false;
}

void UFSMComponent::OnMyAttackEnd()
{
	// 만약 타겟이 공격가능 거리라면 계속 공격하고싶다.

	// 만약 타겟이 공격가능 거리라면 계속 공격하고싶다.
	float dist = Me->GetDistanceTo(Target);
	if ( dist < AttackDistance ) {
		// 플레이어에게 데미지를 입히고싶다.
		Anim->bAttack = true;
		UE_LOG(LogTemp , Warning , TEXT("Enemy -> Player에게 Damage"));
	}
	// 그렇지 않다면 
	else {
		// 이동상태로 전이하고싶다.
		SetState(EEnemyState::MOVE);
	}

}

void UFSMComponent::OnMyDamageEnd()
{
	// 데미지 애니메이션이 종료되면 이동상태로 전이하고싶다.
	SetState(EEnemyState::MOVE);
}


void UFSMComponent::OnMyHit()
{
}


