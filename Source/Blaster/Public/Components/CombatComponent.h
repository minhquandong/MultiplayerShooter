// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class AWeapon;
class ABaseCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Give ABaseCharacter full access to functions and variables of this class, including private and protected
	friend class ABaseCharacter;

	void EquipWeapon(AWeapon* WeaponToEquip);

private:
	UPROPERTY(ReplicatedUsing = OnRep_EquipWeapon)
	AWeapon* EquippedWeapon;

	ABaseCharacter* Character;

	UPROPERTY(Replicated)
	bool bAiming;

	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;
	
	UPROPERTY(EditAnywhere)
	float AimWalkSpeed;

	bool bFireButtonPressed;

protected:
	virtual void BeginPlay() override;
	void SetAiming(bool bIsAiming);

	// Create Remote Procedural Call (RPC) function - which is called on 1 machine and then executed on another machine
	// Reliable - garantie to be execute. Unreliable - could potential be dropped
	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming);

	UFUNCTION()
	void OnRep_EquipWeapon();

	void FireButtonPressed(bool bPressed);

	UFUNCTION(Server, Reliable)
	void ServerFire();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastFire();
};
