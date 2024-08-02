// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "TPSPlayer.generated.h"

UCLASS()
class MTVS3TPS_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 카메라를 설정하고싶다.
	// 스프링암, 카메라
	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* SpringArmComp;
	
	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* CameraComp;


	// 이동 s ----------------------------------------------------------------------
	// 입력을 받아서 
	// IMC_TPSPlayer, IA_Move, IA_Look
	// 그 입력으로 방향을 만들고
	// 그 방향으로 계속 이동하고싶다.

	UPROPERTY(EditDefaultsOnly)
	class UInputMappingContext* IMC_Player;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Move;

	void OnMyActionMove(const FInputActionValue& Value);

	FVector Direction;
	
	// 마우스 입력을 받아서 회전값을 처리하고싶다.
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Look;

	void OnMyActionLook(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Jump;
	void OnMyActionJump(const FInputActionValue& Value);

	// 걷기 뛰기
	//  - 입력값, 함수
	//  - 걷는 속력,
	//  - 뛰는 속력
	UPROPERTY(EditDefaultsOnly)
	float WalkSpeed = 600;

	UPROPERTY(EditDefaultsOnly)
	float RunSpeed = 1000;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Run;

	void OnMyActionRun(const FInputActionValue& Value);
	void OnMyActionWalk(const FInputActionValue& Value);


	// 왼쪽 컨트롤 버튼을 누르고 있을때는 쪼그리기, 떼면 해제 하고싶다.
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_CrouchedCtrl;

	void OnMyActionCrouch(const FInputActionValue& Value);
	void OnMyActionUnCrouch(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_CrouchedC;
	void OnMyActionCrouchToggle(const FInputActionValue& Value);

	bool bCrouched;
	
	UPROPERTY()
	class UTPSPlayerAnimInstance* Anim;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_DiveRoll;

	void OnMyActionDiveRoll(const FInputActionValue& Value);


	// 이동 e ----------------------------------------------------------------------

	// 총쏘기 s ----------------------------------------------------------------------
	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* GunMeshComp;
	
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* SniperMeshComp;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Fire;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_ChooseGun;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_ChooseSniper;

	void OnMyActionFire(const FInputActionValue& Value);
	void OnMyActionChooseGun(const FInputActionValue& Value);
	void OnMyActionChooseSniper(const FInputActionValue& Value);

	// 태어날 때 Gun만 보이게하고싶다.
	// 1번키를 누르면 Gun만 보이게
	// 2번키를 누르면 Sniper만 보이게

	// 태어날 때 조준선UI와 ZoomUI를 생성하고싶다. Viewport에 붙이고싶다. 보이지않게 하고싶다.
	// 1번키를 누르면 조준선UI와 ZoomUI를 안보이게 하고싶다.
	// 2번키를 누르면  조준선UI를 보이게, ZoomUI를 안보이게 하고싶다.
	// Zoom키(마우스오른쪽키)를 누르면 조준선UI를 안보이게, ZoomUI를 보이게 하고싶다.

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Zoom;

	void OnMyActionZoomIn(const FInputActionValue& Value);
	void OnMyActionZoomOut(const FInputActionValue& Value);
	// Factory, 생성된 UI를 기억할 변수
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> CrosshairUIFactory;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> ZoomUIFactory;

	UPROPERTY()
	class UUserWidget* CrosshairUI;

	UPROPERTY()
	class UUserWidget* ZoomUI;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABullet> BulletFactory;

	// 스너이퍼일때만 Zoom을 하고싶다.. UI도...
	bool bChooseSniper;

	// 총을쏘면 맞은 자리에 총알자국VFX을 표현하고싶다.
	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* BulletImpactVFXFactory;


	// ZoomOut을 하면 부드럽게 원래 시야각으로 돌아오고싶다.
	// 목표 시야각
	float TargetFOV = 90;

	// 총쏘기 소리를 내고싶다.
	UPROPERTY(EditDefaultsOnly)
	class USoundBase* FireSFV;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCameraShakeBase> FireCameraShake;
	
	// 총쏘기 e ----------------------------------------------------------------------
};
