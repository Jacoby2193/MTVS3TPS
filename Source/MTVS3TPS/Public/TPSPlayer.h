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

	UPROPERTY(EditDefaultsOnly)
	class UInputMappingContext* IMC_Player;

	UPROPERTY()
	class UTPSPlayerAnimInstance* Anim;

	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* GunMeshComp;
	
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* SniperMeshComp;

	UPROPERTY(EditDefaultsOnly)
	class UTPSPlayerMoveComponent* MoveComp;

	UPROPERTY(EditDefaultsOnly)
	class UTPSPlayerFireComponent* FireComp;
};
