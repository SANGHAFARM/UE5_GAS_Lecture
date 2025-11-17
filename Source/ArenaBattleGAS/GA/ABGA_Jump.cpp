// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/ABGA_Jump.h"

#include "GameFramework/Character.h"
#include "GA/AT/ABAT_JumpAndWaitForLanding.h"

UABGA_Jump::UABGA_Jump()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UABGA_Jump::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	
	// Jump 비활성화
	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	Character->StopJumping();
}

bool UABGA_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	
	if (bResult == false)
	{
		return false;
	}
	
	// Jump 가능 상태
	const ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	return (Character && Character->CanJump());
}

void UABGA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	// // UABAT_JumpAndWaitForLanding의 static 함수인 CreateTask를 호출하여 AT 객체 생성
	// UABAT_JumpAndWaitForLanding* JumpAndWaitForLandingTask = UABAT_JumpAndWaitForLanding::CreateTask(this);
	//
	// // AT 객체에 대한 초기 변수 및 델리게이트 구독과 같은 사전 필요 작업들 진행
	// JumpAndWaitForLandingTask->OnComplete.AddDynamic(this, &UABGA_Jump::OnLandedCallback);
	//
	// // AT의 Activate 함수 호출을 위해 ReadyForActivation 함수 호출
	// JumpAndWaitForLandingTask->ReadyForActivation();
}

void UABGA_Jump::OnLandedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	// 명시적으로 어빌리티 종료 지정 (강제 취소 bWasCancelled == false)
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
