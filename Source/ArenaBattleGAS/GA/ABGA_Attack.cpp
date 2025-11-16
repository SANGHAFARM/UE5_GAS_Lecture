// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/ABGA_Attack.h"
#include "Character/ABCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "ArenaBattleGAS.h"
#include "GameFramework/CharacterMovementComponent.h"

UABGA_Attack::UABGA_Attack()
{
	// 인스턴스 정책 지정
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UABGA_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	ABGAS_LOG(LogABGAS, Warning, TEXT("Begin"));
}

void UABGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	// 아바타 액터 정보 가져오기
	AABCharacterBase* ABCharacter = CastChecked<AABCharacterBase>(ActorInfo->AvatarActor.Get());
	
	// 공격 시 캐릭터 움직임 정지
	UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(ABCharacter->GetCharacterMovement());
	if (CharacterMovement)
	{
		CharacterMovement->SetMovementMode(EMovementMode::MOVE_None);
	}
	
	// 지정한 몽타주를 재생하고 끝날 때까지 기다려주는 Ability Task
	// CreatePlayMontageAndWaitProxy(어빌리티 정보, 태스크 이름, 캐릭터로부터 제공 받을 몽타주 애셋)  
	if (ABCharacter->GetComboActionMontage())
	{
		UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), ABCharacter->GetComboActionMontage());
		
		// AT 생성 후, 종료 및 방해 시 발생되는 델리게이트에 함수 바인딩
		PlayAttackTask->OnCompleted.AddDynamic(this, &UABGA_Attack::OnCompleteCallback);
		PlayAttackTask->OnInterrupted.AddDynamic(this, &UABGA_Attack::OnInterruptedCallback);
		
		// ReadyForActivation 함수를 명시적으로 호출해야 태스크 실행
		PlayAttackTask->ReadyForActivation();
	}
}

void UABGA_Attack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UABGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	AABCharacterBase* ABCharacter = CastChecked<AABCharacterBase>(ActorInfo->AvatarActor.Get());
	
	// 공격 종료 시 캐릭터 움직임 정지 해제
	UCharacterMovementComponent* CharacterMovement = Cast<UCharacterMovementComponent>(ABCharacter->GetCharacterMovement());
	if (CharacterMovement)
	{
		CharacterMovement->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

void UABGA_Attack::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	// 명시적으로 어빌리티 종료 지정 (강제 취소 false)
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UABGA_Attack::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	// 명시적으로 어빌리티 종료 지정 (강제 취소 true)
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}