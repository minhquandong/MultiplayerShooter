// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Casing.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ACasing::ACasing()
{
	PrimaryActorTick.bCanEverTick = false;

	CasingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CasingMesh"));
	SetRootComponent(CasingMesh);
	CasingMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	CasingMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	CasingMesh->SetSimulatePhysics(true);
	CasingMesh->SetEnableGravity(true);
	CasingMesh->SetNotifyRigidBodyCollision(true);
	ShellEjectionImpulse = 14.f;

	SetLifeSpan(3.f);
}

void ACasing::BeginPlay()
{
	Super::BeginPlay();
	
	FVector ForwardVector = GetActorForwardVector();
	ForwardVector.Z = FMath::RandRange(-0.3f, 0.3f);			// Shell will be ejected in random angle
	CasingMesh->AddImpulse(ForwardVector * ShellEjectionImpulse);
	CasingMesh->OnComponentHit.AddDynamic(this, &ACasing::OnHit);
}

void ACasing::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ShellSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShellSound, GetActorLocation());
	}
}

void ACasing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

