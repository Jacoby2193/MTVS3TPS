// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TPSPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MTVS3TPS_API UTPSPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// Vertical
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Speed;

	// Horizontal
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Direction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool IsInAir;
};
