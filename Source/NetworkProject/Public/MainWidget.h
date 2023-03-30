// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

UCLASS()
class NETWORKPROJECT_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnyWhere,meta = (BindWidget))
	class UTextBlock* text_Health;

	UPROPERTY(VisibleAnyWhere,meta = (BindWidget))
	class UTextBlock* text_Ammo;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	class ANetworkProjectCharacter* player;

};
