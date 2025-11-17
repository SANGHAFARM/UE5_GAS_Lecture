// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/ABAT_JumpAndWaitForLanding.h"

#include "GameFramework/Character.h"

UABAT_JumpAndWaitForLanding::UABAT_JumpAndWaitForLanding()
{
}

UABAT_JumpAndWaitForLanding* UABAT_JumpAndWaitForLanding::CreateTask(UGameplayAbility* OwningAbility)
{
	// NewAbilityTask API를 활용해 AT 인스턴스 생성
	UABAT_JumpAndWaitForLanding* NewTask = NewAbilityTask<UABAT_JumpAndWaitForLanding>(OwningAbility);
	
	return NewTask;
}

void UABAT_JumpAndWaitForLanding::Activate()
{
	Super::Activate();
	
	// GetAvatarActor는 TaskOwner(GA)의 아바타를 리턴
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	
	// Jump 전에 ACharacter의 LandedDelegate와 OnLandedCallback 함수 바인딩
	Character->LandedDelegate.AddDynamic(this, &UABAT_JumpAndWaitForLanding::OnLandedCallback);
	Character->Jump();
	
	// 점프가 언제 종료되어야 하는지 모르기 때문에 AT의 상태를 WaitingOnAvatar로 설정
	SetWaitingOnAvatar();
}

void UABAT_JumpAndWaitForLanding::OnDestroy(bool bInOwnerFinished)
{
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	// AT 종료 시 LandedDelegate 델리게이트에 바인딩된 함수 제거
	Character->LandedDelegate.RemoveDynamic(this, &UABAT_JumpAndWaitForLanding::OnLandedCallback);
	
	Super::OnDestroy(bInOwnerFinished);
}

void UABAT_JumpAndWaitForLanding::OnLandedCallback(const FHitResult& Hit)
{
	// ShouldBroadcastAbilityTaskDelegates() 함수는 AT가 Delegate를 브로드캐스트해도 되는 상태인지 여부 리턴
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast();
	}
}
