// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHPWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerHPWidget::UpdateHP(float cur , float max)
{
	HPBar->SetPercent(cur / max);

	FString txt = FString::Printf(TEXT("%d / %d") , (int32)cur , (int32)max);
	HPText->SetText(FText::FromString(txt));
}
