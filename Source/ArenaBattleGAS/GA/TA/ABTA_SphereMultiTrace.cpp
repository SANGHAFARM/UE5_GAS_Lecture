// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/ABTA_SphereMultiTrace.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ArenaBattleGAS.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/Character.h"
#include "Physics/ABCollision.h"
#include "DrawDebugHelpers.h"
#include "Attribute/ABGASCharacterSkillAttributeSet.h"

FGameplayAbilityTargetDataHandle AABTA_SphereMultiTrace::MakeTargetData() const
{
	ABGAS_LOG(LogABGAS, Warning, TEXT("Begin"));
	
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);
	
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (ASC == nullptr)
	{
		ABGAS_LOG(LogABGAS, Error, TEXT("ASC not found!"));
		return FGameplayAbilityTargetDataHandle();
	}
	
	const UABGASCharacterSkillAttributeSet* SkillAttributeSet = ASC->GetSet<UABGASCharacterSkillAttributeSet>();
	if (SkillAttributeSet == nullptr)
	{
		ABGAS_LOG(LogABGAS, Error, TEXT("SkillAttribute not found!"));
		return FGameplayAbilityTargetDataHandle();
	}
	
	// 다수의 오버랩에 대한 정보를 담을 구조체 배열 선언
	TArray<FOverlapResult> Overlaps;
	const float SkillRadius = SkillAttributeSet->GetSkillRange();
	
	FVector Origin = Character->GetActorLocation();
	
	// 단순 콜리전(false 인자)을 사용하고, Character는 무시하는 충돌 쿼리 파라미터 선언
	FCollisionQueryParams Params(SCENE_QUERY_STAT(AABTA_SphereMultiTrace), false, Character);
	
	// Origin을 중심으로 반경 SkillRadius의 구체를 생성하고, 충돌 채널과 Overlap 되는 모든 오브젝트를 검사
	// 오버랩된 결과는 Overlaps 배열에 FOverlapResult 형태로 저장
	GetWorld()->OverlapMultiByChannel(Overlaps, Origin, FQuat::Identity, CCHANNEL_ABACTION, FCollisionShape::MakeSphere(SkillRadius), Params);
	
	// 오버랩된 액터를 저장할 배열
	// TWeakObjectPtr을 사용하여 액터가 Destroy 되었을 때 안전하게 무효
	TArray<TWeakObjectPtr<AActor>> HitActors;
	
	// OverlapMultiByChannel 로 얻은 Overlaps 배열을 순회하면서, 오브젝트에서 Actor를 추출한다.
	for (const FOverlapResult& Overlap : Overlaps)
	{
		AActor* HitActor = Overlap.OverlapObjectHandle.FetchActor<AActor>();
		// 유효한 액터이며, 아직 목록에 없다면(중복 방지) 추가
		if (HitActor && !HitActors.Contains(HitActor))
		{
			HitActors.Add(HitActor);
		}
	}
	
	// ActorArray 기반의 TargetData 구조체를 생성하고 수집된 HitActors 배열을 내부에 설정
	FGameplayAbilityTargetData_ActorArray* ActorsData = new FGameplayAbilityTargetData_ActorArray();
	ActorsData->SetActors(HitActors);
	
#if ENABLE_DRAW_DEBUG
	if (bShowDebug)
	{
		FColor DrawColor = HitActors.Num() > 0 ? FColor::Green : FColor::Red;
		DrawDebugSphere(GetWorld(), Origin, SkillRadius, 16, DrawColor, false, 5.0f);
	}
#endif
	
	return FGameplayAbilityTargetDataHandle(ActorsData);
}
