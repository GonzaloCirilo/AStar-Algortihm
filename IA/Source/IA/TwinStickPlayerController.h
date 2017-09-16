// All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TwinStickShooter.h"
#include "TwinStickPlayerController.generated.h"

/**
 */
UCLASS()
class IA_API ATwinStickPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:
	ATwinStickShooter* GetControlledShooter() const;

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;
	
	void SetupPlayerInputComponent(class UInputComponent* InputComponent);
};
