// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkProject/Public/ServerGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
//������

UServerGameInstance::UServerGameInstance()
{
	sessionID = "Test Session";
}
//���� ����Ǵ� �Լ�
void UServerGameInstance::Init()
{
	Super::Init();
	// ������ �����Ѵ�.
	// �¶��� ���� ����� �����Ǿ� �ִ� IOnlineSubsystem Ŭ������ �����´�.
	IOnlineSubsystem* subsys = IOnlineSubsystem::Get();
	if (subsys)
	{
		sessionInterface = subsys->GetSessionInterface();
		if(sessionInterface != nullptr)
		{
			sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this,&UServerGameInstance::OnCreateSessionComplete);
		}
	}
}

// ���� ���� �Լ�
void UServerGameInstance::CreateMySession(FString roomName, int32 playerCount)
{
	if(sessionInterface != nullptr)
	{
		// ���� ���� ������ �����.
		FOnlineSessionSettings sessionSettings;
		sessionSettings.bAllowInvites = false;
		sessionSettings.bAllowJoinInProgress = true;
		sessionSettings.bAllowJoinViaPresence = true;
		sessionSettings.bIsDedicated = false;
		sessionSettings.bShouldAdvertise = true;
		// �¶��� ����ý����� �̸��� "NULL"�̸� LAN��Ī�� �ϰ�, "Steam�̸� ���������� ��Ī
		sessionSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName()=="NULL";
		sessionSettings.NumPublicConnections = 3;
		sessionSettings.Set(FName("KEY_RoomName"), roomName);
		

		//������ �����Ѵ�.
		sessionInterface->CreateSession(0, sessionID, sessionSettings);

		UE_LOG(LogTemp, Warning, TEXT(" Create Session try!"));
	}
}

// ������ ������ ��������� �� ȣ��� �Լ�
void UServerGameInstance::OnCreateSessionComplete(FName sessionName, bool bisSuccess)
{
	FString result = bisSuccess ? TEXT("Create Session Success!") : TEXT("Create Session ");
	UE_LOG(LogTemp, Warning, TEXT(" % s: % s"), *result,*sessionName.ToString());
}
