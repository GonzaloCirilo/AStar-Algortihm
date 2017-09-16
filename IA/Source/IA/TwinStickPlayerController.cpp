// All rights reserved

#include "TwinStickPlayerController.h"

void ATwinStickPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATwinStickPlayerController::BeginPlay(){
	Super::BeginPlay();
}

ATwinStickShooter* ATwinStickPlayerController::GetControlledShooter() const 
{
	return Cast<ATwinStickShooter>(GetPawn());
}


 void ATwinStickPlayerController::SetupPlayerInputComponent(class UInputComponent* InputComponent)
 {
     //Super::SetupPlayerInputComponent(InputComponent);
 
 }

