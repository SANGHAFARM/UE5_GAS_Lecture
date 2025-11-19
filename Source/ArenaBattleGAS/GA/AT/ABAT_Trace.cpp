// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/ABAT_Trace.h"

#include "AbilitySystemComponent.h"
#include "GA/TA/ABTA_Trace.h"

UABAT_Trace::UABAT_Trace()
{
}

UABAT_Trace* UABAT_Trace::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<AABTA_Trace> TargetActorClass)
{
	UABAT_Trace* NewTask = NewAbilityTask<UABAT_Trace>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;
	
	return NewTask;
}

void UABAT_Trace::Activate()
{
	Super::Activate();
	
	// 타겟 액터를 Deferred로 생성 후 타겟 액터 작업 완료 시 실행할 함수를 타겟 액터의 델리게이트에 바인딩
	SpawnAndInitializeTargetActor();
	// FinishSpawning을 하고, ASC의 SpawnedTargetActors에 타겟 액터 추가
	FinalizeTargetActor();
	
	// 대기 상태 설정
	SetWaitingOnAvatar();
}

void UABAT_Trace::OnDestroy(bool bInOwnerFinished)
{
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}
	
	Super::OnDestroy(bInOwnerFinished);
}

void UABAT_Trace::SpawnAndInitializeTargetActor()
{
	SpawnedTargetActor = Cast<AABTA_Trace>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->SetShowDebug(true);
		// 타겟 액터 델리게이트에 타겟 액터 작업 완료 시 실행할 함수 바인딩
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UABAT_Trace::OnTargetDataReadyCallback);
	}
}

void UABAT_Trace::FinalizeTargetActor()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC)
	{
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		SpawnedTargetActor->FinishSpawning(SpawnTransform);
		
		ASC->SpawnedTargetActors.Add(SpawnedTargetActor);
		// 타겟 액터에게 어빌리티 정보를 전달
		SpawnedTargetActor->StartTargeting(Ability);
		// 타겟 액터에게 타겟 데이터를 생성시키는 함수
		SpawnedTargetActor->ConfirmTargeting();
	}
}

void UABAT_Trace::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		// GA한테 타겟 정보를 전달
		OnComplete.Broadcast(DataHandle);
	}
	
	EndTask();
}
