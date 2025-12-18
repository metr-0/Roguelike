
#include "Enemies/EnemyCharacter.h"
#include "Enemies/EnemyAIController.h"

AEnemyCharacter::AEnemyCharacter()
{
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	HealthComponent->OnDeath.AddDynamic(this, &AEnemyCharacter::OnDeath);
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyCharacter::OnDeath()
{
    Destroy();
}

void AEnemyCharacter::TryAttack(APawn* Target)
{
    if (!bCanAttack || !Target) return;

    if (Target->Implements<UDamageableInterface>())
    {
        IDamageableInterface::Execute_ReceiveDamage(
            Target,
            AttackDamage
        );
    }

    bCanAttack = false;

    GetWorld()->GetTimerManager().SetTimer(
        AttackTimerHandle,
        this,
        &AEnemyCharacter::ResetAttack,
        AttackCooldown
    );
}

void AEnemyCharacter::ReceiveDamage_Implementation(float Damage)
{
    HealthComponent->ApplyDamage(Damage);
}
