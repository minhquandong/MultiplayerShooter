// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseAnimInstance.h"

#include "Character/BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	BaseCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
}

void UBaseAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (BaseCharacter == nullptr)
	{
		BaseCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
	}
	if (BaseCharacter == nullptr) return;

	FVector Velocity = BaseCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bIsInAir = BaseCharacter->GetCharacterMovement()->IsFalling();

	bIsAccelerating = BaseCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;

	bShouldMove = (bIsAccelerating && Speed != 0) ? true : false;
}