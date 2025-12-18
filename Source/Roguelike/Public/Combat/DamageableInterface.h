
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageableInterface.generated.h"

UINTERFACE(BlueprintType)
class UDamageableInterface : public UInterface
{
    GENERATED_BODY()
};

class ROGUELIKE_API IDamageableInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
    void ReceiveDamage(float Damage);
};
