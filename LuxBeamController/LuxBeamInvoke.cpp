#include "LuxBeamInvoke.h"

#include "LuxBeamController.h"

extern "C" LUX_API int Connect()
{
	return LuxBeamController::GetInstance().Connect();
}

extern "C" LUX_API int DisConnect()
{
	LuxBeamController::GetInstance().Shutdown();
	return true;
}

extern "C" LUX_API int SetWorkingMode(int mode)
{
	LuxBeamController::GetInstance().SetWorkingMode(mode);
	return 1;
}

extern "C" LUX_API int SetTriggerMode(int trigMode)
{
	LuxBeamController::GetInstance().SetTriggerMode();
	return 1;
}

extern "C" LUX_API int GetTriggerInfo(int& pe, int& po, int& pere, int& pero, int& pseq)
{
	LuxBeamController::GetInstance().GetTriggerInfo(pe, po, pere, pero, pseq);
	return 1;
}

extern "C" LUX_API int SetExposure(float exp)
{
	LuxBeamController::GetInstance().SetImageHolidingTime(exp);
	return 1;
}

extern "C" LUX_API int StartSequence()
{
	LuxBeamController::GetInstance().StartSequence();
	return 1;
}

extern "C" LUX_API int StopSequence()
{
	LuxBeamController::GetInstance().StopSequence();
	return 1;
}

extern "C" LUX_API int ResetSequence()
{
	LuxBeamController::GetInstance().ResetSequence();
	return 1;
}

extern "C" LUX_API int UnResetSequence()
{
	LuxBeamController::GetInstance().UnResetSequence();
	return 1;
}

extern "C" LUX_API int LoadImage(unsigned char* data, int size, int inum)
{
	LuxBeamController::GetInstance().UploadImageFromBytesToPosition(data, size, inum);
	return 1;
}

extern "C" LUX_API int LoadLongCompactImage(unsigned char* data, int size, int inum, int rows)
{
	if (size == 1920 / 8 * rows && rows < 65536)
	{
		LuxBeamController::GetInstance().UploadLongImageFromBytesToPosition(data, size, inum);
		return true;
	}
	else
	{
		return false;
	}
}