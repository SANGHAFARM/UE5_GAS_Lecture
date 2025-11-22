// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/ABGA_AttackHitCheck.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ArenaBattleGAS.h"
#include "GA/AT/ABAT_Trace.h"
#include "GA/TA/ABTA_Trace.h"
#include "Attribute/ABGASCharacterAttributeSet.h"

UABGA_AttackHitCheck::UABGA_AttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UABGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
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
		ABGAS_LOG(LogABGAS, Warning, TEXT("Target %s Detected"), *(HitResult.GetActor()->GetName()));
		
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
		// TargetASC의 AttributeSet 불러오기
		// 읽기만 가능한 AttributeSet의 값을 변경해야 하기 때문에 const_cast 사용
		UABGASCharacterAttributeSet* TargetAttribute = const_cast<UABGASCharacterAttributeSet*>(TargetASC->GetSet<UABGASCharacterAttributeSet>());
		if (SourceAttribute == nullptr || TargetAttribute == nullptr)
		{
			ABGAS_LOG(LogABGAS, Error, TEXT("Attribute not found!"));
			return;
		}
		
		const float AttackDamage = SourceAttribute->GetAttackRate();
		TargetAttribute->SetHealth(TargetAttribute->GetHealth() - AttackDamage);
	}
	
	// 어빌리티 종료
	bool bReplicatedEndAbility = true;
	bool bWasCanceled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCanceled);
}
