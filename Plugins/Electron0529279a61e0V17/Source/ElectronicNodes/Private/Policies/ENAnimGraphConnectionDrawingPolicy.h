/* Copyright (C) 2024 Hugo ATTAL - All Rights Reserved
* This plugin is downloadable from the Unreal Engine Marketplace
*/

#pragma once

#include "CoreMinimal.h"
#include "AnimGraphConnectionDrawingPolicy.cpp"
#include "AnimationPins/SGraphPinPose.cpp"
#include "ENConnectionDrawingPolicy.h"

class FENAnimGraphConnectionDrawingPolicy : public FAnimGraphConnectionDrawingPolicy
{
public:
	FENAnimGraphConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements, UEdGraph* InGraphObj)
		: FAnimGraphConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect, InDrawElements, InGraphObj)
	{
		this->ConnectionDrawingPolicy = new FENConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect, InDrawElements, InGraphObj);
	}

	virtual void BuildPinToPinWidgetMap(TMap<TSharedRef<SWidget>, FArrangedWidget>& InPinGeometries) override
	{
		FAnimGraphConnectionDrawingPolicy::BuildPinToPinWidgetMap(InPinGeometries);

		ENPinAttributes.Reset();
		ENPanelZoom = 1.0f;
		bool bFoundPanelZoom = false;

		for (const TPair<UEdGraphPin*, TSharedPtr<SGraphPin>>& PinWidgetPair : PinToPinWidgetMap)
		{
			if (PinWidgetPair.Key->Direction == EGPD_Output && UAnimationGraphSchema::IsPosePin(PinWidgetPair.Key->PinType)
				&& PinWidgetPair.Key->GetOwningNode()->IsA<UAnimGraphNode_Base>()
				&& PinWidgetPair.Value.IsValid() && PinWidgetPair.Value->GetType() == TEXT("SGraphPinPose"))
			{
				const TSharedPtr<SGraphPinPose> PosePin = StaticCastSharedPtr<SGraphPinPose>(PinWidgetPair.Value);
				ENPinAttributes.Add(PinWidgetPair.Key, PosePin->GetAttributeInfo());

				if (!bFoundPanelZoom)
				{
					ENPanelZoom = PosePin->GetZoomAmount();
					bFoundPanelZoom = true;
				}
			}
		}
	}

	virtual void DrawConnection(int32 LayerId, const FVector2f& Start, const FVector2f& End, const FConnectionParams& Params) override
	{
		ENBuildAttributeSubWires(Params);

		this->ConnectionDrawingPolicy->SetMousePosition(LocalMousePosition);
		this->ConnectionDrawingPolicy->DrawConnection(LayerId, Start, End, Params);
		SplineOverlapResult = FGraphSplineOverlapResult(this->ConnectionDrawingPolicy->SplineOverlapResult);
	}

	~FENAnimGraphConnectionDrawingPolicy()
	{
		delete ConnectionDrawingPolicy;
	}

private:
	FENConnectionDrawingPolicy* ConnectionDrawingPolicy;
	TMap<UEdGraphPin*, TArrayView<const SGraphPinPose::FAttributeInfo>> ENPinAttributes;
	float ENPanelZoom = 1.0f;

	void ENBuildAttributeSubWires(const FConnectionParams& Params)
	{
		ConnectionDrawingPolicy->SubWires.Reset();

		if (!Params.AssociatedPin1 || !Params.AssociatedPin2 || !UAnimationGraphSchema::IsPosePin(Params.AssociatedPin1->PinType))
		{
			return;
		}

		UEdGraphPin* UsePin1 = Params.AssociatedPin1;
		if (Params.AssociatedPin1->GetOwningNode()->IsA<UK2Node_Knot>())
		{
			UsePin1 = FBlueprintEditorUtils::FindFirstCompilerRelevantLinkedPin(Params.AssociatedPin1);
		}

		UEdGraphPin* UsePin2 = Params.AssociatedPin2;
		if (Params.AssociatedPin2->GetOwningNode()->IsA<UK2Node_Knot>())
		{
			UsePin2 = FBlueprintEditorUtils::FindFirstCompilerRelevantLinkedPin(Params.AssociatedPin2);
		}

		if (!UsePin1 || !UsePin2)
		{
			return;
		}

		const UAnimGraphNode_Base* Node1 = Cast<UAnimGraphNode_Base>(UsePin1->GetOwningNode());
		const UAnimGraphNode_Base* Node2 = Cast<UAnimGraphNode_Base>(UsePin2->GetOwningNode());
		if (!Node1 || !Node2)
		{
			return;
		}

		const TArrayView<const SGraphPinPose::FAttributeInfo> AttributeInfos = ENPinAttributes.FindRef(UsePin1);
		if (AttributeInfos.Num() == 0)
		{
			return;
		}

		const float MaxAttributeWireThickness = 3.0f;
		const float MinAttributeWireThickness = 1.0f;
		const float MaxWireGap = 2.0f;
		const float MinWireGap = 0.5f;

		const float LowLODZoomThreshold = 0.375f;
		const float ZoomLevelAlpha = FMath::GetMappedRangeValueClamped(TRange<float>(LowLODZoomThreshold, 1.0f), TRange<float>(0.0f, 1.0f), ENPanelZoom);
		const float AttributeWireThickness = FMath::Lerp(MinAttributeWireThickness, MaxAttributeWireThickness, ZoomLevelAlpha);
		const float WireGap = FMath::Lerp(MinWireGap, MaxWireGap, ZoomLevelAlpha);

		const TArray<FAnimBlueprintDebugData::FAttributeRecord>* LinkAttributes = nullptr;
		int32 TargetNodeId = INDEX_NONE;
		if (UAnimBlueprint* AnimBlueprint = Cast<UAnimBlueprint>(FBlueprintEditorUtils::FindBlueprintForGraph(GraphObj)))
		{
			if (UAnimBlueprintGeneratedClass* AnimBlueprintClass = Cast<UAnimBlueprintGeneratedClass>(*AnimBlueprint->GeneratedClass))
			{
				const int32 SourceNodeId = AnimBlueprintClass->GetNodeIndexFromGuid(Node1->NodeGuid);
				TargetNodeId = AnimBlueprintClass->GetNodeIndexFromGuid(Node2->NodeGuid);
				if (SourceNodeId != INDEX_NONE && TargetNodeId != INDEX_NONE)
				{
					LinkAttributes = AnimBlueprintClass->GetAnimBlueprintDebugData().NodeOutputAttributesThisFrame.Find(SourceNodeId);
				}
			}
		}

		const float MainWireThickness = Params.WireThickness * GetDefault<UElectronicNodesSettings>()->WireThickness;
		float Cursor = MainWireThickness * 0.5f + WireGap;

		for (const SGraphPinPose::FAttributeInfo& AttributeInfo : AttributeInfos)
		{
			float Thickness = 0.0f;
			switch (AttributeInfo.Blend)
			{
			case EAnimGraphAttributeBlend::Blendable:
				if (LinkAttributes && LinkAttributes->ContainsByPredicate(
					[&AttributeInfo, TargetNodeId](const FAnimBlueprintDebugData::FAttributeRecord& InRecord)
					{
						return InRecord.Attribute == AttributeInfo.Attribute && InRecord.OtherNode == TargetNodeId;
					}))
				{
					Thickness = AttributeWireThickness;
				}
				break;
			case EAnimGraphAttributeBlend::NonBlendable:
				Thickness = AttributeWireThickness;
				break;
			}

			if (Thickness <= 0.0f)
			{
				continue;
			}

			FLinearColor Color = AttributeInfo.Color;
			if (HoveredPins.Num() > 0)
			{
				ApplyHoverDeemphasis(Params.AssociatedPin1, Params.AssociatedPin2, Thickness, Color);
			}

			ConnectionDrawingPolicy->SubWires.Add({Cursor + Thickness * 0.5f, Thickness, Color});
			Cursor += Thickness + WireGap;
		}
	}
};
