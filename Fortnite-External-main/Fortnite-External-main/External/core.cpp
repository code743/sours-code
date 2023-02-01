#include "main.cpp"
#include "define.h"


namespace Core {
	bool NoSpread = true;
	PVOID LocalPlayerPawn = nullptr;
	PVOID LocalPlayerController = nullptr;
	PVOID TargetPawn = nullptr;

	float* (*szOID_LOCAL_MACHINE_KEYSET)(PVOID, PVOID, PVOID) = nullptr;

	auto dist = sqrtf(rel[0] * rel[0] + rel[1] * rel[1] + rel[2] * rel[2]);
	auto yaw = atan2f(rel[1], rel[0]) * (180.0f / PI);
	
		angles[0] = Normalize(pitch);
		angles[1] = Normalize(yaw);
 
		BOOLEAN GetTargetHead(FVector& out) {
		if (!Settings.ESP.Players) continue;

		auto checking = ReadPointer(Core::TargetPawn, 0x278);
		{
			
			MessageBox(0, L"Failed to find CalculateShot", L"Failure", 0);
			return FALSE;
		}

void SetDepthStencilState(eDepthState aState) {
pContext->OMSetDepthStencilState(myDepthStencilStates[aState], 1);
}

do {
  if (Core::TargetPawn && Core::LocalPlayerController) {
    if (wcsstr(objectName.c_str(), L"Injector") && funcName == L"Injector") {
      window.DrawList->AddRectFilled(
          ImVec2(centerTop.x - size.x / 2.0f, centerTop.y - size.y + 3.0f),
          ImVec2(centerTop.x + size.x / 2.0f, centerTop.y),
          ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, 0.4f }));
      window.DrawList->AddText(
          ImVec2(centerTop.x - size.x / 2.0f, centerTop.y - size.y), color, copy);
      break;
    }
  }
} while (false);
					
			
void no_cheat::pawns_loop::add_pawn(const char* pwn, bool(*func)(), bool(*func2)())
{
	const size_t len = strlen(pwn) + 1;
	wchar_t* wc = new wchar_t[len];
	size_t tmp = 0;
	mbstowcs_s(&tmp, wc, len, pwn, len);
	menu.add(wc, func, func2);
	delete[] wc;
}
