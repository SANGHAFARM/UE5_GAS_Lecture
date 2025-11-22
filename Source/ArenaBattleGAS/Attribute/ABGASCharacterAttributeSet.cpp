// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/ABGASCharacterAttributeSet.h"
#include "ArenaBattleGAS.h"

UABGASCharacterAttributeSet::UABGASCharacterAttributeSet() :
	AttackRange(100.0f),
	MaxAttackRange(300.0f),
	AttackRadius(50.0f),
	MaxAttackRadius(150.0f),
	AttackRate(30.0f),
	MaxAttackRate(100.0f),
	MaxHealth(100.0f)
{
	// 매크로를 활용해 Health의 Current와 Base를 MaxHealth로 설정
	InitHealth(GetMaxHealth());
}

void UABGASCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	// 어트리뷰트가 Health일 경우 0 미만으로 떨어지지 않게 설정
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
}

void UABGASCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	if (Attribute == GetHealthAttribute())
	{
		ABGAS_LOG(LogABGAS, Warning, TEXT("Health : %f -> %f"), OldValue, NewValue);
	}
}
