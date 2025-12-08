// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ABGA_AttackHitCheck.generated.h"

class AABTA_Trace;
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
	
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<UGameplayEffect> AttackDamageEffect;
	
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<UGameplayEffect> AttackBuffEffect;
	
	// 애님 노티파이로부터 콤보 단계를 전달받아 저장할 변수
	float CurrentLevel;
	
	// 목표로 삼는 액터를 어빌리티마다 구분하기 위해 사전 정의한 클래스
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<AABTA_Trace> TargetActorClass;
};
