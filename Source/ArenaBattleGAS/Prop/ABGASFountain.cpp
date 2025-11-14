// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGASFountain.h"

#include "ArenaBattleGAS.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
//#include "GA/ABGA_Rotate.h" -> 의존성 제거
#include "Tag/ABGameplayTag.h"
//#include "Abilities/GameplayAbility.h"

AABGASFountain::AABGASFountain()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotateMovement"));
	// ActionTimer 실행 주기를 3초로 설정
	ActionPeriod = 3.0f;
}

class UAbilitySystemComponent* AABGASFountain::GetAbilitySystemComponent() const
{
	return ASC;
}

void AABGASFountain::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	// 컴포넌트 초기화 시 자동 활성화를 해제하고, 컴포넌트 비활성화
	RotatingMovement->bAutoActivate = false;
	RotatingMovement->Deactivate();
	
	// 초기화 시 ASC도 InitAbilityActorInfo 함수를 통해 함께 초기화 해주어야 함 
	// InOwnerActor : 실제 ASC를 구동하고 데이터를 관리하는, 실제 작업이 일어나는 액터
	// InAvatarActor : 실제로 데이터를 처리하지 않고, 비주얼만 수행해주는 액터
	ASC->InitAbilityActorInfo(this, this);
	
	// StartAbilities를 순회하면서 SpecHandle 생성
	for (const auto& StartAbility : StartAbilities)
	{
		FGameplayAbilitySpec StartSpec(StartAbility);
		// ASC에 어빌리티 추가
		ASC->GiveAbility(StartSpec);
	}
}

void AABGASFountain::BeginPlay()
{
	Super::BeginPlay();
	
	// ActionTimer에 ActionPeriod마다 실행할 TimerAction 함수를 등록하고 반복하며 함수 첫 동작은 0.0초 후 실행
	GetWorld()->GetTimerManager().SetTimer(ActionTimer, this, &AABGASFountain::TimerAction, ActionPeriod, true, 0.0f);
}

void AABGASFountain::TimerAction()
{
	ABGAS_LOG(LogABGAS, Warning, TEXT("Begin"));
	
	// 발동시킬 태그에 대한 컨테이너
	FGameplayTagContainer TargetTag(ABTAG_ACTOR_ROTATE);
	
	// 현재 매개 태그가 존재하는지(발동중인지) 체크
	if (!ASC->HasMatchingGameplayTag(ABTAG_ACTOR_ISROTATING))
	{
		ASC->TryActivateAbilitiesByTag(TargetTag);
	}
	else
	{
		ASC->CancelAbilities(&TargetTag);
	}
	
	// FGameplayAbilitySpec* RotateGASpec = ASC->FindAbilitySpecFromClass(UABGA_Rotate::StaticClass());
	// if (!RotateGASpec)
	// {
	// 	ABGAS_LOG(LogABGAS, Error, TEXT("No Rotate Spec Found!"));
	// 	return;
	// }
	//
	// if (!RotateGASpec->IsActive())
	// {
	// 	ASC->TryActivateAbility(RotateGASpec->Handle);
	// }
	// else
	// {
	// 	ASC->CancelAbilityHandle(RotateGASpec->Handle);
	// }
	
	// TimerAction 함수가 호출 시마다 RotatingMovement를 활성화/비활성화 전환
	// if (!RotatingMovement->IsActive())
	// {
	// 	RotatingMovement->Activate(true);
	// }
	// else
	// {
	// 	RotatingMovement->Deactivate();
	// }
}
