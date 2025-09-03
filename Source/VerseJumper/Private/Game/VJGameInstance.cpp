// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/VJGameInstance.h"

#include "MoviePlayer.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/SViewport.h"
//STEAM
#pragma warning(push)
#pragma warning(disable : 4996)
#include "VerseJumper/Steam/steam_api.h"
#pragma warning(pop)

void UVJGameInstance::Init()
{
	Super::Init();

	// Steam Init
	//E:\EpicGames\UE_5.5\Engine\Binaries\Win64에 steam_appid 파일 사용 (테스트용)
	if (SteamAPI_RestartAppIfNecessary(atoi(STEAM_APP_ID)))
	{
		UE_LOG(LogTemp, Error, TEXT("Steam API Restart"));
	}

	if (!SteamAPI_Init())
	{
		UE_LOG(LogTemp, Error, TEXT("SteamAPI_Init Return False"));
	}

	// 언어 설정
	InitCulture();
	
	// 로딩 스크린 표시위한 바인딩
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this,&UVJGameInstance::OnPreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this,&UVJGameInstance::OnPostLoadMap);
}

FString UVJGameInstance::GetCurrentGameLanguage()
{
	if (SteamApps())
	{
		const char* Cur = SteamApps()->GetCurrentGameLanguage(); // 예: "korean"
		const FString SteamToken = UTF8_TO_TCHAR(Cur);           
		const FString IETF = MapSteamToIETF(SteamToken);
		if (FInternationalization::Get().IsCultureAllowed(IETF))
		{
			return IETF;
		}
	}
	// 영어로 폴백
	return FString("en");
}

void UVJGameInstance::InitCulture()
{
	FString Culture;
	// 1. 플레이어가 수정한 언어 설정이 있다면 해당 언어 우선
	if (!GConfig->GetString(TEXT("Internationalization"), TEXT("Culture"), Culture, GGameUserSettingsIni))
	{
		// 2. 없다면 기본 세팅으로
		Culture = GetCurrentGameLanguage();
	}
	FInternationalization::Get().SetCurrentCulture(Culture);

	if (GEngine)
	{
		if (UGameUserSettings* GS = GEngine->GetGameUserSettings())
		{
			GS->LoadSettings();
			GS->ApplySettings(false);
		}
	}
}


void UVJGameInstance::Shutdown()
{
	//Steam
	SteamAPI_Shutdown();

	FCoreUObjectDelegates::PreLoadMap.RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
	
	Super::Shutdown();
}

bool UVJGameInstance::DoesSaveExist() const
{
	return UGameplayStatics::DoesSaveGameExist(LoadSlotName, LoadSlotIndex);
}

void UVJGameInstance::ShowLoadingScreen()
{
	// if (!LoadingWidget && LoadingWidgetClass)
	// {
	// 	UWorld* World = GetWorld();
	// 	if (!World) { return; }
	//
	// 	
	//
	// 	LoadingWidget = CreateWidget<UUserWidget>(World, LoadingWidgetClass);
	// 	if (LoadingWidget)
	// 	{
	// 		GetGameViewportClient()->AddViewportWidgetContent(LoadingWidget->TakeWidget(),INT_MAX);
	// 		//LoadingWidget->AddToViewport(INT_MAX); 
	// 	}
	// }
	if (!IsMoviePlayerEnabled()) return;

	UWorld* World = GetWorld(); // 트래블 직전에는 월드가 있으므로 OK
	TSharedPtr<SWidget> SlateWidget;

	if (LoadingWidgetClass && World)
	{
		if (UUserWidget* W = CreateWidget<UUserWidget>(World, LoadingWidgetClass))
		{
			SlateWidget = W->TakeWidget(); // UMG → Slate 변환
		}
	}

	// MoviePlayer로 해야 기본 로딩 스크린 오버라이딩됨 * FStreamingPauseRenderingModule::BeginStreamingPause( FViewport* GameViewport ) 참고
	FLoadingScreenAttributes Attr;
	Attr.bAutoCompleteWhenLoadingCompletes = true;
	Attr.MinimumLoadingScreenDisplayTime = 0.25f;
	Attr.WidgetLoadingScreen = SlateWidget.IsValid()
		? SlateWidget.ToSharedRef()
		: SNew(SViewport)
			.EnableGammaCorrection(false);
			
	GetMoviePlayer()->SetupLoadingScreen(Attr);
	GetMoviePlayer()->PlayMovie();
}

void UVJGameInstance::HideLoadingScreen()
{
	// if (LoadingWidget)
	// {
	// 	GetGameViewportClient()->RemoveViewportWidgetContent(LoadingWidget->TakeWidget());
	// 	//LoadingWidget->RemoveFromParent();
	// 	LoadingWidget = nullptr;
	// }
	if (IsMoviePlayerEnabled())
	{
		if (auto* MP = GetMoviePlayer())
		{
			if (MP->IsMovieCurrentlyPlaying())
			{
				MP->StopMovie(); // bAutoCompleteWhenLoadingCompletes=false인 경우 수동 종료
			}
		}
	}
}

void UVJGameInstance::OnPreLoadMap(const FString& MapName)
{
	ShowLoadingScreen();	
}

void UVJGameInstance::OnPostLoadMap(UWorld* World)
{
	HideLoadingScreen();
}
