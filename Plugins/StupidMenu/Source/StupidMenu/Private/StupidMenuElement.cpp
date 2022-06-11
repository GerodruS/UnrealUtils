#include "StupidMenuElement.h"

FStupidMenuElement::FStupidMenuElement()
	: OnGetTitle()
	, OnClick()
{
}

FStupidMenuElement::FStupidMenuElement(
	const FStupidMenuElementTitleDelegate& OnGetTitle,
	const FStupidMenuElementCallbackDelegate& OnClick
	)
	: OnGetTitle(OnGetTitle)
	, OnClick(OnClick)
{
}

bool FStupidMenuElement::IsEmpty() const
{
	return !OnGetTitle.IsBound() ||	!OnClick.IsBound();
}
