// Copyright Epic Games, Inc. All Rights Reserved.


#include "ChainLightningPowerPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "ChainLightningPower.h"
#include "UI/AimingUI.h"
#include "Widgets/Input/SVirtualJoystick.h"

void AChainLightningPowerPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// only spawn touch controls on local player controllers
	if (SVirtualJoystick::ShouldDisplayTouchInterface() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);
		} else {

			UE_LOG(LogChainLightningPower, Error, TEXT("Could not spawn mobile controls widget."));
		}
	}
}

void AChainLightningPowerPlayerController::ShowAimingUI()
{
	if (!IsLocalPlayerController() || !AimingUIClass)
	{
		return;
	}

	if (!AimingUI)
	{
		AimingUI = CreateWidget<UAimingUI>(this, AimingUIClass);
	}

	if (AimingUI && !AimingUI->IsInViewport())
	{
		AimingUI->AddToViewport();
	}
}

void AChainLightningPowerPlayerController::HideAimingUI()
{
	if (AimingUI && AimingUI->IsInViewport())
	{
		AimingUI->RemoveFromParent();
	}
}

void AChainLightningPowerPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!SVirtualJoystick::ShouldDisplayTouchInterface())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
}
