
#include "Player/RogueCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"

ARogueCharacter::ARogueCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void ARogueCharacter::BeginPlay()
{
	Super::BeginPlay();
	
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
	
	HealthComponent->OnDeath.AddDynamic(this, &ARogueCharacter::OnDeath);
}

void ARogueCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (!DesiredMoveDirection.IsNearlyZero())
    {
        FRotator TargetRotation = DesiredMoveDirection.Rotation();
        FRotator NewRotation =
            FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, TurnSpeed);

        SetActorRotation(NewRotation);
    }
}

void ARogueCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARogueCharacter::Move);
		
		EnhancedInput->BindAction(
			AttackAction,
			ETriggerEvent::Started,
			this,
			&ARogueCharacter::Attack
		);
	}
}

void ARogueCharacter::Move(const FInputActionValue& Value)
{
    FVector2D Input = Value.Get<FVector2D>();
    if (Input.IsNearlyZero()) return;

    AddMovementInput(FVector::ForwardVector, Input.X);
    AddMovementInput(FVector::RightVector, Input.Y);

    DesiredMoveDirection = FVector(Input.X, Input.Y, 0.f).GetSafeNormal();
}

void ARogueCharacter::ReceiveDamage_Implementation(float Damage)
{
    HealthComponent->ApplyDamage(Damage);
}

void ARogueCharacter::OnDeath()
{
    DisableInput(nullptr);
    // todo
}

void ARogueCharacter::Attack()
{
    FVector Start = GetActorLocation();
    FVector End = Start + GetActorForwardVector() * 150.f;

    TArray<FHitResult> Hits;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(75.f);

    bool bHit = GetWorld()->SweepMultiByChannel(
        Hits,
        Start,
        End,
        FQuat::Identity,
        ECC_Pawn,
        Sphere
    );
	
	UE_LOG(LogTemp, Warning, TEXT("ATTACK"));
	DrawDebugSphere(
		GetWorld(),
		End,
		75.f,
		16,
		FColor::Red,
		false,
		1.f
	);

    if (!bHit) return;

    for (const FHitResult& Hit : Hits)
    {
        AActor* HitActor = Hit.GetActor();
        if (!HitActor || HitActor == this) continue;

        if (HitActor->Implements<UDamageableInterface>())
        {
            IDamageableInterface::Execute_ReceiveDamage(
                HitActor,
                AttackDamage
            );
        }
    }
}

