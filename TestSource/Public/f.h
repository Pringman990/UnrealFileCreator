
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "f.generated.h"

UCLASS(BlueprintType, Blueprintable)
class LUNA_API Af : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Af();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

};

