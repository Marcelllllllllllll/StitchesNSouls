// Copyright N.A. Tools 2025. All Rights Reserved.

#include "SlickNodes.h"

#include "GraphEditorSettings.h"
#include "SlickNodesConfig.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"
#include "Styling/SlateTypes.h"
#include "Styling/SlateColor.h"

#include "ISettingsModule.h"
#include "Modules/ModuleManager.h"
#include "Framework/Application/SlateApplication.h"

#define LOCTEXT_NAMESPACE "FSlickNodesModule"
#define RootToContentDir Style->RootToContentDir

void FSlickNodesModule::StartupModule()
{
    if (GIsEditor && !IsRunningCommandlet())
    {
        ApplyEditorStyle();
        SetBaseColors();
    }
}

void FSlickNodesModule::ApplyEditorStyle_Static()
{
#if WITH_EDITOR
    if (FSlickNodesModule* Module =
        FModuleManager::GetModulePtr<FSlickNodesModule>("SlickNodes"))
    {
        Module->ApplyEditorStyle();
    }
#endif
}

void FSlickNodesModule::SetBaseColors_Static()
{
#if WITH_EDITOR
    if (FSlickNodesModule* Module =
        FModuleManager::GetModulePtr<FSlickNodesModule>("SlickNodes"))
    {
        Module->SetBaseColors();
    }
#endif
}

void FSlickNodesModule::ShutdownModule()
{
    if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
    {
        SettingsModule->UnregisterSettings("Editor", "Plugins", "SlickNodes");
    }
}


void FSlickNodesModule::ApplyEditorStyle()
{
    TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin("SlickNodes");
    if (!Plugin.IsValid()) { return; }

    const USlickNodesConfig* Settings = GetDefault<USlickNodesConfig>();
    if (!Settings) { return; }

    FSlateStyleSet* Style = static_cast<FSlateStyleSet*>(
        const_cast<ISlateStyle*>(&FAppStyle::Get())
    );
    if (!Style) { return; }

    static bool bContentRootSet = false;
    if (!bContentRootSet)
    {
        Style->SetContentRoot(Plugin->GetBaseDir() / TEXT("Resources"));
        bContentRootSet = true;
    }

    // -------------------------------------------------------------------------
    // Folder Nodes/Regular
    // -------------------------------------------------------------------------
    SetStyle(Settings->RegularNodeHeader,         Style, "Graph.Node.ColorSpill",      "Nodes/Regular/RegularNode_color_spill",     true,  FVector2D(0,0), FMargin(16.f/64.f, 12.f/32.f, 16.f/64.f, 12.f/32.f));
    SetStyle(Settings->RegularNodeBody,           Style, "Graph.Node.Body",             "Nodes/Regular/RegularNode_body",            true,  FVector2D(0,0), FMargin(16.f/64.f, 25.f/64.f, 16.f/64.f, 16.f/64.f));
    SetStyle(Settings->RegularNodeBodyGloss,      Style, "Graph.Node.TintedBody",       "Nodes/Regular/TintedNode_body",             true,  FVector2D(0,0), FMargin(16.f/64.f, 25.f/64.f, 16.f/64.f, 16.f/64.f));
    SetStyle(Settings->RegularNodeHeaderGloss,    Style, "Graph.Node.TitleGloss",       "Nodes/Regular/RegularNode_title_gloss",     true,  FVector2D(0,0), FMargin(12.0f/64.0f));
    SetStyle(Settings->RegularNodeTitleHighlight, Style, "Graph.Node.TitleHighlight",   "Nodes/Regular/RegularNode_title_highlight", true,  FVector2D(0,0), FMargin(16.0f/64.0f, 1.0f, 16.0f/64.0f, 0.0f));
    Style->Set("Graph.Node.ShadowSize", FVector2D(12, 12));
    SetStyle(Settings->RegularNodeSelected,       Style, "Graph.Node.ShadowSelected",   "Nodes/Regular/RegularNode_shadow_selected", true,  FVector2D(0,0), FMargin(18.0f/64.0f));
    SetStyle(Settings->RegularNodeShadow,         Style, "Graph.Node.Shadow",           "Nodes/Regular/RegularNode_shadow",          true,  FVector2D(0,0), FMargin(18.0f/64.0f));

    // -------------------------------------------------------------------------
    // Folder Nodes/Variable
    // -------------------------------------------------------------------------
    SetStyle(Settings->VariableNodeBody,             Style, "Graph.VarNode.Body",               "Nodes/Variable/VarNode_body",                  true,  FVector2D(0,0), FMargin(16.f/64.f, 12.f/28.f));
    SetStyle(Settings->VariableNodeHeader,           Style, "Graph.VarNode.ColorSpill",          "Nodes/Variable/VarNode_color_spill",           false, FVector2D(132,28));
    SetStyle(Settings->VariableNodeGloss,            Style, "Graph.VarNode.Gloss",               "Nodes/Variable/VarNode_gloss",                 true,  FVector2D(0,0), FMargin(16.f/64.f, 16.f/28.f, 16.f/64.f, 4.f/28.f));
    SetStyle(Settings->VariableNodeSelected,         Style, "Graph.VarNode.ShadowSelected",      "Nodes/Variable/VarNode_shadow_selected",       true,  FVector2D(0,0), FMargin(26.0f/64.0f));
    SetStyle(Settings->VariableNodeShadow,           Style, "Graph.VarNode.Shadow",              "Nodes/Variable/VarNode_shadow",                true,  FVector2D(0,0), FMargin(26.0f/64.0f));
    SetStyle(Settings->VariableNodeCollapsedBody,    Style, "Graph.CollapsedNode.Body",          "Nodes/Variable/RegularNode_body",              true,  FVector2D(0,0), FMargin(16.f/64.f, 25.f/64.f, 16.f/64.f, 16.f/64.f));
    SetStyle(Settings->VariableNodeCollapsedHeader,  Style, "Graph.CollapsedNode.BodyColorSpill","Nodes/Variable/CollapsedNode_Body_ColorSpill", true,  FVector2D(0,0), FMargin(64.f/256.f, 100.f/256.f, 64.f/256.f, 64.f/256.f));

    Style->Set("KismetExpression.ReadVariable.Body",             new BOX_BRUSH("/Nodes/Variable/Linear_VarNode_Background", FMargin(16.f/64.f, 12.f/28.f)));
    Style->Set("KismetExpression.ReadAutogeneratedVariable.Body",new BOX_BRUSH("/Nodes/Variable/Linear_VarNode_Background", FMargin(16.f/64.f, 12.f/28.f)));

    // -------------------------------------------------------------------------
    // Folder Nodes/ExecPins
    // -------------------------------------------------------------------------
    SetStyle(Settings->RegularExecPinConnected,    Style, "Graph.ExecPin.Connected",          "Nodes/ExecPins/ExecPin_Connected",    false, FVector2D(18.0f, 24.0f));
    SetStyle(Settings->RegularExecPinDisconnected, Style, "Graph.ExecPin.Disconnected",       "Nodes/ExecPins/ExecPin_Disconnected", false, FVector2D(18.0f, 24.0f));
    SetStyle(Settings->RegularExecPinConnected,    Style, "Graph.ExecPin.ConnectedHovered",   "Nodes/ExecPins/ExecPin_Connected",    false, FVector2D(18.0f, 24.0f));
    SetStyle(Settings->RegularExecPinDisconnected, Style, "Graph.ExecPin.DisconnectedHovered","Nodes/ExecPins/ExecPin_Disconnected", false, FVector2D(18.0f, 24.0f));

    // -------------------------------------------------------------------------
    // Folder Nodes/Pins
    // -------------------------------------------------------------------------
    SetStyle(Settings->VariableRoundPinConnected,    Style, "Graph.Pin.Connected_VarA",   "Nodes/Pins/Pin_connected_VarA",    false, FVector2D(15,11));
    SetStyle(Settings->VariableRoundPinDisconnected, Style, "Graph.Pin.Disconnected_VarA","Nodes/Pins/Pin_disconnected_VarA", false, FVector2D(15,11));
    SetStyle(Settings->RegularRoundPinConnected,     Style, "Graph.Pin.Connected",         "Nodes/Pins/Pin_connected",         false, FVector2D(11,11));
    SetStyle(Settings->RegularRoundPinDisconnected,  Style, "Graph.Pin.Disconnected",      "Nodes/Pins/Pin_disconnected",      false, FVector2D(11,11));

    SetStyle(Settings->ArrayTextureConnected,    Style, "Graph.ArrayPin.Connected",          "Nodes/Pins/ArrayPin_connected",    false, FVector2D(11,11));
    SetStyle(Settings->ArrayTextureDisconnected, Style, "Graph.ArrayPin.Disconnected",       "Nodes/Pins/ArrayPin_disconnected", false, FVector2D(11,11));
    SetStyle(Settings->ArrayTexture,             Style, "Kismet.VariableList.ArrayTypeIcon", "Nodes/Pins/ArrayPin_InVariables",  false, FVector2D(16,16));
    SetStyle(Settings->VariableTexture,          Style, "Kismet.VariableList.TypeIcon",      "Nodes/Pins/SinglePin_InVariables", false, FVector2D(16,16));

    SetStyle(Settings->RegularDelegatePinConnected,    Style, "Graph.DelegatePin.Connected",    "Nodes/Pins/DelegatePin_Connected",    false, FVector2D(11,11));
    SetStyle(Settings->RegularDelegatePinDisconnected, Style, "Graph.DelegatePin.Disconnected", "Nodes/Pins/DelegatePin_Disconnected", false, FVector2D(11,11));

    SetStyle(Settings->PinHovered, Style, "Graph.Pin.BackgroundHovered", "Nodes/Pins/Pin_hover_cue", true, FVector2D(0,0), FMargin(1.0f/32.0f, 1.0f/8.0f, 1.0f/32.0f, 1.0f/8.0f));

    // -------------------------------------------------------------------------
    // Graph Background + Simulation Bubble
    // -------------------------------------------------------------------------
    Style->Set("Graph.Panel.SolidBackground", new IMAGE_BRUSH("/Graph/GraphPanel_SolidBackground", FVector2D(16,16), Settings->GridBackgroundColor, ESlateBrushTileType::Both));
    SetStyle(Settings->SimulationBubble, Style, "Graph.ExecutionBubble", "Nodes/Pins/ExecutionBubble", false, FVector2D(32,32));

    if (FSlateApplication::IsInitialized())
    {
        FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
    }
}


void FSlickNodesModule::SetStyle(const TSoftObjectPtr<UTexture2D> CustomAsset, FSlateStyleSet* Style, const FName& Type,
    const FString& TexturePath, bool bIsBoxBrush, const FVector2D& Size, const FMargin& Margin)
{
    const USlickNodesConfig* Settings = GetDefault<USlickNodesConfig>();
    UTexture2D* LoadedAsset = (Settings && Settings->bOverwriteNodes) ? CustomAsset.LoadSynchronous() : nullptr;

    if (LoadedAsset)
    {
        if (bIsBoxBrush)
            Style->Set(Type, new FSlateBoxBrush(LoadedAsset, Margin));
        else
            Style->Set(Type, new FSlateImageBrush(LoadedAsset, Size));
    }
    else
    {
        if (bIsBoxBrush)
            Style->Set(Type, new BOX_BRUSH(TexturePath, Margin));
        else
            Style->Set(Type, new IMAGE_BRUSH(TexturePath, Size));
    }
}


// -----------------------------------------------------------------------------
// CompensateTint
// Die ColorSpill-Textur (#888888, sRGB=false) hat linearen Wert 136/255 = 0.5333.
// Unreal rendert: FinalColor = NodeTitleColor * 0.5333
// Proportionale Kompensation: Farbton exakt erhalten, Helligkeit auf Maximum normiert.
// -----------------------------------------------------------------------------
FLinearColor FSlickNodesModule::CompensateTint(const FLinearColor& DesiredColor)
{
    constexpr float TintFactor = 136.0f / 255.0f;

    float r = DesiredColor.R / TintFactor;
    float g = DesiredColor.G / TintFactor;
    float b = DesiredColor.B / TintFactor;

    const float MaxChannel = FMath::Max3(r, g, b);
    if (MaxChannel > 1.0f)
    {
        r /= MaxChannel;
        g /= MaxChannel;
        b /= MaxChannel;
    }

    return FLinearColor(r, g, b, DesiredColor.A);
}


void FSlickNodesModule::SetBaseColors()
{
    UGraphEditorSettings* GraphSettings = GetMutableDefault<UGraphEditorSettings>();
    const USlickNodesConfig* Settings = GetDefault<USlickNodesConfig>();
    if (!GraphSettings || !Settings) { return; }

    if (Settings->bOverwriteColors)
    {
        // Slick Nodes Farben setzen (mit Tint-Kompensation fuer die #888888 ColorSpill-Textur)
        GraphSettings->EventNodeTitleColor              = CompensateTint(Settings->EventNodeColor);
        GraphSettings->FunctionCallNodeTitleColor       = CompensateTint(Settings->FunctionCallNodeColor);
        GraphSettings->PureFunctionCallNodeTitleColor   = CompensateTint(Settings->PureFunctionNodeColor);
        GraphSettings->ParentFunctionCallNodeTitleColor = CompensateTint(Settings->ParentFunctionNodeColor);
        GraphSettings->FunctionTerminatorNodeTitleColor = CompensateTint(Settings->FunctionTerminatorColor);
        GraphSettings->ExecBranchNodeTitleColor         = CompensateTint(Settings->BranchNodeColor);
        GraphSettings->ExecSequenceNodeTitleColor       = CompensateTint(Settings->SequenceNodeColor);
        GraphSettings->ResultNodeTitleColor             = CompensateTint(Settings->ResultNodeColor);
        GraphSettings->DefaultCommentNodeTitleColor     = CompensateTint(Settings->DefaultCommentNodeColor);

        // PreviewNodeTitleColor wurde in UE 5.3 eingefuehrt - in 5.2 nicht vorhanden.
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3
        GraphSettings->PreviewNodeTitleColor            = CompensateTint(Settings->PreviewNodeColor);
#endif
    }
    else
    {
        GraphSettings->EventNodeTitleColor              = FLinearColor(0.90f, 0.00f, 0.00f);
        GraphSettings->FunctionCallNodeTitleColor       = FLinearColor(0.18f, 0.42f, 0.76f);
        GraphSettings->PureFunctionCallNodeTitleColor   = FLinearColor(0.04f, 0.34f, 0.11f);
        GraphSettings->ParentFunctionCallNodeTitleColor = FLinearColor(0.70f, 0.20f, 0.00f);
        GraphSettings->FunctionTerminatorNodeTitleColor = FLinearColor(0.70f, 0.10f, 0.50f);
        GraphSettings->ExecBranchNodeTitleColor         = FLinearColor(0.18f, 0.18f, 0.18f);
        GraphSettings->ExecSequenceNodeTitleColor       = FLinearColor(0.18f, 0.18f, 0.18f);
        GraphSettings->ResultNodeTitleColor             = FLinearColor(0.34f, 0.05f, 0.05f);
        GraphSettings->DefaultCommentNodeTitleColor     = FLinearColor(0.18f, 0.18f, 0.18f);

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3
        GraphSettings->PreviewNodeTitleColor            = FLinearColor(0.18f, 0.18f, 0.18f);
#endif
    }

    GraphSettings->PostEditChange();
}

#undef LOCTEXT_NAMESPACE
IMPLEMENT_MODULE(FSlickNodesModule, SlickNodes)