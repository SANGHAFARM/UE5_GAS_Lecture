// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "ABAT_JumpAndWaitForLanding.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJumpAndWaitForLandingDelegate);
/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API UABAT_JumpAndWaitForLanding : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UABAT_JumpAndWaitForLanding();
	
	// 자기자신(AT)을 소유하는 GA를 매개변수로 하는 AT 인스턴스 생성 함수
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "JumpAndWaitForLanding", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UABAT_JumpAndWaitForLanding* CreateTask(UGameplayAbility* OwningAbility);
	
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;
	
	UPROPERTY(BlueprintAssignable)
	FJumpAndWaitForLandingDelegate OnComplete;
	
protected:
	// Jump가 끝나는 상황을 AT가 파악할 수 있도록
	// ACharacter에 있는 FHitResult를 매개변수로 하는 LandedDelegate와 바인딩할 함수
	UFUNCTION()
	void OnLandedCallback(const FHitResult& Hit);
};
