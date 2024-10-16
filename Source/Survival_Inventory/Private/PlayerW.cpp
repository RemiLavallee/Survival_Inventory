#include "PlayerW.h"

#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"

void UPlayerW::SetPrompt_F(bool IsVisible)
{
	if(TextBlock)
	{
		TextBlock->SetVisibility(IsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}