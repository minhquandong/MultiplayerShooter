// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseAnimInstance.h"

#include "Character/BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapons/Weapon.h"

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

	// Setting variables
	bIsInAir = BaseCharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = BaseCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	bShouldMove = (bIsAccelerating && Speed != 0) ? true : false;
	bWeaponEquiped = BaseCharacter->IsWeaponEquiped();
	EquippedWeapon = BaseCharacter->GetEquippedWeapon();
	bIsCrouched = BaseCharacter->bIsCrouched;
	bIsAiming = BaseCharacter->IsAiming();
	TurningInPlace = BaseCharacter->GetTurningInPlace();

	AO_Yaw = BaseCharacter->GetAO_Yaw();
	AO_Pitch = BaseCharacter->GetAO_Pitch();

	/*
	// Set YawOffset for strafing
	FRotator AimRotation = BaseCharacter->GetBaseAimRotation();			// AimRotation is Global rotation
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(BaseCharacter->GetVelocity());
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 6.f);
	YawOffset = DeltaRot.Yaw;
	
	// Character Lean = Delta YawRotation between the character itself and its own YawRotation from the previous frame
	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = BaseCharacter->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float Target = Delta.Yaw / DeltaTime;
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.f);
	Lean = FMath::Clamp(Interp, -90.f, 90.f);
	*/

	if (bWeaponEquiped && EquippedWeapon && EquippedWeapon->GetWeaponMesh() && BaseCharacter->GetMesh())
	{
		// Set Left hand position
		LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);
		FVector OutPosition;
		FRotator OutRotation;
		BaseCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
		LeftHandTransform.SetLocation(OutPosition);
		LeftHandTransform.SetRotation(FQuat(OutRotation));

		if (BaseCharacter->IsLocallyControlled())
		{
			bLocallyControlled = true;
			FTransform RightHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("hand_r"), ERelativeTransformSpace::RTS_World);
			// The forward vector X of the bone "hand_r" is pointing up the arm. So we have to rotate it backward
			RightHandRotation = UKismetMathLibrary::FindLookAtRotation(RightHandTransform.GetLocation(), RightHandTransform.GetLocation() + (RightHandTransform.GetLocation() - BaseCharacter->GetHitTarget()));
		}
	}
}