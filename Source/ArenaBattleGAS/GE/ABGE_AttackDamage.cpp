// Fill out your copyright notice in the Description page of Project Settings.


#include "GE/ABGE_AttackDamage.h"

#include "Attribute/ABGASCharacterAttributeSet.h"

UABGE_AttackDamage::UABGE_AttackDamage()
{
	// Gameplay Effect의 적용 지속 방식을 정의하는 정책 설정
	DurationPolicy = EGameplayEffectDurationType::Instant;
	
	// GAS에서 변경할 어트리뷰트 정의
	FGameplayModifierInfo HealthModifier;
	HealthModifier.Attribute = FGameplayAttribute(FindFieldChecked<FProperty>(UABGASCharacterAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UABGASCharacterAttributeSet, Health)));
	// 변경 옵션을 '더하기'로 설정
	HealthModifier.ModifierOp = EGameplayModOp::Additive;
	
	// 실제로 적용할 값 지정
	FScalableFloat DamageAmount(-30.f);
	FGameplayEffectModifierMagnitude ModMagnitude(DamageAmount);
	
	HealthModifier.ModifierMagnitude = ModMagnitude;
	
	Modifiers.Add(HealthModifier);
}
