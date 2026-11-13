// Copyright N.A. Tools 2025. All Rights Reserved.

#include "SlickNodesConfig.h"
#include "SlickNodes.h"
#include "UObject/UnrealType.h"

#define LOCTEXT_NAMESPACE "SlickNodes"

USlickNodesConfig::USlickNodesConfig(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void USlickNodesConfig::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	SaveConfig();

#if WITH_EDITOR
	FSlickNodesModule::ApplyEditorStyle_Static();
	FSlickNodesModule::SetBaseColors_Static();
#endif
}

#if WITH_EDITOR

FText USlickNodesConfig::GetSectionText() const
{
	return LOCTEXT("UserSettingsDisplayName", "Slick Nodes");
}

FText USlickNodesConfig::GetSectionDescription() const
{
	return LOCTEXT("UserSettingsDescription", "Configure options of Slick Nodes");
}

#endif // WITH_EDITOR
#undef LOCTEXT_NAMESPACE