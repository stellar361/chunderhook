#pragma once
#include "../../../SDK/SDK.h"

class CFakeAngle
{
public:
	void Run(CTFPlayer* pLocal);
	bool SetupFakeBones(CTFPlayer* pLocal, matrix3x4* aOut, const Vec2& vAngles);

	matrix3x4 aBones[MAXSTUDIOBONES];
	bool bBonesSetup = false;

	bool bDrawChams = false;
};

ADD_FEATURE(CFakeAngle, FakeAngle);