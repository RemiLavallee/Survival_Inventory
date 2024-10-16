#include "PlayerW.h"

#include "LandscapeRender.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Elements/Framework/TypedElementQueryBuilder.h"

void UPlayerW::SetPrompt_F(bool IsVisible)
{
	if(TextBlock)
	{
		TextBlock->SetVisibility(IsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}