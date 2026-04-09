#include "pch.h"
#include "ErrorUtil.h"

// 出错则return true
bool ErrorUtil::CheckIsErrorStatus(ErrorStatus es, AcString message) {
	if (es == eOk)
		return false;

	acutPrintf("\n" + message + L"errorCode: %d; %s", es, acadErrorStatusText(es));
	return true;
}