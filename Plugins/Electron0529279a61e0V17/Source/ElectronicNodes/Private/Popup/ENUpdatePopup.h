#pragma once
#include "Framework/Text/SlateHyperlinkRun.h"
#include "Styling/SlateTypes.h"

class ENUpdatePopup
{
public:
	static void Register();
	static void Open();
	static FString GetPluginVersion();
	static void OnNeverShowAgainChanged(ECheckBoxState NewState);
	static void OnBrowserLinkClicked(const FSlateHyperlinkRun::FMetadata& Metadata);
};
