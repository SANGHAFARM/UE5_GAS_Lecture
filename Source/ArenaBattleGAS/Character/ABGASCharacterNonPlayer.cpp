// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABGASCharacterNonPlayer.h"

#include "AbilitySystemComponent.h" 
#include "Attribute/ABGASCharacterAttributeSet.h"

AABGASCharacterNonPlayer::AABGASCharacterNonPlayer()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	
	AttributeSet = CreateDefaultSubobject<UABGASCharacterAttributeSet>(TEXT("AttributeSet"));
	
	Level = 1.0f;
}

UAbilitySystemComponent* AABGASCharacterNonPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void AABGASCharacterNonPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	ASC->InitAbilityActorInfo(this, this);
	
	// GEContext에 대한 GEContextHandle 생성 후 SourceObject(this) 추가
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	
	// GEContextHandle로 GESpec 생성 후 GESpecHandle에 저장
	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InitStatEffect, Level, EffectContextHandle);
	if (EffectSpecHandle.IsValid())
	{
		// 생성된 GameplayEffectSpec을 현재 ASC 소유자에게 적용하는 Blueprint 노출 함수
		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}
}
