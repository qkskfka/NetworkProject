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
	//Ȥ�� �� �Է� �ʵ��� ���� �� ������ �ʱ�ȭ �Ѵ�.
	editText_id->SetText(FText::FromString(""));
	editText_RoomName->SetText(FText::FromString(""));
	sliderPlayerCount->SetValue(2.0f);
	playerCount->SetText(FText::FromString("2"));

	// ��ư Ŭ�� �̺�Ʈ�� �Լ� ���ε��ϱ�
	buttonStart->OnClicked.AddDynamic(this, &ULoginWidget::ClickStart);
	buttonCreateSession->OnClicked.AddDynamic(this, &ULoginWidget::CreateServer);
	sliderPlayerCount->OnValueChanged.AddDynamic(this, &ULoginWidget::OnMoveSlider);

	gameInstance = Cast<UServerGameInstance>(GetGameInstance());

}

void ULoginWidget::ClickStart()
{
	// ����, ID�� �� ĭ�� �ƴ϶�� 0��-> 1�� ĵ������ �����Ѵ�.
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

// �����̴� ���� �����̴� �Լ�
void ULoginWidget::OnMoveSlider(float value)
{
	FString num2str =FString::Printf(TEXT("%d"),value);
	playerCount->SetText(FText::FromString(num2str));
}
