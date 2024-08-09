// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"




UCLASS()
class MTVS3TPS_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly)
	class UFSMComponent* FSMComp;


	// 체력UI를 생성해서 머리위에 배치하고싶다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UWidgetComponent* HPComp;
	
	UPROPERTY()
	class UEnemyHPWidget* HPWidget;
};
