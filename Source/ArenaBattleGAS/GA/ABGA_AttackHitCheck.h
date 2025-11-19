// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ABGA_AttackHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API UABGA_AttackHitCheck : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UABGA_AttackHitCheck();
	
	// 어빌리티를 활성화하고, 어빌리티 태스크를 생성할 함수
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
protected:
	// 어빌리티 태스크가 작업을 마치면 델리게이트를 통해 실행될 함수
	UFUNCTION()
	void OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);
};
