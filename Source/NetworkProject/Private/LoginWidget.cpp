// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidget.h"
#include "LoginWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/Slider.h"
#include "Components/WidgetSwitcher.h"
#include "ServerGameInstance.h"

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

	gameInstance = Cast<UServerGameInstance>(GetGameInstance());

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

	gameInstance->CreateMySession(editText_RoomName->GetText().ToString(), sliderPlayerCount->GetValue());
}

// 슬라이더 값을 움직이는 함수
void ULoginWidget::OnMoveSlider(float value)
{
	FString num2str =FString::Printf(TEXT("%d"),value);
	playerCount->SetText(FText::FromString(num2str));
}
