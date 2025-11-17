// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ABGA_Attack.generated.h"

class UABComboActionData;
/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API UABGA_Attack : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UABGA_Attack();
	
public:
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	
	// 어빌리티 활성화
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	// 어빌리티 비활성화
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	
	// 어빌리티 종료
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
protected:
	UFUNCTION()
	void OnCompleteCallback();
	
	UFUNCTION()
	void OnInterruptedCallback();
	
	// 몽타주의 섹션 정보 불러오기
	FName GetNextSection();
	
	// 콤보 타이머 발동
	void StartComboTimer();
	// 지정된 유효 시간 내에 콤보 입력이 들어왔는지 체크
	void CheckComboInput();
	
protected:
	// 콤보 정보
	UPROPERTY()
	TObjectPtr<UABComboActionData> CurrentComboData;
	
	uint8 CurrentCombo = 0;
	FTimerHandle ComboTimerHandle;
	bool HasNextComboInput = false;
};
