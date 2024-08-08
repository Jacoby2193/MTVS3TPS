// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHPWidget.h"
#include "Components/ProgressBar.h"

void UEnemyHPWidget::NativeTick(const FGeometry& MyGeometry , float InDeltaTime)
{
	Super::NativeTick(MyGeometry , InDeltaTime);
}

void UEnemyHPWidget::SetHPBar(float cur , float max)
{
	// 구현하세요.
	// HPBar
	HPBar->SetPercent(cur / max);
}
