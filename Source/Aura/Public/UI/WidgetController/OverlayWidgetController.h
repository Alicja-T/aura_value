// Copyright Philosophical Games

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/ValueWidgetController.h"
#include "OverlayWidgetController.generated.h"

class UValueUserWidget;

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase {
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  FGameplayTag MessageTag = FGameplayTag();
  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  FText Message = FText();

  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  TSubclassOf<UValueUserWidget> MessageWidget;
  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  UTexture2D* MessageImage = nullptr;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float,
                                    NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature,
                                            FUIWidgetRow, Row);


 /**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UValueWidgetController
{
	GENERATED_BODY()
 public:
   virtual void BroadcastInitialValues() override;
   virtual void BindCallbacksToDependencies() override;
  UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
  FOnAttributeChangedSignature OnHealthChanged;

  UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
  FOnAttributeChangedSignature OnMaxHealthChanged;
  UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
  FOnAttributeChangedSignature OnManaChanged;

  UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
  FOnAttributeChangedSignature OnMaxManaChanged;
  UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
  FMessageWidgetRowSignature MessageWidgetRowDelegate;

 protected:
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
  TObjectPtr<UDataTable> MessageWidgetDataTable;
  void BindCallbacksToAttributeSet(
      const FGameplayAttribute& Attribute,
      FOnAttributeChangedSignature* AttributeChangeDelegate);
  template <typename T>
  T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable,
                                                  const FGameplayTag& Tag) {
  return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}