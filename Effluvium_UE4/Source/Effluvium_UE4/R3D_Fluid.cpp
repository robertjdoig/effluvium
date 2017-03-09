// Fill out your copyright notice in the Description page of Project Settings.

#include "Effluvium_UE4.h"
#include "R3D_Fluid.h"
#include "Effluvium_UE4Projectile.h"
#include "MaterialBillboardComponent.generated.h"

// Sets default values
AR3D_Fluid::AR3D_Fluid()
{
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AR3D_Fluid::BeginPlay()
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

          FTransform trans = world_trans + FTransform(FRotator::ZeroRotator, FVector(10, 10 + (i * (object_size.X*2)), 0 + (j * (object_size.Y*2))),object_size);
          AActor* t_ref = World->SpawnActor<AActor>(ProjectileClass, trans);
          refs.Add(t_ref);

          TArray <UStaticMeshComponent *> Components;
          t_ref->GetComponents<UStaticMeshComponent>(Components);
              
          UMaterialInstanceDynamic* Mat = Components[0]->CreateDynamicMaterialInstance(0);
          materials.Add(Mat);

        }
      }

    }
  }

}

// Called every frame
void AR3D_Fluid::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

  static int frameCounter;

  float dt = 1.0f / 30;
  int N = dimensions.X - 1;

  FVector dim = FVector(dimensions.X - 1, dimensions.Y - 1, dimensions.Z -1); 

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

  int x = (int)dimensions.X*0.5f, y = (int)dimensions.Y * 0.5f;
  density[IX(x, y, dim)] += density_amount * dt;
  u[IX(x, y, dim)] += c * (vx_speed * dt);
  v[IX(x, y, dim)] += s * (vy_speed * dt);

  velocity_step(dim, u, v, u_prev, v_prev, visc, dt);
  density_step(dim, dens, dens_prev, u, v, diff, dt);


  for (int i = 0; i < dimensions.X; i++)
  {
    for (int j = 0; j < dimensions.Y; j++)
    {
      FLinearColor col = FLinearColor(FMath::Max(0.0f, FMath::Min(density[i + (dimensions.X + 1)*j], 1.0f)), 0, 0, density[i + (dimensions.X + 1) * j]);
      materials[i + (dimensions.X*j)]->SetVectorParameterValue("particleColor", col);

      //UE_LOG(LogTemp, Log, TEXT("Message"));
    }
  }


  //refs[0]->SetActorRelativeRotation(FRotator(0,0,test));

  frameCounter++;

}


void AR3D_Fluid::set_boundary(FVector dim, int b, float * x)
{

  for (int i = 1; i <= dim.X; i++) {
    x[IX(0, i, dim)] = b == 1 ? -x[IX(1, i, dim)] : x[IX(1, i, dim)];
    x[IX(dim.X + 1, i, dim)] = b == 1 ? -x[IX(dim.X, i, dim)] : x[IX(dim.X, i, dim)];
  
  
    x[IX(i, 0, dim)] = b == 2 ? -x[IX(i, 1, dim)] : x[IX(i, 1, dim)];
    x[IX(i, dim.Y + 1, dim)] = b == 2 ? -x[IX(i, dim.Y, dim)] : x[IX(i, dim.Y, dim)];
  }

  x[IX(0, 0, dim)] = 0.5f*(x[IX(1, 0, dim)] + x[IX(0, 1, dim)]);
  x[IX(0, dim.Y + 1, dim)] = 0.5f*(x[IX(1, dim.Y + 1, dim)] + x[IX(0, dim.Y, dim)]);
  x[IX(dim.X + 1, 0, dim)] = 0.5f*(x[IX(dim.X, 0, dim)] + x[IX(dim.X + 1, 1, dim)]);
  x[IX(dim.X + 1, dim.Y + 1, dim)] = 0.5f*(x[IX(dim.X, dim.Y + 1, dim)] + x[IX(dim.X + 1, dim.Y, dim)]);

}

//lin_solve in orginal
void AR3D_Fluid::gauss_siedel(FVector dim, int b, float * x, float * x0, float a, float c)
{

  for (int k = 0; k<20; k++) {
    for (int i = 1; i <= dim.X; i++) {
      for (int j = 1; j <= dim.Y; j++) {
        x[IX(i, j, dim)] = (x0[IX(i, j, dim)] + a*(x[IX(i - 1, j, dim)] + x[IX(i + 1, j, dim)] + x[IX(i, j - 1, dim)] + x[IX(i, j + 1, dim)])) / c;
      }
    }
    set_boundary(dim, b, x);
  }
}

void AR3D_Fluid::diffuse(FVector dim, int b, float * x, float * x0, float diff, float dt)
{
  float a = dt * diff * (dim.X * dim.Y);
  gauss_siedel(dim, b, x, x0, a, 1 + 4 * a);
}

void AR3D_Fluid::advect(FVector dim, int b, float * d, float * d0, float * u, float * v, float dt)
{

  float dt0 = dt*dim.X;
  for (int i = 1; i <= dim.X; i++) {
    for (int j = 1; j <= dim.Y; j++) {
      // (x, y) is the address to copy from
      float x = i - dt0*u[IX(i, j, dim)], y = j - dt0*v[IX(i, j, dim)];

      // clamp x and y
      if (x < 0.5f) x = 0.5f; else if (x > dim.X + 0.5f) x = dim.X + 0.5f;
      if (y < 0.5f) y = 0.5f; else if (y > dim.Y + 0.5f) y = dim.Y + 0.5f;

      // s1 and s0 are lerp coordinates [0,1) within the source cell
      int i0 = (int)x, i1 = i0 + 1;
      int j0 = (int)y, j1 = j0 + 1;
      float s1 = x - i0, s0 = 1 - s1;
      float t1 = y - j0, t0 = 1 - t1;

      // sample the source
      d[IX(i, j, dim)] =
        s0*(t0*d0[IX(i0, j0, dim)] + t1*d0[IX(i0, j1, dim)]) +
        s1*(t0*d0[IX(i1, j0, dim)] + t1*d0[IX(i1, j1, dim)])
        ;
    }
  }
  set_boundary(dim, b, d);
}

void AR3D_Fluid::project(FVector dim, float * u, float * v, float * p, float * div)
{
  // stablisation step. adjust the velocity to prevent increase in energy
  // in the system.

  // calculate divergence into div
  // set initial value of p
  for (int i = 1; i <= dim.X; i++) {
    for (int j = 1; j <= dim.Y; j++) {
      div[IX(i, j, dim)] = -0.5f*(u[IX(i + 1, j, dim)] - u[IX(i - 1, j, dim)] + v[IX(i, j + 1, dim)] - v[IX(i, j - 1, dim)]) / dim.X;
      p[IX(i, j, dim)] = 0;
    }
  }

  // copy pixels to boundary
  set_boundary(dim, 0, div);
  set_boundary(dim, 0, p);

  // p += div[x+/-1, y+/-1] * 4;
  gauss_siedel(dim, 0, p, div, 1, 4);

  // calculate velocity from pressure-like "p"
  for (int i = 1; i <= dim.X; i++) {
    for (int j = 1; j <= dim.Y; j++) {
      // u from left and right
      u[IX(i, j, dim)] -= 0.5f*dim.X*(p[IX(i + 1, j, dim)] - p[IX(i - 1, j, dim)]);

      // v from up and down.
      v[IX(i, j, dim)] -= 0.5f*dim.Y*(p[IX(i, j + 1, dim)] - p[IX(i, j - 1, dim)]);
    }
  }

  // copy velocity to boundary
  set_boundary(dim, 1, u);
  set_boundary(dim, 2, v);
}

void AR3D_Fluid::density_step(FVector dim, float * x, float * x0, float * u, float * v, float diff, float dt)
{
  // apply diffusion to density. If there is no velocity, the value will still spread.  
  Swap(x0, x);
  //fSwap(x0, x);
  diffuse(dim, 0, x, x0, diff, dt);

  // use the velocity field to carry density around.
  Swap(x0, x);
  //fSwap(x0, x);
  advect(dim, 0, x, x0, u, v, dt);
}

void AR3D_Fluid::velocity_step(FVector dim, float * u, float * v, float * u0, float * v0, float visc, float dt)
{
  /// Compute the new velocity field.
  // diffuse into neighouring cells
  Swap(u0, u);
  diffuse(dim, 1, u, u0, visc, dt);
  Swap(v0, v);
  diffuse(dim, 2, v, v0, visc, dt);

  // stabilise the system using poisson
  project(dim, u, v, u0, v0);
  Swap(u0, u);
  Swap(v0, v);

  // use advection to move the velocity itself
  advect(dim, 1, u, u0, u0, v0, dt);
  advect(dim, 2, v, v0, u0, v0, dt);

  // stabilise the system using poisson
  project(dim, u, v, u0, v0);
}

int AR3D_Fluid::IX(int i, int j, FVector dim) {
  return (i)+((dim.X + 2) * (j));
}
