// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterNonPlayer.h"
#include "AbilitySystemInterface.h"

#include "ABGASCharacterNonPlayer.generated.h"

class UABGASWidgetComponent;
class UGameplayEffect;
class UABGASCharacterAttributeSet;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API AABGASCharacterNonPlayer : public AABCharacterNonPlayer, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AABGASCharacterNonPlayer();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	virtual void PossessedBy(AController* NewController) override;
	
	UFUNCTION()
	virtual void OnOutOfHealth();
	
protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<UAbilitySystemComponent> ASC;
	
	UPROPERTY()
	TObjectPtr<UABGASCharacterAttributeSet> AttributeSet;
	
	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<UGameplayEffect> InitStatEffect;
	
	UPROPERTY(EditAnywhere, Category = GAS)
	float Level;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UABGASWidgetComponent> HpBar;
};
