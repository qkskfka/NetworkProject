// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/Slider.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ScrollBox.h"
#include "ServerGameInstance.h"
#include "SessionSlotWidget.h"

void ULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//혹시 모를 입력 필드의 값을 빈 같으로 초기화 한다.
	editText_id->SetText(FText::FromString(""));
	editText_RoomName->SetText(FText::FromString(""));
	sliderPlayerCount->SetValue(2.0f);
	playerCount->SetText(FText::FromString("2"));

	// 버튼 클릭 이벤트에 함수 바인딩하기
	buttonStart->OnClicked.AddDynamic(this, &ULoginWidget::ClickStart);
	buttonCreateSession->OnClicked.AddDynamic(this, &ULoginWidget::CreateServer);
	sliderPlayerCount->OnValueChanged.AddDynamic(this, &ULoginWidget::OnMoveSlider);
	btn_GoCreate->OnClicked.AddDynamic(this, &ULoginWidget::GoCreate);
	btn_GoFind->OnClicked.AddDynamic(this, &ULoginWidget::GoFind);
	btn_CreateBack->OnClicked.AddDynamic(this, &ULoginWidget::GoBack);
	btn_FindBack->OnClicked.AddDynamic(this, &ULoginWidget::GoBack);
	
	gameInstance = Cast<UServerGameInstance>(GetGameInstance());

	if (gameInstance != nullptr)
	{
		gameInstance->SearchResultDele.AddDynamic(this, &ULoginWidget::AddNewSlot);
	}

}

void ULoginWidget::ClickStart()
{
	// 만일, ID가 빈 칸이 아니라면 0번-> 1번 캔버스로 변경한다.
	if (!editText_id->GetText().IsEmpty())
	{
		WidgetSwitcher->SetActiveWidgetIndex(1);
		gameInstance->sessionID = FName(*editText_id->GetText().ToString());
	}
}

void ULoginWidget::CreateServer()
{
	int32 playerCnt = FMath::RoundHalfFromZero(sliderPlayerCount->GetValue());

	gameInstance->CreateMySession(editText_RoomName->GetText().ToString(), playerCnt);
}

// 슬라이더 값을 움직이는 함수
void ULoginWidget::OnMoveSlider(float value)
{
	int32 newVal = FMath::RoundHalfFromZero(value);
	playerCount->SetText(FText::AsNumber(newVal));
}

inline void ULoginWidget::GoCreate()
{
	WidgetSwitcher->SetActiveWidgetIndex(2);
}

inline void ULoginWidget::GoFind()
{
	WidgetSwitcher->SetActiveWidgetIndex(3);
	gameInstance->FindMySession();
}

void ULoginWidget::GoBack()
{
	WidgetSwitcher->SetActiveWidgetIndex(1);
}

//게임 인스턴스로부터 검색 완료 이벤트를 받았을 때 실행될 함수
void ULoginWidget::AddNewSlot(FString roomName, int32 currentPlayers, int32 maxPlayers, int32 ping)
{
	USessionSlotWidget* SlotWidget = CreateWidget<USessionSlotWidget>(this, sessionSlot);
	if (SlotWidget != nullptr)
	{
		SlotWidget->text_roomName->SetText(FText::FromString(roomName));
		SlotWidget->text_playerInfo->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"),currentPlayers, maxPlayers)));
		SlotWidget->text_ping->SetText(FText::FromString(FString::Printf(TEXT("%d ms"), ping)));
		sbox_Roomlist->AddChild(SlotWidget);
	}
}
