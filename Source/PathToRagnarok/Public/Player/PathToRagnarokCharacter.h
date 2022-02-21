#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PathToRagnarokCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UDecalComponent;
class UInventoryComponent;

UCLASS()
class APathToRagnarokCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* InventoryComponent;

public:
	APathToRagnarokCharacter();

	FORCEINLINE UCameraComponent* GetCamera() const { return CameraComponent; }

	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE UInventoryComponent* GetInventory() const { return InventoryComponent; };

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

private:
	void MoveForward(float Value);

	void MoveRight(float Value);
};

