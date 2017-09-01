// Fill out your copyright notice in the Description page of Project Settings.

#include "PathAIController.h"
#include <queue>
#include <vector>
#include <functional>
#include "Engine/World.h"
#include "Navigation/PathFollowingComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Controller.h"
typedef std::pair<float, FVector2D> fV2D;
//x
#define MAP_WIDTH 76
//y
#define MAP_HEIGHT 76

#define MAP_BOX_SIZE 50
TArray<FVector2D> MoveDirections = { FVector2D(1,0),FVector2D(-1,0),FVector2D(0,1),FVector2D(0,-1) };


void APathAIController::BeginPlay()
{
	Super::BeginPlay();
	Dest = FVector::ZeroVector;
	ControlledPawn = GetPawn();
}



void APathAIController::AStar()
{
	std::priority_queue<fV2D, std::vector<fV2D>, std::greater<fV2D>>open;
	std::vector<FVector2D>closed;

	//TODO find a way to create a map matrix around level

	std::vector<std::vector<float>>map;
	std::vector<std::vector<float>>mapG;
	std::vector<std::vector<FVector2D>>Parentmap;

	auto dest = ControlledPawn->GetActorLocation();


	Parentmap = std::vector<std::vector<FVector2D>>(MAP_HEIGHT, std::vector<FVector2D>(MAP_WIDTH, { 0,0 }));
	mapG = std::vector<std::vector<float>>(MAP_HEIGHT, std::vector<float>(MAP_WIDTH, 0));
	map = std::vector<std::vector<float>>(MAP_HEIGHT, std::vector<float>(MAP_WIDTH, -1));

	open.push({ DistanceTo(Dest), FVector2D::ZeroVector });
	while (!open.empty()) 
	{
		auto aux = open.top(); open.pop();
		auto auxIndex = WorldCordinatesToMapIndex(aux.second);

		if (aux.second.X == dest.X&&aux.second.Y == dest.Y)break;

		for (int i = 0; i < MoveDirections.Num(); i++)
		{
			float newX = aux.second.X + 50.f, newY = aux.second.Y + 50.f;
			if (CheckMap(aux.second)/*Check Edges*/) 
			{
				int G = mapG[auxIndex.Y][auxIndex.X] + 50;
				auto newZ = ControlledPawn->GetActorLocation().Z;
				int F = G + DistanceTo(FVector(newX, newY,newZ));
				auto newIndex = WorldCordinatesToMapIndex(FVector2D(newX, newY));
				if (map[newIndex.Y][newIndex.X] == -1 || map[newIndex.Y][newIndex.X] > F && find(closed.begin(), closed.end(), newIndex) == closed.end())
				{
					Parentmap[newIndex.Y][newIndex.X] = aux.second;
					map[newIndex.Y][newIndex.X] = F;
					mapG[newIndex.Y][newIndex.X] = G;
					open.push(fV2D({ F,newIndex }));
				}
			}
		}
		closed.push_back(auxIndex);
	}
	//TODO Trace path and do get the first destination
}

FVector2D APathAIController::WorldCordinatesToMapIndex(FVector2D WorldLocation)
{
	if (CheckMap(FVector2D(WorldLocation.X,WorldLocation.Y))) 
	{
		int x = std::ceil(WorldLocation.X + 1900.f / MAP_BOX_SIZE);
		int y = std::ceil(WorldLocation.Y + 1900.f / MAP_BOX_SIZE);
		return FVector2D(x, y);
	}
	return FVector2D();
}



FVector APathAIController::MapIndexToWorldLocation(FVector2D MapIndex)
{
	if (CheckMap(MapIndex))
	{
		float WX = MapIndex.X*MAP_BOX_SIZE + (MAP_BOX_SIZE / 2) - 1900.f;
		float WY = MapIndex.Y*MAP_BOX_SIZE + (MAP_BOX_SIZE / 2) - 1900.f;
		return FVector(WX, WY, ControlledPawn->GetActorLocation().Z);
	}
	return FVector();
}

bool APathAIController::CheckMap(FVector2D V)
{
	return V.X >= -1900.f&&V.X <= 1900.f&&V.Y >= -1900.f&&V.Y <= 1900.f;
}

float APathAIController::DistanceTo(FVector Dest)
{
	return FVector::Distance(Dest,ControlledPawn->GetActorLocation());
}
//76*76 array map from -1900 to 1900
void APathAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (GetPawn())
	{
		for (int i = 0; i < MoveDirections.Num(); i++) 
		{
			auto PawnLocation = ControlledPawn->GetActorLocation();
			auto dir = (FVector(MoveDirections[i], 0) * 100);
			auto var = PawnLocation + dir;
			DrawDebugLine(GetWorld(), PawnLocation, PawnLocation + dir, FColor::Blue);
		}
	}
	

}
