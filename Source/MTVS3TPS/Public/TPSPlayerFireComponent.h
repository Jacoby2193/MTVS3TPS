// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "TPSPlayerFireComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MTVS3TPS_API UTPSPlayerFireComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTPSPlayerFireComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupInputBinding(class UEnhancedInputComponent* input);

	UPROPERTY()
	class ATPSPlayer* Me;


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
		
};
