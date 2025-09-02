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
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Steam API Restart : False"));
	}

	if ( !SteamAPI_Init() )
	{
		UE_LOG(LogTemp, Error, TEXT("SteamAPI_Init Return False"));
	}
	

	FString Culture;
	
	if (!GConfig->GetString(TEXT("Internationalization"), TEXT("Culture"), Culture, GGameUserSettingsIni))
	{
		GConfig->GetString(TEXT("Internationalization"), TEXT("Culture"), Culture, GGameIni);
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
	// 로딩 스크린 표시위한 바인딩
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this,&UVJGameInstance::OnPreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this,&UVJGameInstance::OnPostLoadMap);
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
