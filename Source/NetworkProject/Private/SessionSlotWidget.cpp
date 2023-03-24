// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionSlotWidget.h"
#include "ServerGameInstance.h"
#include "Components/Button.h"

void USessionSlotWidget::NativeConstruct()
{
	//Super::NativeConstruct();
	GameInstance = Cast<UServerGameInstance>(GetGameInstance());

	btn_roomName->OnClicked.AddDynamic(this, &USessionSlotWidget::JoinRoom);
}

void USessionSlotWidget::JoinRoom()
{
	if(GameInstance != nullptr)
	{
		GameInstance->joinMySession(index);
	}
}
