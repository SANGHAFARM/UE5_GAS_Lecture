// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySystemInterface.h"

#include "ABGASUserWidget.generated.h"

class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API UABGASUserWidget : public UUserWidget, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	// 위젯 컴포넌트의 오너가 가지고 있는 ASC를 가져와서 Set
	virtual void SetAbilitySystemComponent(AActor* InOwner);
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "GAS")
	UAbilitySystemComponent* ASC;
};
