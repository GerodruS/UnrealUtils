#include "StupidMenuElement.h"

const FStupidMenuElement FStupidMenuElement::Empty = FStupidMenuElement();

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

FStupidMenuElement::FStupidMenuElement(
	TFunctionRef<FText()> OnGetTitle,
	TFunctionRef<void()> OnClick
	)
	: FStupidMenuElement()
{
	FStupidMenuElement::OnGetTitle.BindLambda(OnGetTitle);
	FStupidMenuElement::OnClick.BindLambda(OnClick);
}

FStupidMenuElement::FStupidMenuElement(
	FText Title,
	TFunctionRef<void()> OnClick
	)
	: FStupidMenuElement()
{
	FStupidMenuElement::OnGetTitle.BindLambda([Title]
	{
		return Title;
	});
	FStupidMenuElement::OnClick.BindLambda(OnClick);
}

bool FStupidMenuElement::IsEmpty() const
{
	return !OnGetTitle.IsBound() ||	!OnClick.IsBound();
}
