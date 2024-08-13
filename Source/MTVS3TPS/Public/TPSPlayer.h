// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "TPSPlayer.generated.h"


//DECLARE_DELEGATE_OneParam(FDelegate, int32)
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateDynamicMulti)

DECLARE_MULTICAST_DELEGATE_OneParam(FInputBindingDelegate, class UEnhancedInputComponent*)

UCLASS()
class MTVS3TPS_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

	FInputBindingDelegate InputBindingDelegate;

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
	class UTPSPlayerBaseComponent* MoveComp;

	UPROPERTY(EditDefaultsOnly)
	class UTPSPlayerBaseComponent* FireComp;


	// 체력을 만들고싶다.
	// 적이 나를 때리면 체력을 감소시키고싶다.
	// UI에도 반영하고싶다.
	UPROPERTY(EditDefaultsOnly)
	int32 MaxHP = 2;
	UPROPERTY(EditDefaultsOnly)
	int32 HP = MaxHP;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> PlayerHPWidgetFactory;

	class UPlayerHPWidget* PlayerHPUI;

	void OnMyHitDamage(int32 damage);

	
	// 구현은 C++
	//UFUNCTION(BlueprintCallable)
	//void Test(){

	//}
	// 구현은 블루프린트에서 하고싶다.
	UFUNCTION(BlueprintImplementableEvent)
	void ShowGameOverUI();

	// 현재 선택한 총의 종류를 알고싶다. 구현부는 C++ 블루프린트
	UFUNCTION(BlueprintNativeEvent)
	void ChooseSniperGun(bool bSiperGun);
};
