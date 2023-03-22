// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkProject/Public/ServerGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
//생성자

UServerGameInstance::UServerGameInstance()
{
	sessionID = "Test Session";
}
//최초 실행되는 함수
void UServerGameInstance::Init()
{
	Super::Init();
	// 세션을 실행한다.
	// 온라인 세션 기능이 구현되어 있는 IOnlineSubsystem 클래스를 가져온다.
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

// 세션 생성 함수
void UServerGameInstance::CreateMySession(FString roomName, int32 playerCount)
{
	if(sessionInterface != nullptr)
	{
		// 세션 생성 정보를 만든다.
		FOnlineSessionSettings sessionSettings;
		sessionSettings.bAllowInvites = false;
		sessionSettings.bAllowJoinInProgress = true;
		sessionSettings.bAllowJoinViaPresence = true;
		sessionSettings.bIsDedicated = false;
		sessionSettings.bShouldAdvertise = true;
		// 온라인 서브시스템의 이름이 "NULL"이면 LAN매칭을 하고, "Steam이면 스팀서버로 매칭
		sessionSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName()=="NULL";
		sessionSettings.NumPublicConnections = 3;
		sessionSettings.Set(FName("KEY_RoomName"), roomName);
		

		//세션을 생성한다.
		sessionInterface->CreateSession(0, sessionID, sessionSettings);

		UE_LOG(LogTemp, Warning, TEXT(" Create Session try!"));
	}
}

// 세션이 서버에 만들어졌을 때 호출된 함수
void UServerGameInstance::OnCreateSessionComplete(FName sessionName, bool bisSuccess)
{
	FString result = bisSuccess ? TEXT("Create Session Success!") : TEXT("Create Session ");
	UE_LOG(LogTemp, Warning, TEXT(" % s: % s"), *result,*sessionName.ToString());
}
