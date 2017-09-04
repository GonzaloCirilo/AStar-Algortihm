// Fill out your copyright notice in the Description page of Project Settings.

#include "PathAIController.h"
#include "GenericPlatformMath.h"
#include "Engine/World.h"
#include "Navigation/PathFollowingComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Controller.h"

struct Map {
	float weight;
	FVector2D position;
};

struct MapOperator
{
	bool operator()(const Map& A, const Map& B) const
	{
		// Inverted compared to std::priority_queue - higher priorities float to the top
		return A.weight < B.weight;
	}
};

//x
#define MAP_WIDTH 76
//y
#define MAP_HEIGHT 76
#define MAP_BOX_SIZE 50

void APathAIController::BeginPlay()
{
	Super::BeginPlay();
	MoveDirections = { FVector2D(1,0),FVector2D(-1,0),FVector2D(0,1),FVector2D(0,-1) };
	ControlledPawn = GetPawn();
	Dest = ControlledPawn->GetActorLocation();
	Dest = MapIndexToWorldLocation(WorldCordinatesToMapIndex(FVector2D(Dest.X, Dest.Y)));
	AStar();
}

TArray<TArray<float>>map;
TArray<TArray<FVector2D>>Parentmap;

void APathAIController::AStar()
{
	TArray<Map>open;
	TArray<FVector2D>closed;

	TArray<TArray<float>>mapG;

	TArray<FVector2D> templates;
	templates.Init({ 0,0 }, MAP_WIDTH);
	Parentmap.Init(templates, MAP_HEIGHT);
	TArray<float> t;
	t.Init(0, MAP_WIDTH);
	mapG.Init(t, MAP_HEIGHT);
	t.Init(-1, MAP_WIDTH);
	map.Init(t, MAP_HEIGHT);

	open.HeapPush({ DistanceTo(FVector::ZeroVector), FVector2D::ZeroVector },MapOperator());
	auto cont = 0;
	while (open.Num()!=0 && cont!=200) 
	{
		Map aux; open.HeapPop(aux, MapOperator(), false);
		auto auxIndex = WorldCordinatesToMapIndex(aux.position);
		auto destIndex = WorldCordinatesToMapIndex(FVector2D(Dest.X, Dest.Y));
		if (auxIndex==destIndex)break;
		for (int32 i = 0; i < MoveDirections.Num(); i++)
		{
			float newX = aux.position.X + (MoveDirections[i].X*50.f), newY = aux.position.Y + (MoveDirections[i].Y*50.f);
			if (CheckMap(FVector2D(newX,newY))/*Check Edges*/) 
			{
				int32 G = mapG[auxIndex.Y][auxIndex.X] + 50;
				auto newZ = ControlledPawn->GetActorLocation().Z;
			//		UE_LOG(LogTemp,Warning,TEXT("IN LOOP"))
				float F = G + DistanceTo(FVector(newX, newY,newZ));
				//UE_LOG(LogTemp,Warning,TEXT("%f OUT LOOP"),F)
				auto newIndex = WorldCordinatesToMapIndex(FVector2D(newX, newY));
				if ((map[newIndex.Y][newIndex.X] == -1 || map[newIndex.Y][newIndex.X] > F) && closed.Find(newIndex))
				{
					Parentmap[newIndex.Y][newIndex.X] = aux.position;
					map[newIndex.Y][newIndex.X] = F;
					mapG[newIndex.Y][newIndex.X] = G;
					auto var = MapIndexToWorldLocation(newIndex);
					open.HeapPush(Map({ F, /*FVector2D(var.X,var.Y)*/FVector2D(newX,newY) }),MapOperator());
				}
			}
		}
		/*Print open array*/
		UE_LOG(LogTemp, Warning, TEXT("Iteration %d:"),cont)
		UE_LOG(LogTemp,Warning,TEXT("OPEN"))
		for (int32 k = 0; k < open.Num(); k++) 
		{
			UE_LOG(LogTemp,Warning,TEXT("%d: %f %s"),k,open[k].weight,*open[k].position.ToString())
		}
		closed.Push(auxIndex); cont++;
		UE_LOG(LogTemp,Warning,TEXT("Closed"))
		for (int i = 0; i < closed.Num(); i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *closed[i].ToString())
		}
	}/*
	for (int i = 0; i < closed.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *closed[i].ToString())
	}*/
	auto StartIndex = WorldCordinatesToMapIndex(FVector2D::ZeroVector);
	auto EndIndex = WorldCordinatesToMapIndex(FVector2D(Dest.X, Dest.Y));

	UE_LOG(LogTemp,Warning,TEXT("STARTING POINT:%s ENDINGPOINT: %s"),*StartIndex.ToString(),*EndIndex.ToString())
	Parentmap[StartIndex.Y][StartIndex.X] = { 0,0 };
	map[EndIndex.Y][EndIndex.X] = 0;
	auto NextDest = pathFinder(Parentmap[EndIndex.Y][EndIndex.X]);
	//UE_LOG(LogTemp,Warning,TEXT("%s"),*NextDest.ToString())
}

FVector2D APathAIController::WorldCordinatesToMapIndex(FVector2D WorldLocation)
{
	if (CheckMap(WorldLocation)) 
	{
		//UE_LOG(LogTemp, Warning, TEXT("Input: %s"), *WorldLocation.ToString())
		int x = FPlatformMath::CeilToInt((WorldLocation.X + 1900.f) / MAP_BOX_SIZE);
		int y = FPlatformMath::CeilToInt((WorldLocation.Y + 1900.f) / MAP_BOX_SIZE);
		//UE_LOG(LogTemp, Warning, TEXT("Output: %s"), *FVector2D(x, y).ToString())
		return FVector2D(x, y);
	}
	return FVector2D();
}

FVector APathAIController::MapIndexToWorldLocation(FVector2D MapIndex)
{
	/*
	LogTemp:Warning: Input: X=40.000 Y=38.000
	LogTemp:Warning: Input: X=125.000 Y=25.000 Z=190.000
	*/
	if (CheckMap(MapIndex))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Input: %s"), *MapIndex.ToString())
		float WX = MapIndex.X*MAP_BOX_SIZE + (MAP_BOX_SIZE / 2) - 1900.f;
		float WY = MapIndex.Y*MAP_BOX_SIZE + (MAP_BOX_SIZE / 2) - 1900.f;
		//UE_LOG(LogTemp, Warning, TEXT("Input: %s"), *FVector(WX, WY, ControlledPawn->GetActorLocation().Z).ToString())
		return FVector(WX, WY, ControlledPawn->GetActorLocation().Z);
	}
	return FVector();
}

bool APathAIController::CheckMap(FVector2D V)
{
	return V.X >= -1900.f&&V.X <= 1900.f&&V.Y >= -1900.f&&V.Y <= 1900.f;
}
//TODO Change to Manhattan cause this garbage isn´t working
float APathAIController::DistanceTo(FVector _Dest)
{
	FVector2D _DestIndex = WorldCordinatesToMapIndex(FVector2D(_Dest.X, _Dest.Y));
	FVector2D DestIndex = WorldCordinatesToMapIndex(FVector2D(Dest.X, Dest.Y));
	FVector2D R = DestIndex - _DestIndex;
	float r = R.Size();
	UE_LOG(LogTemp, Warning, TEXT("DistanceTo: %f Points: %s %s"), r, *_DestIndex.ToString(), *DestIndex.ToString())
	return r;
}

FVector2D APathAIController::pathFinder(FVector2D position)
{
	TArray<FVector2D>s; s.Push(position);
	while (true)
	{
		auto p = s.Top(); s.Pop();
		if (p == FVector2D::ZeroVector) 
		{
			return p;
		}
		map[p.Y][p.X] = 0;
		s.Push(Parentmap[p.Y][p.X]);
	}

	return FVector2D();
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
