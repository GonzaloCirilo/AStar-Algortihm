// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PathAIController.generated.h"

/**
 * 
 */
UCLASS()
class IA_API APathAIController : public AAIController
{
	GENERATED_BODY()

		FVector Dest;

private:
	virtual void Tick(float DeltaTime)override;

	virtual void BeginPlay()override;

	void AStar();

	FVector2D WorldCordinatesToMapIndex(FVector2D WorldLocation);

	FVector MapIndexToWorldLocation(FVector2D MapIndex);

	bool CheckMap(FVector2D);

	//Heuristic for Astar
	float DistanceTo(FVector Dest);

	APawn* ControlledPawn;
	
};