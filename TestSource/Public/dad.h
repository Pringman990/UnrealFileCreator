
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "dad.generated.h"

UCLASS(BlueprintType, Blueprintable)
class LUNA_API Adad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Adad();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

};

