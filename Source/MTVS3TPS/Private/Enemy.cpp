// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "FSMComponent.h"
#include "EnemyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "EnemyHPWidget.h"

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



	HPComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPComp"));
	HPComp->SetupAttachment(RootComponent);

	ConstructorHelpers::FClassFinder<UEnemyHPWidget> TempHPUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/TPS/UI/WBP_EnemyHp.WBP_EnemyHp_C'"));

	if ( TempHPUI.Succeeded() )
	{
		HPComp->SetWidgetClass(TempHPUI.Class);
		HPComp->SetDrawSize(FVector2D(100 , 20));
		HPComp->SetRelativeLocation(FVector(0 , 0 , 120));
	}

	FSMComp = CreateDefaultSubobject<UFSMComponent>(TEXT("FSMComp"));
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	HPWidget = Cast<UEnemyHPWidget>(HPComp->GetWidget());
	int a = 0;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// 사용자의 카메라를 찾고
	FVector target = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
	// 그 카메라를 바라보는 방향을 만들어서
	FVector dir = target - HPComp->GetComponentLocation();
	dir.Normalize();

	// HPComp를 회전하고싶다.(Billboard 기법)
	FRotator rot = dir.ToOrientationRotator();
	HPComp->SetWorldRotation(rot);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

