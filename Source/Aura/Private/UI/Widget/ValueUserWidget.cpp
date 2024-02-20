// Copyright Philosophical Games


#include "UI/Widget/ValueUserWidget.h"

void UValueUserWidget::SetWidgetController(UObject* InWidgetController) {
  WidgetController = InWidgetController;
  WidgetControllerSet();
}
