// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/VerseStateMeshSet.h"

UStaticMesh* UVerseStateMeshSet::GetMeshFor(FName Type, FGameplayTag State) const
{
	
	if (Entries.IsEmpty()) return nullptr;

	for (const FVerseStateMeshEntry& Entry : Entries)
	{
		if (Entry.Type == Type && Entry.VerseState == State)
		{
			return Entry.Mesh;
		}
	}
	
	return  nullptr;
}
