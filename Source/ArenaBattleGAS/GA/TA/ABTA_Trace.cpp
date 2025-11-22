// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/ABTA_Trace.h"

#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Physics/ABCollision.h"
#include "AbilitySystemComponent.h"
#include "Attribute/ABGASCharacterAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ArenaBattleGAS.h"

AABTA_Trace::AABTA_Trace()
{
}

void AABTA_Trace::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
	
	// AT를 발동한 소스 액터 정보를 불러와 저장
	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void AABTA_Trace::ConfirmTargetingAndContinue()
{
	if (SourceActor)
	{
		// 타겟 데이터 생성 과정이 완료되어 AT에게 전달
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

FGameplayAbilityTargetDataHandle AABTA_Trace::MakeTargetData() const
{
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);
	
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (ASC == nullptr)
	{
		ABGAS_LOG(LogABGAS, Error, TEXT("ASC not found!"));
		return FGameplayAbilityTargetDataHandle();
	}
	
	const UABGASCharacterAttributeSet* AttributeSet = ASC->GetSet<UABGASCharacterAttributeSet>();
	if (AttributeSet == nullptr)
	{
		ABGAS_LOG(LogABGAS, Error, TEXT("ABCharacterAttributeSet not found!"));
		return FGameplayAbilityTargetDataHandle();
	}
	
	// ASC의 AttributeSet의 AttackRange와 AttackRadius로 설정
	FHitResult OutHitResult;
	const float AttackRange = AttributeSet->GetAttackRange();
	const float AttackRadius = AttributeSet->GetAttackRadius();
	
	FCollisionQueryParams Params(SCENE_QUERY_STAT(UABTA_Trace), false, Character);
	const FVector Forward = Character->GetActorForwardVector();
	const FVector Start = Character->GetActorLocation() + Forward * Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + Forward * AttackRange;
	
	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_ABACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
	
	FGameplayAbilityTargetDataHandle DataHandle;
	if (HitDetected)
	{
		// 언리얼 엔진 GAS에서 하나의 검출이 가능한 HitResult를 만들어주는 상위 래퍼 클래스
		// HitResult를 인자로 넣으면 시작 지점과 끝 지점을 설정하지 않아도 지정해줌
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(OutHitResult);
		DataHandle.Add(TargetData);
	}
	
#if ENABLE_DRAW_DEBUG
	if (bShowDebug)
	{
		FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
		float CapsuleHalfHeight = AttackRange * 0.5f;
		FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
		DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(Forward).ToQuat(), DrawColor, false, 5.0f);
	}
#endif
	
	return DataHandle;
}
