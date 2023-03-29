// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkProject/Public/ServerGameInstance.h"

#include "LoginWidget.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"
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
			sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UServerGameInstance::OnFindSessionComplete);
			sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UServerGameInstance::OnJoinSessionComplete);
		}

		// 연결 플랫폼의 이름을 출력한다.
		FString platformname = subsys->GetSubsystemName().ToString();
		UE_LOG(LogTemp, Warning, TEXT("Platform Name: %s"), *platformname);
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
		sessionSettings.NumPublicConnections = playerCount;
		sessionSettings.Set(FName("KEY_RoomName"), roomName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		

		//세션을 생성한다.
		sessionInterface->CreateSession(0, sessionID, sessionSettings);

		UE_LOG(LogTemp, Warning, TEXT(" Create Session try!"));
	}
}

	//만들어져 있는 세션을 찾는함수.
void UServerGameInstance::FindMySession()
{
	//찾으려는 세션 쿼리를 생성한다.
	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	SessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NUL";
	SessionSearch->MaxSearchResults = 30;
	//특정 키로 검색 조건을 필터링하고 싶을때 추가하는 쿼리
	//SessionSearch->QuerySettings.Set(SEARCH_KEYWORD,FTEXT(" roomName"),EOnlineComparisonOp::GreaterThanEquals);

	// Presence로 생성된 세션을 필터링하고 싶을때
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	
	// 앞에서 만든 쿼리를 이용해서 세션을 찾는다.
	sessionInterface->FindSessions(0,  SessionSearch.ToSharedRef());
}
// 찾은 세션 리스트 중에서 특정 세션에 들어가고자 할때 사용할 함수
void UServerGameInstance::joinMySession(int32 sessionIdx)
{
	// 인텍스로 세션을 선택하고 그 세션으로 조인한다.
	FOnlineSessionSearchResult selectedSession  = SessionSearch->SearchResults[sessionIdx];
	sessionInterface->JoinSession(0, sessionID, selectedSession);
}

// 세션이 서버에 만들어졌을 때 호출된 함수
void UServerGameInstance::OnCreateSessionComplete(FName sessionName, bool bisSuccess)
{
	FString result = bisSuccess ? TEXT("Create Session Success!") : TEXT("Create Session ");
	UE_LOG(LogTemp, Warning, TEXT(" % s: % s"), *result,*sessionName.ToString());

	// 세션 생성에 성공했다면, 전투 맵 쪽으로 세션에 접속한 모든 인원을 이동시킨다.
	if (bisSuccess)
	{
		GetWorld()->ServerTravel("/Game/Maps/MainMap?Listen");
	}
	
}
// 세션 검색이 끝났을때 호출될 함수
void UServerGameInstance::OnFindSessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		TArray<FOnlineSessionSearchResult> searchResults = SessionSearch->SearchResults;
		UE_LOG(LogTemp, Warning, TEXT("Find Session count: %d"), searchResults.Num());

		//검색 결과들을 모두 순회한다.
		for(int32 i = 0; i < searchResults.Num(); i++)
		{
			FSessionInfo searchedSessionInfo;
			
			FString roomName;
			searchResults[i].Session.SessionSettings.Get(FName("KEY_RoomName"),roomName);
			searchedSessionInfo.roomName = roomName;
			
			searchedSessionInfo.maxPlayers = searchResults[i].Session.SessionSettings.NumPublicConnections;
			searchedSessionInfo.currentPlayers = searchedSessionInfo.maxPlayers - searchResults[i].Session.NumOpenPublicConnections;
			searchedSessionInfo.ping = searchResults[i].PingInMs;
			searchedSessionInfo.idx = i;
			// 슬롯 생성에 필요한 정보를 이벤트로 송출한다.
			SearchResultDele.Broadcast(searchedSessionInfo);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Find Sessions Failed..."));
	}

	// 새로고침 버튼을 다시 활성화한다.
	SearchFinishedDele.Broadcast();
}
// 다른 세션에 합류 처리가 끝났을때 호출되는 이벤트 함수
void UServerGameInstance::OnJoinSessionComplete(FName sessionName, enum EOnJoinSessionCompleteResult::Type joinResult)
{
	// 만일, join에 성공했다면 해당 IP Address로 레벨 이동을 한다.
	if(joinResult == EOnJoinSessionCompleteResult::Success)
	{
		// 세션 이름으로 IP 주소를 획득한다.
		FString joinAddress;
		// GetResolvedConnectString 은 세션IP를 넘겨주는 함수이다.
		sessionInterface->GetResolvedConnectString(sessionName, joinAddress);

		UE_LOG(LogTemp, Warning, TEXT("Join Address: %s"), *joinAddress);
		
		if(APlayerController* pc = GetWorld()->GetFirstPlayerController())
		{
			pc->ClientTravel(joinAddress, ETravelType::TRAVEL_Absolute);
		}
	}
}
