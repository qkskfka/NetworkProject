// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "NetworkProjectCharacter.generated.h"


UCLASS(config=Game)
class ANetworkProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	UPROPERTY(VisibleAnywhere)
	class UMainWidget* playerInfoUI;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

public:
	ANetworkProjectCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Fire();
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(Server, Unreliable, WithValidation)
	void ServerFire(int32 damage);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastFire(int32 damage);

	UFUNCTION(Client, Unreliable)
	void ClientFire(int32 damage);

	UPROPERTY(EditDefaultsOnly, Category = MySettings)
	TSubclassOf<class ABulletActor> BulletFactory;

	UPROPERTY(EditDefaultsOnly,Category = MySettings)
	int32 maxHP = 100;

	UPROPERTY(EditDefaultsOnly,Replicated,Category = MySettings)
	int32 curHP;

	UPROPERTY(EditDefaultsOnly,Replicated, Category = MySettings)
	int32 ammo;

	UPROPERTY(VisibleDefaultsOnly,Category = MySettings)
	class UPlayerInfoWidget* infoWidget;

	UFUNCTION()
	void SetHealth(int32 value);

	UFUNCTION()
	void AddHealth(int32 value);

	UFUNCTION(Server, Unreliable)
	void DamageProcess(int32 value);

	FORCEINLINE int32 GetHealth() { return curHP; };
	FORCEINLINE int32 GetAmmo() { return ammo; };

private:
	FString PrintInfo();

	int32 number;

	UPROPERTY(Replicated)
	int32 repnumber;

	UPROPERTY(Replicated)
	FString myName;

};

