// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

#include "ABGASCharacterSkillAttributeSet.generated.h"

// 어트리뷰트 세트 접근자 매크로 정의
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API UABGASCharacterSkillAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UABGASCharacterSkillAttributeSet();
	
	// 어트리뷰트 변경 전에 호출
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
	// 어트리뷰트 세트 접근자 매크로 설정
	ATTRIBUTE_ACCESSORS(UABGASCharacterSkillAttributeSet, SkillRange);
	ATTRIBUTE_ACCESSORS(UABGASCharacterSkillAttributeSet, MaxSkillRange);
	ATTRIBUTE_ACCESSORS(UABGASCharacterSkillAttributeSet, SkillAttackRate);
	ATTRIBUTE_ACCESSORS(UABGASCharacterSkillAttributeSet, MaxSkillAttackRate);
	ATTRIBUTE_ACCESSORS(UABGASCharacterSkillAttributeSet, SkillEnergy);
	ATTRIBUTE_ACCESSORS(UABGASCharacterSkillAttributeSet, MaxSkillEnergy);
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SkillRange;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxSkillRange;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SkillAttackRate;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxSkillAttackRate;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SkillEnergy;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxSkillEnergy;
};
