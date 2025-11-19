// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "ABAT_Trace.generated.h"

class AABTA_Trace;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTraceResultDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API UABAT_Trace : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UABAT_Trace();
	
	// GA에서 태스크 생성 시 호출할 static 함수
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "WaitForTrace", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UABAT_Trace* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<AABTA_Trace> TargetActorClass);
	
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;
	
	void SpawnAndInitializeTargetActor();
	void FinalizeTargetActor();
	
protected:
	// OnComplete 델리게이트를 브로드캐스트 하기 위한 함수
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle);
	
public:
	UPROPERTY(BlueprintAssignable)
	FTraceResultDelegate OnComplete;
	
protected:
	// 생성할 타겟 액터의 클래스 정보
	UPROPERTY()
	TSubclassOf<AABTA_Trace> TargetActorClass;
	
	// 타겟 액터의 클래스로 생성된 액터를 저장할 액터
	UPROPERTY()
	TObjectPtr<AABTA_Trace> SpawnedTargetActor; 
};
