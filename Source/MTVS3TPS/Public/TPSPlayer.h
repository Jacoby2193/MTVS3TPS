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
	// 이동 e ----------------------------------------------------------------------

	// 총쏘기 s ----------------------------------------------------------------------
	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* GunMeshComp;
	
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* SniperMeshComp;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Fire;

	void OnMyActionFire(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABullet> BulletFactory;
	// 총쏘기 e ----------------------------------------------------------------------
};
