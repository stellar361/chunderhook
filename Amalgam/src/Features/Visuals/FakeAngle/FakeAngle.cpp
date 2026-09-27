#include "FakeAngle.h"

#include "../../PacketManip/AntiAim/AntiAim.h"
#include "../../Ticks/Ticks.h"
#include "../Groups/Groups.h"

bool CFakeAngle::SetupFakeBones(CTFPlayer* pLocal, matrix3x4* aOut, const Vec2& vAngles)
{
	auto pAnimState = pLocal->m_PlayerAnimState();
	if (!pAnimState)
		return false;

	float flOldFrameTime = I::GlobalVars->frametime;
	int nOldSequence = pLocal->m_nSequence();
	float flOldCycle = pLocal->m_flCycle();
	auto pOldPoseParams = pLocal->m_flPoseParameter();
	char pOldAnimState[sizeof(CTFPlayerAnimState)];
	memcpy(pOldAnimState, pAnimState, sizeof(CTFPlayerAnimState));

	I::GlobalVars->frametime = 0.f;
	Vec2 vAngle = { std::clamp(vAngles.x, -89.f, 89.f), vAngles.y };
	if (pLocal->IsTaunting() && pLocal->m_bAllowMoveDuringTaunt())
		pLocal->m_flTauntYaw() = vAngle.y;
	pAnimState->Update(pAnimState->m_flCurrentFeetYaw = /*pAnimState->m_flEyeYaw =*/ vAngle.y, vAngle.x);
	pLocal->InvalidateBoneCache();
	const bool bSetup = pLocal->SetupBones(aOut, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, I::GlobalVars->curtime);

	I::GlobalVars->frametime = flOldFrameTime;
	pLocal->m_nSequence() = nOldSequence;
	pLocal->m_flCycle() = flOldCycle;
	pLocal->m_flPoseParameter() = pOldPoseParams;
	memcpy(pAnimState, pOldAnimState, sizeof(CTFPlayerAnimState));
	return bSetup;
}

void CFakeAngle::Run(CTFPlayer* pLocal)
{
	if (!pLocal || !pLocal->IsAlive() || pLocal->IsAGhost()
		|| !F::AntiAim.AntiAimOn() && (!Vars::Fakelag::Fakelag.Value || F::Ticks.m_iShiftedTicks == F::Ticks.m_iMaxShift))
	{
		bBonesSetup = false;
		return;
	}

	Group_t* pGroup = nullptr;
	if (!F::Groups.GetGroup(TargetsEnum::FakeAngle, pGroup) || !pGroup->m_tChams(true) && !pGroup->m_tGlow())
	{
		bBonesSetup = false;
		return;
	}

	bBonesSetup = SetupFakeBones(pLocal, aBones, { F::AntiAim.vFakeAngles.x, F::AntiAim.vFakeAngles.y });
}