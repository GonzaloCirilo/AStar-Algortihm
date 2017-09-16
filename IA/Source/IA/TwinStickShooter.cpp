// All rights reserved

#include "TwinStickShooter.h"
#include "Components/InputComponent.h"

// Sets default values
ATwinStickShooter::ATwinStickShooter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATwinStickShooter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATwinStickShooter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	const float ForwardValue = GetInputAxisValue("MoveForward");
	const float RightValue = GetInputAxisValue("MoveRight");

	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);


	Movement(MoveDirection, DeltaTime);

}

// Called to bind functionality to input
void ATwinStickShooter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward");
	PlayerInputComponent->BindAxis("MoveRight");
}

void ATwinStickShooter::Movement(FVector MoveDirection, float DeltaTime)
{
	const FVector Movement = MoveDirection * MoveSpeed * DeltaTime;

	MoveDirection *= MoveSpeed*DeltaTime;
	if (MoveDirection.Size() > 0)
	{
		SetActorRotation(MoveDirection.Rotation());
		AddActorWorldOffset(MoveDirection, true);
	}
}

