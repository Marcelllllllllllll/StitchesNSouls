// Copyright N.A. Tools 2025. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Brushes/SlateImageBrush.h"
#include "Modules/ModuleManager.h"
#include "Styling/SlateStyle.h"

class FSlickNodesModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    static void SetBaseColors();
    static void SetBaseColors_Static();
    static void ApplyEditorStyle_Static();
    static void ApplyEditorStyle();

private:
    
    static FLinearColor CompensateTint(const FLinearColor& DesiredColor);

    static void SetStyle(
        const TSoftObjectPtr<UTexture2D> CustomAsset,
        FSlateStyleSet* Style,
        const FName& Type,
        const FString& TexturePath,
        bool bIsBoxBrush = false,
        const FVector2D& Size = FVector2D(16.0f, 16.0f),
        const FMargin& Margin = FMargin(0.0f)
    );
};