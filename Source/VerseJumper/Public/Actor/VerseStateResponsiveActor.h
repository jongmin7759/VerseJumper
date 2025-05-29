// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DataAsset/VerseStateMeshSet.h"
#include "GameFramework/Actor.h"
#include "Interface/VerseStateInterface.h"
#include "Types/VerseStateTypes.h"
#include "VerseStateResponsiveActor.generated.h"

class UVerseStateMeshSet;

UCLASS()
class VERSEJUMPER_API AVerseStateResponsiveActor : public AActor , public IVerseStateInterface
{
	GENERATED_BODY()
	
public:	
	AVerseStateResponsiveActor();

	UFUNCTION(BlueprintCallable)
	void SetCurrentStateEntry(FName Type, FGameplayTag State);
	UFUNCTION(BlueprintCallable)
	UStaticMesh* GetCurrentStaticMesh() const;
	UFUNCTION(BlueprintCallable)
	float GetCurrentMeshScale() const;
	UFUNCTION(BlueprintCallable)
	FGameplayTag GetCurrentState() const {return CurrentState;}

protected:
	virtual void BeginPlay() override;
	
	virtual void Internal_OnVerseStateChanged(const FGameplayTag& NewState) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnVerseStateChanged(const FGameplayTag& NewState);

	UPROPERTY(EditAnywhere)
	TObjectPtr<UVerseStateMeshSet> VerseStateMeshSet = nullptr;

	UPROPERTY(EditAnywhere,meta=(GetOptions="GetVerseStateTypes"))
	FName ActorType = NAME_None;

private:
	FGameplayTag CurrentState = FGameplayTag();

	const FVerseStateMeshEntry* CurrentStateEntry = nullptr;

	void InitializeCurrentState(const FGameplayTag& InitialState);

// 타입 드롭박스 만들기
public:
	UFUNCTION()
	static TArray<FName> GetVerseStateTypes()
	{
		static TArray<FName> CachedTypes;
		if (CachedTypes.Num() == 0)
		{
			// TODO : DataTable 만들어서 추가하기
			const UDataTable* TypeTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/DataTable/ActorType/DT_ActorType.DT_ActorType"));
			if (TypeTable)
			{
				TArray<FActorTypeRow*> Rows;
				TypeTable->GetAllRows(TEXT("GetVerseStateTypes"), Rows);

				for (const auto* Row : Rows)
				{
					CachedTypes.Add(Row->Type);
				}
			}
		}
		return CachedTypes;
	}
};