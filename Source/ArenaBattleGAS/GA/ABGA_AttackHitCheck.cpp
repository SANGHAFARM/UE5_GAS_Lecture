// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/ABGA_AttackHitCheck.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ArenaBattleGAS.h"
#include "GA/AT/ABAT_Trace.h"
#include "GA/TA/ABTA_Trace.h"
#include "Attribute/ABGASCharacterAttributeSet.h"
#include "Tag/ABGameplayTag.h"

UABGA_AttackHitCheck::UABGA_AttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UABGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	// 애님 노티파이로부터 전달된 EventData의 값을 CurrentLevel로 설정
	CurrentLevel = TriggerEventData->EventMagnitude;
	
	// 어빌리티 태스크 생성
	UABAT_Trace* AttackTraceTask = UABAT_Trace::CreateTask(this, AABTA_Trace::StaticClass());
	// 어빌리티 태스크 작업 완료 시 호출할 함수를 델리게이트에 바인딩
	AttackTraceTask->OnComplete.AddDynamic(this, &UABGA_AttackHitCheck::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();
}

void UABGA_AttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	// 어빌리티 태스크로부터 받은 정보로 공격 판정
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		ABGAS_LOG(LogABGAS, Log, TEXT("Target %s Detected"), *(HitResult.GetActor()->GetName()));
		
		// 현재 이 GA를 소유한 액터의 ASC 불러오기
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
		// 공격한 액터의 ASC 불러오기
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());
		if (SourceASC == nullptr || TargetASC == nullptr)
		{
			ABGAS_LOG(LogABGAS, Error, TEXT("ASC not found!"));
			return;
		}
		
		// SourceASC의 AttributeSet 불러오기
		const UABGASCharacterAttributeSet* SourceAttribute = SourceASC->GetSet<UABGASCharacterAttributeSet>();
		
		// 영향을 주는 Effect 명세 정의
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentLevel);
		if (EffectSpecHandle.IsValid())
		{
			// 대미지 적용
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);
			
			// Context는 누가 누구를 어떻게 맞췄는지와 같은 메타 데이터 저장
			FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
			CueContextHandle.AddHitResult(HitResult);
			// GameplayCue 를 실행할 때 사용할 Cue Parameter 구조체를 생성하여 방금 만든 Context(히트 정보 포함)를 Cue 파라미터에 삽입
			FGameplayCueParameters CueParam;
			CueParam.EffectContext = CueContextHandle;
			
			// TargetASC에 태그를 주입하고, 해당 태그를 가진 GameplayCue 실행
			TargetASC->ExecuteGameplayCue(ABTAG_GAMEPLAYCUE_CHARACTER_ATTACKHIT, CueParam);
		}
		
		FGameplayEffectSpecHandle BuffEffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackBuffEffect);
		if (BuffEffectSpecHandle.IsValid())
		{
			// 버프 적용
			ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, BuffEffectSpecHandle);
		}
	}
	
	// 어빌리티 종료
	bool bReplicatedEndAbility = true;
	bool bWasCanceled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCanceled);
}
