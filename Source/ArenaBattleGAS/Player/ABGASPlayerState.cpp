// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ABGASPlayerState.h"

#include "AbilitySystemComponent.h"

AABGASPlayerState::AABGASPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	// ASC는 서버에서 클라이언트로 전송이 돼야 하기 때문에 컴포넌트가 리플리케이션이 되도록 설정
	//ASC->SetIsReplicated(true);
	
}

UAbilitySystemComponent* AABGASPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
