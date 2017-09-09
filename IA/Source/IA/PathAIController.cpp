// Fill out your copyright notice in the Description page of Project Settings.

#include "PathAIController.h"
#include "GenericPlatformMath.h"
#include "Engine/World.h"
#include "Navigation/PathFollowingComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"

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
#define MAP_WIDTH 38
//y
#define MAP_HEIGHT 38
#define MAP_BOX_SIZE 100

void APathAIController::BeginPlay()
{
	Super::BeginPlay();
	MoveDirections = { FVector2D(1,0),FVector2D(-1,0),FVector2D(0,1),FVector2D(0,-1) };
	ControlledPawn = GetPawn();
	if (ControlledPawn)
	{
		Dest = ControlledPawn->GetActorLocation();
		PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
		PlayerLocationIndex = WorldCordinatesToMapIndex(FVector2D(PlayerLocation.X, PlayerLocation.Y));
		DestIndex = WorldCordinatesToMapIndex(FVector2D(Dest.X, Dest.Y));
		AStar();
	}
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

	open.HeapPush({ DistanceTo(PlayerLocationIndex), PlayerLocationIndex },MapOperator());
	int32 cont = 0;
	while (open.Num()!=0 /*&& cont!=75*/) 
	{
		Map aux; open.HeapPop(aux, MapOperator(), false);
		if (aux.position == DestIndex) break;
		for (int32 i = 0; i < MoveDirections.Num(); i++) 
		{
			FVector2D newPosition = aux.position + MoveDirections[i];
			FHitResult HitResult;
			GetWorld()->LineTraceSingleByChannel(HitResult, MapIndexToWorldLocation(aux.position), MapIndexToWorldLocation(newPosition), ECollisionChannel::ECC_WorldStatic);
			if (CheckMap(newPosition)&& !HitResult.GetActor())
			{
				float G = mapG[aux.position.Y][aux.position.X] + MAP_BOX_SIZE;
				float F = G + DistanceTo(newPosition);
				if ((map[newPosition.Y][newPosition.X] == -1 || map[newPosition.Y][newPosition.X] > F) && closed.Find(newPosition))
				{
					Parentmap[newPosition.Y][newPosition.X] = aux.position;
					map[newPosition.Y][newPosition.X] = F;
					mapG[newPosition.Y][newPosition.X] = G;
					open.HeapPush(Map({ F, newPosition }), MapOperator());
				}
			}
		}
		closed.Push(aux.position);
		cont++;
	}	
	Parentmap[PlayerLocationIndex.Y][PlayerLocationIndex.X] = { 0,0 };
	map[DestIndex.Y][DestIndex.X] = 0;
	pathFinder(Parentmap[DestIndex.Y][DestIndex.X]);
}

FVector2D APathAIController::WorldCordinatesToMapIndex(FVector2D WorldLocation)
{
		int x = FPlatformMath::CeilToInt((WorldLocation.X + 1900.f) / MAP_BOX_SIZE);
		int y = FPlatformMath::CeilToInt((WorldLocation.Y + 1900.f) / MAP_BOX_SIZE);
		return FVector2D(x, y);
}

FVector APathAIController::MapIndexToWorldLocation(FVector2D MapIndex)
{
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
	return V.X >= 0 && V.X < MAP_WIDTH && V.Y >= 0 && V.Y < MAP_HEIGHT;
}
//TODO Change to Manhattan cause this garbage isn´t working
float APathAIController::DistanceTo(FVector2D _Dest)
{
	FVector2D DestIndex = WorldCordinatesToMapIndex(FVector2D(Dest.X, Dest.Y));
	if (CheckMap(DestIndex))
	{
		//FVector2D R = DestIndex - _Dest;//38,38
		//float r = R.Size();
		//TODO try manhattan
		float r = (FPlatformMath::Abs(_Dest.X - Dest.X) + FPlatformMath::Abs(_Dest.Y - Dest.Y)) * 100;
		return r;
	}
	return -1.f;
}

void APathAIController::pathFinder(FVector2D position)
{
	Path.Empty();
	Path.Push(Dest);
	TArray<FVector2D>s; s.Push(position);
	while (true)
	{
		auto p = s.Top(); s.Pop();
		if (p == FVector2D::ZeroVector) break;
		Path.Push(MapIndexToWorldLocation(p));
		map[p.Y][p.X] = 0;
		s.Push(Parentmap[p.Y][p.X]);
	}
	Path.RemoveAt(Path.Num() - 1);
	Path[Path.Num() - 1] = PlayerLocation;
}

//76*76 array map from -1900 to 1900
void APathAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (GetPawn())
	{
		//Posible way to intercept colisions fo A*
		for (int i = 0; i < MoveDirections.Num(); i++) 
		{
			auto PawnLocation = ControlledPawn->GetActorLocation();
			auto dir = (FVector(MoveDirections[i], 0) * 100);
			auto var = PawnLocation + dir;
			DrawDebugLine(GetWorld(), PawnLocation, PawnLocation + dir, FColor::White);
		}
	}
	if (PlayerLocation != GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation())
	{
		Dest = ControlledPawn->GetActorLocation();
		PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
		PlayerLocationIndex = WorldCordinatesToMapIndex(FVector2D(PlayerLocation.X, PlayerLocation.Y));
		DestIndex = WorldCordinatesToMapIndex(FVector2D(Dest.X, Dest.Y));
		AStar();
	}
	for (int i = 0; i < Path.Num()-1; i++)
	{
		DrawDebugLine(GetWorld(), Path[i], Path[i + 1], FColor::White);
	}
	
}
