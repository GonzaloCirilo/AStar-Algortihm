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
		return A.weight < B.weight;
	}
};

//x
#define MAP_WIDTH 40
//y
#define MAP_HEIGHT 40
#define MAP_BOX_SIZE 100

void APathAIController::BeginPlay()
{
	Super::BeginPlay();
	MoveDirections = { FVector2D(1,0),FVector2D(-1,0),FVector2D(0,1),FVector2D(0,-1),FVector2D(-1,-1) ,FVector2D(1,-1),FVector2D(-1,1),FVector2D(1,1) };
	ControlledPawn = GetPawn();
	if (ControlledPawn)
	{
		AStar();
	}
}

void APathAIController::AStar()
{
	Dest = ControlledPawn->GetActorLocation();
	PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	PlayerLocationIndex = WorldCordinatesToMapIndex(FVector2D(PlayerLocation.X, PlayerLocation.Y));
	DestIndex = WorldCordinatesToMapIndex(FVector2D(Dest.X, Dest.Y));

	TArray<Map>open;
	TArray<FVector2D>closed;

	TArray<TArray<float>>mapG;

	//Initializing the arrays
	TArray<FVector2D> templates;
	templates.Init({ 0,0 }, MAP_WIDTH);
	Parentmap.Init(templates, MAP_HEIGHT);
	TArray<float> t;
	t.Init(0, MAP_WIDTH);
	mapG.Init(t, MAP_HEIGHT);
	t.Init(-1, MAP_WIDTH);
	map.Init(t, MAP_HEIGHT);

	//Begin Astar
	open.HeapPush({ Manhattan(PlayerLocationIndex), PlayerLocationIndex },MapOperator());
	while (open.Num()!=0) 
	{
		Map aux; open.HeapPop(aux, MapOperator(), false);
		if (aux.position == DestIndex) break;
		for (int32 i = 0; i < MoveDirections.Num(); i++) 
		{
			FVector2D newPosition = aux.position + MoveDirections[i];
			if (CheckMap(newPosition) && CheckWalls(aux.position,MoveDirections[i],newPosition))
			{
				float G = FPlatformMath::Abs(MoveDirections[i].X)==FPlatformMath::Abs(MoveDirections[i].Y)? mapG[aux.position.Y][aux.position.X] + (MAP_BOX_SIZE*1.4): mapG[aux.position.Y][aux.position.X] + MAP_BOX_SIZE;
				float F = G + Manhattan(newPosition);
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
	}	
	Parentmap[PlayerLocationIndex.Y][PlayerLocationIndex.X] = { 0,0 };
	map[DestIndex.Y][DestIndex.X] = 0;
	pathFinder(Parentmap[DestIndex.Y][DestIndex.X]);
}

FVector2D APathAIController::WorldCordinatesToMapIndex(FVector2D WorldLocation)
{
		int x = FPlatformMath::CeilToInt((WorldLocation.X + 2000.f) / MAP_BOX_SIZE);
		int y = FPlatformMath::CeilToInt((WorldLocation.Y + 2000.f) / MAP_BOX_SIZE);
		return FVector2D(x, y);
}

FVector APathAIController::MapIndexToWorldLocation(FVector2D MapIndex)
{
	if (CheckMap(MapIndex))
	{
		float WX = MapIndex.X*MAP_BOX_SIZE + (MAP_BOX_SIZE / 2) - 2000.f;
		float WY = MapIndex.Y*MAP_BOX_SIZE + (MAP_BOX_SIZE / 2) - 2000.f;
		return FVector(WX, WY, ControlledPawn->GetActorLocation().Z);
	}
	return FVector();
}

bool APathAIController::CheckMap(FVector2D V)
{
	return V.X >= 0 && V.X < MAP_WIDTH && V.Y >= 0 && V.Y < MAP_HEIGHT;
}

float APathAIController::Manhattan(FVector2D _Dest)
{
	FVector2D DestIndex = WorldCordinatesToMapIndex(FVector2D(Dest.X, Dest.Y));
	if (CheckMap(DestIndex))
	{
		float r = (FPlatformMath::Abs(_Dest.X - Dest.X) + FPlatformMath::Abs(_Dest.Y - Dest.Y)) * MAP_BOX_SIZE;
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

	//TODO if it gets weird comment the following if
	if (Path.Num() > 3)
	{
		Path.RemoveAt(1);
	}
	///up to here
	if (Path.Num() > 1)
	{
		Path.RemoveAt(Path.Num() - 1);
		Path[Path.Num() - 1] = PlayerLocation;
	}
}

bool APathAIController::CheckWalls(FVector2D position, FVector2D movedir,FVector2D newPosition)
{
	FHitResult HitResult1, HitResult2, HitResult3, HitResult4;
	FVector2D invert = FVector2D(movedir.Y, movedir.X);
	FVector2D checkwall1 = position + (invert * 1);
	FVector2D checkwall2 = position + (invert*-1);
	FVector2D checkwall3 = position - movedir;
	GetWorld()->LineTraceSingleByChannel(HitResult1, MapIndexToWorldLocation(position), MapIndexToWorldLocation(newPosition), ECollisionChannel::ECC_WorldStatic);
	GetWorld()->LineTraceSingleByChannel(HitResult2, MapIndexToWorldLocation(position), MapIndexToWorldLocation(checkwall1), ECollisionChannel::ECC_WorldStatic);
	GetWorld()->LineTraceSingleByChannel(HitResult3, MapIndexToWorldLocation(position), MapIndexToWorldLocation(checkwall2), ECollisionChannel::ECC_WorldStatic);
	GetWorld()->LineTraceSingleByChannel(HitResult4, MapIndexToWorldLocation(position), MapIndexToWorldLocation(checkwall3), ECollisionChannel::ECC_WorldStatic);
	return !HitResult1.GetActor() && !HitResult2.GetActor() && !HitResult3.GetActor();
}

//76*76 array map from -1900 to 1900
void APathAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlayerLocation != GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation())
	{
		AStar();
	}
	for (int i = 0; i < Path.Num()-1; i++)
	{
		if (Path.Num() > 1)
		{
			//DrawDebugLine(GetWorld(), Path[i], Path[i + 1], FColor::Green, false, -1.0, (uint8)'\000', 10);
			DrawDebugPoint(GetWorld(),Path[i],10,FColor::Red);
		}
	}
	//movement
	if (Path.Num() > 1) 
	{	
		auto MyLocation = GetPawn()->GetActorLocation();
		FVector MyLocationRounded = FVector(FPlatformMath::RoundToInt(MyLocation.X), FPlatformMath::RoundToInt(MyLocation.Y), FPlatformMath::RoundToInt(MyLocation.Z));
		if (WorldCordinatesToMapIndex(FVector2D(MyLocationRounded.X,MyLocationRounded.Y)) == WorldCordinatesToMapIndex(FVector2D(Path[1].X,Path[1].Y)))
		{
			Path.RemoveAt(1);
		}
		FVector Dir = -MyLocation + Path[1];
		Dir.Normalize();
		Dir *= MovementSpeed*DeltaTime;
		if (Dir.Size() > 0)
		{
			GetPawn()->SetActorRotation(Dir.Rotation());
			GetPawn()->AddActorWorldOffset(Dir, true);
		}
	}
}
