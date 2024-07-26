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

// Sets default values
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 스프링암을 생성해서 루트에 붙이고싶다.
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeLocation(FVector(0, 40, 80));
	SpringArmComp->TargetArmLength = 200;

	// 카메라를 생성해서 스프링암에 붙이고싶다.
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	
	// Mesh에 퀸을 로드해서 넣고싶다.
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));

	// 만약 파일읽기를 성공했다면
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
	}

	// 총들의 에셋을 로드해서 설정하세요.
	// 총 컴포넌트 처리
	GunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	GunMeshComp->SetupAttachment(GetMesh());
	GunMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/TPS/Models/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));

	// 만약 파일읽기를 성공했다면
	if (TempGunMesh.Succeeded())
	{
		GunMeshComp->SetSkeletalMesh(TempGunMesh.Object);}

	SniperMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperMeshComp"));
	SniperMeshComp->SetupAttachment(GetMesh());
	SniperMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperMesh(TEXT("/Script/Engine.StaticMesh'/Game/TPS/Models/SniperGun/sniper1.sniper1'"));

	// 만약 파일읽기를 성공했다면
	if (TempSniperMesh.Succeeded())
	{
		SniperMeshComp->SetStaticMesh(TempSniperMesh.Object);
	}
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	// IMC_Player를 추가하고싶다.

	// 1. 컨트롤러를 가져와서 PlayerController인지 캐스팅 해본다.
	auto* pc = Cast<APlayerController>(Controller);
	// 2. 캐스팅 성공했다면 
	if (pc)
	{
		// 3. 그 객체를 이용해서 EnhancedInputLocalPlayerSubsystem을 가져온다.
		UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		// 4. SubSys를 이용해서 AddMappingContext를 한다.
		if (subSys)
		{
			subSys->AddMappingContext(IMC_Player, 0);
		}
	}


	// 태어날 때 조준선UI와 ZoomUI를 생성하고싶다.
	CrosshairUI = CreateWidget(GetWorld() , CrosshairUIFactory);
	ZoomUI = CreateWidget(GetWorld() , ZoomUIFactory);
	// Viewport에 붙이고싶다.
	// 보이지않게 하고싶다.
	if ( CrosshairUI ) {
		CrosshairUI->AddToViewport();
	}
	if ( ZoomUI ) {
		ZoomUI->AddToViewport();
	}
	
	// 태어날때 Gun만 보이게하고싶다.
	OnMyActionChooseGun(FInputActionValue());
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 이동하고 싶다.
	// 회전 방향으로 이동하고싶다.
	// 1. ControlRotation 을 이용해서 Transform을 만들고
	FTransform ttt = FTransform(GetControlRotation());
	Direction = ttt.TransformVector(Direction);
	// 2. TransformDirection기능 이용해서 방향을 만들어서
	// 3. 그 방향으로 이동
	AddMovementInput(Direction);
	Direction = FVector::ZeroVector;
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	UEnhancedInputComponent* input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
	if (input)
	{
		input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ATPSPlayer::OnMyActionMove);
		input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ATPSPlayer::OnMyActionLook);
		
		input->BindAction(IA_Jump, ETriggerEvent::Started, this, &ATPSPlayer::OnMyActionJump);
		input->BindAction(IA_Fire, ETriggerEvent::Started, this, &ATPSPlayer::OnMyActionFire);

		input->BindAction(IA_ChooseGun, ETriggerEvent::Started, this, &ATPSPlayer::OnMyActionChooseGun);
		input->BindAction(IA_ChooseSniper, ETriggerEvent::Started, this, &ATPSPlayer::OnMyActionChooseSniper);
		
		input->BindAction(IA_Zoom, ETriggerEvent::Started, this, &ATPSPlayer::OnMyActionZoomIn);
		input->BindAction(IA_Zoom, ETriggerEvent::Completed, this, &ATPSPlayer::OnMyActionZoomOut);
	}
}

void ATPSPlayer::OnMyActionMove(const FInputActionValue& Value)
{
	FVector2D v = Value.Get<FVector2D>();
	Direction.X = v.X;
	Direction.Y = v.Y;
	Direction.Normalize();
}

void ATPSPlayer::OnMyActionLook(const FInputActionValue& Value)
{
	FVector2D v = Value.Get<FVector2D>();

	AddControllerPitchInput(-v.Y);
	AddControllerYawInput(v.X);
}

void ATPSPlayer::OnMyActionJump(const FInputActionValue& Value)
{
	Jump();
}

void ATPSPlayer::OnMyActionFire(const FInputActionValue& Value)
{
	if (bChooseSniper)
	{
		// 카메라위치에서 카메라의 앞방향 100000cm 으로 보고싶다.
		FHitResult OutHit;
		FVector Start = CameraComp->GetComponentLocation();
		FVector End = Start + CameraComp->GetForwardVector() * 100000.f;
		ECollisionChannel TraceChannel = ECC_Visibility;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit , Start , End , TraceChannel , Params);
		if ( bHit )
		{
			// 바라본곳에 뭔가 있다.
			DrawDebugLine(GetWorld(), Start, OutHit.ImpactPoint, FColor::Red, false, 3);
		}
		else{
			// 허공
			DrawDebugLine(GetWorld() , Start , End , FColor::Red , false , 3);
		}
	}
	else
	{
		// 총알공장에서 총알을 만들어서 배치하고싶다.
		// FirePosition이라는 소켓의 Transform을 사용하고싶다.
		FTransform t = GunMeshComp->GetSocketTransform(TEXT("FirePosition"));
		GetWorld()->SpawnActor<ABullet>(BulletFactory , t);
	}
}

void ATPSPlayer::OnMyActionChooseGun(const FInputActionValue& Value)
{
	bChooseSniper = false;

	// 1번키를 누르면 Gun만 보이게 Sniper는 안보이게
	GunMeshComp->SetVisibility(true);
	SniperMeshComp->SetVisibility(false);

	// 조준선, 줌 UI를 모두 보이지않게 하고싶다.
	CrosshairUI->SetVisibility(ESlateVisibility::Hidden);
	ZoomUI->SetVisibility(ESlateVisibility::Hidden);

	// 원래 보이던데로 복원하고싶다.
	CameraComp->SetFieldOfView(90);
}

// 2번키를 누르면 Sniper만 보이게 Gun은 안보이게
void ATPSPlayer::OnMyActionChooseSniper(const FInputActionValue& Value)
{
	bChooseSniper = true;

	GunMeshComp->SetVisibility(false);
	SniperMeshComp->SetVisibility(true);

	// 조준선 보이게, 줌 UI 보이지않게 하고싶다.
	CrosshairUI->SetVisibility(ESlateVisibility::Visible);
	ZoomUI->SetVisibility(ESlateVisibility::Hidden);
}

void ATPSPlayer::OnMyActionZoomIn(const FInputActionValue& Value)
{
	// 만약 스나이퍼라면 아래 일을 하고싶다.
	if ( false == bChooseSniper )
	{
		return;
	}
	// 조준선 보이게, 줌 UI 보이지않게 하고싶다.
	CrosshairUI->SetVisibility(ESlateVisibility::Hidden);
	ZoomUI->SetVisibility(ESlateVisibility::Visible);

	// 가까이 보이게 하고싶다.
	CameraComp->SetFieldOfView(30);
}

void ATPSPlayer::OnMyActionZoomOut(const FInputActionValue& Value)
{
	// 만약 스나이퍼라면 아래 일을 하고싶다.
	if ( false == bChooseSniper )
	{
		return;
	}
	// 조준선 보이게, 줌 UI 보이지않게 하고싶다.
	CrosshairUI->SetVisibility(ESlateVisibility::Visible);
	ZoomUI->SetVisibility(ESlateVisibility::Hidden);

	// 원래 보이던데로 복원하고싶다.
	CameraComp->SetFieldOfView(90);
}

