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

	// ī�޶� �����ϰ�ʹ�.
	// ��������, ī�޶�
	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* SpringArmComp;
	
	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* CameraComp;


	// �̵� s ----------------------------------------------------------------------
	// �Է��� �޾Ƽ� 
	// IMC_TPSPlayer, IA_Move, IA_Look
	// �� �Է����� ������ �����
	// �� �������� ��� �̵��ϰ�ʹ�.

	UPROPERTY(EditDefaultsOnly)
	class UInputMappingContext* IMC_Player;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Move;

	void OnMyActionMove(const FInputActionValue& Value);

	FVector Direction;
	
	// ���콺 �Է��� �޾Ƽ� ȸ������ ó���ϰ�ʹ�.
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Look;

	void OnMyActionLook(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Jump;
	void OnMyActionJump(const FInputActionValue& Value);
	// �̵� e ----------------------------------------------------------------------

	// �ѽ�� s ----------------------------------------------------------------------
	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* GunMeshComp;
	
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* SniperMeshComp;

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Fire;

	void OnMyActionFire(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABullet> BulletFactory;
	// �ѽ�� e ----------------------------------------------------------------------
};
