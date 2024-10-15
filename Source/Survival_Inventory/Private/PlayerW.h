#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerW.generated.h"

UCLASS()
class UPlayerW : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Custom Event")
	void SetPrompt_F();

protected:
	UPROPERTY(meta = (BindWidget = "Panel"))
	class UCanvasPanel* CanvasPanel;

	UPROPERTY(meta = (BindWidget = "Prompt_F"))
	class UTextBlock* TextBlock;

private:
	bool IsVisible;
};
