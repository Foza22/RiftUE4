// Fill out your copyright notice in the Description page of Project Settings.


#include "RiftHUD.h"
#include "Engine/Canvas.h"

void ARiftHUD::DrawHUD()
{
	Super::DrawHUD();

	DrawCrosshair();
}

// Set up and draw simple crosshair
void ARiftHUD::DrawCrosshair()
{
	const TInterval<float> Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);

	const float HalfLineSize = 7.f;
	const float LineThickness = 2.f;
	const FLinearColor LineColor = FLinearColor::White;

	DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness);
	DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);
}
