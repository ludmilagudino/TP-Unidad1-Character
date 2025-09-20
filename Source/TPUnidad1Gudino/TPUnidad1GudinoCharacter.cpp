// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPUnidad1GudinoCharacter.h"
#include "Engine/LocalPlayer.h"
//usa el componente de cámara y la capsula para la colision
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
//añade la movilidad del personaje
#include "GameFramework/CharacterMovementComponent.h"
//componente del brazo de la camara
#include "GameFramework/SpringArmComponent.h"
//accede al controller que maneja el personaje
#include "GameFramework/Controller.h"
//maneja los inputs de accion que va a tener el personaje
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
//el header del character
#include "TPUnidad1Gudino.h"

ATPUnidad1GudinoCharacter::ATPUnidad1GudinoCharacter()
{
	// Set size for collision capsule
	//configura el tamaño de la capsula de colision. Si se cambian los valores, afecta a la colisión y que tan grande va a ser el character
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	//evita que se rote el personaje cuando se mueva la camara, la camara sigue al controller
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	//Hace que el character rote en la direccion del movimiento. Si esta en false, el character puede caminar de espaldas o de costado sin rotar
	GetCharacterMovement()->bOrientRotationToMovement = true;

	//Es la velocidad de rotación cuando el character se gira a donde le dice el movimiento. Cambiando el valor, gira más rápido o más lento
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them

	//Configura la velocidad del salto en el eje Z. Si se aumenta el valor salta mas alto
	GetCharacterMovement()->JumpZVelocity = 500.f;

	//Configura el control que tiene el jugador en el aire.Cuanto más cerca de 1, más control tiene
	GetCharacterMovement()->AirControl = 0.35f;

	//Configura la velocidad máxima al caminar
	GetCharacterMovement()->MaxWalkSpeed = 500.f;

	//Configura la velocidad mínima al caminar cuando se mueve con joystick, para que tenga un movimiento suave
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;

	//Configura la deceleración cuando frena la caminata del character. Cuánto más alto el valor, más rápido va a frenar
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	//Configura la deceleración al caer. Cuánto más alto el valor, más rápido va a frenar cuando aterrice.
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)

	//Crea el brazo de la cámara que mantiene la distancia y evita que pueda atravesar objetos
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));

	//Adjunta el brazo al root del character para que siga los movimientos
	CameraBoom->SetupAttachment(RootComponent);

	//Es la distancia del brazo de la cámara con respecto al character. Cambiar el valor acerca o aleja la cámara
	CameraBoom->TargetArmLength = 400.0f;

	//Hace que el brazo siga la rotación del controller, para que la cámara mire hacia donde mire el jugador
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	//Crea la camara que sigue al character
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));

	//Ajunta la cámara al extremo del brazo
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	//Hace que la cámara no rote con el controller, porque el brazo ya maneja la rotación
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ATPUnidad1GudinoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{


	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		//cuando se inicia la accion de salto, llama a la funcion Jump()
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);

		//cuando se termina la accion de salto, llama a la funcion StopJumping()
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		//cuando el input de movimiento esta activo, llama a la funcion Move()
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATPUnidad1GudinoCharacter::Move);

		//cuando se mueve el mouse o el joystick para mirar, llama a la funcion Look()
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ATPUnidad1GudinoCharacter::Look);

		// Looking
		//cuando se activa por teclado la acción de mirar , llama a la funcion Look()
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATPUnidad1GudinoCharacter::Look);


		// cuando se presiona la tecla asignada, muestra mensaje en pantalla
		EnhancedInputComponent->BindAction(MostrarMensajeAction, ETriggerEvent::Started, this, &ATPUnidad1GudinoCharacter::MostrarMensaje);

		// cuando se presiona la tecla de salto, inicia Jump()
		EnhancedInputComponent->BindAction(SaltoAction, ETriggerEvent::Started, this, &ATPUnidad1GudinoCharacter::IniciarSalto);

		// cuando se suelta la tecla de salto, termina Jump()
		EnhancedInputComponent->BindAction(SaltoAction, ETriggerEvent::Completed, this, &ATPUnidad1GudinoCharacter::TerminarSalto);

		// cuando se presiona la tecla de agacharse, ejecuta Crouch()
		EnhancedInputComponent->BindAction(AgacharseAction, ETriggerEvent::Started, this, &ATPUnidad1GudinoCharacter::Agacharse);

	}
	else
	{
		// si no encuentra EnhancedInputComponent, muestra error en log
		UE_LOG(LogTPUnidad1Gudino, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ATPUnidad1GudinoCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	//obtiene los valores de movimiento horizontal y vertical del input
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	//llama a DoMove() con los valores X e Y para mover el character
	DoMove(MovementVector.X, MovementVector.Y);
}

void ATPUnidad1GudinoCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	//obtiene los valores de mirada horizontal y vertical 
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	//llama a DoLook() con los valores X e Y para rotar la cámara y el character
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ATPUnidad1GudinoCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		//obtiene la rotacion actual de donde mira el jugador (controller)
		const FRotator Rotation = GetController()->GetControlRotation();
		//solo considera la rotacion del yaw para mover el character horizontalmente
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		//calcula el vector de avance en base a la rotacion del character
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		//calcula el vector de lateral derecho
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		//mueve el character hacia adelante o atrás según el foward
		AddMovementInput(ForwardDirection, Forward);
		//mueve el character lateralmente según right
		AddMovementInput(RightDirection, Right);
	}
}

void ATPUnidad1GudinoCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		// 
		//agrega rotacion horizontal al controller
		AddControllerYawInput(Yaw);

		//agrega rotacion vertical al controller
		AddControllerPitchInput(Pitch);
	}
}

void ATPUnidad1GudinoCharacter::DoJumpStart()
{
	// signal the character to jump
	//le indica al character que salte
	Jump();
}

void ATPUnidad1GudinoCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	//le indica al character que deje de saltar
	StopJumping();
}

// acción que muestra un mensaje en pantalla
void ATPUnidad1GudinoCharacter::MostrarMensaje()
{
	if (GEngine)
	{
		// muestra un mensaje de debug verde durante 2 segundos
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("¡Acción ejecutada: Mostrar Mensaje!"));
	}
}

// función que inicia el salto
void ATPUnidad1GudinoCharacter::IniciarSalto()
{
	Jump(); // función propia de ACharacter
}

// Función que termina el salto
void ATPUnidad1GudinoCharacter::TerminarSalto()
{
	StopJumping(); // función propia de ACharacter
}

// función que hace que el personaje se agache
void ATPUnidad1GudinoCharacter::Agacharse()
{
	Crouch(); // función propia de ACharacter
}