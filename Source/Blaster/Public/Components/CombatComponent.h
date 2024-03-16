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

	// Give ABaseCharacter full access to functions and variables of this class, including private and protected
	friend class ABaseCharacter;

	void EquipWeapon(AWeapon* WeaponToEquip);

private:
	AWeapon* EquipedWeapon;
	ABaseCharacter* Character;

protected:
	virtual void BeginPlay() override;
		
};
