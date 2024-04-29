// Fill out your copyright notice in the Description page of Project Settings.


#include "icohedryon.h"


// Sets default values
Aicohedryon::Aicohedryon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Icohedryon"));
	ProceduralMesh->SetupAttachment(RootComponent);
	HFixUVProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("IcohedryonUVFIX"));
	HFixUVProceduralMesh->SetupAttachment(RootComponent);





}

void Aicohedryon::OnConstruction(const FTransform& Transform)
{
	//ClearMeshData();
	//Generate mesh

	GenerateMesh();
	Generate_Hex_Sphere();
	ProceduralMesh->SetMaterial(0, HMaterial);
	HFixUVProceduralMesh->SetMaterial(0, HMaterial);
}
void Aicohedryon::BeginPlay()
{
	Super::BeginPlay();

	

}
void Aicohedryon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Add your tick logic here
	DebugTiles(tiles);
}
void Aicohedryon::Generate_Hex_Sphere()
{
	ClearHexSData();
	GenerateMesh();
	SubDiV_Trignalges(subDivide);
	tosphere(radius);
	mapVertFace();
	vmap = sortVertFace(vmap);
	createFanTiles();


	HmapVertFace();
	//GenerateNormals(HNormals);
	Genreate_UV0s(HVertices);
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(HVertices, HTriangles, HUV0, HNormals, HTangents);
	FixWrappedUV(DetectWrappedUVCoordinates());

	//DetectWrappedUVCoordinates();

	DebugVertexLines(HNormals);
	DebugTiles(tiles);


	ProceduralMesh->CreateMeshSection_LinearColor(0, HVertices, HTriangles, HNormals, HUV0, HVertexColors, HTangents, true);
}
void Aicohedryon::ClearHexSData()
{
	HVertices.Empty();
	HTriangles.Empty();
	HUV0.Empty();
	HNormals.Empty();
	HVertexColors.Empty();
	HTangents.Empty();
	vmap.Empty();
	Hvmap.Empty();
	tiles.Empty();
	Vertex_nomals_Debug.Empty();
	Tiles_Line.Empty();
}
void Aicohedryon::GenerateMesh()
{
	ClearMeshData();
	hAngle1 = -pi / 2 - H_Angle / 2;
	hAngle2 = -pi / 2;
	//radius = 1000;

	Vertices.Add(FVector(0, 0, radius));//the TOp vertex
	for (int i = 1; i <= 5; i++)
	{
		/*RowIndexpointer1 = i * 3;
		RowIndexpointer2 = (i + 5) * 3;*/
		X = radius * cosf(V_Angle);
		Y = radius * cosf(V_Angle);
		Z = radius * sinf(V_Angle);
		Vertices.Add(FVector(X * cosf(hAngle1), Y * sinf(hAngle1), Z));


		hAngle1 += H_Angle;

	}
	hAngle1 = -pi / 2 - H_Angle / 2;
	hAngle2 = -pi / 2;
	for (int i = 1; i <= 5; i++)
	{
		X = radius * cosf(V_Angle);
		Y = radius * cosf(V_Angle);
		Z = radius * sinf(V_Angle);
		Vertices.Add(FVector(X * cosf(hAngle2), Y * sinf(hAngle2), -Z));

		hAngle2 += H_Angle;

	}
	Vertices.Add(FVector(0, 0, -radius));



	Triangles = { 0,1,5,0,5,4,0,4,3,0,3,2,0,2,1,1,6,10,1,10,5 ,5,10,9,5,9,4,4,9,8,4,8,3,3,8,7,3,7,2,2,7,6,2,6,1,11,10,6,11,9,10,11,8,9,11,7,8,11,6,7 };
	for (int j = 0; j < Triangles.Num() / 3; j++) { Trig_Faces.Add(F2DArray()); }
	for (int i = 0; i < Triangles.Num(); i++) {

		int f = i;
		Trig_Faces[f / 3].Ar.Add(Triangles[i++]);
		Trig_Faces[f / 3].Ar.Add(Triangles[i++]);
		Trig_Faces[f / 3].Ar.Add(Triangles[i]);

	}















	//ClearMeshData();

	//Vertices = { FVector(0,0,0), FVector(0,500,0), FVector(500,0,0) };
	//Triangles = { 0,1,2 };
	/*UV0.Init(FVector2D(0, 0), 60);
	Normals.Init(FVector(0, 0, 1.0f), 60);
	VertexColors.Init(FLinearColor(0, 0, 0, 1.0f), 60);
	Tangents.Init(FProcMeshTangent(1.0f, 0, 0), 60);
	ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true); */





}
void Aicohedryon::ClearMeshData()
{
	Vertices.Empty();
	Triangles.Empty();
	UV0.Empty();
	Normals.Empty();
	VertexColors.Empty();
	Tangents.Empty();
	Trig_Faces.Empty();
}
TArray<int> Aicohedryon::subDivide_Edge(int a, int b, float subdivide, TMap<FString, int>& cache)
{
	float lineDiv = 1.0 / (subdivide + 1.0);
	int indx = Vertices.Num();
	TArray<int> rtrn;
	rtrn.Add(a);
	FVector v0 = Vertices[a];
	FVector v1 = Vertices[b];
	//float x, y, z;
	FVector newpoint;
	FString Ckey;
	for (int i = 1; i <= subdivide; i++)
	{
		newpoint = FMath::Lerp(v0, v1, lineDiv * i);

		Ckey = GetDecimicalResultS(newpoint.X, 1) + "_" + GetDecimicalResultS(newpoint.Y, 1) + "_" + GetDecimicalResultS(newpoint.Z, 1);
		if (cache.Find(Ckey) != NULL) rtrn.Add(cache[Ckey]);
		else {
			Vertices.Add(newpoint);
			cache.Add(Ckey, indx);
			rtrn.Add(indx++); //add index then inc.
		}
	}
	rtrn.Add(b);
	return rtrn;
}
void Aicohedryon::SubDiV_Trignalges(float subdivide)
{
	TArray<F2DArray> facess;
	TMap<FString, int> cache;
	TArray<int> eleft;
	TArray<int> eright;
	TArray<int> topline;
	TArray<int> botline;
	int d;
	int t;
	int tmax;
	int F2Dindex = 0;
	for (F2DArray face : Trig_Faces)
	{
		eleft = subDivide_Edge(face[0], face[1], subdivide, cache);
		eright = subDivide_Edge(face[0], face[2], subdivide, cache);
		topline = { eleft[1],eright[1] };
		facess.Add(F2DArray());
		facess[F2Dindex++].Ar.Append(TArray<int>{ face[0], eleft[1], eright[1] });

		for (d = 1; d <= subdivide; d++)
		{
			botline = subDivide_Edge(eleft[d + 1], eright[d + 1], d, cache);
			tmax = topline.Num() - 1;

			for (t = 0; t <= tmax; t++)
			{
				facess.Add(F2DArray());
				facess[F2Dindex++].Ar.Append(TArray<int>{topline[t], botline[t], botline[t + 1]});
				if (t < tmax)
				{
					facess.Add(F2DArray());

					facess[F2Dindex++].Ar.Append(TArray<int>{topline[t], botline[t + 1], topline[t + 1]});
				}

			}
			topline = botline;

		}

	}
	Trig_Faces = facess;
}
void Aicohedryon::tosphere(float Radius)
{
	TArray<FVector> v = Vertices;
	//float d;
	for (int i = 0; i < v.Num(); i++) {
		//d = 1.0 / FMath::Sqrt(v[i][0] * v[i][0] + v[i][1] * v[i][1] + v[i][2] * v[i][2]); // Vector Length


		v[i].Normalize();
		//v[i][0] = v[i].X; //*= d; //Normalize Vertex, Also Save it as Normal Vector
		//v[i][1] = v[i].Y; //*= d;
		//v[i][2] = v[i].Z; //*= d;

		if (Radius != 1) {
			v[i].X *= Radius;
			v[i].Y *= Radius;
			v[i].Z *= Radius;
		}
	}
	Vertices = v;
}
TArray<int> Aicohedryon::rotate_To_Start(TArray<int> vertcies, int pos)
{
	int target = vertcies[pos];
	int temp;
	while (vertcies.Find(target) != 0)
	{
		temp = vertcies[0];
		vertcies[0] = vertcies[2];
		vertcies[2] = vertcies[1];
		vertcies[1] = temp;

	}

	return vertcies;
}
TArray<int> Aicohedryon::clone(TArray<int> src_array, TArray<int> dest)
{
	dest.Empty();
	for (int i = 0; i < src_array.Num(); i++) {
		dest.Add(src_array[i]);

	}
	return dest;
	/*std::memcpy(&dest, &src_array, sizeof(src_array));*/

}
TArray<int> Aicohedryon::clone(F2DArray src_array, TArray<int> dest)
{
	dest.Empty();
	for (int i = 0; i < src_array.Ar.Num(); i++) {
		dest.Add(src_array.Ar[i]);

	}

	return dest;
	//std::memcpy(&dest, &src_array, sizeof(src_array));
}
void Aicohedryon::mapVertFace()
{
	TArray<int> f;
	int pos;
	for (int i = 0; i < Vertices.Num(); i++)
	{
		vmap.Add(F3DArray());
	}
	for (F2DArray face : Trig_Faces) {
		for (int i = 0; i < 3; i++) {
			f = clone(face.Ar, f);
			pos = f.Find(face.Ar[i]);

			if (pos != 0) f = rotate_To_Start(f, pos);
			vmap[face[i]].Add(F2DArray());

			//int bbgg = vmap[face[i]].F3DDArray[0].Get_Length();

			//vmap[face[i]].F3DDArray[vmap[face[i]].F3DDArray.Num()-1].Add(1);



			vmap[face[i]].F3DDArray[vmap[face[i]].Get_Length() - 1].Ar = clone(f, vmap[face[i]].F3DDArray[vmap[face[i]].Get_Length() - 1].Ar);


		}

	}



}
TArray<F3DArray> Aicohedryon::sortVertFace(TArray<F3DArray> Vmap)
{
	TArray<F2DArray> olist;
	TArray<F2DArray> list;
	int a;
	int b;
	TArray<int> face;
	int i;
	int p;
	for (int m = 0; m < Vmap.Num(); m++) {
		list = Vmap[m].F3DDArray;
		olist.Empty();
		for (int j = 0; j < list.Num(); j++) {

			olist.Add(F2DArray());
		}
		olist[0] = list[0];
		a = list[0][0];
		b = list[0][1];
		p = 1;
		i = 0;
		list.RemoveAt(0);
		while (list.Num() > 0 && i < list.Num())
		{
			face = list[i].Ar;
			if (face.Find(a) != -1 && face.Find(b) != -1)
			{
				list.RemoveAt(i);
				olist[p++].Ar = face;
				a = face[0];
				b = face[1];
				i = 0;
				continue;
			}
			i++;
		}
		Vmap[m].F3DDArray = olist;

	}
	return Vmap;
}
FVector Aicohedryon::return_centroid(TArray<int> face, FVector out)
{
	out[0] = out[1] = out[2] = 0;	//If out exists, reset it
									//else create a new array

	FVector v;
	for (int i = 0; i < face.Num(); i++) {
		v = Vertices[face[i]];
		out.X += v.X;
		out.Y += v.Y;
		out.Z += v.Z;

	}

	float invLen = 1 / 3;
	out.X = out.X / 3;
	out.Y = out.Y / 3;
	out.Z = out.Z / 3;

	return out;
}
FString Aicohedryon::GetDecimicalResultS(float Number, int Accuracy)
{
	//Just example, you can set accuracy as an function parameter
	const int BigRoundResult = FMath::RoundToInt(Number * Accuracy);
	const FString IntPart = FString::FromInt(BigRoundResult / Accuracy);
	const FString DigPart = FString::FromInt(BigRoundResult > 0 ? BigRoundResult % Accuracy : (-1) * BigRoundResult % Accuracy);
	return IntPart + "." + DigPart;
}
float Aicohedryon::GetDecimicalResultF(float Number, int Accuracy)
{
	const int BigRoundResult = FMath::RoundToInt(Number * Accuracy);
	const FString IntPart = FString::FromInt(BigRoundResult / Accuracy);
	const FString DigPart = FString::FromInt(BigRoundResult > 0 ? BigRoundResult % Accuracy : (-1) * BigRoundResult % Accuracy);

	return 	UKismetStringLibrary::Conv_StringToFloat(IntPart + TEXT(".") + DigPart);
}
void Aicohedryon::createFanTiles()
{
	FVector Face_centroid = FVector(0, 0, 0);
	FString key;
	TMap<FString, int> cache;
	int ip;//index of current point
	int i = 0;
	int face_index = 0;
	int tile_index = 0;
	int cp;//current point
	int vcnt = 0;
	FVerticies_Avgerage V_AVG;
	TArray<int> edge;
	TArray<F2DArray>faces;
	FVector DBG;
	for (auto face : vmap)
	{
		HVertices.Add(Vertices[face[0].Ar[0]]);
		cp = vcnt++;


		edge.Empty();
		V_AVG.Verticies_SUM = FVector(0, 0, 0);


		TArray<int> tile;

		for (i = 0; i < face.Get_Length(); i++) {

			Face_centroid = return_centroid(face.F3DDArray[i].Ar, Face_centroid);
			key = GetDecimicalResultS(Face_centroid[0], 1) + "_" + GetDecimicalResultS(Face_centroid[1], 1) + "_" + GetDecimicalResultS(Face_centroid[2], 1);
			if (cache.Find(key) != NULL)
			{
				ip = cache.FindChecked(key);
			}
			else
			{
				ip = vcnt++;
				cache.Add(key, ip);
				HVertices.Add(Face_centroid);
			}
			edge.Add(ip);
			V_AVG.add(Face_centroid);
			if (i > 0)
			{
				tile.Add(faces.Num());

				faces.Add(F2DArray());
				faces[face_index++].Ar = TArray<int>{ cp,edge[i],edge[i - 1] };
				HTriangles.Append(TArray<int>{cp, edge[i], edge[i - 1]});

			}


		}
		tile.Add(faces.Num());

		faces.Add(F2DArray());
		faces[face_index++].Ar = TArray<int>{ cp,edge[0],edge[i - 1] };
		HTriangles.Append(TArray<int>{cp, edge[0], edge[i - 1]});

		HVertices[cp] = V_AVG.avg(face.Get_Length(), HVertices[cp]);




		tiles.Add(F2DArray());
		tiles[tile_index++].Ar = tile;

	}
	Trig_Faces = faces;
}
void Aicohedryon::HmapVertFace()
{

	TArray<int> f;
	int pos;
	for (int i = 0; i < HVertices.Num(); i++)
	{
		Hvmap.Add(F3DArray());
	}
	for (F2DArray face : Trig_Faces) {
		for (int i = 0; i < 3; i++) {
			f = clone(face.Ar, f);
			pos = f.Find(face.Ar[i]);

			if (pos != 0) f = rotate_To_Start(f, pos);
			Hvmap[face[i]].Add(F2DArray());

			//int bbgg = vmap[face[i]].F3DDArray[0].Get_Length();

			//vmap[face[i]].F3DDArray[vmap[face[i]].F3DDArray.Num()-1].Add(1);



			Hvmap[face[i]].F3DDArray[Hvmap[face[i]].Get_Length() - 1].Ar = clone(f, Hvmap[face[i]].F3DDArray[Hvmap[face[i]].Get_Length() - 1].Ar);


		}

	}

}
void Aicohedryon::GenerateNormals(TArray<FVector>& Normss)
{
	FVector nomral = FVector{ 0,0,0 };
	FVector a;
	FVector b;
	FVector c;
	FVector ab;
	FVector ac;
	FVector abXac;

	FVector avg_trigs_atPoint;



	/*TArray<F2DArray>Trig_Facess;
	for (int j = 0; j < Trigs.Num() / 3; j++) { Trig_Facess.Add(F2DArray()); }
	for (int i = 0; i < Trigs.Num(); i++) {

		int f = i;
		Trig_Facess[f / 3].Ar.Add(Trigs[i++]);
		Trig_Facess[f / 3].Ar.Add(Trigs[i++]);
		Trig_Facess[f / 3].Ar.Add(Trigs[i]);

	}*/
	for (int i = 0; i < Hvmap.Num(); i++) {
		float numOfV = 0;
		avg_trigs_atPoint = FVector{ 0,0,0 };
		for (F2DArray face : Hvmap[i].F3DDArray) {
			a = HVertices[face.Ar[0]];
			b = HVertices[face.Ar[1]];
			c = HVertices[face.Ar[2]];

			ab = FVector{ b.X - a.X, b.Y - a.Y, b.Z - a.Z };
			ac = FVector{ c.X - a.X, c.Y - a.Y, c.Z - a.Z };

			abXac = FVector::CrossProduct(ab, ac);


			avg_trigs_atPoint += abXac;
			numOfV++;


		}

		//avg_trigs_atPoint = avg_trigs_atPoint / numOfV;
		avg_trigs_atPoint = avg_trigs_atPoint.GetSafeNormal();
		avg_trigs_atPoint.X *= -1.0;
		avg_trigs_atPoint.Y *= -1.0;
		avg_trigs_atPoint.Z *= -1.0;
		Normss.Add(avg_trigs_atPoint);





	}



	/*for (F2DArray face : Trig_Facess) {
		a = Vertices[face.Ar[0]];
		b = Vertices[face.Ar[1]];
		c = Vertices[face.Ar[2]];

		ab = FVector{ b.X - a.X, b.Y - a.Y, b.Z - a.Z };
		ac = FVector{ c.X - a.X, c.Y - a.Y, c.Z - a.Z };

		abXac = FVector::CrossProduct(ab, ac);



		abXac.GetSafeNormal();
		Normss.Add(abXac);

	}*/
}
void Aicohedryon::Genreate_UV0s(TArray<FVector> Verts)
{

	FVector Normalized_Verts;
	FVector2D ICOuv;
	float invHalfPI = 1.0 / PI * 0.5;
	//float invPi = 1.0 /pi;
	for (int i = 0; i < Verts.Num(); i++)
	{
		Normalized_Verts = Verts[i].GetSafeNormal();
		ICOuv = FVector2D{ 0,0 };
		ICOuv.X = (UKismetMathLibrary::Atan2(Normalized_Verts.X, Normalized_Verts.Z) + PI) * invHalfPI;
		ICOuv.Y = (UKismetMathLibrary::Acos(Normalized_Verts.Y) + PI) / PI - 1;
		HUV0.Add(ICOuv);
	}

}
TArray<int> Aicohedryon::DetectWrappedUVCoordinates()
{
	TArray<int> Flipped_trigs_indices;
	for (int i = 0; i < Trig_Faces.Num(); i++)
	{
		int A = Trig_Faces[i].Ar[0];
		int B = Trig_Faces[i].Ar[1];
		int C = Trig_Faces[i].Ar[2];
		FVector texA = FVector(HUV0[A], 0);
		FVector texB = FVector(HUV0[B], 0);
		FVector texC = FVector(HUV0[C], 0);
		FVector texNormal = FVector::CrossProduct(texB - texA, texC - texA);
		if (texNormal.Z < 0)
		{
			Flipped_trigs_indices.Add(i);
		}

	}
	/*for (int i : Flipped_trigs_indices)
	{
		Switch_render_side(i);
	}*/

	/*for (int i : Flipped_trigs_indices) {
		Switch_render_side(Trig_Faces[i].Ar);
	}*/
	return Flipped_trigs_indices;

}
void Aicohedryon::Switch_render_side(int index_inTrigFaces)
{

	int temp = HTriangles[(index_inTrigFaces * 3) + 2];
	HTriangles[(index_inTrigFaces * 3) + 2] = HTriangles[(index_inTrigFaces * 3)];
	HTriangles[(index_inTrigFaces * 3)] = temp;

	/*HNormals[(index_inTrigFaces * 3)] = { 0,0,0 };
	HNormals[(index_inTrigFaces * 3)+1] = { 0,0,0 };
	HNormals[(index_inTrigFaces * 3)+2] = { 0,0,0 };*/



	//HTriangles[(index_inTrigFaces * 3) + 1] = B_indx;


}
void Aicohedryon::Switch_render_side(TArray<int>& Trianlge_flip)
{
	int temp = Trianlge_flip[2];
	Trianlge_flip[2] = Trianlge_flip[0];
	Trianlge_flip[0] = temp;
}
void Aicohedryon::getUniqueverts(TArray<int> trigs/*Trigs index that wioll be cloned*/, TArray<FVector>& meshvertsloc, TArray<int>& Trigs, TArray<FVector>& norms, TArray<FVector> hVerts, TArray<FVector> hnorms)
{


	int trig_index = 0;
	TMap<int, int> cache;
	for (int i : trigs) {
		for (int j = 0; j < 3; j++) {
			if (!cache.Find(Trig_Faces[i].Ar[j])) {
				cache.Add(Trig_Faces[i].Ar[j], trig_index++);
				meshvertsloc.Add(hVerts[Trig_Faces[i].Ar[j]]);
				norms.Add(hnorms[Trig_Faces[i].Ar[j]]);

				//HNormals[Trig_Faces[i].Ar[j]] *= 0;
			}
			Trigs.Add(cache.FindRef(Trig_Faces[i].Ar[j]));
		}
	}
	TArray<FVector> normDUMMY;
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(meshvertsloc, Trigs, HUVUV0, normDUMMY, HUVTangents);
}
void Aicohedryon::FixWrappedUV(TArray<int> Fliped_trigs_indx)
{//-------------------------------------


	HUVVertices.Empty();
	HUVTriangles.Empty();
	HUVUV0.Empty();
	HUVNormals.Empty();
	HUVVertexColors.Empty();
	HUVTangents.Empty();



	//------------------------------




	//------
	TArray<FVector> tempVs = HVertices;
	TArray<FVector>tempnorms = HNormals;
	int tempVs_lastindx = tempVs.Num() - 1;
	TMap<int, int> cache;

	for (int currentFlippedTrig_indx : Fliped_trigs_indx) {

		int A_indx = Trig_Faces[currentFlippedTrig_indx].Ar[0];
		//HUVNormals.Add(HNormals[A_indx]);

		int B_indx = Trig_Faces[currentFlippedTrig_indx].Ar[1];
		//HUVNormals.Add(HNormals[B_indx]);

		int C_indx = Trig_Faces[currentFlippedTrig_indx].Ar[2];
		//HUVNormals.Add(HNormals[C_indx]);

		FVector Va = HVertices[A_indx];
		FVector Vb = HVertices[B_indx];
		FVector Vc = HVertices[C_indx];
		if (HUV0[A_indx].X < 0.25f)
		{
			int TempA_indx = A_indx;
			if (!cache.Find(A_indx)) {

				HUVUV0.Add(FVector2D{ HUV0[A_indx].X + 1.0f,HUV0[A_indx].Y });
				//HUV0.Add(FVector2D{ 0,0});
				//HUV0[A_indx].X += 1.0;
				//HUV0[A_indx] = { 0, 0 };
				//HUV0[A_indx].X *= -1.0;
				tempVs.Add(Va);
				tempnorms.Add(HNormals[A_indx]);
				tempVs_lastindx++;
				cache.Add(A_indx, tempVs_lastindx);
				TempA_indx = tempVs_lastindx;
				A_indx = TempA_indx;
			}
			else {
				A_indx = cache.FindRef(A_indx);
			}
		}
		else {
			HUVUV0.AddUnique(FVector2D{ HUV0[A_indx].X,HUV0[A_indx].Y });
		}

		if (HUV0[B_indx].X < 0.25f)
		{
			int TempB_indx = B_indx;
			if (!cache.Find(B_indx)) {

				HUVUV0.Add(FVector2D{ HUV0[B_indx].X + 1.0f,HUV0[B_indx].Y });
				//HUV0.Add(FVector2D{ 0,0 });
				//HUV0[B_indx].X += 1.0;
				//HUV0[B_indx] = { 0, 0 };
				//HUV0[B_indx].X *= 1.0;
				tempVs.Add(Vb);
				tempnorms.Add(HNormals[B_indx]);
				tempVs_lastindx++;
				cache.Add(B_indx, tempVs_lastindx);
				TempB_indx = tempVs_lastindx;
				B_indx = TempB_indx;
			}
			else {
				B_indx = cache.FindRef(B_indx);
			}
		}
		else { HUVUV0.AddUnique(FVector2D{ HUV0[B_indx].X,HUV0[B_indx].Y }); }

		if (HUV0[C_indx].X < 0.25f)
		{
			int TempC_indx = C_indx;
			if (!cache.Find(C_indx)) {

				HUVUV0.Add(FVector2D{ HUV0[C_indx].X + 1.0f,HUV0[C_indx].Y });
				//HUV0.Add(FVector2D{ 0,0 });
				//HUV0[C_indx].X += 1.0;
				//HUV0[C_indx]= {0, 0};
				//HUV0[C_indx].X *= -1.0;
				tempVs.Add(Vc);
				tempnorms.Add(HNormals[C_indx]);
				tempVs_lastindx++;
				cache.Add(C_indx, tempVs_lastindx);
				TempC_indx = tempVs_lastindx;
				C_indx = TempC_indx;
			}
			else {
				C_indx = cache.FindRef(C_indx);
			}
		}
		else { HUVUV0.AddUnique(FVector2D{ HUV0[C_indx].X,HUV0[C_indx].Y }); }


		TArray<int> newtrig = TArray<int>{ A_indx,B_indx,C_indx };

		/*Trig_Faces[currentFlippedTrig_indx].Ar[0] = A_indx;
		HTriangles[(currentFlippedTrig_indx * 3)] = A_indx;

		Trig_Faces[currentFlippedTrig_indx].Ar[1] = B_indx;
		HTriangles[(currentFlippedTrig_indx * 3)+1] = B_indx;

		Trig_Faces[currentFlippedTrig_indx].Ar[2] = C_indx;
		HTriangles[(currentFlippedTrig_indx * 3)+2] = C_indx;*/
		//HUVTriangles.Append(newtrig);



	}
	getUniqueverts(Fliped_trigs_indx, HUVVertices, HUVTriangles, HUVNormals, tempVs, tempnorms);

	for (int i : Fliped_trigs_indx)
	{
		Switch_render_side(i);
	}

	HFixUVProceduralMesh->CreateMeshSection_LinearColor(0, HUVVertices, HUVTriangles, HUVNormals, HUVUV0, HUVVertexColors, HUVTangents, true);
}
void Aicohedryon::DebugVertexLines(TArray<FVector> Vertex_normals)
{
	FVector Worldpos = GetTransform().GetLocation();

	ULineBatchComponent* const LineBatcherr = GetWorld()->ForegroundLineBatcher;
	for (int i = 0; i < Vertex_normals.Num(); i++)
	{
		Vertex_nomals_Debug.Add(FBatchedLine(UKismetMathLibrary::TransformLocation(GetTransform(), HVertices[i]), UKismetMathLibrary::TransformLocation(GetTransform(), (HNormals[i] * debugLineLength)), FLinearColor::Red, 0, 1, SDPG_Foreground));
	}
	LineBatcherr->DrawLines(Vertex_nomals_Debug);

}
void Aicohedryon::DebugTiles(TArray<F2DArray> tiless)
{
	//Tiles_Line.Empty();
	FVector Worldpos = GetTransform().GetLocation();
	ULineBatchComponent* const LineBatcher = GetWorld()->ForegroundLineBatcher;
	for (int i = 0; i < tiless.Num();i++)//each tiles contains the indexes of the trinalge forming it 
	{	
		// for each triangle inside the tile get its vertcies index
		if (tiless[i].Ar.Num() == 6)
		{
			isWalkable.Add(i, true);
			for (int Tindex : tiless[i].Ar)
			{

				// for each vertcies index draw 3 lines [0]->[1],[1]->[2],[2]->[0] 
				// get the pos of these line and feed it to the BatchedLinesComponent;
				FVector A = HVertices[Trig_Faces[Tindex].Ar[0]];
				FVector B = HVertices[Trig_Faces[Tindex].Ar[1]];
				FVector C = HVertices[Trig_Faces[Tindex].Ar[2]];
				//Tiles_Line.Add(FBatchedLine(UKismetMathLibrary::TransformLocation(GetTransform(), A), UKismetMathLibrary::TransformLocation(GetTransform(), B), FLinearColor::Green, 10, Tiles_debugLinetickness, SDPG_Foreground));
				Tiles_Line.Add(FBatchedLine(UKismetMathLibrary::TransformLocation(GetTransform(), B), UKismetMathLibrary::TransformLocation(GetTransform(), C), FLinearColor::Green, 10, Tiles_debugLinetickness, SDPG_Foreground));
				//Tiles_Line.Add(FBatchedLine(UKismetMathLibrary::TransformLocation(GetTransform(), C), UKismetMathLibrary::TransformLocation(GetTransform(), A), FLinearColor::Green, 10, Tiles_debugLinetickness, SDPG_Foreground));
			}
		}
		else
		{
			isWalkable.Add(i, false);
			for (int Tindex : tiless[i].Ar)
			{
				FVector A = HVertices[Trig_Faces[Tindex].Ar[0]];
				FVector B = HVertices[Trig_Faces[Tindex].Ar[1]];
				FVector C = HVertices[Trig_Faces[Tindex].Ar[2]];
				//Tiles_Line.Add(FBatchedLine(UKismetMathLibrary::TransformLocation(GetTransform(), A), UKismetMathLibrary::TransformLocation(GetTransform(), B), FLinearColor::Green, 10, Tiles_debugLinetickness, SDPG_Foreground));
				Tiles_Line.Add(FBatchedLine(UKismetMathLibrary::TransformLocation(GetTransform(), B), UKismetMathLibrary::TransformLocation(GetTransform(), C), FLinearColor::Red, 10, Tiles_debugLinetickness, SDPG_Foreground));
				//Tiles_Line.Add(FBatchedLine(UKismetMathLibrary::TransformLocation(GetTransform(), C), UKismetMathLibrary::TransformLocation(GetTransform(), A), FLinearColor::Green, 10, Tiles_debugLinetickness, SDPG_Foreground));
			}
		}
	}
	LineBatcher->DrawLines(Tiles_Line);
}






















