// Fill out your copyright notice in the Description page of Project Settings.
//Ora è una copia di blinky, va cambiato

#include "Pinky.h"

AGridBaseNode* APinky::GetPlayerRelativeTarget()
{
	FVector2D coords;
	if (this->PhantomState == Chase) {
		return Super::GetTargetNode();
	}
	else if (this->PhantomState == Scatter) {
		coords.X = 0, coords.Y = 0;
		return TheGridGen->GetNodeByCoords(coords);
	}
	else{
		int32 XCoords = rand() % 31;
		int32 YCoords = rand() % 30;
		coords.X = XCoords, coords.Y = YCoords;
		return TheGridGen->GetNodeByCoords(coords);
	}
}

APinky::APinky()
{
	CurrentGridCoords = FVector2D(27, 26);
}