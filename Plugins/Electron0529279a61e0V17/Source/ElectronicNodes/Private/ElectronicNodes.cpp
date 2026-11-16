/* Copyright (C) 2024 Hugo ATTAL - All Rights Reserved
* This plugin is downloadable from the Unreal Engine Marketplace
*/

#include "ElectronicNodes.h"
#include "ENConnectionDrawingPolicy.h"
#include "ENCommands.h"
#include "NodeFactory.h"
#include "Interfaces/IPluginManager.h"
#include "Lib/HotPatch.h"
#include "Interfaces/IMainFrameModule.h"
#include "Patch/NodeFactoryPatch.h"
#include "Popup/ENUpdatePopup.h"
#include "ISettingsEditorModule.h"
#include "Lib/UserConfig.h"

#define LOCTEXT_NAMESPACE "FElectronicNodesModule"

void FElectronicNodesModule::StartupModule()
{
	ENConnectionFactory = MakeShareable(new FENConnectionDrawingPolicyFactory);
	FEdGraphUtilities::RegisterVisualPinConnectionFactory(ENConnectionFactory);

	auto const CommandBindings = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame").
		GetMainFrameCommandBindings();
	ENCommands::Register();

	CommandBindings->MapAction(
		ENCommands::Get().ToggleMasterActivation,
		FExecuteAction::CreateRaw(this, &FElectronicNodesModule::ToggleMasterActivation)
	);

	ENUserConfig::Initialize();
	GlobalSettingsFile = ENUserConfig::GetFilePath();

	ElectronicNodesSettings = GetMutableDefault<UElectronicNodesSettings>();
	ElectronicNodesSettings->OnSettingChanged().AddRaw(this, &FElectronicNodesModule::ReloadConfiguration);

	if (ElectronicNodesSettings->UseGlobalSettings && ENUserConfig::HasGlobalSettings(ElectronicNodesSettings))
	{
		ElectronicNodesSettings->LoadConfig(nullptr, *GlobalSettingsFile);
	}

	if (ElectronicNodesSettings->UseHotPatch && ElectronicNodesSettings->MasterActivate)
	{
#if (PLATFORM_WINDOWS || PLATFORM_LINUX) && !UE_BUILD_SHIPPING
		FHotPatch::Hook(&FNodeFactory::CreateConnectionPolicy, &FNodeFactoryPatch::CreateConnectionPolicy_Hook);
#endif
	}

	if (ElectronicNodesSettings->ActivatePopupOnUpdate)
	{
		ENUpdatePopup::Register();
	}
}


void FElectronicNodesModule::ReloadConfiguration(UObject* Object, struct FPropertyChangedEvent& Property)
{
	const FName PropertyName = Property.GetPropertyName();

	if (PropertyName == "UseGlobalSettings")
	{
		if (ElectronicNodesSettings->UseGlobalSettings)
		{
			if (ENUserConfig::HasGlobalSettings(ElectronicNodesSettings))
			{
				ElectronicNodesSettings->LoadConfig(nullptr, *GlobalSettingsFile);
			}
			else
			{
				ElectronicNodesSettings->SaveConfig(CPF_Config, *GlobalSettingsFile);
			}
		}
	}

	if (PropertyName == "UseHotPatch")
	{
		ISettingsEditorModule* SettingsEditorModule = FModuleManager::GetModulePtr<ISettingsEditorModule>(
			"SettingsEditor");
		if (SettingsEditorModule)
		{
			SettingsEditorModule->OnApplicationRestartRequired();
		}
	}

	if (ElectronicNodesSettings->LoadGlobalSettings)
	{
		ENUserConfig::ReloadFromDisk();
		if (ENUserConfig::HasGlobalSettings(ElectronicNodesSettings))
		{
			ElectronicNodesSettings->LoadConfig(nullptr, *GlobalSettingsFile);
		}
		ElectronicNodesSettings->LoadGlobalSettings = false;
	}

	ElectronicNodesSettings->SaveConfig();

	if (ElectronicNodesSettings->UseGlobalSettings)
	{
		ElectronicNodesSettings->SaveConfig(CPF_Config, *GlobalSettingsFile);
	}
}

void FElectronicNodesModule::ShutdownModule()
{
	if (ENConnectionFactory.IsValid())
	{
		FEdGraphUtilities::UnregisterVisualPinConnectionFactory(ENConnectionFactory);
		ENConnectionFactory.Reset();
	}

	if (UObjectInitialized() && ElectronicNodesSettings != nullptr)
	{
		ElectronicNodesSettings->OnSettingChanged().RemoveAll(this);
	}

	ENCommands::Unregister();
}

void FElectronicNodesModule::ToggleMasterActivation() const
{
	ElectronicNodesSettings->ToggleMasterActivation();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FElectronicNodesModule, ElectronicNodes)
