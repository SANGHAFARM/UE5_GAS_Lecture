// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "ABTA_Trace.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API AABTA_Trace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	AABTA_Trace();
	
	// AT로부터 어빌리티 정보 수신하여 저장
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	// 타겟 데이터 생성 후 델리게이트를 통해 데이터 전달
	virtual void ConfirmTargetingAndContinue() override;
	
	void SetShowDebug(bool InShowDebug) { bShowDebug = InShowDebug; }
protected:
	// 공격 판정 데이터를 생성
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const;
	
	bool bShowDebug = false;
};