#include "PlayerW.h"

#include "LandscapeRender.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Elements/Framework/TypedElementQueryBuilder.h"

void UPlayerW::SetPrompt_F()
{
	if(IsVisible)
	{
		TextBlock->SetVisibility(static_cast<ESlateVisibility>(true));
	}
}