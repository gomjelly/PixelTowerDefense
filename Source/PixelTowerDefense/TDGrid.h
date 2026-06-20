// TDGrid.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TDGrid.generated.h"

UCLASS()
class PIXELTOWERDEFENSE_API ATDGrid : public AActor
{
	GENERATED_BODY()

public:
	ATDGrid();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid")
	int32 GridCols = 20;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid")
	int32 GridRows = 20;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid")
	float CellSize = 100.f;

	// World position → grid coordinate. Returns (-1,-1) if out of bounds.
	UFUNCTION(BlueprintCallable, Category = "Grid")
	FIntPoint WorldToGrid(FVector WorldPos) const;

	// Grid coordinate → world center of that cell (at grid Z).
	UFUNCTION(BlueprintCallable, Category = "Grid")
	FVector GridToWorld(FIntPoint Cell) const;

	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool IsValidCell(FIntPoint Cell) const;

	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool IsOccupied(FIntPoint Cell) const;

	UFUNCTION(BlueprintCallable, Category = "Grid")
	void SetOccupied(FIntPoint Cell, bool bOccupied);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	TArray<bool> OccupiedCells;

	int32 CellIndex(FIntPoint Cell) const { return Cell.Y * GridCols + Cell.X; }
	void DrawDebugGrid() const;
};
