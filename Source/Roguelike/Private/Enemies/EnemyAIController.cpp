#include "Enemies/EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Enemies/EnemyCharacter.h"

void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();

    PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void AEnemyAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!PlayerPawn) return;

    APawn* ControlledPawn = GetPawn();
    if (!ControlledPawn) return;

    float Distance = FVector::Dist(
        ControlledPawn->GetActorLocation(),
        PlayerPawn->GetActorLocation()
    );

    AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(ControlledPawn);
    if (!Enemy) return;

    if (Distance > Enemy->AttackRange)
    {
        MoveToActor(PlayerPawn, 100.f);
    }
    else
    {
        Enemy->TryAttack(PlayerPawn);
    }
}
