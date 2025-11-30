// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ABGASWeaponBox.h"

#include "AbilitySystemBlueprintLibrary.h"

void AABGASWeaponBox::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	// OtherActor의 ASC를 찾고, ASC 내부에서 WeaponEventTag에 해당하는 delegate를 조회 후, Delegate가 있다면 Broadcast
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OtherActor, WeaponEventTag, FGameplayEventData());
}
