// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "FSMComponent.h"
#include "EnemyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Mesh에 퀸을 로드해서 넣고싶다.
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/TPS/Models/Enemy/Model/vampire_a_lusth.vampire_a_lusth'"));

	// 만약 파일읽기를 성공했다면
	if ( TempMesh.Succeeded() )
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0 , 0 , -90) , FRotator(0 , -90 , 0));
		GetMesh()->SetRelativeScale3D(FVector(0.85f));
	}
	// ABP_Enemy 를 로드해서 애니메이션 적용을 하고싶다.
	ConstructorHelpers::FClassFinder<UEnemyAnimInstance> TempEnemyAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/TPS/Blueprints/Anim/ABP_Enemy.ABP_Enemy_C'"));

	if ( TempEnemyAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(TempEnemyAnim.Class);
	}

	// 이동방향으로 회전하도록 처리하고싶다.
	GetCharacterMovement()->bOrientRotationToMovement = true;

	FSMComp = CreateDefaultSubobject<UFSMComponent>(TEXT("FSMComp"));
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

