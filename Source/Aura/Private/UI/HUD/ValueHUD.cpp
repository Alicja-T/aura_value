// Copyright Philosophical Games


#include "UI/HUD/ValueHUD.h"
#include "UI/Widget/ValueUserWidget.h"

void AValueHUD::BeginPlay() { 
  Super::BeginPlay();
  UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
  Widget->AddToViewport();
}
