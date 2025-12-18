
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Combat/HealthComponent.h"
#include "Combat/DamageableInterface.h"
#include "RogueCharacter.generated.h"

UCLASS()
class ROGUELIKE_API ARogueCharacter : public ACharacter, public IDamageableInterface
{
	GENERATED_BODY()

public:
	ARogueCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnSpeed = 10.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, Category="Combat")
	float AttackDamage = 25.f;

	FVector DesiredMoveDirection;
	
	UFUNCTION()
	void OnDeath();
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* AttackAction;

	virtual void ReceiveDamage_Implementation(float Damage) override;

public:
	void Attack();
};
