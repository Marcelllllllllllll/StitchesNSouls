/* Copyright (C) 2024 Hugo ATTAL - All Rights Reserved
* This plugin is downloadable from the Unreal Engine Marketplace
*/

#pragma once

#include "CoreMinimal.h"
#include "HAL/FileManager.h"
#include "HAL/PlatformProcess.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

namespace ENUserConfig
{
	inline const FString& GetFilePath()
	{
		static const FString FilePath = FConfigCacheIni::NormalizeConfigIniPath(
			FPaths::Combine(FPlatformProcess::UserSettingsDir(), TEXT("UnrealEngine"), TEXT("ElectronicNodes"), TEXT("ElectronicNodes.ini")));
		return FilePath;
	}

	inline FString GetLegacyGlobalSettingsPath()
	{
		const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("ElectronicNodes"));
		return Plugin.IsValid() ? Plugin->GetBaseDir() / TEXT("Settings.ini") : FString();
	}

	inline void Initialize()
	{
		const FString& FilePath = GetFilePath();
		if (FPaths::FileExists(FilePath))
		{
			return;
		}

		const FString LegacyFilePath = GetLegacyGlobalSettingsPath();
		if (!LegacyFilePath.IsEmpty() && FPaths::FileExists(LegacyFilePath))
		{
			IFileManager::Get().Copy(*FilePath, *LegacyFilePath);
		}
		else
		{
			FFileHelper::SaveStringToFile(FString(), *FilePath);
		}
	}

	inline bool HasGlobalSettings(const UObject* Settings)
	{
		return GConfig->DoesSectionExist(*Settings->GetClass()->GetPathName(), GetFilePath());
	}

	inline void ReloadFromDisk()
	{
		GConfig->UnloadFile(GetFilePath());
	}
}
