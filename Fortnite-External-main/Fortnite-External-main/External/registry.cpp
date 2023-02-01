#pragma once
#include <Windows.h>
#include <iostream>
#include <winternl.h>
#include <ntstatus.h>
#include <atomic>
#include <mutex>
#include <TlHelp32.h>
#include <d3d9.h>
#include <xmmintrin.h>


typedef enum _KEY_VALUE_INFORMATION_CLASS {
	KeyValueBasicInformation,
	KeyValueFullInformation,
	KeyValuePartialInformation,
	KeyValueFullInformationAlign64,
	KeyValuePartialInformationAlign64,
	KeyValueLayerInformation,
	MaxKeyValueInfoClass  // MaxKeyValueInfoClass should always be the last enum
} KEY_VALUE_INFORMATION_CLASS;

typedef struct _KEY_VALUE_FULL_INFORMATION {
	ULONG   TitleIndex;
	ULONG   Type;
	ULONG   DataOffset;
	ULONG   DataLength;
	ULONG   NameLength;
	WCHAR   Name[1];            // Variable size
//          Data[1];            // Variable size data not declared
} KEY_VALUE_FULL_INFORMATION, * PKEY_VALUE_FULL_INFORMATION;


#ifdef __cplusplus
extern "C++"
{
	char _RTL_CONSTANT_STRING_type_check(const char* s);
	char _RTL_CONSTANT_STRING_type_check(const WCHAR* s);
	// __typeof would be desirable here instead of sizeof.
	template <size_t N> class _RTL_CONSTANT_STRING_remove_const_template_class;
template <> class _RTL_CONSTANT_STRING_remove_const_template_class<sizeof(char)> { public: typedef  char T; };
template <> class _RTL_CONSTANT_STRING_remove_const_template_class<sizeof(WCHAR)> { public: typedef WCHAR T; };
#define _RTL_CONSTANT_STRING_remove_const_macro(s) \
    (const_cast<_RTL_CONSTANT_STRING_remove_const_template_class<sizeof((s)[0])>::T*>(s))
}
#else
char _RTL_CONSTANT_STRING_type_check(const void* s);
#define _RTL_CONSTANT_STRING_remove_const_macro(s) (s)
#endif
#define RTL_CONSTANT_STRING(s) \
{ \
    sizeof( s ) - sizeof( (s)[0] ), \
    sizeof( s ) / sizeof(_RTL_CONSTANT_STRING_type_check(s)), \
    _RTL_CONSTANT_STRING_remove_const_macro(s) \
}

extern "C" {
	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwQueryValueKey(
			_In_ HANDLE KeyHandle,
			_In_ PUNICODE_STRING ValueName,
			_In_ KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
			_Out_writes_bytes_to_opt_(Length, *ResultLength) PVOID KeyValueInformation,
			_In_ ULONG Length,
			_Out_ PULONG ResultLength
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwClose(
			_In_ HANDLE Handle
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwOpenKey(
			_Out_ PHANDLE KeyHandle,
			_In_ ACCESS_MASK DesiredAccess,
			_In_ POBJECT_ATTRIBUTES ObjectAttributes
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwQueryValueKey(
			_In_ HANDLE KeyHandle,
			_In_ PUNICODE_STRING ValueName,
			_In_ KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
			_Out_writes_bytes_to_opt_(Length, *ResultLength) PVOID KeyValueInformation,
			_In_ ULONG Length,
			_Out_ PULONG ResultLength
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwSetValueKey(
			_In_ HANDLE KeyHandle,
			_In_ PUNICODE_STRING ValueName,
			_In_opt_ ULONG TitleIndex,
			_In_ ULONG Type,
			_In_reads_bytes_opt_(DataSize) PVOID Data,
			_In_ ULONG DataSize
		);

	NTSYSAPI NTSTATUS ZwCreateKey(
		PHANDLE            KeyHandle,
		ACCESS_MASK        DesiredAccess,
		POBJECT_ATTRIBUTES ObjectAttributes,
		ULONG              TitleIndex,
		PUNICODE_STRING    Class,
		ULONG              CreateOptions,
		PULONG             Disposition
	);
}

namespace RegistryUtils
{
    template <typename T>
    __forceinline T ReadRegistry(const std::wstring& regPath, const std::wstring& key)
    {
        HANDLE hKey;
        OBJECT_ATTRIBUTES objAttr;
        NTSTATUS status = STATUS_UNSUCCESSFUL;

        UNICODE_STRING uRegPath;
        RtlInitUnicodeString(&uRegPath, regPath.c_str());

        UNICODE_STRING uKey;
        RtlInitUnicodeString(&uKey, key.c_str());

        InitializeObjectAttributes(&objAttr, &uRegPath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, nullptr, nullptr);

        status = ZwOpenKey(&hKey, KEY_ALL_ACCESS, &objAttr);

        if (!NT_SUCCESS(status))
        {
            return T();
        }

        ULONG keySize = 0;
        status = ZwQueryValueKey(hKey, &uKey, KeyValueFullInformation, nullptr, 0, &keySize);

        if (!NT_SUCCESS(status))
        {
            ZwClose(hKey);
            return T();
        }

        std::vector<BYTE> keyInfo(keySize);
        PKEY_VALUE_FULL_INFORMATION pKeyInfo = reinterpret_cast<PKEY_VALUE_FULL_INFORMATION>(keyInfo.data());

        status = ZwQueryValueKey(hKey, &uKey, KeyValueFullInformation, pKeyInfo, keySize, &keySize);

        if (!NT_SUCCESS(status))
        {
            ZwClose(hKey);
            return T();
        }

        ZwClose(hKey);
        return *reinterpret_cast<T*>(reinterpret_cast<LONG64>(pKeyInfo) + pKeyInfo->DataOffset);
    }
}



DWORD Daimkey = VK_RBUTTON;		//aimkey
int aimheight = 46;				//aim height value
unsigned int asdelay = 90;		//use x-999 (shoot for xx millisecs, looks more legit)
bool IsPressed = false;			//
DWORD astime = timeGetTime();	//autoshoot timer

//init only once
bool firstTime = true;

//viewport
UINT vps = 1;
D3D11_VIEWPORT viewport;
float ScreenCenterX;
float ScreenCenterY;

//vertex
ID3D11Buffer *veBuffer;
UINT Stride = 24;
UINT veBufferOffset = 0;
D3D11_BUFFER_DESC vedesc;

//index
ID3D11Buffer *inBuffer;
DXGI_FORMAT inFormat;
UINT        inOffset;
D3D11_BUFFER_DESC indesc;

//rendertarget
ID3D11Texture2D* RenderTargetTexture;
ID3D11RenderTargetView* RenderTargetView = NULL;

//shader
ID3D11PixelShader* psRed = NULL;
ID3D11PixelShader* psGreen = NULL;

//pssetshaderresources
UINT pssrStartSlot;
D3D11_SHADER_RESOURCE_VIEW_DESC  Descr;
ID3D11ShaderResourceView* ShaderResourceView;
D3D11_TEXTURE2D_DESC texdesc;

//psgetConstantbuffers
ID3D11Buffer *pcsBuffer;
D3D11_BUFFER_DESC pscdesc;
UINT pscStartSlot;

//vsgetconstantbuffers
ID3D11Buffer *mConstantBuffers;
UINT vsConstant_StartSlot;

UINT psStartSlot;
UINT vsStartSlot;


	const auto pcall_present_discord = Helper::PatternScan(Discord::GetDiscordModuleBase(), xorstr("FF 15 ? ? ? ? 8B D8 E8 ? ? ? ? E8 ? ? ? ? EB 10"));
	auto presentSceneAdress = Helper::PatternScan(Discord::GetDiscordModuleBase(),
		xorstr("56 57 53 48 83 EC 30 44 89 C6"));

	DISCORD.HookFunction(presentSceneAdress, (uintptr_t)PresentHook, (uintptr_t)&PresentOriginal);

	DISCORD.HookFunction(presentSceneAdress, (uintptr_t)ResizeHook, (uintptr_t)&PresentOriginal);
	{
		
	return true;
};
