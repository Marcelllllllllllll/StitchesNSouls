// Copyright N.A. Tools 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Styling/SlateTypes.h"
#include "UObject/CoreNative.h"
#include "SlickNodesConfig.generated.h"

struct FPropertyChangedEvent; // forward declare

UCLASS(config=EditorPerProjectUserSettings, defaultconfig, meta=(DisplayName="Slick Nodes"))
class SLICKNODES_API USlickNodesConfig : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	USlickNodesConfig(const FObjectInitializer& ObjectInitializer);

	virtual FName GetContainerName() const override { return TEXT("Editor"); }
	virtual FName GetCategoryName() const override  { return TEXT("Plugins"); }
	virtual FName GetSectionName() const override   { return TEXT("SlickNodes"); }

#if WITH_EDITOR
	virtual FText GetSectionText() const override;
	virtual FText GetSectionDescription() const override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif


	// -------------------------------------------------------------------------
	// General
	// -------------------------------------------------------------------------
	UPROPERTY(config, EditAnywhere, Category = "Node Settings", DisplayName = "Overwrite Nodes")
	bool bOverwriteNodes;


	// -------------------------------------------------------------------------
	// Customize | RegularNode
	// -------------------------------------------------------------------------
	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|RegularNode", DisplayName = "Header (64x32)",
		meta = (EditCondition = "bOverwriteNodes", AllowedClasses = "/Script/Engine.Texture2D",
			Tooltip = "Set a 64x32 texture to override the icon for the node header.\nIMPORTANT: Texture Group = UI, Compression Settings = VectorDisplacementmap, sRGB = false",
			ConfigRestartRequired = true))
	TSoftObjectPtr<UTexture2D> RegularNodeHeader;

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|RegularNode", DisplayName = "Header Highlight (64x22)",
		meta = (EditCondition = "bOverwriteNodes", AllowedClasses = "/Script/Engine.Texture2D",
			Tooltip = "Set a 64x22 texture to override the icon for the node header highlight.\nIMPORTANT: Texture Group = UI, Compression Settings = VectorDisplacementmap, sRGB = false",
			ConfigRestartRequired = true))
	TSoftObjectPtr<UTexture2D> RegularNodeTitleHighlight;

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|RegularNode", DisplayName = "Header Gloss (64x25)",
		meta = (EditCondition = "bOverwriteNodes", AllowedClasses = "/Script/Engine.Texture2D",
			Tooltip = "Set a 64x25 texture to override the icon for the node header gloss.\nIMPORTANT: Texture Group = UI, Compression Settings = VectorDisplacementmap, sRGB = false",
			ConfigRestartRequired = true))
	TSoftObjectPtr<UTexture2D> RegularNodeHeaderGloss;

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|RegularNode", DisplayName = "Body (64x64)",
		meta = (EditCondition = "bOverwriteNodes", AllowedClasses = "/Script/Engine.Texture2D",
			Tooltip = "Set a 64x64 texture to override the icon for the node body.\nIMPORTANT: Texture Group = UI, Compression Settings = VectorDisplacementmap, sRGB = false",
			ConfigRestartRequired = true))
	TSoftObjectPtr<UTexture2D> RegularNodeBody;

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|RegularNode", DisplayName = "Body Gloss (64x64)",
		meta = (EditCondition = "bOverwriteNodes", AllowedClasses = "/Script/Engine.Texture2D",
			Tooltip = "Set a 64x64 texture to override the icon for the node body gloss.\nIMPORTANT: Texture Group = UI, Compression Settings = VectorDisplacementmap, sRGB = false",
			ConfigRestartRequired = true))
	TSoftObjectPtr<UTexture2D> RegularNodeBodyGloss;

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|RegularNode", DisplayName = "Shadow (64x64)",
		meta = (EditCondition = "bOverwriteNodes", AllowedClasses = "/Script/Engine.Texture2D",
			Tooltip = "Set a 64x64 texture to override the icon for the node shadow.\nIMPORTANT: Texture Group = UI, Compression Settings = VectorDisplacementmap, sRGB = false",
			ConfigRestartRequired = true))
	TSoftObjectPtr<UTexture2D> RegularNodeShadow;

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|RegularNode", DisplayName = "Selected Border (64x64)",
		meta = (EditCondition = "bOverwriteNodes", AllowedClasses = "/Script/Engine.Texture2D",
			Tooltip = "Set a 64x64 texture to override the icon for the node selection border.\nIMPORTANT: Texture Group = UI, Compression Settings = VectorDisplacementmap, sRGB = false",
			ConfigRestartRequired = true))
	TSoftObjectPtr<UTexture2D> RegularNodeSelected;


	// -------------------------------------------------------------------------
	// Customize | VariableNode
	// -------------------------------------------------------------------------
	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|VariableNode", DisplayName = "Header (132x28)",
		meta = (EditCondition = "bOverwriteNodes", AllowedClasses = "/Script/Engine.Texture2D",
			Tooltip = "Set a 132x28 texture.\nIMPORTANT: Texture Group = UI, Compression Settings = VectorDisplacementmap, sRGB = false",
			ConfigRestartRequired = true))
	TSoftObjectPtr<UTexture2D> VariableNodeHeader;

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|VariableNode", DisplayName = "Body (64x28)",
		meta = (EditCondition = "bOverwriteNodes", AllowedClasses = "/Script/Engine.Texture2D", ConfigRestartRequired = true))
	TSoftObjectPtr<UTexture2D> VariableNodeBody;

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|VariableNode", DisplayName = "Shadow (64x64)",
		meta = (EditCondition = "bOverwriteNodes", AllowedClasses = "/Script/Engine.Texture2D", ConfigRestartRequired = true))
	TSoftObjectPtr<UTexture2D> VariableNodeShadow;

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|VariableNode", DisplayName = "Selected Border (64x64)",
		meta = (EditCondition = "bOverwriteNodes", AllowedClasses = "/Script/Engine.Texture2D", ConfigRestartRequired = true))
	TSoftObjectPtr<UTexture2D> VariableNodeSelected;

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|VariableNode", DisplayName = "Gloss (64x28)",
		meta = (EditCondition = "bOverwriteNodes", AllowedClasses = "/Script/Engine.Texture2D", ConfigRestartRequired = true))
	TSoftObjectPtr<UTexture2D> VariableNodeGloss;

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|VariableNode", DisplayName = "Collapsed Header (64x28)",
		meta = (EditCondition = "bOverwriteNodes", AllowedClasses = "/Script/Engine.Texture2D", ConfigRestartRequired = true))
	TSoftObjectPtr<UTexture2D> VariableNodeCollapsedHeader;

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|VariableNode", DisplayName = "Collapsed Body (64x28)",
		meta = (EditCondition = "bOverwriteNodes", AllowedClasses = "/Script/Engine.Texture2D", ConfigRestartRequired = true))
	TSoftObjectPtr<UTexture2D> VariableNodeCollapsedBody;


	// -------------------------------------------------------------------------
	// Customize | Icons
	// -------------------------------------------------------------------------
	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|Icons", DisplayName = "Variable Texture (11x11)",
		meta = (EditCondition = "bOverwriteNodes", AllowedClasses = "/Script/Engine.Texture2D", ConfigRestartRequired = true))
	TSoftObjectPtr<UTexture2D> VariableTexture;

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|Icons", DisplayName = "Array Texture (16x16)",
		meta = (EditCondition = "bOverwriteNodes", AllowedClasses = "/Script/Engine.Texture2D", ConfigRestartRequired = true))
	TSoftObjectPtr<UTexture2D> ArrayTexture;

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|Icons", DisplayName = "Array Connected (11x11)",
		meta = (EditCondition = "bOverwriteNodes", AllowedClasses = "/Script/Engine.Texture2D", ConfigRestartRequired = true))
	TSoftObjectPtr<UTexture2D> ArrayTextureConnected;

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|Icons", DisplayName = "Array Disconnected (11x11)",
		meta = (EditCondition = "bOverwriteNodes", AllowedClasses = "/Script/Engine.Texture2D", ConfigRestartRequired = true))
	TSoftObjectPtr<UTexture2D> ArrayTextureDisconnected;

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|Icons", DisplayName = "Map Connected (16x16)",
		meta = (EditCondition = "bOverwriteNodes", AllowedClasses = "/Script/Engine.Texture2D", ConfigRestartRequired = true))
	TSoftObjectPtr<UTexture2D> MapTextureConnected;

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|Icons", DisplayName = "Map Disconnected (16x16)",
		meta = (EditCondition = "bOverwriteNodes", AllowedClasses = "/Script/Engine.Texture2D", ConfigRestartRequired = true))
	TSoftObjectPtr<UTexture2D> MapTextureDisconnected;

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|Icons", DisplayName = "ExecPin Connected (96x128)",
		meta = (EditCondition = "bOverwriteNodes", AllowedClasses = "/Script/Engine.Texture2D", ConfigRestartRequired = true))
	TSoftObjectPtr<UTexture2D> RegularExecPinConnected;

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|Icons", DisplayName = "ExecPin Disconnected (96x128)",
		meta = (EditCondition = "bOverwriteNodes", AllowedClasses = "/Script/Engine.Texture2D", ConfigRestartRequired = true))
	TSoftObjectPtr<UTexture2D> RegularExecPinDisconnected;

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|Icons", DisplayName = "Delegate Pin Connected (16x16)",
		meta = (EditCondition = "bOverwriteNodes", AllowedClasses = "/Script/Engine.Texture2D", ConfigRestartRequired = true))
	TSoftObjectPtr<UTexture2D> RegularDelegatePinConnected;

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|Icons", DisplayName = "Delegate Pin Disconnected (16x16)",
		meta = (EditCondition = "bOverwriteNodes", AllowedClasses = "/Script/Engine.Texture2D", ConfigRestartRequired = true))
	TSoftObjectPtr<UTexture2D> RegularDelegatePinDisconnected;

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|Icons", DisplayName = "Regular Round Pin Connected (15x11)",
		meta = (EditCondition = "bOverwriteNodes", AllowedClasses = "/Script/Engine.Texture2D", ConfigRestartRequired = true))
	TSoftObjectPtr<UTexture2D> RegularRoundPinConnected;

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|Icons", DisplayName = "Regular Round Pin Disconnected (15x11)",
		meta = (EditCondition = "bOverwriteNodes", AllowedClasses = "/Script/Engine.Texture2D", ConfigRestartRequired = true))
	TSoftObjectPtr<UTexture2D> RegularRoundPinDisconnected;

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|Icons", DisplayName = "Variable Round Pin Connected (15x11)",
		meta = (EditCondition = "bOverwriteNodes", AllowedClasses = "/Script/Engine.Texture2D", ConfigRestartRequired = true))
	TSoftObjectPtr<UTexture2D> VariableRoundPinConnected;

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|Icons", DisplayName = "Variable Round Pin Disconnected (15x11)",
		meta = (EditCondition = "bOverwriteNodes", AllowedClasses = "/Script/Engine.Texture2D", ConfigRestartRequired = true))
	TSoftObjectPtr<UTexture2D> VariableRoundPinDisconnected;

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|Icons", DisplayName = "Pin Hovered (32x8)",
		meta = (EditCondition = "bOverwriteNodes", AllowedClasses = "/Script/Engine.Texture2D", ConfigRestartRequired = true))
	TSoftObjectPtr<UTexture2D> PinHovered;

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|Icons", DisplayName = "Simulation Mode Bubble (32x32)",
		meta = (EditCondition = "bOverwriteNodes", AllowedClasses = "/Script/Engine.Texture2D", ConfigRestartRequired = true))
	TSoftObjectPtr<UTexture2D> SimulationBubble;


	// -------------------------------------------------------------------------
	// Customize | Colors | Nodes
	// -------------------------------------------------------------------------
	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|Colors|Nodes", DisplayName = "Overwrite Colors")
	bool bOverwriteColors = true;

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|Colors|Nodes", DisplayName = "Event Node",
		meta = (EditCondition = "bOverwriteColors"))
	FLinearColor EventNodeColor = FColor::FromHex(TEXT("#FC2F2FFF"));

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|Colors|Nodes", DisplayName = "Function Call Node",
		meta = (EditCondition = "bOverwriteColors"))
	FLinearColor FunctionCallNodeColor = FColor::FromHex(TEXT("#0088FFFF"));

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|Colors|Nodes", DisplayName = "Pure Function Node",
		meta = (EditCondition = "bOverwriteColors"))
	FLinearColor PureFunctionNodeColor = FColor::FromHex(TEXT("#00C16BFF"));

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|Colors|Nodes", DisplayName = "Parent Function Call Node",
		meta = (EditCondition = "bOverwriteColors"))
	FLinearColor ParentFunctionNodeColor = FColor::FromHex(TEXT("#FF931BFF"));

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|Colors|Nodes", DisplayName = "Function Terminator Node",
		meta = (EditCondition = "bOverwriteColors"))
	FLinearColor FunctionTerminatorColor = FColor::FromHex(TEXT("#C93EFFFF"));

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|Colors|Nodes", DisplayName = "Branch Node",
		meta = (EditCondition = "bOverwriteColors"))
	FLinearColor BranchNodeColor = FColor::FromHex(TEXT("#656565FF"));

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|Colors|Nodes", DisplayName = "Sequence Node",
		meta = (EditCondition = "bOverwriteColors"))
	FLinearColor SequenceNodeColor = FColor::FromHex(TEXT("#656565FF"));

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|Colors|Nodes", DisplayName = "Result Node",
		meta = (EditCondition = "bOverwriteColors"))
	FLinearColor ResultNodeColor = FColor::FromHex(TEXT("#F8CFA8FF"));

	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|Colors|Nodes", DisplayName = "Default Comment Node",
		meta = (EditCondition = "bOverwriteColors"))
	FLinearColor DefaultCommentNodeColor = FColor::FromHex(TEXT("#656565FF"));
	
	UPROPERTY(config, EditAnywhere, Category = "Node Settings|Customize|Colors|Nodes", DisplayName = "Preview Node",
		meta = (EditCondition = "bOverwriteColors"))
	FLinearColor PreviewNodeColor = FColor::FromHex(TEXT("#2770FCFF"));


	// -------------------------------------------------------------------------
	// Graph
	// -------------------------------------------------------------------------
	UPROPERTY(config, EditAnywhere, Category = "Graph", DisplayName = "Grid Background Color",
		meta = (sRGB = "true", AllowReset = "true"))
	FLinearColor GridBackgroundColor = FColor::FromHex(TEXT("#282828FF"));

	TMap<FString, FSlateBrush*> SlateBrushes;
};