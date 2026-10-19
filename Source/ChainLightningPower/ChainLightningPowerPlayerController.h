// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ChainLightningPowerPlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;
class UAimingUI;

/**
 *  Basic PlayerController class for a third person game
 *  Manages input mappings
 */
UCLASS(abstract)
class AChainLightningPowerPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category ="Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> MobileExcludedMappingContexts;

	/** Mobile controls widget to spawn */
	UPROPERTY(EditAnywhere, Category="Input|Touch Controls")
	TSubclassOf<UUserWidget> MobileControlsWidgetClass;

	/** Pointer to the mobile controls widget */
	TObjectPtr<UUserWidget> MobileControlsWidget;

	/** Aiming UI widget to spawn (set in the controller Blueprint) */
	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UAimingUI> AimingUIClass;

	/** Pointer to the aiming UI widget */
	UPROPERTY(Transient)
	TObjectPtr<UAimingUI> AimingUI;

	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** Input mapping context setup */
	virtual void SetupInputComponent() override;

public:

	/** Adds the aiming UI to the viewport. Does nothing on non-local controllers */
	UFUNCTION(BlueprintCallable, Category="UI")
	void ShowAimingUI();

	/** Removes the aiming UI from the viewport */
	UFUNCTION(BlueprintCallable, Category="UI")
	void HideAimingUI();

};
