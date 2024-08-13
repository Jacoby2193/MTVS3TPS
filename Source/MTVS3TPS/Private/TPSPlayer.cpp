// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Bullet.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"
#include "FSMComponent.h"
#include "TPSPlayerAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "TPSPlayerMoveComponent.h"
#include "TPSPlayerFireComponent.h"
#include "PlayerHPWidget.h"

// Sets default values
ATPSPlayer::ATPSPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 스프링암을 생성해서 루트에 붙이고싶다.
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeLocation(FVector(0 , 40 , 80));
	SpringArmComp->TargetArmLength = 200;

	// 카메라를 생성해서 스프링암에 붙이고싶다.
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	// Mesh에 퀸을 로드해서 넣고싶다.
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));

	// 만약 파일읽기를 성공했다면
	if ( TempMesh.Succeeded() )
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0 , 0 , -90) , FRotator(0 , -90 , 0));
	}

	// 총들의 에셋을 로드해서 설정하세요.
	// 총 컴포넌트 처리
	GunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	GunMeshComp->SetupAttachment(GetMesh() , TEXT("hand_r"));
	GunMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/TPS/Models/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));

	// 만약 파일읽기를 성공했다면
	if ( TempGunMesh.Succeeded() )
	{
		GunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		GunMeshComp->SetRelativeLocationAndRotation(FVector(-7.512286f , -1.674651f , 1.055855) ,
		FRotator(10.014844f , 101.370947f , -5.159758f));
		GunMeshComp->SetRelativeScale3D(FVector(0.85f));
	}

	SniperMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperMeshComp"));
	SniperMeshComp->SetupAttachment(GetMesh() , TEXT("hand_r"));
	SniperMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperMesh(TEXT("/Script/Engine.StaticMesh'/Game/TPS/Models/SniperGun/sniper1.sniper1'"));

	// 만약 파일읽기를 성공했다면
	if ( TempSniperMesh.Succeeded() )
	{
		SniperMeshComp->SetStaticMesh(TempSniperMesh.Object);
		SniperMeshComp->SetRelativeLocationAndRotation(FVector(-33.151786f , -6.529402f , 4.496872f) ,
		FRotator(9.890367f , -6.529402f , 4.496872f));
		SniperMeshComp->SetRelativeScale3D(FVector(0.13f));
	}



	// Mesh의 AminInstace를 파일로드해서 적용하고싶다.
	ConstructorHelpers::FClassFinder<UTPSPlayerAnimInstance> TempAnimInst(TEXT("/Script/Engine.AnimBlueprint'/Game/TPS/Blueprints/Anim/ABP_TPSPlayer.ABP_TPSPlayer_C'"));

	// 만약 파일읽기를 성공했다면
	if ( TempAnimInst.Succeeded() )
	{
		GetMesh()->SetAnimInstanceClass(TempAnimInst.Class);
	}


	// 쭈그리기를 활성화 하고싶다.
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;


	MoveComp = CreateDefaultSubobject<UTPSPlayerMoveComponent>(TEXT("MoveComp"));
	FireComp = CreateDefaultSubobject<UTPSPlayerFireComponent>(TEXT("FireComp"));
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	Anim = Cast<UTPSPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	// IMC_Player를 추가하고싶다.

	// 1. 컨트롤러를 가져와서 PlayerController인지 캐스팅 해본다.
	auto* pc = Cast<APlayerController>(Controller);
	// 2. 캐스팅 성공했다면 
	if ( pc )
	{
		// 3. 그 객체를 이용해서 EnhancedInputLocalPlayerSubsystem을 가져온다.
		UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		// 4. SubSys를 이용해서 AddMappingContext를 한다.
		if ( subSys )
		{
			subSys->AddMappingContext(IMC_Player , 0);
		}
	}

	// 태어날 때 마우스커서 안보이게 게임에서 입력가지도록 처리하고싶다.
	UGameplayStatics::SetGamePaused(GetWorld() , false);
	auto* pcon = GetWorld()->GetFirstPlayerController();
	pcon->SetShowMouseCursor(false);
	pcon->SetInputMode(FInputModeGameOnly());

	// 체력UI를 만들고
	PlayerHPUI = Cast<UPlayerHPWidget>(CreateWidget(GetWorld() , PlayerHPWidgetFactory));
	// 화면에 보이게하고
	PlayerHPUI->AddToViewport();
	// 체력을 UI에 반영하고싶다.
	HP = MaxHP;
	PlayerHPUI->UpdateHP(HP , MaxHP);
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	InputBindingDelegate.Broadcast(input);

	//MoveComp->SetupInputBinding(input);
	//FireComp->SetupInputBinding(input);
}

void ATPSPlayer::OnMyHitDamage(int32 damage)
{
	HP = FMath::Max(0 , HP - damage);
	PlayerHPUI->UpdateHP(HP , MaxHP);
	if ( HP <= 0 )
	{
		// 게임오버...
		UGameplayStatics::SetGamePaused(GetWorld() , true);
		// 마우스 커서를 보이게 하고싶다.
		auto* pc = GetWorld()->GetFirstPlayerController();
		pc->SetShowMouseCursor(true);
		pc->SetInputMode(FInputModeUIOnly());
		// 게임오버 UI를 블루프린트에서 보이게 하는 기능을 호출하고싶다.
		ShowGameOverUI();
	}
}

void ATPSPlayer::ChooseSniperGun_Implementation(bool bSiperGun)
{

}

