// ErrorInfo.h: interface for the CErrorInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined _ErrorInfo_EM
#define _ErrorInfo_EM

class CErrorInfo 
{

public:
	CErrorInfo();
	 ~CErrorInfo();

private:
	int		nVariableIndex;
	int		nErrorCode;
	char	sError[256];

public:
	int GetVariableIndex();
	void AddVariableIndex(int nAdd = 1);

	int GetLastError();
	char *GetLastErrorInfo(char *sError);

	void SetLastError(int nCode);
	void SetLastErrorInfo(char *sError);

};

#endif
