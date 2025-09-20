// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "TPUnidad1GudinoCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)

//la clase hereda de ACharacter por lo que tiene movimiento, colisión, mesh y cámara 

class ATPUnidad1GudinoCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	//componente que posiciona la camara detras del character
	// VisibleAnywhere: se puede ver en editor.
	// BlueprintReadOnly: no editable desde Blueprint.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	//cámara que sigue al character, se adjunta al cameraboom
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
protected:

	/** Jump Input Action */
	//acción de input para saltar.Editable en el editor
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	//acción de input para moverse.Se puede asignar desde bp o editor
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	//acción de input para mirar (camara)
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	//acción de input para mirar con el mouse
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MostrarMensajeAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SaltoAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* AgacharseAction;

public:

	/** Constructor */
	//constructor de la clase. Se usa para inicializar componentes y valores por defecto
	ATPUnidad1GudinoCharacter();	

protected:

	/** Initialize input action bindings */
	// función que se llama para registrar las acciones de input.
	// override: reemplaza la función de ACharacter.
	// se asignan las funciones Move(), Look(), Jump()
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// funciones que se van a ejecutar con los inputs
	void MostrarMensaje();
	void IniciarSalto();
	void TerminarSalto();
	void Agacharse();

protected:

	/** Called for movement input */
	// función llamada cada frame cuando hay input de movimiento
	// el parámetro Value contiene la dirección de movimiento
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	
	// función llamada cada frame cuando hay input de cámara
	// el parámetro Value contiene los ejes de rotación
	void Look(const FInputActionValue& Value);

public:

	/** Handles move inputs from either controls or UI interfaces */
	//funcion que aplica el movimiento del character. Se puede llamar desde blueprint. Los parámetros right y foward controlan la dirección
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	//funcion que aplica la rotación del character. Se puede llamar desde blueprint. Los parámetros yawn y pitch controlan la rotación horizontal y vertical
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	//funcion para saltar. Se puede usar desde blueprint
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	//funcion para terminar el salto
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();



public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

