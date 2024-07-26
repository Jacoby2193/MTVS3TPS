// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);
	SphereComp->SetSphereRadius(13.0f);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetRelativeScale3D(FVector(0.25f));
	
	// 충돌설정
	SphereComp->SetCollisionProfileName(TEXT("BlockAll"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 발사체 컴포넌트 설정
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	MovementComp->SetUpdatedComponent(RootComponent);

	MovementComp->InitialSpeed = 3000;
	MovementComp->MaxSpeed = 3000;
	MovementComp->bShouldBounce = true;

	//SetLifeSpan(5);

}

//void MyAdd(int32 in)
//{
//	auto lambdaFunction = [&, in](int a , int b)->int32
//		{
//			return a + b;
//		};
//
//	int32 result = lambdaFunction(10 , 20);
//}


// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	// 0.1f 초 후에 파괴되게 하고싶다.

	FTimerHandle handle;
	//GetWorld()->GetTimerManager().SetTimer(handle, this, &ABullet::Die, 0.1f, false);
	// 람다식Lambda과 익명함수

	//int32 temp = 0;
	//// 람다식 : 대단히 소 중하다
	//auto lambdaFunction =  [&, temp] (int a, int b)->int32 
	//{
	//	return a + b;
	//};

	//int32 result = lambdaFunction(10 , 20);

	//GetWorld()->GetTimerManager().SetTimer(handle , this , &ABullet::Die , 0.1f , false);
	//GetWorld()->GetTimerManager().SetTimer(handle , FTimerDelegate::CreateLambda([]() {}) , 0.1f, false);
	GetWorld()->GetTimerManager().SetTimer(handle , [&]() {
		this->Destroy();
	} , 3.f , false);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// P = P0 + vt
	//float Speed = 1000;
	//FVector Dir = GetActorForwardVector();
	//FVector Velo = Dir*Speed;
	//SetActorLocation(GetActorLocation()+Velo*DeltaTime);
}

void ABullet::Die()
{
	Destroy();
}

