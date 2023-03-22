// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "ServerGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKPROJECT_API UServerGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UServerGameInstance();
	virtual void Init() override;
	// ��������
	IOnlineSessionPtr sessionInterface;
	FName sessionID;

	// �Լ�
	void CreateMySession(FString roomName, int32 playerCount);

	UFUNCTION()
	void OnCreateSessionComplete(FName sessionName,bool bisSuccess);
};






