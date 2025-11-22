// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

#include "ABGASCharacterAttributeSet.generated.h"

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
class ARENABATTLEGAS_API UABGASCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UABGASCharacterAttributeSet();
	
	// 어트리뷰트 변경 전에 호출
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	// 어트리뷰트 변경 후에 호출
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	
	// 어트리뷰트 세트 접근자 매크로 설정
	ATTRIBUTE_ACCESSORS(UABGASCharacterAttributeSet, AttackRange);
	ATTRIBUTE_ACCESSORS(UABGASCharacterAttributeSet, MaxAttackRange);
	ATTRIBUTE_ACCESSORS(UABGASCharacterAttributeSet, AttackRadius);
	ATTRIBUTE_ACCESSORS(UABGASCharacterAttributeSet, MaxAttackRadius);
	ATTRIBUTE_ACCESSORS(UABGASCharacterAttributeSet, AttackRate);
	ATTRIBUTE_ACCESSORS(UABGASCharacterAttributeSet, MaxAttackRate);
	ATTRIBUTE_ACCESSORS(UABGASCharacterAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UABGASCharacterAttributeSet, MaxHealth);
	
	// 캐릭터 어트리뷰트
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRange;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRange;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRadius;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRadius;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRate;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRate;
	
	UPROPERTY(BlueprintReadOnly, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;
	
	UPROPERTY(BlueprintReadOnly, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;
	
};
