// Fill out your copyright notice in the Description page of Project Settings.

#include "Effluvium_UE4.h"
#include "Fluid_Array.h"
#include "Effluvium_UE4Projectile.h"


// Sets default values
AFluid_Array::AFluid_Array()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
  
}

// Called when the game starts or when spawned
void AFluid_Array::BeginPlay()
{
	Super::BeginPlay();

  //dimensions = FVector(30, 30, 12);
  int size = (dimensions.X + 1)*(dimensions.Y + 1);
  for (int i = 0; i < size; i++) 
  {
    density.Add(0.0f);
    prev_density.Add(0.0f);

    vx.Add(0.0f);
    vy.Add(0.0f);

    prev_vx.Add(0.0f);
    prev_vy.Add(0.0f);

  }

  // try and fire a projectile
  if (ProjectileClass != NULL)
  {
  UWorld* const World = GetWorld();
    if (World != NULL)
    {
      for (int i = 0; i < dimensions.X; i++) {
        for (int j = 0; j < dimensions.Y; j++) {

          FTransform trans = FTransform(FRotator::ZeroRotator, FVector(10, 10 + (i * 20), 200 + (j * 20)));
          AActor* t_ref= World->SpawnActor<AActor>(ProjectileClass, trans);
          refs.Add(t_ref);
        }
      }
    
    }
  }
	
}

// Called every frame
void AFluid_Array::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

  static int frameCounter;

  float dt = 1.0f / 30;
  int N = dimensions.X - 1;
    
  float* u = vx.GetData();
  float* v = vy.GetData();
  float* u_prev = prev_vx.GetData();
  float* v_prev = prev_vy.GetData();
  float* dens = density.GetData();
  float* dens_prev = prev_density.GetData();
  float visc = 0.0f;
  float diff = 0.0f;

  // STD::FILL 
  int size = (dimensions.X + 1)*(dimensions.Y + 1);
  for (int i = 0; i < size; i++) 
  {
    prev_density[i] = 0.0f;
    prev_vx[i] = 0.0f;
    prev_vy[i] = 0.0f;
  }

  float c = cos(frameCounter * 0.01f);
  float s = sin(frameCounter * 0.01f);

  int x = (int)dimensions.X*0.5f, y = (int)dimensions.X * 0.5f;
  density[IX(x,y,N)] += 3 * dt;
  u[IX(x,y, N)] += c * (10 * dt);
  v[IX(x,y,N)] += s * (10 * dt);

  velocity_step(N, u, v, u_prev, v_prev, visc, dt);
  density_step(N, dens, dens_prev, u, v, diff, dt);




  for (int i = 0; i < dimensions.X; i++) 
  {
    for (int j = 0; j < dimensions.Y; j++) 
    {

      TArray <UStaticMeshComponent *> Components;
      refs[i+(dimensions.X*j)]->GetComponents<UStaticMeshComponent>(Components);
      UMaterialInstanceDynamic* Mat = Components[0]->CreateDynamicMaterialInstance(0);
    
      //FLinearColor col = FLinearColor(density[i + (dimensions.X + 1)*j], 0, 0);
      FLinearColor col = FLinearColor(FMath::Max(0.0f,FMath::Min(density[i + (dimensions.X + 1)*j],1.0f)),0, 0, density[i + (dimensions.X + 1) * j]);
      Mat->SetVectorParameterValue("particleColor", col);
      
      
      //v.color = vec3p(std::max(0.0f, std::min(density[i + j*stride], 1.0f)), 0, 0);
      //std::max(0.0f, std::min(density[i + j*stride], 1.0f))
      //refs[i + (dimensions.X*j)]->SetActorRelativeRotation(FRotator(u[i + (int)(dimensions.X*j)],v[i + (int)(dimensions.X*j)],0));
      // Color** dens[i + (int)((dimensions.X+1)*j)]
      UE_LOG(LogTemp, Log, TEXT("Message"));
    }
  }


  //refs[0]->SetActorRelativeRotation(FRotator(0,0,test));

  frameCounter++;

}

void AFluid_Array::set_boundary(int N, int b, float * x) 
{

    for (int i = 1; i <= N; i++) {
      x[IX(0, i,N)] = b == 1 ? -x[IX(1, i,N)] : x[IX(1, i,N)];
      x[IX(N + 1, i,N)] = b == 1 ? -x[IX(N, i,N)] : x[IX(N, i,N)];
      x[IX(i, 0,N)] = b == 2 ? -x[IX(i, 1,N)] : x[IX(i, 1,N)];
      x[IX(i, N + 1,N)] = b == 2 ? -x[IX(i, N,N)] : x[IX(i, N,N)];
    }

    x[IX(0, 0,N)] = 0.5f*(x[IX(1, 0,N)] + x[IX(0, 1,N)]);
    x[IX(0, N + 1,N)] = 0.5f*(x[IX(1, N + 1,N)] + x[IX(0, N,N)]);
    x[IX(N + 1, 0,N)] = 0.5f*(x[IX(N, 0,N)] + x[IX(N + 1, 1,N)]);
    x[IX(N + 1, N + 1,N)] = 0.5f*(x[IX(N, N + 1,N)] + x[IX(N + 1, N,N)]);

}

//lin_solve in orginal
void AFluid_Array::gauss_siedel(int N, int b, float * x, float * x0, float a, float c) 
{

  for (int k = 0; k<20; k++) {
    for (int i = 1; i <= N; i++) {
      for (int j = 1; j <= N; j++) {
        x[IX(i,j,N)] = (x0[IX(i,j,N)] + a*(x[IX(i - 1, j,N)] + x[IX(i + 1, j,N)] + x[IX(i, j - 1,N)] + x[IX(i, j + 1,N)])) / c;
      }
    }
    set_boundary(N, b, x);
  }
}

void AFluid_Array::diffuse(int N, int b, float * x, float * x0, float diff, float dt) 
{
    float a = dt * diff * (N * N);
    gauss_siedel(N, b, x, x0, a, 1 + 4 * a);
}

void AFluid_Array::advect(int N, int b, float * d, float * d0, float * u, float * v, float dt) 
{

    float dt0 = dt*N;
    for (int i = 1; i <= N; i++) {
      for (int j = 1; j <= N; j++) {
        // (x, y) is the address to copy from
        float x = i - dt0*u[IX(i,j,N)], y = j - dt0*v[IX(i,j,N)];

        // clamp x and y
       if (x < 0.5f) x = 0.5f; else if (x > N + 0.5f) x = N + 0.5f;
       if (y < 0.5f) y = 0.5f; else if (y > N + 0.5f) y = N + 0.5f;

        // s1 and s0 are lerp coordinates [0,1) within the source cell
        int i0 = (int)x, i1 = i0 + 1;
        int j0 = (int)y, j1 = j0 + 1;
        float s1 = x - i0, s0 = 1 - s1;
        float t1 = y - j0, t0 = 1 - t1;

        // sample the source
        d[IX(i,j,N)] =
          s0*(t0*d0[IX(i0, j0,N)] + t1*d0[IX(i0, j1,N)]) +
          s1*(t0*d0[IX(i1, j0,N)] + t1*d0[IX(i1, j1,N)])
          ;
      }
    }
    set_boundary(N, b, d);
}

void AFluid_Array::project(int N, float * u, float * v, float * p, float * div) 
{
  // stablisation step. adjust the velocity to prevent increase in energy
  // in the system.

    // calculate divergence into div
    // set initial value of p
    for (int i = 1; i <= N; i++) {
      for (int j = 1; j <= N; j++) {
        div[IX(i,j,N)] = -0.5f*(u[IX(i + 1, j,N)] - u[IX(i - 1, j,N)] + v[IX(i, j + 1,N)] - v[IX(i, j - 1,N)]) / N;
        p[IX(i,j,N)] = 0;
      }
    }

    // copy pixels to boundary
    set_boundary(N, 0, div);
    set_boundary(N, 0, p);

    // p += div[x+/-1, y+/-1] * 4;
    gauss_siedel(N, 0, p, div, 1, 4);

    // calculate velocity from pressure-like "p"
    for (int i = 1; i <= N; i++) {
      for (int j = 1; j <= N; j++) {
        // u from left and right
        u[IX(i, j, N)] -= 0.5f*N*(p[IX(i + 1, j,N)] - p[IX(i - 1, j,N)]);

        // v from up and down.
        v[IX(i,j,N)] -= 0.5f*N*(p[IX(i, j + 1,N)] - p[IX(i, j - 1,N)]);
      }
    }

    // copy velocity to boundary
    set_boundary(N, 1, u);
    set_boundary(N, 2, v);
}

void AFluid_Array::density_step(int N, float * x, float * x0, float * u, float * v, float diff, float dt) 
{
  // apply diffusion to density. If there is no velocity, the value will still spread.  
  Swap(x0, x);
  //fSwap(x0, x);
  diffuse(N, 0, x, x0, diff, dt);

  // use the velocity field to carry density around.
  Swap(x0, x);
  //fSwap(x0, x);
  advect(N, 0, x, x0, u, v, dt);
}

void AFluid_Array::velocity_step(int N, float * u, float * v, float * u0, float * v0, float visc, float dt) 
{
  /// Compute the new velocity field.
    // diffuse into neighouring cells
    Swap(u0, u);
    diffuse(N, 1, u, u0, visc, dt);
    Swap(v0, v);
    diffuse(N, 2, v, v0, visc, dt);

    // stabilise the system using poisson
    project(N, u, v, u0, v0);
    Swap(u0, u);
    Swap(v0, v);

    // use advection to move the velocity itself
    advect(N, 1, u, u0, u0, v0, dt);
    advect(N, 2, v, v0, u0, v0, dt);

    // stabilise the system using poisson
    project(N, u, v, u0, v0);
}

int AFluid_Array::IX(int i, int j, int N) {
  return ((i) + (N + 2) * (j));
}