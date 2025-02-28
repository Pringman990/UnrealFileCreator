
#include "MyACtorTest.h"

AMyACtorTest::AMyACtorTest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMyACtorTest::BeginPlay()
{
	Super::BeginPlay();

}

void AMyACtorTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}