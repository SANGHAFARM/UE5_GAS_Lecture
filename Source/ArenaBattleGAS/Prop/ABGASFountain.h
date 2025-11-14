// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Prop/ABFountain.h"
#include "AbilitySystemInterface.h"

#include "ABGASFountain.generated.h"

class UGameplayAbility;
class URotatingMovementComponent;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API AABGASFountain : public AABFountain, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AABGASFountain();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	
	// ActionTimer에 등록할 함수
	virtual void TimerAction();
	
protected:
	// ABGASFountain을 회전시킬 컴포넌트 
	UPROPERTY(VisibleAnywhere, Category = Movement)
	TObjectPtr<URotatingMovementComponent> RotatingMovement;
	
	// ActionTimer 주기
	UPROPERTY(EditAnywhere, Category = Timer)
	float ActionPeriod;
	
	// 어빌리티 시스템 컴포넌트
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<UAbilitySystemComponent> ASC;
	
	// 시작할 때 발동시킬 어빌리티들에 대한 정보를 묶은 TArray
	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<UGameplayAbility>> StartAbilities;
	
	FTimerHandle ActionTimer;
};
