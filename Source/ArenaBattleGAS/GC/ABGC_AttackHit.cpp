// Fill out your copyright notice in the Description page of Project Settings.


#include "GC/ABGC_AttackHit.h"

#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

UABGC_AttackHit::UABGC_AttackHit()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionRef(TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"));
	if (ExplosionRef.Object)
	{
		ParticleSystem = ExplosionRef.Object;
	}
}

bool UABGC_AttackHit::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	// 전달받은 EffectContext 안에서 HitResult(피격 정보)를 꺼내 저장
	const FHitResult* HitResult = Parameters.EffectContext.GetHitResult();
	if (HitResult)
	{
		UGameplayStatics::SpawnEmitterAtLocation(MyTarget, ParticleSystem, HitResult->ImpactPoint, FRotator::ZeroRotator, true);
	}
	
	return Super::OnExecute_Implementation(MyTarget, Parameters);
}
