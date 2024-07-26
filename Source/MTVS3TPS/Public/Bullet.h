// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class MTVS3TPS_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 충돌체, 외관을 만들고싶다.
	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* SphereComp;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* MeshComp;

	// 발사체 컴포넌트를 만들고싶다.
	UPROPERTY(EditDefaultsOnly)
	class UProjectileMovementComponent* MovementComp;

private:
	void Die();
};
