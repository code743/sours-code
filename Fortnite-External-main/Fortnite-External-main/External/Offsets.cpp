W2S
 
struct Camera
{
	Vector3 Location;
	Vector3 Rotation;
	float FieldOfView;
}; Camera vCamera;
 
Camera GetCamera(__int64 a1)
{
	Camera FGC_Camera;
	__int64 v1;
	__int64 v6;
	__int64 v7;
	__int64 v8;
 
	v1 = read<__int64>(settings::Localplayer + 0xd0);
	__int64 v9 = read<__int64>(v1 + 0x10); // 0x10
 
	FGC_Camera.FieldOfView = 80.0f / (read<double>(v9 + 0x680) / 1.19f); // 0x600
 
	FGC_Camera.Rotation.x = read<double>(v9 + 0x7D0);
	FGC_Camera.Rotation.y = read<double>(a1 + 0x148);
 
	uint64_t FGC_Pointerloc = read<uint64_t>(settings::Uworld + 0x100);
	FGC_Camera.Location = read<Vector3>(FGC_Pointerloc);
 
 
	//v6 = driver.read<__int64>(Localplayer + 0x70);
	//v7 = driver.read<__int64>(v6 + 0x98);
	//v8 = driver.read<__int64>(v7 + 0xF8);
	//FGC_Camera.Location = driver.read<Vector3>(Uworld + 0x100); // 0x20
 
	return FGC_Camera;
}
 
Vector3 ProjectWorldToScreen(Vector3 WorldLocation)
{
	Camera vCamera = GetCamera(settings::Rootcomp);
	vCamera.Rotation.x = (asin(vCamera.Rotation.x)) * (180.0 / M_PI);
 
 
	D3DMATRIX tempMatrix = Matrix(vCamera.Rotation);
 
	Vector3 vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
	Vector3 vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
	Vector3 vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);
 
	Vector3 vDelta = WorldLocation - vCamera.Location;
	Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));
 
	if (vTransformed.z < 1.f)
		vTransformed.z = 1.f;
 
	return Vector3((Width / 2.0f) + vTransformed.x * (((Width / 2.0f) / tanf(vCamera.FieldOfView * (float)M_PI / 360.f))) / vTransformed.z, (Height / 2.0f) - vTransformed.y * (((Width / 2.0f) / tanf(vCamera.FieldOfView * (float)M_PI / 360.f))) / vTransformed.z, 0);
}
 
GetBone Functions
 
FTransform GetBoneIndex(DWORD_PTR mesh, int index)
{
	DWORD_PTR bonearray;
	bonearray = read<DWORD_PTR>(mesh + 0x5b0);
 
	if (bonearray == NULL)
	{
		bonearray = read<DWORD_PTR>(mesh + 0x5b0);
	}
	return read<FTransform>(bonearray + (index * 0x60));
}
 
Vector3 GetBoneWithRotation(DWORD_PTR mesh, int id)
{
	FTransform bone = GetBoneIndex(mesh, id);
	FTransform ComponentToWorld = read<FTransform>(mesh + 0x240);
 
	D3DMATRIX Matrix;
	Matrix = MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());
 
	return Vector3(Matrix._41, Matrix._42, Matrix._43);
}
 
#define OFFSET_UWORLD 0xDF5D928
 
namespace Offsets {
 
 
    uintptr_t GameInstance = 0x1a8;
    uintptr_t LocalPlayers = 0x38;
    uintptr_t PlayerController = 0x30;
    uintptr_t LocalPawn = 0x328;
 
    uintptr_t PlayerState = 0x2a8;
    uintptr_t RootComponent = 0x190;
    uintptr_t Persistentlevel = 0x30;
    uintptr_t TeamIndex = 0x1098;
    uintptr_t AcotrCount = 0xA0;
    uintptr_t AAcotrs = 0x98;
    uintptr_t Mesh = 0x310;
    uintptr_t ReviveFromDBNOTime = 0x4180;
    uintptr_t RelativeLocation = 0x128;
    uintptr_t display_name = 0x90;
    uintptr_t bIsDying = 0x6f8;
 
    uintptr_t bIsDBNO = 0x782;
    uintptr_t CurrentWeapon = 0x828;
    uintptr_t is_reloading = 0x329;
    uintptr_t CurrWeapon = 0x828;
    uintptr_t weapondata = 0x3f0;
    uintptr_t tier = 0x74;
    //uintptr_t DisplayName = 0xFA8;
 
    /uintptr_t CurrentWeapon = 0x790;
    uintptr_t bAlreadySearched = 0xF51;
    uintptr_t WeaponData = 0x3D8;/
    uintptr_t CurrentActor = 0x8;
    //uintptr_t PrimaryPickupItemEntry = 0x2F8;
    //uintptr_t ItemDefinition = 0x18;
    uintptr_t Velocity = 0x170;
    uintptr_t ComponentToWorld = 0x240;//0x188
    uintptr_t BoneArray = 0x5b0; //retard goofy ass
    //uintptr_t bIsDying = 0x548;
    //uintptr_t bIsDBNO = 0x572;
 
}
 
 
 
namespace W2S {
    uintptr_t chain69 = 0xC8;
    uintptr_t chain699 = 0x7D0;
    uintptr_t chain = 0x70;
    uintptr_t chain1 = 0x98;
    uintptr_t chain2 = 0xF8;
    uintptr_t vDelta = 0x20;
    uintptr_t zoom = 0x690;
}
