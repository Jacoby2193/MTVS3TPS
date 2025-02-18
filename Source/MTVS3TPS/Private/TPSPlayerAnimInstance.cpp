﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerAnimInstance.h"
#include "TPSPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

void UTPSPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ATPSPlayer* player = Cast<ATPSPlayer>(TryGetPawnOwner());
	if ( nullptr == player )
	{
		return;
	}

	FVector velocity = player->GetVelocity();
	FVector forwardVector = player->GetActorForwardVector();
	FVector rightVector = player->GetActorRightVector();

	// 앞 뒤 속도
	Speed = FVector::DotProduct(forwardVector , velocity);

	// 좌우 속도
	Direction = FVector::DotProduct(rightVector , velocity);

	// 캐릭터가 공중에 떠있는가?
	IsInAir = player->GetCharacterMovement()->IsFalling();

	// 주인공의 AimRotation값을 가져와서 Pitch값을 채우고싶다.

	Pitch = player->GetBaseAimRotation().Pitch;

	//UE_LOG(LogTemp , Warning , TEXT("Pitch : %f") , Pitch);
}

void UTPSPlayerAnimInstance::PlayFireMontage()
{
	Montage_Play(FireMontage);
}

void UTPSPlayerAnimInstance::PlayDiveRollMontage()
{
	Montage_Play(DiveRollMontage);
}
