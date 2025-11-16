// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"

#include "ABGASPlayerState.generated.h"

class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API AABGASPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AABGASPlayerState();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
protected:
	// 어빌리티 시스템 컴포넌트
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<UAbilitySystemComponent> ASC;
};
