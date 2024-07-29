// Fill out your copyright notice in the Description page of Project Settings.


#include "FSMComponent.h"

// Sets default values for this component's properties
UFSMComponent::UFSMComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFSMComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}



void UFSMComponent::TickComponent(float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime , TickType , ThisTickFunction);

	FString myState = UEnum::GetValueAsString(State);
	DrawDebugString(GetWorld() , GetOwner()->GetActorLocation(), myState , nullptr , FColor::Yellow , 0, true, 1);

	switch ( State )
	{
	case EEnemyState::IDLE:		TickIdle(DeltaTime);		break;
	case EEnemyState::MOVE:		TickMove(DeltaTime);		break;
	case EEnemyState::ATTACK:	TickAttack(DeltaTime);		break;
	}
}

void UFSMComponent::TickIdle(const float& DeltaTime)
{

}

void UFSMComponent::TickMove(const float& DeltaTime)
{

}

void UFSMComponent::TickAttack(const float& DeltaTime)
{

}

