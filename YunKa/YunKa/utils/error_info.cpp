#include <string.h>
#include "error_info.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CErrorInfo::CErrorInfo()
{
	//³õÊ¼»¯
	nVariableIndex = 0;

	this->nErrorCode = 0;
	strcpy_s(this->sError, "");
}

CErrorInfo::~CErrorInfo()
{
}

int CErrorInfo::GetLastError()
{	

	return this->nErrorCode;
}

void CErrorInfo::SetLastError(int nCode)
{
	this->nErrorCode = nCode;
}

char *CErrorInfo::GetLastErrorInfo(char *sErr)
{
	if(sErr == NULL)
		return NULL;

	strcpy_s(sErr, 256, this->sError);
	return sErr;
}

void CErrorInfo::SetLastErrorInfo(char *sErr)
{
	if(sErr == NULL)
	{
		strcpy_s(this->sError, "");
	}
	else
	{
		strcpy_s(this->sError, sErr);
	}
	return;
}

void CErrorInfo::AddVariableIndex(int nAdd)
{
	this->nVariableIndex += nAdd;
}

int CErrorInfo::GetVariableIndex()
{
	return this->nVariableIndex;
}
