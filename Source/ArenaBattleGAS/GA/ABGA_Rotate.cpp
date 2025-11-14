// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/ABGA_Rotate.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Tag/ABGameplayTag.h"

UABGA_Rotate::UABGA_Rotate()
{
	// 어빌리티를 대표하는 태그 등록
	AbilityTags.AddTag(ABTAG_ACTOR_ROTATE);
	// 어빌리티 활성화 시 상태를 나타낼 태그 등록
	ActivationOwnedTags.AddTag(ABTAG_ACTOR_ISROTATING);
}

void UABGA_Rotate::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	// 아바타 액터 정보 가져오기
	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	if (AvatarActor)
	{
		// 아바타 액터의 RotatingMovementComponent를 불러와서 활성화
		URotatingMovementComponent* RotatingMovement = Cast<URotatingMovementComponent>(AvatarActor->GetComponentByClass(URotatingMovementComponent::StaticClass()));
		if (RotatingMovement)
		{
			RotatingMovement->Activate(true);
		}
	}
}

void UABGA_Rotate::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	
	// 아바타 액터 정보 가져오기
	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	if (AvatarActor)
	{
		// 아바타 액터의 RotatingMovementComponent를 불러와서 비활성화
		URotatingMovementComponent* RotatingMovement = Cast<URotatingMovementComponent>(AvatarActor->GetComponentByClass(URotatingMovementComponent::StaticClass()));
		if (RotatingMovement)
		{
			RotatingMovement->Deactivate();
		}
	}
}
