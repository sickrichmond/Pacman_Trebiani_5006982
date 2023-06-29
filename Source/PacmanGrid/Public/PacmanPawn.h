// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridPawn.h"
#include "GridGenerator.h"
#include "GridPlayerController.h"
#include "GridBaseNode.h"
#include "PowerNode.h"
#include <Blinky.h>
#include <Pinky.h>
#include <Clyde.h>
#include <Inky.h>
#include "PacmanPawn.generated.h"

/**
 * 
 */
UCLASS()
class PACMANGRID_API APacmanPawn : public AGridPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APacmanPawn();
	UPROPERTY(EditAnywhere)
	int Points;
	UPROPERTY(EditAnywhere)
	int Lifes;
	UPROPERTY(EditAnywhere)
	int EatToWin;
	UPROPERTY(EditAnywhere)
	bool CanEat;
	void AddPoints(int Pointz);
	void StopEating();
	void PacmanSpawn();
	int ScatterCount;
	void StopScatter();
	void StartScatter();
	void PhantomInvert();
	FTimerHandle CanEatTimer;
	FTimerHandle ScatterTimer;
	virtual void Tick(float DeltaTime) override;
	virtual void SetVerticalInput(float AxisValue) override;
	virtual void SetHorizontalInput(float AxisValue) override;
	// called on left mouse click (binding)
	virtual	void OnClick() override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	int counter = 0;
	UFUNCTION()
		ABlinky* GetBlinky() const;
	UFUNCTION()
		AInky* GetInky() const;
	UFUNCTION()
		APinky* GetPinky() const;
	UFUNCTION()
		AClyde* GetClyde() const;
	

protected:
	virtual void BeginPlay() override;
	virtual void HandleMovement() override;
	virtual void SetTargetNode(AGridBaseNode* Node) override;
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
private:
	UPROPERTY(VisibleAnywhere)
		class ABlinky* BlinkyPawn;

	UPROPERTY(VisibleAnywhere)
		class AInky* InkyPawn;

	UPROPERTY(VisibleAnywhere)
		class APinky* PinkyPawn;

	UPROPERTY(VisibleAnywhere)
		class AClyde* ClydePawn;
};
