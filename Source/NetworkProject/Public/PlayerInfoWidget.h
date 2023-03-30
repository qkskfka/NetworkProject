// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKPROJECT_API UPlayerInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick() override;

public:
	UPPROPERTY(meta=(BindWidget))
		class UTextBlock* text_Name;

	UPPROPERTY(meta=(BindWidget))
		class UTextBlock* pb_HP;

	void SetPlayer(class ANetworkProjectCharacter* player)

	UFUNCTION()
	void SeverSetHealthBar(const int32 value);

	UFUNCTION()
	void MulticastSetHealthBar(const int32 value);
private:
	class ANetworkProjectCharacter* myPlayer;
};
