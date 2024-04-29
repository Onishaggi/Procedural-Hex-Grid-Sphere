// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "KismetProceduralMeshLibrary.h"
#include "Components/LineBatchComponent.h"
#include "ProceduralMeshComponent.h"
#include "icohedryon.generated.h"
class UProceduralMeshCompoenet;




USTRUCT(BlueprintType)
struct F2DArray
{
	GENERATED_BODY()

public:
	

	TArray<int> Ar;
	/*F2DArray(TArray<int> Arr) {
		Ar = Arr;
	}*/

	int operator[] (int32 i) {
		return Ar[i];

	}

	void Add(int num) {
		Ar.Add(num);
	}
	int Get_Length() {
		return Ar.Num();

	}
	void Remove_At(int index) {

		Ar.RemoveAt(index);
	}
};
USTRUCT(BlueprintType)
struct F3DArray
{

	GENERATED_BODY()

public:

	TArray<F2DArray> F3DDArray;

	F2DArray operator[] (int32 i) {
		return F3DDArray[i];
	}

	void Add(F2DArray rune) {
		F3DDArray.Add(rune);
	}
	int Get_Length() {
		return F3DDArray.Num();

	}
	void Remove_At(int index) {
		F3DDArray.RemoveAt(index);
	}
};

USTRUCT(BlueprintType)

struct FVerticies_Avgerage
{

	GENERATED_BODY()

public:
	FVector Verticies_SUM = FVector(0, 0, 0);

	void add(FVector v) {

		Verticies_SUM.X = Verticies_SUM.X + v.X;
		Verticies_SUM.Y = Verticies_SUM.Y + v.Y;
		Verticies_SUM.Z = Verticies_SUM.Z + v.Z;
	}

	FVector avg(int V_NUM, FVector Destnation_Vertex) {

		float ss = 1 / V_NUM;

		Destnation_Vertex.X = Verticies_SUM.X/V_NUM;
		Destnation_Vertex.Y = Verticies_SUM.Y/V_NUM;
		Destnation_Vertex.Z = Verticies_SUM.Z/V_NUM;

		return Destnation_Vertex;
	}
	/*FVector clearVerticies_SUM(FVector Verticies_SUM) {
		Verticies_SUM = FVector(0, 0, 0);

		return Verticies_SUM;
	}*/
};




UCLASS()
class PROCEDUAL_MESH_TEST_API Aicohedryon : public AActor
{
	GENERATED_BODY()
		

	






public:
	// Sets default values for this actor's properties
	Aicohedryon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	virtual void OnConstruction(const FTransform& Transform) override;
	// variables for mesh generation
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		UProceduralMeshComponent* ProceduralMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		TArray<FVector> Vertices;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		TArray<int> Triangles;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		TArray<FLinearColor> VertexColors;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		TArray<FColor> UPVertexColors;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		TArray<FProcMeshTangent> Tangents;
	TArray<F3DArray>vmap;

	//Icosahedron constants
	const double pi = 3.14159265358979323846;
	const float H_Angle = PI / 180 * 72;//360/5=72 degree for the upper 5 vertcies
	const float V_Angle = PI / 180 * 26.565;//lattitude angle
	int RowIndexpointer1;
	int RowIndexpointer2;
	float hAngle1;
	float hAngle2;
	float X;
	float Y;
	float Z;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		float radius;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		TArray<F2DArray> Trig_Faces;
	//meseh generation function

	void Generate_Hex_Sphere();
	void ClearHexSData();
	void GenerateMesh();
	void ClearMeshData();

	TArray<int> subDivide_Edge(int a,int b,float subdivide,TMap<FString,int>& cache);
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		float subDivide=2;
	void SubDiV_Trignalges(float subdivide);
	void tosphere(float Radius);
	TArray<int> rotate_To_Start(TArray<int> vertcies, int pos);
	TArray<int> clone(TArray<int> src_array, TArray<int> dest);
	TArray<int> clone(F2DArray src_array, TArray<int> dest);
	void mapVertFace();
	TArray<F3DArray> sortVertFace(TArray<F3DArray> Vmap);
	FVector return_centroid(TArray<int> face, FVector out);
	static FString GetDecimicalResultS(float Number, int Accuracy);
	static float GetDecimicalResultF(float Number, int Accuracy);
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		TArray<F2DArray> tiles;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		TArray<FVector> HVertices;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		TArray<int> HTriangles;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		TArray<FVector> HNormals;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Defaults")
		TArray<FVector2D> HUV0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		TArray<FLinearColor> HVertexColors;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		UMaterialInterface* HMaterial;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		TArray<FColor> HUPVertexColors;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		TArray<FProcMeshTangent> HTangents;
	TArray<F3DArray>Hvmap;
	void createFanTiles();
	void HmapVertFace();
	void GenerateNormals(TArray<FVector>& Normss);
	void Genreate_UV0s(TArray<FVector> Verts /*Vertices to normalize them*/);
	TArray<int> DetectWrappedUVCoordinates();
	void Switch_render_side(TArray<int>& Trianlge_flip /*triangle to be flipped*/);
	void Switch_render_side(int index_inTrigFaces);
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		UProceduralMeshComponent* HFixUVProceduralMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		TArray<FVector> HUVVertices;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		TArray<int> HUVTriangles;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		TArray<FVector> HUVNormals;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		TArray<FVector2D> HUVUV0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		TArray<FLinearColor> HUVVertexColors;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		UMaterialInterface* HUVMaterial;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		TArray<FColor> HUVUPVertexColors;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		TArray<FProcMeshTangent> HUVTangents;
	void getUniqueverts(TArray<int> trigs/*Trigs index that wioll be cloned*/, TArray<FVector>& meshvertsloc, TArray<int>& Trigs, TArray<FVector>& norms, TArray<FVector> hVerts, TArray<FVector> hnorms);
	void FixWrappedUV(TArray<int> Fliped_trigs_indx);
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		int debugLineLength=1000;
		TArray<FBatchedLine> Vertex_nomals_Debug;
	void DebugVertexLines(TArray<FVector> Vertex_normals);
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
		float Tiles_debugLinetickness = 1;
	TArray<FBatchedLine> Tiles_Line;
	void DebugTiles(TArray<F2DArray> tiless);
	TMap<int, bool> isWalkable;

	//FVector Vertex_avg(int s,FVector out);



	




	//void GenerateNormals_Newellsmethod(TArray<FVector>&Normss);
	

	


};
