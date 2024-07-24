// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"

// Sets default values
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ���������� �����ؼ� ��Ʈ�� ���̰�ʹ�.
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeLocation(FVector(0, 40, 80));
	SpringArmComp->TargetArmLength = 200;

	// ī�޶� �����ؼ� �������Ͽ� ���̰�ʹ�.
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	// IMC_Player�� �߰��ϰ�ʹ�.

	// 1. ��Ʈ�ѷ��� �����ͼ� PlayerController���� ĳ���� �غ���.
	auto* pc = Cast<APlayerController>(Controller);
	// 2. ĳ���� �����ߴٸ� 
	if (pc)
	{
		// 3. �� ��ü�� �̿��ؼ� EnhancedInputLocalPlayerSubsystem�� �����´�.
		UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		// 4. SubSys�� �̿��ؼ� AddMappingContext�� �Ѵ�.
		if (subSys)
		{
			subSys->AddMappingContext(IMC_Player, 0);
		}
	}

	
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// �̵��ϰ� �ʹ�.
	// ȸ�� �������� �̵��ϰ�ʹ�.
	// 1. ControlRotation �� �̿��ؼ� Transform�� �����
	FTransform ttt = FTransform(GetControlRotation());
	Direction = ttt.TransformVector(Direction);
	// 2. TransformDirection��� �̿��ؼ� ������ ����
	// 3. �� �������� �̵�
	AddMovementInput(Direction, 1);
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

