// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"

#include "ABGASItemBox.generated.h"

class UGameplayEffect;
class UBoxComponent;

UCLASS()
class ARENABATTLEGAS_API AABGASItemBox : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	AABGASItemBox();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
protected:
	virtual void PostInitializeComponents() override;
	
	// 상자를 습득한 대상에게 게임플레이 이펙트 발동
	void ApplyEffectToTarget(AActor* Target);
	
	// 상자의 이펙트 재생
	void InvokeGameplayCue(AActor* Target);
	
protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;
	
	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<UBoxComponent> Trigger;
	
	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<UStaticMeshComponent> Mesh;
	
	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<UGameplayEffect> GameplayEffectClass;
	
	// Meta = (Categories = GameplayCue)라는 메타 정보를 입력하면 접두사가 GameplayCue인 태그들만 출력
	UPROPERTY(EditAnywhere, Category = GAS, Meta = (Categories = GameplayCue))
	FGameplayTag GameplayCueTag;
};
