// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerMoveComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "TPSPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TPSPlayerAnimInstance.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"

// Sets default values for this component's properties
UTPSPlayerMoveComponent::UTPSPlayerMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTPSPlayerMoveComponent::BeginPlay()
{
	Super::BeginPlay();



	
}


// Called every frame
void UTPSPlayerMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 이동하고 싶다.
		// 회전 방향으로 이동하고싶다.
		// 1. ControlRotation 을 이용해서 Transform을 만들고
	FTransform ttt = FTransform(Me->GetControlRotation());
	Direction = ttt.TransformVector(Direction);
	Direction.Z = 0;
	Direction.Normalize();
	// 2. TransformDirection기능 이용해서 방향을 만들어서
	// 3. 그 방향으로 이동
	Me->AddMovementInput(Direction);
	Direction = FVector::ZeroVector;
}

void UTPSPlayerMoveComponent::SetupInputBinding(UEnhancedInputComponent* input)
{
	input->BindAction(IA_Move , ETriggerEvent::Triggered , this , &UTPSPlayerMoveComponent::OnMyActionMove);
	input->BindAction(IA_Look , ETriggerEvent::Triggered , this , &UTPSPlayerMoveComponent::OnMyActionLook);

	input->BindAction(IA_Jump , ETriggerEvent::Started , this , &UTPSPlayerMoveComponent::OnMyActionJump);

	input->BindAction(IA_Run , ETriggerEvent::Started , this , &UTPSPlayerMoveComponent::OnMyActionRun);
	input->BindAction(IA_Run , ETriggerEvent::Completed , this , &UTPSPlayerMoveComponent::OnMyActionWalk);

	input->BindAction(IA_CrouchedCtrl , ETriggerEvent::Started , this , &UTPSPlayerMoveComponent::OnMyActionCrouch);
	input->BindAction(IA_CrouchedCtrl , ETriggerEvent::Completed , this , &UTPSPlayerMoveComponent::OnMyActionUnCrouch);

	input->BindAction(IA_CrouchedC , ETriggerEvent::Started , this , &UTPSPlayerMoveComponent::OnMyActionCrouchToggle);

	input->BindAction(IA_DiveRoll , ETriggerEvent::Started , this , &UTPSPlayerMoveComponent::OnMyActionDiveRoll);
}


void UTPSPlayerMoveComponent::OnMyActionMove(const FInputActionValue& Value)
{
	FVector2D v = Value.Get<FVector2D>();
	Direction.X = v.X;
	Direction.Y = v.Y;
	Direction.Normalize();
}

void UTPSPlayerMoveComponent::OnMyActionLook(const FInputActionValue& Value)
{
	FVector2D v = Value.Get<FVector2D>();

	Me->AddControllerPitchInput(-v.Y);
	Me->AddControllerYawInput(v.X);
}

void UTPSPlayerMoveComponent::OnMyActionJump(const FInputActionValue& Value)
{
	Me->Jump();
}

void UTPSPlayerMoveComponent::OnMyActionRun(const FInputActionValue& Value)
{
	// 최대속력을  RunSpeed 로 하고싶다.
	Me->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void UTPSPlayerMoveComponent::OnMyActionWalk(const FInputActionValue& Value)
{
	// 최대속력을  WalkSpeed 로 하고싶다.
	Me->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void UTPSPlayerMoveComponent::OnMyActionCrouch(const FInputActionValue& Value)
{
	Me->Anim->IsCrouched = true;
	Me->Crouch();
}

void UTPSPlayerMoveComponent::OnMyActionUnCrouch(const FInputActionValue& Value)
{
	Me->Anim->IsCrouched = false;
	Me->UnCrouch();
}

void UTPSPlayerMoveComponent::OnMyActionCrouchToggle(const FInputActionValue& Value)
{
	bCrouched = !bCrouched;
	Me->Anim->IsCrouched = bCrouched;
	if ( bCrouched )
	{
		Me->Crouch();
	}
	else
	{
		Me->UnCrouch();
	}
}

void UTPSPlayerMoveComponent::OnMyActionDiveRoll(const FInputActionValue& Value)
{
	if ( Me->Anim )
	{
		Me->Anim->PlayDiveRollMontage();
	}
}