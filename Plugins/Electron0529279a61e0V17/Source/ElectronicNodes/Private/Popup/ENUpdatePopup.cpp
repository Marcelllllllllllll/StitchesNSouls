#include "ENUpdatePopup.h"
#include "EditorStyleSet.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Text/SRichTextBlock.h"
#include "Widgets/Input/SCheckBox.h"
#include "SWebBrowser.h"
#include "Interfaces/IPluginManager.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "Lib/UserConfig.h"

void ENUpdatePopup::OnBrowserLinkClicked(const FSlateHyperlinkRun::FMetadata& Metadata)
{
	const FString* URL = Metadata.Find(TEXT("href"));

	if (URL)
	{
		FPlatformProcess::LaunchURL(**URL, nullptr, nullptr);
	}
}

static const TCHAR* PopupConfigSection = TEXT("ElectronicNodes.UpdatePopup");
static const TCHAR* LastShownVersionKey = TEXT("LastShownVersion");
static const TCHAR* NeverShowAgainKey = TEXT("NeverShowAgain");

void ENUpdatePopup::OnNeverShowAgainChanged(ECheckBoxState NewState)
{
	GConfig->SetBool(PopupConfigSection, NeverShowAgainKey, NewState == ECheckBoxState::Checked, ENUserConfig::GetFilePath());
	GConfig->Flush(false, ENUserConfig::GetFilePath());
}

FString ENUpdatePopup::GetPluginVersion()
{
	const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("ElectronicNodes"));
	return Plugin.IsValid() ? Plugin->GetDescriptor().VersionName : FString();
}

void ENUpdatePopup::Register()
{
	bool bNeverShowAgain = false;
	GConfig->GetBool(PopupConfigSection, NeverShowAgainKey, bNeverShowAgain, ENUserConfig::GetFilePath());
	if (bNeverShowAgain)
	{
		return;
	}

	const FString CurrentPluginVersion = GetPluginVersion();
	FString LastShownVersion;
	GConfig->GetString(PopupConfigSection, LastShownVersionKey, LastShownVersion, ENUserConfig::GetFilePath());

	if (LastShownVersion == CurrentPluginVersion)
	{
		return;
	}

	GConfig->SetString(PopupConfigSection, LastShownVersionKey, *CurrentPluginVersion, ENUserConfig::GetFilePath());
	GConfig->Flush(false, ENUserConfig::GetFilePath());

	FCoreDelegates::GetOnPostEngineInit().AddLambda([]()
	{
		Open();
	});
}

void ENUpdatePopup::Open()
{
	if (!FSlateApplication::Get().CanDisplayWindows())
	{
		return;
	}

	TSharedRef<SBorder> WindowContent = SNew(SBorder)
		.BorderImage(FCoreStyle::Get().GetBrush("ToolPanel.GroupBorder"))
		.Padding(FMargin(8.0f, 8.0f));

	TSharedPtr<SWindow> Window = SNew(SWindow)
		.AutoCenter(EAutoCenter::PreferredWorkArea)
		.SupportsMaximize(false)
		.SupportsMinimize(false)
		.SizingRule(ESizingRule::FixedSize)
		.ClientSize(FVector2D(600, 400))
		.Title(FText::FromString("Electronic Nodes"))
		.IsTopmostWindow(true)
		[
			WindowContent
		];

	const FSlateFontInfo HeadingFont = FCoreStyle::GetDefaultFontStyle("Regular", 24);
	const FSlateFontInfo ContentFont = FCoreStyle::GetDefaultFontStyle("Regular", 12);

	TSharedRef<SVerticalBox> InnerContent = SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10)
		[
			SNew(STextBlock)
			.Font(HeadingFont)
			.Text(FText::FromString(FString::Printf(TEXT("Electronic Nodes v%s"), *GetPluginVersion())))
		]
		+ SVerticalBox::Slot()
		.FillHeight(1.0)
		.Padding(10)
		[
			SNew(SBorder)
			.Padding(10)
			.BorderImage(FAppStyle::GetBrush("ToolPanel.DarkGroupBorder"))
			[
				SNew(SScrollBox)
				+ SScrollBox::Slot()
				[
					SNew(SRichTextBlock)
					.Text(FText::FromString(R"(
<LargeText>Hello and thank you for using Electronic Nodes!</>

First thing first, if you've been enjoying using it, it would mean a lot if you could just drop <a id="browser" href="https://www.fab.com/listings/d6148766-27b1-47db-a730-832c53b7a895">a small review on the marketplace page</> :). I also wanted to mention that I made another plugin to update the engine theme called <a id="browser" href="https://www.fab.com/listings/7fcc3b88-bddc-4ac7-8132-1be496bb3caf">Darker Nodes</>.

But let's keep it short, here are the cool new features (and bugfixes) of version 3.24!


<LargeText>Version 3.24</>

<RichTextBlock.Bold>Features</>

* Add a "Never show this again" option to this popup

<RichTextBlock.Bold>Bugfixes</>

* Fix this popup showing on every launch and global settings not saving when the plugin folder is read-only (Fab install, source control). Both are now stored in your user folder and shared across engine versions
* Fix missing attribute wires (curves, etc.) on Anim Graphs (<a id="browser" href="https://github.com/hugoattal/ElectronicNodes/issues/99">issue #99</>)
* Fix wire alignment "Left" being ignored on some Subway wires depending on zoom (<a id="browser" href="https://github.com/hugoattal/ElectronicNodes/issues/101">issue #101</>)
* Fix ribbon wires grouping differently depending on zoom
* Fix hover and Alt-click detection on curved wire parts
* Fix slice line not cutting curved wires
* Fix short wires not being hoverable or sliceable
* Fix short spline wires orientation on Behavior Trees
* Fix bubbles not following the wire on 45-degree corners
* Fix hot patch failing on Linux when the patch crosses a memory page
* Improve performance when bubbles are displayed on selection
* Clean up the module on shutdown


<LargeText>Version 3.23</>

<RichTextBlock.Bold>Bugfixes</>

* Fix wire colors (<a id="browser" href="https://github.com/hugoattal/ElectronicNodes/issues/121">issue #121</>)


<LargeText>Version 3.22</>

<RichTextBlock.Bold>Features</>

* Add PCG graph color support (<a id="browser" href="https://github.com/hugoattal/ElectronicNodes/issues/118">issue #118</>)

<RichTextBlock.Bold>Bugfixes</>

* Fix disappearing wires with ribbon (<a id="browser" href="https://github.com/hugoattal/ElectronicNodes/issues/120">issue #120</>)


<LargeText>Version 3.21</>

<RichTextBlock.Bold>Features</>

* Add a custom hook for other plugins (<a id="browser" href="https://github.com/hugoattal/ElectronicNodes/issues/116">issue #116</> - Thanks TheGeebus for the help!)

<RichTextBlock.Bold>Bugfixes</>

* Fix unstyled wire on Master Activate Off (<a id="browser" href="https://github.com/hugoattal/ElectronicNodes/issues/117">issue #117</>)


<LargeText>Version 3.20</>

<RichTextBlock.Bold>Features</>

* Add hot patch support for Linux (<a id="browser" href="https://github.com/hugoattal/ElectronicNodes/issues/112">issue #112</>)

<RichTextBlock.Bold>Bugfixes</>

* Fix wire crossing on spline (<a id="browser" href="https://github.com/hugoattal/ElectronicNodes/issues/111">issue #111</>)
* Fix PCG wire color (<a id="browser" href="https://github.com/hugoattal/ElectronicNodes/issues/113">issue #113</>)


<a id="browser" href="https://github.com/hugoattal/ElectronicNodes#changelog">See complete changelog</>
)"))
					.TextStyle(FAppStyle::Get(), "NormalText")
					.DecoratorStyleSet(&FAppStyle::Get())
					.AutoWrapText(true)
					+ SRichTextBlock::HyperlinkDecorator(
						TEXT("browser"), FSlateHyperlinkRun::FOnClick::CreateStatic(&OnBrowserLinkClicked))
				]
			]
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10, 0)
		[
			SNew(SCheckBox)
			.IsChecked(ECheckBoxState::Unchecked)
			.OnCheckStateChanged_Static(&OnNeverShowAgainChanged)
			[
				SNew(STextBlock)
				.Text(FText::FromString("Never show this again"))
			]
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().FillWidth(1.0f)
			[
				SNew(SButton)
				.Text(FText::FromString("Leave a review <3"))
				.HAlign(HAlign_Center)
				.OnClicked_Lambda([]()
				{
					const FString URL = "https://www.fab.com/listings/d6148766-27b1-47db-a730-832c53b7a895";
					FPlatformProcess::LaunchURL(*URL, nullptr, nullptr);

					return FReply::Handled();
				})
			]
			+ SHorizontalBox::Slot().AutoWidth()
			[
				SNew(SSpacer)
				.Size(FVector2D(20, 10))
			]
			+ SHorizontalBox::Slot().FillWidth(1.0f)
			[
				SNew(SButton)
				.Text(FText::FromString("Discover Darker Nodes"))
				.HAlign(HAlign_Center)
				.OnClicked_Lambda([]()
				{
					const FString URL = "https://www.fab.com/listings/7fcc3b88-bddc-4ac7-8132-1be496bb3caf";
					FPlatformProcess::LaunchURL(*URL, nullptr, nullptr);

					return FReply::Handled();
				})
			]
			+ SHorizontalBox::Slot().AutoWidth()
			[
				SNew(SSpacer)
				.Size(FVector2D(20, 10))
			]
			+ SHorizontalBox::Slot().FillWidth(1.0f)
			[
				SNew(SButton)
				.Text(FText::FromString("Close this window"))
				.HAlign(HAlign_Center)
				.OnClicked_Lambda([Window]()
				{
					Window->RequestDestroyWindow();

					return FReply::Handled();
				})
			]
		];

	WindowContent->SetContent(InnerContent);
	Window = FSlateApplication::Get().AddWindow(Window.ToSharedRef());
}
