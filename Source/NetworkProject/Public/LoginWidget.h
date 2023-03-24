// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"

UCLASS()
class NETWORKPROJECT_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UEditableText* editText_id;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UButton* buttonStart;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UEditableText* editText_RoomName;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class USlider* sliderPlayerCount;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* playerCount;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UButton* buttonCreateSession;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UWidgetSwitcher* WidgetSwitcher;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UButton*btn_GoCreate;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UButton*btn_GoFind;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UButton*btn_Refresh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UButton*btn_CreateBack;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UButton*btn_FindBack;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
		class UScrollBox*sbox_Roomlist;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class USessionSlotWidget> sessionSlot;

protected:
	virtual void NativeConstruct() override;

private:
	class UServerGameInstance* gameInstance;

	UFUNCTION()
	void ClickStart();

	UFUNCTION()
	void CreateServer();

	UFUNCTION()
	void OnMoveSlider(float value);

	UFUNCTION()
	void GoCreate();

	UFUNCTION()
	void GoFind();

	UFUNCTION()
	void GoBack();

	UFUNCTION()
	void AddNewSlot(FSessionInfo SessionInfo);

	UFUNCTION()
	void Refreshlist();

	UFUNCTION()
	void RefreshEnabled();
};




