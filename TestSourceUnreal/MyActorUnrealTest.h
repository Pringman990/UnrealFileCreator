
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyACtorTest.generated.h"

UCLASS(BlueprintType, Blueprintable)
class H_API AMyACtorTest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyACtorTest();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

};

