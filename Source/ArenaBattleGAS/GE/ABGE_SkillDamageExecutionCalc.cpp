// Fill out your copyright notice in the Description page of Project Settings.


#include "GE/ABGE_SkillDamageExecutionCalc.h"

#include "AbilitySystemComponent.h"
#include "Attribute/ABGASCharacterAttributeSet.h"
#include "Attribute/ABGASCharacterSkillAttributeSet.h"

void UABGE_SkillDamageExecutionCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
	
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	
	if (SourceASC && TargetASC)
	{
		// 스킬 범위 불러오기
		const float MaxDamageRange = SourceASC->GetNumericAttributeBase(UABGASCharacterSkillAttributeSet::GetSkillRangeAttribute());
		// 스킬 대미지 불러오기
		const float MaxDamage = SourceASC->GetNumericAttributeBase(UABGASCharacterSkillAttributeSet::GetSkillAttackRateAttribute());
		
		AActor* SourceActor = SourceASC->GetAvatarActor();
		AActor* TargetActor = TargetASC->GetAvatarActor();
		
		if (SourceActor && TargetActor)
		{
			const float Distance = FMath::Clamp(SourceActor->GetDistanceTo(TargetActor), 0.0f, MaxDamageRange);
			// 가까울수록 크고 멀어질수록 작도록 값을 설정하기 위해 대미지 비율 반전 
			const float InvDamageRatio = 1.0f - (Distance / MaxDamageRange);
			
			// 실제 대미지 계산
			float Damage = InvDamageRatio * MaxDamage;
			
			// GameplayEffect에서 계산된 Damage 값을 Attribute에 Additive 방식으로 적용
			OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UABGASCharacterAttributeSet::GetDamageAttribute(), EGameplayModOp::Additive, Damage));
		}
	}
}
