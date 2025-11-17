// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/ABGA_Attack.h"
#include "Character/ABCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "ArenaBattleGAS.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/ABComboActionData.h"

UABGA_Attack::UABGA_Attack()
{
	// 인스턴스 정책 지정
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UABGA_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	// 입력 시 콤보 타이머가 유효하지 않은 경우 콤보 중단
	if (ComboTimerHandle.IsValid() == false)
	{
		HasNextComboInput = false;
	}
	else
	{
		HasNextComboInput = true;
	}
}

void UABGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	// 아바타 액터 정보 가져오기
	AABCharacterBase* ABCharacter = CastChecked<AABCharacterBase>(ActorInfo->AvatarActor.Get());
	
	// 현재 콤보 정보 불러오기
	CurrentComboData = ABCharacter->GetComboActionData();
	
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
		UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), ABCharacter->GetComboActionMontage(), 1.0f, GetNextSection());
		
		// AT 생성 후, 종료 및 방해 시 발생되는 델리게이트에 함수 바인딩
		PlayAttackTask->OnCompleted.AddDynamic(this, &UABGA_Attack::OnCompleteCallback);
		PlayAttackTask->OnInterrupted.AddDynamic(this, &UABGA_Attack::OnInterruptedCallback);
		
		// ReadyForActivation 함수를 명시적으로 호출해야 태스크 실행
		PlayAttackTask->ReadyForActivation();
	}
	
	// 콤보 시작
	StartComboTimer();
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
	
	// 콤보 초기화
	CurrentComboData = nullptr;
	CurrentCombo = 0;
	HasNextComboInput = false;
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

FName UABGA_Attack::GetNextSection()
{
	// 다음 몽타주 섹션의 정보(이름)를 리턴
	CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, CurrentComboData->MaxComboCount);
	FName NextSection = *FString::Printf(TEXT("%s%d"), *CurrentComboData->MontageSectionNamePrefix, CurrentCombo);
	
	return NextSection;
}

void UABGA_Attack::StartComboTimer()
{
	// ComboIndex가 EffectiveFrameCount 배열 내에 유효한 인덱스인지 체크
	int32 ComboIndex = CurrentCombo - 1;
	ensure(CurrentComboData->EffectiveFrameCount.IsValidIndex(ComboIndex));
	
	// 해당 콤보 공격의 유효 시간이 몇 초인지 계산
	const float ComboEffectiveTime = CurrentComboData->EffectiveFrameCount[ComboIndex] / CurrentComboData->FrameRate;
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UABGA_Attack::CheckComboInput, ComboEffectiveTime, false);
	}
}

void UABGA_Attack::CheckComboInput()
{
	ComboTimerHandle.Invalidate();
	if (HasNextComboInput)
	{
		// 게임플레이 어빌리티 클래스 안에서 해당 캐릭터를 아바타로 가지고 있는 경우,
		// 별도로 애니메이션 인스턴스를 가져오지 않아도 사용 가능하도록 MontageJumpToSection 함수 내에 유용한 기능들이 이미 구현되어 있음
		MontageJumpToSection(GetNextSection());
		StartComboTimer();
		HasNextComboInput = false;
	}
}
