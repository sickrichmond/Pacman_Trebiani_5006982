// Fill out your copyright notice in the Description page of Project Settings.


#include "Clyde.h"
#include "PacmanPawn.h"

AGridBaseNode* AClyde::GetPlayerRelativeTarget()
{
	FVector2D coords;
	if (this->PhantomState == Chase) {
		FVector2D Zeros = (0, 0);
		FVector2D PacmanCoords = Super::GetPlayer()->GetLastNodeCoords();
		FVector2D ClydeCoords = this->GetLastNodeCoords();
		double Distance = sqrt((PacmanCoords.X - ClydeCoords.X) * (PacmanCoords.X - ClydeCoords.X) + (PacmanCoords.Y - ClydeCoords.Y) * (PacmanCoords.Y - ClydeCoords.Y));
		if (Distance >= 8) {
			return Super::GetPlayerRelativeTarget();
		}
		else {
			return TheGridGen->GetNodeByCoords(Zeros);
		}
	}
	else if (this->PhantomState == Scatter) {
		coords.X = 0, coords.Y = 29;
		return TheGridGen->GetNodeByCoords(coords);
	}
	else{
		int32 XCoords = rand() % 31;
		int32 YCoords = rand() % 30;
		coords.X = XCoords, coords.Y = YCoords;
		return TheGridGen->GetNodeByCoords(coords);
	}
}

AClyde::AClyde()
{
	CurrentGridCoords = FVector2D(5, 26);
}
