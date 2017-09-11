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
	FVector PlayerLocation;

	FVector2D PlayerLocationIndex;
	FVector2D DestIndex;

private:
	virtual void Tick(float DeltaTime)override;

	virtual void BeginPlay()override;

	void AStar();

	FVector2D WorldCordinatesToMapIndex(FVector2D WorldLocation);

	FVector MapIndexToWorldLocation(FVector2D MapIndex);

	bool CheckMap(FVector2D);

	//Heuristic for Astar
	float Manhattan(FVector2D Dest);

	void pathFinder(FVector2D);

	bool CheckWalls(FVector2D position, FVector2D movedir, FVector2D newposition);

	APawn* ControlledPawn;

	TArray<FVector2D> MoveDirections;
	TArray<FVector> Path;
	TArray<TArray<float>>map;
	TArray<TArray<FVector2D>>Parentmap;
	float MovementSpeed=100.f;
	
};