
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Combat/HealthComponent.h"
#include "Combat/DamageableInterface.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class ROGUELIKE_API AEnemyCharacter : public ACharacter, public IDamageableInterface
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, Category="Combat")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, Category="Combat")
	float AttackDamage = 10.f;

public:
	UPROPERTY(EditAnywhere, Category="Combat")
	float AttackRange = 120.f;

protected:
	UPROPERTY(EditAnywhere, Category="Combat")
	float AttackCooldown = 1.2f;

	UFUNCTION()
	void OnDeath();

	virtual void ReceiveDamage_Implementation(float Damage) override;

public:
	void TryAttack(APawn* Target);
	
	FTimerHandle AttackTimerHandle;

	void ResetAttack()
	{
		bCanAttack = true;
	}

private:
    bool bCanAttack = true;
};
