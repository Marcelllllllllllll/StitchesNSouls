/* Copyright (C) 2024 Hugo ATTAL - All Rights Reserved
* This plugin is downloadable from the Unreal Engine Marketplace
*/

#include "ENConnectionDrawingPolicy.h"
#include "ENPathDrawer.h"
#include "EdGraph/EdGraphSchema.h"
#include "SGraphPanel.h"
#include "Framework/Application/SlateApplication.h"
#include "Lib/Utils.h"
#include "MaterialGraph/MaterialGraphSchema.h"
#include "Policies/ENAnimGraphConnectionDrawingPolicy.h"
#include "Policies/ENBehaviorTreeConnectionDrawingPolicy.h"

FConnectionDrawingPolicy* FENConnectionDrawingPolicyFactory::CreateConnectionPolicy(
	const class UEdGraphSchema* Schema, int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor,
	const class FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements,
	class UEdGraph* InGraphObj) const
{
	const UElectronicNodesSettings& ElectronicNodesSettings = *GetDefault<UElectronicNodesSettings>();
	if (!ElectronicNodesSettings.MasterActivate)
	{
		return nullptr;
	}

	const FName ClassName = Schema->GetClass()->GetFName();

	if (ElectronicNodesSettings.DisplaySchemaName)
	{
		UE_LOG(LogTemp, Log, TEXT("[EN] %s"), *ClassName.ToString());
	}

	if (ElectronicNodesSettings.ActivateOnAnimation)
	{
		if (ClassName == "AnimationTransitionSchema")
		{
			return new FENAnimGraphConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect,
			                                               InDrawElements, InGraphObj);
		}

		if (ClassName == "AnimationGraphSchema" || ClassName == "AnimationStateGraphSchema")
		{
			return new FENAnimGraphConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect,
			                                               InDrawElements, InGraphObj);
		}
	}

	if (ElectronicNodesSettings.ActivateOnPCG && ClassName == "PCGEditorGraphSchema")
	{
		return new FENConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect,
		                                      InDrawElements, InGraphObj);
	}

	if (ElectronicNodesSettings.ActivateOnVoxelPlugin && ClassName == "VoxelGraphSchema")
	{
		return new FENConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect, InDrawElements,
		                                      InGraphObj);
	}

	if (ElectronicNodesSettings.ActivateOnNiagara && ClassName == "EdGraphSchema_Niagara")
	{
		return new FENConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect, InDrawElements,
		                                      InGraphObj);
	}

	if (ElectronicNodesSettings.ActivateOnBehaviorTree && ClassName == "EdGraphSchema_BehaviorTree")
	{
		return new FENBehaviorTreeConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect,
		                                                  InDrawElements, InGraphObj);
	}

	if (ElectronicNodesSettings.ActivateOnControlRig && ClassName == "ControlRigGraphSchema")
	{
		return new FENConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect, InDrawElements,
		                                      InGraphObj);
	}

	if (ElectronicNodesSettings.ActivateOnMetasound && ClassName == "MetasoundEditorGraphSchema")
	{
		return new FENConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect, InDrawElements,
		                                      InGraphObj);
	}

	if (ElectronicNodesSettings.ActivateOnReferenceViewer && ClassName == "ReferenceViewerSchema")
	{
		return new FENConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect, InDrawElements,
		                                      InGraphObj);
	}

	if (ElectronicNodesSettings.ActivateOnBlueprint && Schema->IsA(UEdGraphSchema_K2::StaticClass()))
	{
		return new FENConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect, InDrawElements,
		                                      InGraphObj);
	}

	if (ElectronicNodesSettings.ActivateOnMaterial && Schema->IsA(UMaterialGraphSchema::StaticClass()))
	{
		return new FENConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect, InDrawElements,
		                                      InGraphObj);
	}

	for (const auto& Type : ElectronicNodesSettings.CustomGraphSchemas)
	{
		if (Schema->IsA(Type))
		{
			return new FENConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect,
			                                      InDrawElements, InGraphObj);
		}
	}

	if (ElectronicNodesSettings.ActivateFallback)
	{
		return new FENConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect, InDrawElements,
		                                      InGraphObj);
	}

	return nullptr;
}

void FENConnectionDrawingPolicy::DetermineWiringStyle(UEdGraphPin* OutputPin, UEdGraphPin* InputPin, FConnectionParams& Params)
{
	FKismetConnectionDrawingPolicy::DetermineWiringStyle(OutputPin, InputPin, Params);

	if (OutputPin == nullptr)
	{
		return;
	}

	const UEdGraphSchema* Schema = OutputPin->GetSchema();
	if (Schema != nullptr && Schema->GetClass()->GetFName() == "PCGEditorGraphSchema")
	{
		Params.WireColor = Schema->GetPinColor(OutputPin);
	}
}

void FENConnectionDrawingPolicy::DrawConnection(int32 LayerId, const FVector2f& Start, const FVector2f& End,
                                                const FConnectionParams& Params)
{
	const bool RightPriority = ENIsRightPriority(Params);

	this->_LayerId = LayerId;
	this->_Params = &Params;
	ClosestDistanceSquared = MAX_FLT;
	bSliceLineIntersection = false;

	FENPathDrawer PathDrawer(LayerId, ZoomFactor, RightPriority, &Params, &DrawElementsList, this);
	FLinearColor WireColor = Params.WireColor;

	if (WireColor == FLinearColor::Black) // Fix for PCG Graph
	{
		WireColor = FLinearColor::White;
	}

	ENComputeBubbleState();

	FVector2f StartDirection = (Params.StartDirection == EGPD_Output) ? FVector2f(1.0f, 0.0f) : FVector2f(-1.0f, 0.0f);
	FVector2f EndDirection = (Params.EndDirection == EGPD_Input) ? FVector2f(1.0f, 0.0f) : FVector2f(-1.0f, 0.0f);

	if (IsTree)
	{
		StartDirection = FVector2f(0.0f, 1.0f);
		EndDirection = FVector2f(0.0f, 1.0f);
	}

	if (FVector2f::Distance(Start, End) < ElectronicNodesSettings.MinDistanceToStyle * ZoomFactor)
	{
		switch (ElectronicNodesSettings.MinDistanceStyle)
		{
		case EMinDistanceStyle::Line:
			PathDrawer.DrawLine(Start, End);
			break;
		case EMinDistanceStyle::Spline:
			PathDrawer.DrawDefaultWire(Start, StartDirection, End, EndDirection);
			break;
		}
	}
	else
	{
		FVector2f NewStart = Start;
		FVector2f NewEnd = End;

		ENCorrectZoomDisplacement(NewStart, NewEnd);
		ENProcessRibbon(&PathDrawer, _LayerId, NewStart, StartDirection, NewEnd, EndDirection, Params);

		EWireStyle WireStyle = ElectronicNodesSettings.WireStyle;

		if (ElectronicNodesSettings.OverwriteExecWireStyle && ENIsExecWire(Params)
			&& ElectronicNodesSettings.WireStyleForExec != EWireStyle::Default)
		{
			WireStyle = ElectronicNodesSettings.WireStyleForExec;
		}

		this->ENDrawMainWire(&PathDrawer, WireStyle, NewStart, StartDirection, NewEnd, EndDirection, Params);
	}

	if (Settings->bTreatSplinesLikePins)
	{
		const float QueryDistanceTriggerThresholdSquared = FMath::Square(
			Settings->SplineHoverTolerance + Params.WireThickness * 0.5f);
		const float QueryDistanceForCloseSquared = FMath::Square(
			FMath::Sqrt(QueryDistanceTriggerThresholdSquared) + Settings->SplineCloseTolerance);

		if (ClosestDistanceSquared < QueryDistanceTriggerThresholdSquared)
		{
			if (ClosestDistanceSquared < SplineOverlapResult.GetDistanceSquared())
			{
				const float SquaredDistToPin1 = (Params.AssociatedPin1 != nullptr)
					                                ? (Start - ClosestPoint).SizeSquared()
					                                : FLT_MAX;
				const float SquaredDistToPin2 = (Params.AssociatedPin2 != nullptr)
					                                ? (End - ClosestPoint).SizeSquared()
					                                : FLT_MAX;

				SplineOverlapResult = FGraphSplineOverlapResult(Params.AssociatedPin1, Params.AssociatedPin2,
				                                                ClosestDistanceSquared, SquaredDistToPin1,
				                                                SquaredDistToPin2, false);
			}
		}
		else if (ClosestDistanceSquared < QueryDistanceForCloseSquared)
		{
			SplineOverlapResult.SetCloseToSpline(true);
		}

		if (bSliceLineIntersection)
		{
			ConnectionsIntersectingSliceLine.Emplace(Params.AssociatedPin1, Params.AssociatedPin2);
			WireColor.A *= SliceDeemphasisAlphaMultiplier;
		}
	}

	for (int32 Index = SubWires.Num() - 1; Index >= 0; --Index)
	{
		const FENSubWire& SubWire = SubWires[Index];
		PathDrawer.DoDrawOffset(SubWire.Color, SubWire.Thickness, SubWire.Offset);
	}
	SubWires.Reset();

	PathDrawer.DoDraw(WireColor);
}

void FENConnectionDrawingPolicy::ENDrawMainWire(FENPathDrawer* PathDrawer, EWireStyle WireStyle, FVector2f& InStart,
                                                FVector2f& StartDirection, FVector2f& InEnd, FVector2f& EndDirection,
                                                const FConnectionParams& Params)
{
	const float Offset = ElectronicNodesSettings.HorizontalOffset * ZoomFactor;

	FVector2f Start = InStart;
	FVector2f End = InEnd;

	if (ElectronicNodesSettings.DisablePinOffset)
	{
		if (!((Params.AssociatedPin1 != nullptr) && (Params.AssociatedPin1->GetName() == "OutputPin")))
		{
			PathDrawer->DrawOffset(Start, StartDirection, Offset, false);
		}
		if (!((Params.AssociatedPin2 != nullptr) && (Params.AssociatedPin2->GetName() == "InputPin")))
		{
			PathDrawer->DrawOffset(End, EndDirection, Offset, true);
		}
	}
	else
	{
		PathDrawer->DrawOffset(Start, StartDirection, Offset, false);
		PathDrawer->DrawOffset(End, EndDirection, Offset, true);
	}

	switch (WireStyle)
	{
	case EWireStyle::Manhattan:
		PathDrawer->DrawManhattanWire(Start, StartDirection, End, EndDirection);
		break;
	case EWireStyle::Subway:
		PathDrawer->DrawSubwayWire(Start, StartDirection, End, EndDirection);
		break;
	default:
		PathDrawer->DrawDefaultWire(Start, StartDirection, End, EndDirection);
	}
}

void FENConnectionDrawingPolicy::ENCorrectZoomDisplacement(FVector2f& Start, FVector2f& End) const
{
	if (ElectronicNodesSettings.FixZoomDisplacement)
	{
		const float ZoomDisplacement = ZoomFactor * -19.0f + 8.0f;
		if (ZoomDisplacement > 0)
		{
			Start.X += ZoomDisplacement / 2.0f;
			End.X -= ZoomDisplacement / 2.0f;
		}
	}
}

void FENConnectionDrawingPolicy::ENProcessRibbon(FENPathDrawer* PathDrawer, int32 LayerId, FVector2f& Start,
                                                 FVector2f& StartDirection,
                                                 FVector2f& End, FVector2f& EndDirection,
                                                 const FConnectionParams& Params)
{
	int32 DepthOffsetX = 0;
	int32 DepthOffsetY = 0;

	if (ElectronicNodesSettings.ActivateRibbon && !IsTree)
	{
		const float RibbonOffset = ElectronicNodesSettings.RibbonOffset * ZoomFactor;

		for (const ENRibbonConnection& RibbonConnection : RibbonConnections)
		{
			if (RibbonConnection.Horizontal)
			{
				if (FMath::Abs(Start.Y - RibbonConnection.Main) < RibbonOffset)
				{
					const float CurrentMax = FMath::Max(Start.X, End.X);
					const float CurrentMin = FMath::Min(Start.X, End.X);
					const float RibbonMax = FMath::Max(RibbonConnection.Start, RibbonConnection.End);
					const float RibbonMin = FMath::Min(RibbonConnection.Start, RibbonConnection.End);

					if (this->IsRibbonEqual(RibbonMin, RibbonMax, CurrentMin, CurrentMax))
					{
						continue;
					}

					if (FMath::Min(CurrentMax, RibbonMax) > FMath::Max(CurrentMin, RibbonMin) - 1.0f)
					{
						if (End.Y - RibbonConnection.Sub > 0)
						{
							DepthOffsetY = FMath::Max(DepthOffsetY, FMath::Max(1, RibbonConnection.Depth + 1));
						}
						else
						{
							DepthOffsetY = FMath::Min(DepthOffsetY, FMath::Min(-1, RibbonConnection.Depth - 1));
						}
					}
				}
			}
		}
		for (const ENRibbonConnection& RibbonConnection : RibbonConnections)
		{
			if (!RibbonConnection.Horizontal)
			{
				if (FMath::Abs(End.X - RibbonConnection.Main) < RibbonOffset)
				{
					const float CurrentMax = FMath::Max(Start.Y, End.Y);
					const float CurrentMin = FMath::Min(Start.Y, End.Y);
					const float RibbonMax = FMath::Max(RibbonConnection.Start, RibbonConnection.End);
					const float RibbonMin = FMath::Min(RibbonConnection.Start, RibbonConnection.End);

					if (this->IsRibbonEqual(RibbonMin, RibbonMax, CurrentMin, CurrentMax))
					{
						continue;
					}

					if (FMath::Min(CurrentMax, RibbonMax) > FMath::Max(CurrentMin, RibbonMin) - 1.0f)
					{
						if ((Start.Y - RibbonConnection.Start) * FMath::Sign(End.Y - Start.Y) > 0 ||
							ElectronicNodesSettings.RibbonPushOutside)
						{
							DepthOffsetX = FMath::Max(DepthOffsetX, FMath::Max(1, RibbonConnection.Depth + 1));
						}
						else
						{
							DepthOffsetX = FMath::Min(DepthOffsetX, FMath::Min(-1, RibbonConnection.Depth - 1));
						}

						if (DepthOffsetY != 0)
						{
							DepthOffsetX = FMath::Sign(End.Y - Start.Y) * DepthOffsetY;
						}
					}
				}
			}
		}

		if (DepthOffsetY != 0)
		{
			FVector2f NewStart = Start;
			NewStart.X += ElectronicNodesSettings.RibbonMergeOffset * ZoomFactor * StartDirection.X;
			NewStart.Y += RibbonOffset * DepthOffsetY;

			PathDrawer->DrawManhattanWire(Start, StartDirection, NewStart, StartDirection);

			Start = NewStart;
		}

		if (DepthOffsetX != 0)
		{
			FVector2f NewEnd = End;
			NewEnd.X -= RibbonOffset * DepthOffsetX * EndDirection.X;
			
			PathDrawer->DrawManhattanWire(NewEnd, EndDirection, End, EndDirection);
			
			End = NewEnd;
		}

		RibbonConnections.Add(ENRibbonConnection(Start.Y, End.Y, true, Start.X, End.X, DepthOffsetY));
		RibbonConnections.Add(ENRibbonConnection(End.X, Start.X, false, Start.Y, End.Y, DepthOffsetX));
	}
}

bool FENConnectionDrawingPolicy::ENIsRightPriority(const FConnectionParams& Params)
{
	bool RightPriority = (ElectronicNodesSettings.WireAlignment == EWireAlignment::Right);
	EWirePriority WirePriority = ElectronicNodesSettings.WirePriority;

	if (ElectronicNodesSettings.OverwriteExecWireStyle && ENIsExecWire(Params))
	{
		RightPriority = (ElectronicNodesSettings.WireAlignmentForExec == EWireAlignment::Right);
		WirePriority = ElectronicNodesSettings.WirePriorityForExec;
	}

	if (WirePriority != EWirePriority::None)
	{
		if ((Params.AssociatedPin1 != nullptr) && (Params.AssociatedPin2 != nullptr))
		{
			const bool IsOutputPin = (Params.AssociatedPin1->GetName() == "OutputPin");
			const bool IsInputPin = (Params.AssociatedPin2->GetName() == "InputPin");
			if (IsOutputPin ^ IsInputPin)
			{
				switch (WirePriority)
				{
				case EWirePriority::Node:
					RightPriority = IsOutputPin;
					break;
				case EWirePriority::Pin:
					RightPriority = IsInputPin;
					break;
				default:
					break;
				}
			}
		}
	}

	return RightPriority;
}

bool FENConnectionDrawingPolicy::ENIsExecWire(const FConnectionParams& Params)
{
	return ((Params.AssociatedPin1 != nullptr) && Params.AssociatedPin1->PinType.PinCategory == UEdGraphSchema_K2::PC_Exec) ||
		((Params.AssociatedPin2 != nullptr) && Params.AssociatedPin2->PinType.PinCategory == UEdGraphSchema_K2::PC_Exec);
}

int32 FENConnectionDrawingPolicy::ENGetZoomLevel() const
{
	const float ZoomLevels[] = {
		2.0f, 1.875f, 1.75f, 1.675f, 1.5f, 1.375f, 1.25f, 1.0f, 0.875f, 0.75f, 0.675f, 0.5f, 0.375f, 0.25f, 0.225f,
		0.2f, 0.175f, 0.15f, 0.125f, 0.1f
	};

	for (int32 i = 0; i < static_cast<int32>(UE_ARRAY_COUNT(ZoomLevels)); i++)
	{
		if (ZoomFactor > ZoomLevels[i] - KINDA_SMALL_NUMBER)
		{
			return 7 - i;
		}
	}

	return -12;
}

TSharedPtr<SGraphPanel> FENConnectionDrawingPolicy::GetGraphPanel()
{
	FSlateApplication& SlateApplication = FSlateApplication::Get();
	const TSharedPtr<SWidget> Widget = SlateApplication.GetUserFocusedWidget(0);
	if (Widget.IsValid() && Widget->GetTypeAsString() == "SGraphPanel")
	{
		return StaticCastSharedPtr<SGraphPanel>(Widget);
	}
	return nullptr;
}

void FENConnectionDrawingPolicy::BuildRelatedNodes(UEdGraphNode* Node, TArray<UEdGraphNode*>& RelatedNodes,
                                                   bool InputCheck = true, bool OutputCheck = true)
{
	if (RelatedNodes.Find(Node) != INDEX_NONE && (!InputCheck || !OutputCheck))
	{
		return;
	}
	RelatedNodes.Push(Node);

	for (auto Pin : Node->Pins)
	{
		if (InputCheck && Pin->Direction == EGPD_Input)
		{
			for (auto LinkedPin : Pin->LinkedTo)
			{
				UEdGraphNode* LinkedNode = LinkedPin->GetOwningNode();
				if (ElectronicNodesSettings.SelectionRule == ESelectionRule::Far || LinkedNode->GetName().StartsWith(
					"K2Node_Knot_"))
				{
					BuildRelatedNodes(LinkedNode, RelatedNodes, true, false);
				}
				else
				{
					RelatedNodes.Push(LinkedNode);
				}
			}
		}

		if (OutputCheck && Pin->Direction == EGPD_Output)
		{
			for (auto LinkedPin : Pin->LinkedTo)
			{
				UEdGraphNode* LinkedNode = LinkedPin->GetOwningNode();
				if (ElectronicNodesSettings.SelectionRule == ESelectionRule::Far || LinkedNode->GetName().StartsWith(
					"K2Node_Knot_"))
				{
					BuildRelatedNodes(LinkedNode, RelatedNodes, false, true);
				}
				else
				{
					RelatedNodes.Push(LinkedNode);
				}
			}
		}
	}
}

void FENConnectionDrawingPolicy::ENComputeBubbleState()
{
	const bool ForcedBubbles = ElectronicNodesSettings.ForceDrawBubbles
		&& (ElectronicNodesSettings.BubbleZoomThreshold <= ENGetZoomLevel())
		&& (!ElectronicNodesSettings.DrawBubblesOnlyOnExec || ENIsExecWire(*_Params));

	_BubblesEnabled = _Params->bDrawBubbles || ForcedBubbles;
	_BubblesLinked = true;

	if (!_BubblesEnabled || _Params->bDrawBubbles)
	{
		return;
	}

	_BubblesLinked = false;

	if (ElectronicNodesSettings.BubbleDisplayRule == EBubbleDisplayRule::DisplayOnSelection ||
		ElectronicNodesSettings.BubbleDisplayRule == EBubbleDisplayRule::MoveOnSelection)
	{
		const TSharedPtr<SGraphPanel> GraphPanel = this->GetGraphPanel();
		if (GraphPanel.IsValid() && _Params->AssociatedPin1 != nullptr && _Params->AssociatedPin2 != nullptr)
		{
			for (UObject* SelectedNode : GraphPanel->SelectionManager.SelectedNodes)
			{
				UEdGraphNode* SelectedGraphNode = Cast<UEdGraphNode>(SelectedNode);
				if (SelectedGraphNode == nullptr)
				{
					continue;
				}

				TArray<UEdGraphNode*> RelatedNodes;
				this->BuildRelatedNodes(SelectedGraphNode, RelatedNodes);

				if (RelatedNodes.Contains(_Params->AssociatedPin1->GetOwningNode()) &&
					RelatedNodes.Contains(_Params->AssociatedPin2->GetOwningNode()))
				{
					_BubblesLinked = true;
					break;
				}
			}
		}
	}

	if (!_BubblesLinked && ElectronicNodesSettings.BubbleDisplayRule == EBubbleDisplayRule::DisplayOnSelection)
	{
		_BubblesEnabled = false;
	}
}

void FENConnectionDrawingPolicy::ENDrawBubbles(const FVector2f& Start, const FVector2f& StartTangent,
                                               const FVector2f& End, const FVector2f& EndTangent)
{
	if (_BubblesEnabled)
	{
		const bool LinkedBubbles = _BubblesLinked;

		const float SplineLength = (Start - End).Size();
		int32 NumBubbles = FMath::CeilToInt(SplineLength / (ElectronicNodesSettings.BubbleSpace * ZoomFactor));
		NumBubbles = FMath::Min(NumBubbles, 1000);

		float BubbleSpeed = ElectronicNodesSettings.BubbleSpeed;
		if (!LinkedBubbles && ElectronicNodesSettings.BubbleDisplayRule == EBubbleDisplayRule::MoveOnSelection)
		{
			BubbleSpeed = 0.0f;
		}
		FVector2f BubbleSize = BubbleImage->ImageSize * ZoomFactor * 0.1f * ElectronicNodesSettings.BubbleSize *
			FMath::Sqrt(_Params->WireThickness);
		if (_Params->bDrawBubbles)
		{
			BubbleSize *= 1.25f;
		}
		const float Time = (FPlatformTime::Seconds() - GStartTime);

		const float AlphaOffset = FMath::Frac(Time * BubbleSpeed);

		for (int32 i = 0; i < NumBubbles; ++i)
		{
			const float Alpha = (AlphaOffset + i) / NumBubbles;
			FVector2f BubblePos;
			if (StartTangent != FVector2f::ZeroVector && EndTangent != FVector2f::ZeroVector)
			{
				if ((StartTangent != EndTangent) && ((StartTangent * EndTangent) == FVector2f::ZeroVector))
				{
					BubblePos = Start + StartTangent * FMath::Sin(Alpha * PI / 2.0f) + EndTangent * (1.0f - FMath::Cos(
						Alpha * PI / 2.0f));
				}
				else
				{
					BubblePos = FMath::CubicInterp(Start, StartTangent, End, EndTangent, Alpha);
				}
			}
			else
			{
				BubblePos = FMath::Lerp(Start, End, Alpha);
			}
			BubblePos -= (BubbleSize * 0.5f);

			FSlateDrawElement::MakeBox(
				DrawElementsList,
				_LayerId,
				FPaintGeometry(BubblePos, BubbleSize, ZoomFactor),
				BubbleImage,
				ESlateDrawEffect::None,
				_Params->WireColor
			);
		}
	}
}

void FENConnectionDrawingPolicy::ENDrawArrow(const FVector2f& Start, const FVector2f& End)
{
	if (MidpointImage != nullptr)
	{
		const FVector2f MidpointDrawPos = (Start + End) / 2.0f - MidpointRadius * 0.75f;
		const FVector2f SlopeUnnormalized = (End - Start);
		const float AngleInRadians = FMath::Atan2(SlopeUnnormalized.Y, SlopeUnnormalized.X);

		FSlateDrawElement::MakeRotatedBox(DrawElementsList, _LayerId,
		                                  FPaintGeometry(MidpointDrawPos, MidpointImage->ImageSize * ZoomFactor * 0.75f,
		                                                 ZoomFactor * 0.75f),
		                                  MidpointImage, ESlateDrawEffect::None, AngleInRadians, TOptional<FVector2f>(),
		                                  FSlateDrawElement::RelativeToElement, _Params->WireColor);
	}
}

void FENConnectionDrawingPolicy::DrawDebugPoint(const FVector2f& Position, FLinearColor Color)
{
	const FVector2f BubbleSize = BubbleImage->ImageSize * ZoomFactor * 0.1f * ElectronicNodesSettings.BubbleSize *
		FMath::Sqrt(_Params->WireThickness);
	const FVector2f BubblePos = Position - (BubbleSize * 0.5f);

	FSlateDrawElement::MakeBox(
		DrawElementsList,
		_LayerId,
		FPaintGeometry(BubblePos, BubbleSize, ZoomFactor),
		BubbleImage,
		ESlateDrawEffect::None,
		Color
	);
}

bool FENConnectionDrawingPolicy::IsRibbonEqual(const float RibbonAMin, const float RibbonAMax, const float RibbonBMin,
                                               const float RibbonBMax)
{
	return FMath::IsNearlyEqual(RibbonAMin, RibbonBMin, KINDA_SMALL_NUMBER) &&
		FMath::IsNearlyEqual(RibbonAMax, RibbonBMax, KINDA_SMALL_NUMBER);
}

void FENConnectionDrawingPolicy::MakeDrawSpline(FSlateWindowElementList& ElementList, uint32 InLayer,
                                                const FVector2D InStart, const FVector2D InStartDir,
                                                const FVector2D InEnd, const FVector2D InEndDir,
                                                float InThickness, ESlateDrawEffect InDrawEffects,
                                                const FLinearColor& InTint)
{
	FSlateDrawElement::MakeDrawSpaceSpline(ElementList, InLayer,
	                                       InStart, InStartDir, InEnd, InEndDir,
	                                       InThickness, InDrawEffects, InTint);
}

void FENConnectionDrawingPolicy::ENComputeClosestPoint(const FVector2f& Start, const FVector2f& End)
{
	const FVector2f TemporaryPoint = FMath::ClosestPointOnSegment2D(LocalMousePosition, Start, End);
	const float TemporaryDistance = (LocalMousePosition - TemporaryPoint).SizeSquared();

	if (TemporaryDistance < ClosestDistanceSquared)
	{
		ClosestDistanceSquared = TemporaryDistance;
		ClosestPoint = TemporaryPoint;
	}

	if (SliceLine.IsValid())
	{
		FVector2f OutIntersectionPoint;
		const bool bIntersection = ENIntersectionHelpers::SegmentIntersection2D(
			SliceLine.StartPoint, SliceLine.EndPoint, Start, End, OutIntersectionPoint);
		if (bIntersection)
		{
			bSliceLineIntersection = true;
		}
	}
}

void FENConnectionDrawingPolicy::ENComputeClosestPointDefault(const FVector2f& Start, const FVector2f& StartTangent,
                                                              const FVector2f& End, const FVector2f& EndTangent)
{
	const float Offset = 50.0 * ZoomFactor;
	const FVector2f MinStart = FVector2f(FMath::Min(Start.X, End.X) - Offset, FMath::Min(Start.Y, End.Y));
	const FVector2f MaxEnd = FVector2f(FMath::Max(Start.X, End.X) + Offset, FMath::Max(Start.Y, End.Y));

	const FBox2f Bounds(MinStart, MaxEnd);
	const bool bCloseToSpline = Bounds.ComputeSquaredDistanceToPoint(LocalMousePosition) < 1.0f;
	const bool bCheckSlice = SliceLine.IsValid() && !bSliceLineIntersection;

	if (!bCloseToSpline && !bCheckSlice)
	{
		return;
	}

	constexpr int32 NumSteps = 16;
	FVector2f PointOnSpline1 = Start;
	for (int32 Step = 1; Step <= NumSteps; Step++)
	{
		const float Alpha = static_cast<float>(Step) / NumSteps;
		const FVector2f PointOnSpline2 = FMath::CubicInterp(Start, StartTangent, End, EndTangent, Alpha);

		if (bCloseToSpline)
		{
			const FVector2f ClosestPointToSegment = FMath::ClosestPointOnSegment2D(
				LocalMousePosition, PointOnSpline1, PointOnSpline2);
			const float DistanceSquared = (LocalMousePosition - ClosestPointToSegment).SizeSquared();

			if (DistanceSquared < ClosestDistanceSquared)
			{
				ClosestDistanceSquared = DistanceSquared;
				ClosestPoint = ClosestPointToSegment;
			}
		}

		if (bCheckSlice && !bSliceLineIntersection)
		{
			FVector2f OutIntersectionPoint;
			if (ENIntersectionHelpers::SegmentIntersection2D(SliceLine.StartPoint, SliceLine.EndPoint,
			                                                 PointOnSpline1, PointOnSpline2, OutIntersectionPoint))
			{
				bSliceLineIntersection = true;
			}
		}

		PointOnSpline1 = PointOnSpline2;
	}
}
