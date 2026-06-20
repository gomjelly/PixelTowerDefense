// TDGrid.cpp
#include "TDGrid.h"
#include "DrawDebugHelpers.h"

ATDGrid::ATDGrid()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATDGrid::BeginPlay()
{
	Super::BeginPlay();
	OccupiedCells.Init(false, GridRows * GridCols);
}

void ATDGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrawDebugGrid();
}

FIntPoint ATDGrid::WorldToGrid(FVector WorldPos) const
{
	FVector Local = WorldPos - GetActorLocation();
	FIntPoint Cell(
		FMath::FloorToInt(Local.X / CellSize),
		FMath::FloorToInt(Local.Y / CellSize)
	);
	return IsValidCell(Cell) ? Cell : FIntPoint(-1, -1);
}

FVector ATDGrid::GridToWorld(FIntPoint Cell) const
{
	FVector Origin = GetActorLocation();
	return Origin + FVector(
		(Cell.X + 0.5f) * CellSize,
		(Cell.Y + 0.5f) * CellSize,
		0.f
	);
}

bool ATDGrid::IsValidCell(FIntPoint Cell) const
{
	return Cell.X >= 0 && Cell.X < GridCols && Cell.Y >= 0 && Cell.Y < GridRows;
}

bool ATDGrid::IsOccupied(FIntPoint Cell) const
{
	return !IsValidCell(Cell) || OccupiedCells[CellIndex(Cell)];
}

void ATDGrid::SetOccupied(FIntPoint Cell, bool bOccupied)
{
	if (IsValidCell(Cell))
	{
		OccupiedCells[CellIndex(Cell)] = bOccupied;
	}
}

void ATDGrid::DrawDebugGrid() const
{
	UWorld* World = GetWorld();
	if (!World) return;

	const FVector Origin = GetActorLocation() + FVector(0.f, 0.f, 1.f);
	const float W = GridCols * CellSize;
	const float H = GridRows * CellSize;
	const FColor GridColor(80, 200, 120);
	constexpr float Thickness = 1.5f;

	for (int32 Col = 0; Col <= GridCols; ++Col)
	{
		FVector A = Origin + FVector(Col * CellSize, 0.f, 0.f);
		FVector B = Origin + FVector(Col * CellSize, H, 0.f);
		DrawDebugLine(World, A, B, GridColor, false, -1.f, 0, Thickness);
	}
	for (int32 Row = 0; Row <= GridRows; ++Row)
	{
		FVector A = Origin + FVector(0.f, Row * CellSize, 0.f);
		FVector B = Origin + FVector(W, Row * CellSize, 0.f);
		DrawDebugLine(World, A, B, GridColor, false, -1.f, 0, Thickness);
	}
}
