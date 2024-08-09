// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "TPSPlayerMoveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MTVS3TPS_API UTPSPlayerMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTPSPlayerMoveComponent();

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
	

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_DiveRoll;

	void OnMyActionDiveRoll(const FInputActionValue& Value);
		
};
