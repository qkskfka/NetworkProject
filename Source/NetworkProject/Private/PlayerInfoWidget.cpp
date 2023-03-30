// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfoWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "../NetworkProjectCharacter.h"


void UPlayerInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	player = Cast<ANetworkProjectCharacter>(GetOwningPlayerPawn());

	if (player == nullptr)
	{
		ServerHealthBar(player->GetHealth());
	}
}

void UPlayerInfoWidget::NativeTick()
{
	Super::NativeTick(MyGeomtry, InDeltaTime);

	if (player == nullptr)
	{
		return;
	}
}

void UPlayerInfoWidget::SetPlayer(class ANetworkProjectCharacter* player)
{
	myPlayer = player;
}

void UPlayerInfoWidget::SeverSetHealthBar(const int32 value)
{
	MulticastSetHealthBar(value);
}

void UPlayerInfoWidget::MulticastSetHealthBar(const int32 value)
{
	float calcHP = (float)player->GetHealth() / (float)player->maxHP;
	pb_HP->SetPercent(calcHP);
}
