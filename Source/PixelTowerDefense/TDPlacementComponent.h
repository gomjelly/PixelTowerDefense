// TDPlacementComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TDPlacementComponent.generated.h"

class ATDGrid;
class UStaticMesh;
class UStaticMeshComponent;
class UMaterialInterface;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTowerPlaced, FIntPoint, Cell);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PIXELTOWERDEFENSE_API UTDPlacementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTDPlacementComponent();

	// Reference to the grid actor in the level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
	ATDGrid* TargetGrid = nullptr;

	// Optional: material tint for valid / invalid placement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
	UMaterialInterface* ValidMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
	UMaterialInterface* InvalidMaterial = nullptr;

	UPROPERTY(BlueprintAssignable, Category = "Placement")
	FOnTowerPlaced OnTowerPlaced;

	UFUNCTION(BlueprintCallable, Category = "Placement")
	void StartPlacement(UStaticMesh* Mesh);

	UFUNCTION(BlueprintCallable, Category = "Placement")
	void ConfirmPlacement();

	UFUNCTION(BlueprintCallable, Category = "Placement")
	void CancelPlacement();

	UFUNCTION(BlueprintPure, Category = "Placement")
	bool IsInPlacementMode() const { return bIsPlacementMode; }

	UFUNCTION(BlueprintPure, Category = "Placement")
	FIntPoint GetHoveredCell() const { return HoveredCell; }

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	bool bIsPlacementMode = false;
	FIntPoint HoveredCell = FIntPoint(-1, -1);

	UPROPERTY()
	UStaticMeshComponent* GhostMesh = nullptr;

	void UpdateGhostTransform();
	void DestroyGhost();
	APlayerController* GetPlayerController() const;
};
