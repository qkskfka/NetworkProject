// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
// Sets default values
ABulletActor::ABulletActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	sphereComp->SetSphereRadius(15.0f);
	sphereComp->SetCollisionProfileName(FName("BulletPreset"));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetWorldScale3D(FVector(0.3f));
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ABulletActor::BeginPlay()
{
	Super::BeginPlay();
	
	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABulletActor::OnOverlap);
}

// Called every frame
void ABulletActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + GetActorForwardVector() * moveSpeed * DeltaTime);

	FString ownerName = GetOwner() != nullptr ? GetOwner()->GetName() : TEXT("NoOwner");
	DrawDebugString(GetWorld(), GetActorLocation(), ownerName, nullptr, FColor::White, true);
}

void ABulletActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 부딫히면 불꽃 이펙트를 출력한 다음 제거한다
	
	ServerSpawnEffect();
}

void ABulletActor::ServerSpawnEffect_Implementation()
{
	MulticastSpawnEffect();
}

void ABulletActor::MulticastSpawnEffect_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString("Spawn Effect!"), true,
		FVector2D(1.2f));

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosion,
		GetActorLocation(), FRotator::ZeroRotator, true);
	//Destroy();
}