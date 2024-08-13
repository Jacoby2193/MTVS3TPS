// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerFireComponent.h"
#include "TPSPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Blueprint/UserWidget.h"
#include "Enemy.h"
#include "Bullet.h"
#include "FSMComponent.h"
#include "TPSPlayerAnimInstance.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"

// Sets default values for this component's properties
UTPSPlayerFireComponent::UTPSPlayerFireComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// FireSFV를 로드해서 적용하고싶다.
	ConstructorHelpers::FObjectFinder<USoundBase> TempFireSFV(TEXT("/Script/Engine.SoundWave'/Game/TPS/Models/SniperGun/Rifle.Rifle'"));
	if ( TempFireSFV.Succeeded() )
	{
		FireSFV = TempFireSFV.Object;
	}

}


// Called when the game starts
void UTPSPlayerFireComponent::BeginPlay()
{
	Super::BeginPlay();

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
void UTPSPlayerFireComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 카메라의 FOV가 TargetFOV의 값으로 이동하고 싶다.
	Me->CameraComp->FieldOfView = FMath::Lerp(Me->CameraComp->FieldOfView , TargetFOV , DeltaTime * 5);

}

void UTPSPlayerFireComponent::SetupInputBinding(UEnhancedInputComponent* input)
{
	input->BindAction(IA_Fire , ETriggerEvent::Started , this , &UTPSPlayerFireComponent::OnMyActionFire);

	input->BindAction(IA_ChooseGun , ETriggerEvent::Started , this , &UTPSPlayerFireComponent::OnMyActionChooseGun);
	input->BindAction(IA_ChooseSniper , ETriggerEvent::Started , this , &UTPSPlayerFireComponent::OnMyActionChooseSniper);

	input->BindAction(IA_Zoom , ETriggerEvent::Started , this , &UTPSPlayerFireComponent::OnMyActionZoomIn);
	input->BindAction(IA_Zoom , ETriggerEvent::Completed , this , &UTPSPlayerFireComponent::OnMyActionZoomOut);
}


void UTPSPlayerFireComponent::OnMyActionFire(const FInputActionValue& Value)
{
	// AnimInstance의 PlayFireMontage함수를 호출하고싶다.
	check(Me->Anim);
	if ( Me->Anim )
	{
		Me->Anim->PlayFireMontage();
	}

	check(FireSFV);
	if ( FireSFV )
	{
		UGameplayStatics::PlaySound2D(GetWorld() , FireSFV);
	}

	check(FireCameraShake);
	if ( FireCameraShake )
	{
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(FireCameraShake);
	}


	if ( bChooseSniper )
	{
		// 카메라위치에서 카메라의 앞방향 100000cm 으로 보고싶다.
		FHitResult OutHit;
		FVector Start = Me->CameraComp->GetComponentLocation();
		FVector End = Start + Me->CameraComp->GetForwardVector() * 100000.f;
		ECollisionChannel TraceChannel = ECC_Visibility;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Me);

		bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit , Start , End , TraceChannel , Params);
		if ( bHit )
		{
			// 바라본곳에 뭔가 있다.

			// 폭발VFX를 표현하고싶다.
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld() , BulletImpactVFXFactory , OutHit.ImpactPoint);

			auto* hitComp = OutHit.GetComponent();
			// 만약 부딪힌 물체의 물리가 켜져있다면
			if ( hitComp && hitComp->IsSimulatingPhysics() )
			{
				float mass = hitComp->GetMass();
				// 힘 = 방향 = (부딪힌위치 - 출발위치) * 100000 * Mass
				FVector force = OutHit.ImpactPoint - OutHit.TraceStart;
				force.Normalize();
				force *= 10000 * mass;
				// hitComp에게 힘을 가하고싶다.
				hitComp->AddImpulse(force);
			}

			// 만약 부딪힌 액터가 AEnemy라면
			AEnemy* enemy = Cast<AEnemy>(OutHit.GetActor());
			// OnMyTakeDamage를 호출하고싶다.
			if ( enemy )
			{
				auto* fsm = enemy->GetComponentByClass<UFSMComponent>();
				if ( fsm )
				{
					fsm->OnMyTakeDamage(1);
				}
			}

			//DrawDebugLine(GetWorld(), Start, OutHit.ImpactPoint, FColor::Red, false, 3);
		}
		else {
			// 허공
			//DrawDebugLine(GetWorld() , Start , End , FColor::Red , false , 3);
		}
	}
	else
	{
		// 총알공장에서 총알을 만들어서 배치하고싶다.
		// FirePosition이라는 소켓의 Transform을 사용하고싶다.
		FTransform t = Me->GunMeshComp->GetSocketTransform(TEXT("FirePosition"));
		GetWorld()->SpawnActor<ABullet>(BulletFactory , t);
	}
}

void UTPSPlayerFireComponent::OnMyActionChooseGun(const FInputActionValue& Value)
{
	bChooseSniper = false;

	Me->ChooseSniperGun(false);

	// 1번키를 누르면 Gun만 보이게 Sniper는 안보이게
	Me->GunMeshComp->SetVisibility(true);
	Me->SniperMeshComp->SetVisibility(false);

	// 조준선, 줌 UI를 모두 보이지않게 하고싶다.
	CrosshairUI->SetVisibility(ESlateVisibility::Hidden);
	ZoomUI->SetVisibility(ESlateVisibility::Hidden);

	// 원래 보이던데로 복원하고싶다.
	Me->CameraComp->SetFieldOfView(90);
}

// 2번키를 누르면 Sniper만 보이게 Gun은 안보이게
void UTPSPlayerFireComponent::OnMyActionChooseSniper(const FInputActionValue& Value)
{
	bChooseSniper = true;

	Me->ChooseSniperGun(true);

	Me->GunMeshComp->SetVisibility(false);
	Me->SniperMeshComp->SetVisibility(true);

	// 조준선 보이게, 줌 UI 보이지않게 하고싶다.
	CrosshairUI->SetVisibility(ESlateVisibility::Visible);
	ZoomUI->SetVisibility(ESlateVisibility::Hidden);
}

void UTPSPlayerFireComponent::OnMyActionZoomIn(const FInputActionValue& Value)
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
	Me->CameraComp->SetFieldOfView(30);
	TargetFOV = 30;
}

void UTPSPlayerFireComponent::OnMyActionZoomOut(const FInputActionValue& Value)
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
	TargetFOV = 90;
	//CameraComp->SetFieldOfView(90);

}


