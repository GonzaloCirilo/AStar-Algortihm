// All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TwinStickShooter.generated.h"

UCLASS()
class IA_API ATwinStickShooter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATwinStickShooter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float MoveSpeed=200.f;

public:	


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Movement(FVector MoveDirection, float DeltaTime);
	
};
