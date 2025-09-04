# Verse Jumper
## 1. 개요
- 장르 : 1인칭, 플랫포머, 어드벤처
- 엔진 : Unreal 5
- 개발인원 : 1인
- 개발기간 : 2025.04. - 진행중
- 소개
    > 언리얼 엔진의 기능을 학습하기 위해 만들기 시작한 플랫포머 게임입니다.
</br>근미래 SF 세계관을 배경으로,우연히 ‘쓰레기통 세상’으로 떨어진 플레이어가 </br>여러 멀티버스를 탐험하며 원래 세상으로 돌아가기 위한 여정을 그리는 이야기입니다.
</br>**플레이어의 상태**에 따라 동일한 공간이지만 각기 다른 세계로 느껴지도록, 
**환경과 액터가 동적으로 변화**하게 만들었습니다
- 소개영상 : [유튜브 링크](https://www.youtube.com/watch?v=hHfp1EjeWY4)


## 2. 주요 기술

### World Partition의 Data Layer Visibility
- 특정 멀티버스에서만 등장시키고 싶은 액터를 **Data Layer**에 배치.
- 플레이어의 상태에 따라 Visibility On/Off를 제어해 액터들을 일괄 토글.
- 에디터에서도 실시간으로 Visibility를 조절해 레벨 디자인 효율성을 높임.

### 게임 비주얼 관리 : StateVisualHandlerComponent
- 플레이어 상태 변화에 따라 액터의 비주얼 상태를 관리.
- Data Layer와 VerseStateVisualMap으로 상태와 비주얼의 매칭 관리.

### UI - MVC 패턴 도입
- 위젯(View)와 게임 로직(Model) 간의 직접 의존성을 줄이기 위해 Controller 클래스를 도입.
- Controller 클래스를 통해 Delegate로 상태 변경을 효율적으로 알림.
- 불필요한 캐스팅과 Tick 사용을 최소화하여 최적화.

### Subsystem & Delegate
- VerseStateSubsystem을 싱글톤처럼 활용하여 상태 관리의 일관성 유지.
- Delegate로 옵저버 패턴을 적용, 상태 변화 시 다양한 클래스가 동적으로 반응.

### Interface & Component 활용
- 상호작용 가능한 액터에 한정하지 않고 특정 환경 강조 기능까지 확장 가능하도록 **HighlightInterface**와 **InteractionComponent**로 분리.
- 액터 간 종속성을 최소화해 유지보수성과 확장성을 확보.

### Data-Driven 설계
- FGameplayTag와 함께 DataAsset, DataTable을 적극적으로 활용.
- 작업 중 실수나 하드코딩을 최소화하고, 협업과 유지보수에 용이한 구조 설계.
- 예) VerseStateTag, ActorType, VerseStateSoundMap, VerseStateVisualMap, SurfaceSoundMap 등

### Save & Load 시스템
- SaveGame 클래스와 SaveGame 플래그를 이용한 직렬화 이용

## 3. 사용 기술 요약
- Unreal Engine 5
- Blueprint & C++
- FGameplayTag 기반 데이터 관리
- DataAsset & DataTable 기반 데이터-드리븐 설계
- MVC 패턴 기반 UI 시스템
- 싱글톤 패턴과 옵저버 패턴
- World Partition 및 Data Layer Visibility

## 4. 사용된 도구
### 협업 도구 & 언어
- Git, Github, Git Fork
- C++
### 개발 환경
- Unreal Engine 5.5
- Rider
- Windows11

## 5. 프로젝트 구조
```
VerseJumper/
 ├── Source/
 │    ├── VerseJumper/
 │    │    ├── Public/
 │    │    |    ├── Actor/        #상태 변화에 반응하는 액터 & 상호작용 액터 클래스
 │    │    |    ├── Character/    #CharacterBase & PlayerCharacter 클래스
 │    │    |    ├── Component/    #Footstep, IngameAudio, Interaction, StateVisualHandler
 │    │    |    ├── DataAsset/    #데이터 애셋 클래스
 │    │    |    ├── Game/         #게임모드, 게임스테이트
 │    │    |    ├── Interface/    #외곽선 인터페이스
 │    │    |    ├── Player/       #PlayerController 클래스
 │    │    |    ├── Subsystem/    #VerseState상태 관리 서브시스템
 │    │    |    ├── Types/        #현재는 액터 타입 관리용 TableRow만 존재
 |    │    │    └── UI/           #HUD, UserWidget, WidgetController 클래스
 │    └── ...
 ├── Content/
 │    ├── Audio/                  #BGM, SFX 애셋
 │    ├── Blueprint/              #C++ 상속 BP 클래스
 │    ├── Data/                   #DataAsset, DataTable
 │    ├── Level/                  #TestLevel, GameLevel
 │    └── ...
 └── VerseJumper.uproject
```
