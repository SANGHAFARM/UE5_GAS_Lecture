// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"

#include "AnimNotify_GASAttackHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API UAnimNotify_GASAttackHitCheck : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAnimNotify_GASAttackHitCheck();
	
protected:
	// 애님 노티파이의 이름을 에디터에 표시
	virtual FString GetNotifyName_Implementation() const override;
	
	// 애님 노티파이가 재생될 때 실행
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
protected:
	UPROPERTY(EditAnywhere)
	FGameplayTag TriggerGameplayTag;
	
};