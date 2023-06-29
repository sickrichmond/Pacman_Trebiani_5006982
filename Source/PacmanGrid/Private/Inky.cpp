// Fill out your copyright notice in the Description page of Project Settings.


#include "Inky.h"
#include "Blinky.h"
#include "TestGridGameMode.h"
#include "PacmanPawn.h"

AGridBaseNode* AInky::GetPlayerRelativeTarget()
{
	FVector2D InkyTarget;
	if (this->PhantomState == Chase) {
		FVector2D BlinkyCoords = GetPlayer()->GetBlinky()->GetLastNodeCoords();
		FVector BlinkyDir = GetPlayer()->GetBlinky()->GetLastValidDirection();
		FVector2D PacmanCoords = Super::GetPlayer()->GetLastNodeCoords();
		FVector PacmanDir = Super::GetPlayer()->GetLastValidDirection();
		if (PacmanDir[0] != 0) {
			PacmanCoords.X += PacmanDir[0] * 2;
		}
		if (PacmanDir[1] != 0) {
			PacmanCoords.Y += PacmanDir[1] * 2;
		}
		InkyTarget = PacmanCoords - BlinkyCoords;
		if (BlinkyDir[0] != 0) {
			InkyTarget.X += BlinkyDir[0] * 2;
		}
		if (BlinkyDir[1] != 0) {
			InkyTarget.Y += BlinkyDir[1] * 2;
		}
	}
	else if (this->PhantomState == Scatter) {
		InkyTarget.X = 30, InkyTarget.Y = 29;
	}
	else{
		int32 XCoords = rand() % 31;
		int32 YCoords = rand() % 30;
		InkyTarget.X = XCoords, InkyTarget.Y = YCoords;
	}
	return TheGridGen->GetNodeByCoords(InkyTarget);
}

AInky::AInky()
{
	CurrentGridCoords = FVector2D(5, 26);
}
