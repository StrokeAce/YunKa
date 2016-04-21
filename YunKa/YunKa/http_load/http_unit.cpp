//---------------------------------------------------------------------------

#include "http_unit.h"
#include "convert.h"

#pragma hdrstop

//---------------------------------------------------------------------------

void UseSwitchInHttpDownload(bool bUse)
{
	http_useswitch(bUse);
}

//带超时的连接
int Connect_t(SOCKET& sockfd, const struct sockaddr  * pserv_addr,const unsigned int& namelen,int nsecond)
{
	unsigned long ul = 1;
	ioctlsocket(sockfd, FIONBIO, &ul);  //设置为非阻塞模式
	int nret = SOCKET_ERROR;
	if( connect(sockfd, pserv_addr,namelen) == -1)
	{
		
		int error=-1, len;
		timeval tm;
		fd_set set;
		tm.tv_sec  = nsecond;
		tm.tv_usec = 0;
		FD_ZERO(&set);
		FD_SET(sockfd, &set);
		if( select(sockfd+1, NULL, &set, NULL, &tm) > 0)
		{
			len=sizeof(error);
			getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (char*)&error, &len);
			if(error == 0)
				nret = 0;
		} 
    }else
	{
		nret = 0;
	}
	ul = 0;
	ioctlsocket(sockfd, FIONBIO, &ul);  //设置为阻塞模式
	
	return nret;
}

SOCKET ConnectToServer(const char* cDomain,unsigned short port,
					   const char *proxyip,unsigned short proxyport)
{
	SOCKET s=socket(AF_INET,SOCK_STREAM,0);
	if (INVALID_SOCKET ==s)
	{
		return s;
	}
	int timeout = 5000;//设置socket超时，防止认证过程阻塞
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
	sockaddr_in saddr;
	if (proxyip!=NULL && strlen(proxyip) > 0)
	{
		saddr.sin_addr.s_addr=inet_addr(proxyip);
		saddr.sin_family=AF_INET;
		saddr.sin_port=htons(proxyport);
		
		if (SOCKET_ERROR==Connect_t(s,(sockaddr*)&saddr,sizeof(saddr), 5))
		{
			closesocket(s);
			s=INVALID_SOCKET;
			return s;
		}
		
		char buf[2000];
		sprintf(buf,"CONNECT %s:%u HTTP/1.1\r\nAccept: */*\r\nContent-Type: text/html\r\nProxy-Connection: Keep-Alive\r\nContent-length: 0\r\n\r\n",
			cDomain,port);
		send(s,buf,strlen(buf),0);
		
		recv(s,buf,2000,0);
		if (strstr(buf,"established")==NULL)
		{
			return s;
		}
		
	}else
	{
		try
		{
			struct hostent * he = gethostbyname(cDomain);
			if (he==NULL)
			{
				closesocket(s);
				s=INVALID_SOCKET;
				return s;
			}
			
			memset(saddr.sin_zero,0,8);
			saddr.sin_addr=*((struct   in_addr*)he->h_addr);
			saddr.sin_family=AF_INET;
			saddr.sin_port=htons(port);
			
			if (0!=Connect_t(s,(sockaddr*)&saddr,sizeof(saddr), 5))
			{
				closesocket(s);
				s=INVALID_SOCKET;
			}
		}
		catch (...)
		{
			return INVALID_SOCKET;
		}
		
	}
	return s;
}

int HttpDownloadFile(string url,
				 string &body,
				 string &urlfile,
				 string postfile,
				 HWND hWnd,
				 const string& AdditionHead,
				 const char* pszProxyip,
				 unsigned short proxyport)
{
	body="";
	urlfile="";
	int pos=url.find ('/',8);
	if(pos==-1)
	{
		return 404;
	}
	
	
	string host=url.substr(7,pos-7);
	string remotepath=url.substr (pos,url.length ());
	pos=host.find(':');
	int port=80;
	if(pos!=-1)
	{
        port=atoi(host.substr(pos+1,host.length()).c_str()); 
		if(port==0)
			port=80;
		host=host.substr(0,pos);
	}
	
    
	SOCKET sServer=INVALID_SOCKET;
	string strError;

	if (pszProxyip == NULL || strlen(pszProxyip) == 0)
	{
		sServer = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
		if( sServer == INVALID_SOCKET)
			return 0;


		struct sockaddr_in inAddr;

		inAddr.sin_addr.S_un.S_addr=inet_addr(host.c_str());
		inAddr.sin_family=AF_INET;
		inAddr.sin_port=htons(port);
		if(inAddr.sin_addr.S_un.S_addr == INADDR_NONE)
		{
			struct hostent* phost = gethostbyname(host.c_str());
			if(phost == NULL)
				return 404;
			inAddr.sin_addr = *((struct in_addr*)phost->h_addr);
		}
		if(connect(sServer, (SOCKADDR*)&inAddr, sizeof(inAddr)) != 0)
		{
			//cerr<<"无法连接服务器"<<endl;
			closesocket(sServer);
			sServer=INVALID_SOCKET;
			strError.Format("host = %s, port = %d",
				convertstring(host).c_str(), port);
		}
	}else
	{
		sServer=ConnectToServer(host.c_str(), port, pszProxyip, proxyport);
		strError.Format("host = %s, port = %d, proxyip = %s, proxyport = %d",
			convertstring(host).c_str(), port, convertstring(pszProxyip).c_str(), proxyport);

	}
	if (sServer==INVALID_SOCKET)
	{
		return 404;
	}
	
	string cmd;
	CFile myFile;
	CFileException e;
	WIN32_FIND_DATA FindFileData;
	DWORD dwRead;
	int nCurDel, nDelToSend;
	int nCurRead, nCurSend;
	char data[1024];
	int onereadlen = 1000;
	int nHttpCode = 404;	  
	

	if( myFile.Open(postfile, CFile::modeRead | CFile::typeBinary |CFile::shareDenyWrite, &e ) == false)
	{
		return 404;
	}
	
	FindClose(FindFirstFile(postfile, &FindFileData));
	
	dwRead = 0;
	myFile.Seek(dwRead, CFile::begin);
	
	nDelToSend = FindFileData.nFileSizeLow/100;
	nCurDel=0;

	if(postfile.IsEmpty())
	{
		cmd="GET "+remotepath+" HTTP/1.0\r\nHost: "+host+"\r\nUser-Agent:Tracq\r\n"+AdditionHead+"\r\n";
	}
	else
	{
		char buff[256];
		sprintf(buff, "%d", FindFileData.nFileSizeLow);
		cmd="POST "+remotepath+" HTTP/1.0\r\nHost: "+host+"\r\nUser-Agent:Tracq\r\nContent-Length: "+ buff +"\r\n"+AdditionHead+"\r\n";
	}
	
	send(sServer, cmd.c_str(),cmd.length(), 0);
	if( hWnd != NULL);

	while(dwRead < FindFileData.nFileSizeLow)
	{
		nCurRead = myFile.Read(data, onereadlen);
				
		if(nCurRead == 0)
		{
			break;
		}
		else if(nCurRead < 0)
		{
			myFile.Close();
			return 404;
		}
				
		dwRead += nCurRead;
		nCurDel += nCurRead;
				
		nCurSend = send(sServer, data, nCurRead, 0);
		if( nCurSend <= 0)
		{
			myFile.Close();			
			closesocket(sServer);
			return 404;
		}

		if(nCurDel > nDelToSend || dwRead == nCurRead)
		{
			if( hWnd != NULL);
			nCurDel = 0;
		}
				
		if(FindFileData.nFileSizeLow <= onereadlen)
			break;
	}
			
	myFile.Close();			


	char *buf=new char[15535];  
	int nLenth = 0;   
	int nRecv;
	nRecv = recv(sServer, buf, 15534, 0);
	if (nRecv > 0)
	{
		buf[nRecv]=0;

		char* pStatue = strchr(buf,' ');
		char* headend = strstr(buf,"\r\n\r\n");
		*headend = '\0';
		headend += 4;
		nHttpCode = atoi(pStatue);
		if (nHttpCode != 200)
		{
			closesocket(sServer);
			delete [] buf;
			return nHttpCode;
		}
		
		string retstring = buf;
		int index = retstring.find("FileUrl:");
		if(index != string::npos)
		{
			int endindex = retstring.find("\n", index);
			if(endindex != string::npos)
			{
				urlfile = retstring.substr(index+9, endindex - index - 9);
			}
		}
		
		pStatue = strstr(buf, "Content-Length:");
		pStatue += 15;
		nLenth = atoi(pStatue);  
		
		string strNewURL;
		char* pNewURL= strstr(buf, "Last-Modified:");
		if(pNewURL != NULL)
		{
			pNewURL+=14;
			while((*pNewURL)==' ')
			{
				pNewURL++;
			}
			char* pEnd=strchr(pNewURL,'\n'); 
			if(pEnd) 
				*pEnd='\0';
			pEnd = strchr(pNewURL,'\r');
			if(pEnd) 
				*pEnd='\0';
		}
		
		int headlen=headend-buf;
		body = string(headend, nRecv - headlen); 
		
		nLenth = nLenth - nRecv + headlen;
		while(nLenth > 0)
		{
			nRecv = recv(sServer, buf, 15534, 0);
			if (nRecv<=0)
			{
				break;
			}
			buf[nRecv]=0;
			body += string(buf, nRecv);
			nLenth -= nRecv;
		}
		if (nLenth>0) 
		{
			nHttpCode=404;
		}
	}
	

	closesocket(sServer);

	delete [] buf;
    return nHttpCode;
}

int HttpDownload(string url,
				 string post,
				 string &body,
				 HWND hWnd,
				 const string& AdditionHead,
				 const char* pszProxyip,
				 unsigned short proxyport)
{
	body="";
	int pos=url.find ('/',8);
	if(pos==-1)
	{
		return 404;
	}
	
	
	string host=url.substr(7,pos-7);
	string remotepath=url.substr (pos,url.length ());
	pos=host.find(':');
	int port=80;
	if(pos!=-1)
	{
        port=atoi(host.substr(pos+1,host.length()).c_str()); 
		if(port==0)
			port=80;
		host=host.substr(0,pos);
	}
	
    
	SOCKET sServer=INVALID_SOCKET;
	string strError;

	if (pszProxyip == NULL || strlen(pszProxyip) == 0)
	{
		sServer = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
		if( sServer == INVALID_SOCKET)
			return 0;


		struct sockaddr_in inAddr;

		inAddr.sin_addr.S_un.S_addr=inet_addr(host.c_str());
		inAddr.sin_family=AF_INET;
		inAddr.sin_port=htons(port);
		if(inAddr.sin_addr.S_un.S_addr == INADDR_NONE)
		{
			struct hostent* phost = gethostbyname(host.c_str());
			if(phost == NULL)
				return 404;
			inAddr.sin_addr = *((struct in_addr*)phost->h_addr);
		}
		if(connect(sServer, (SOCKADDR*)&inAddr, sizeof(inAddr)) != 0)
		{
			//cerr<<"无法连接服务器"<<endl;
			closesocket(sServer);
			sServer=INVALID_SOCKET;
			strError.Format(_T("host = %s, port = %d"),
				convertstring(host).c_str(), port);
		}
	}else
	{
		sServer=ConnectToServer(host.c_str(), port, pszProxyip, proxyport);
		strError.Format(_T("host = %s, port = %d, proxyip = %s, proxyport = %d"),
			convertstring(host).c_str(), port, convertstring(pszProxyip).c_str(), proxyport);

	}
	if (sServer==INVALID_SOCKET)
	{
		//MessageBox(progDlg->GetSafeHwnd(), strError, _T("Error"), MB_OK);
		return 404;
	}
	
	string cmd;
	if(post=="")
	{
		cmd="GET "+remotepath+" HTTP/1.0\r\nHost: "+host+"\r\nUser-Agent:Tracq\r\n"+AdditionHead+"\r\n";
	}
	else
	{
		cmd="POST "+remotepath+" HTTP/1.0\r\nHost: "+host+"\r\nUser-Agent:Tracq\r\nContent-Length: "+ CConvert::IntToStr(post.length())+"\r\n"+AdditionHead+"\r\n"+post;
	}
	
	tstring str;
	str = GetModuleFileDir() + _T("\\postu.txt");
	CConvert::SaveFile(str, cmd.c_str(), cmd.length());//modify by zyl
	
	send(sServer, cmd.c_str(),cmd.length(), 0);
	
	int nHttpCode = 404;	  
	
	char *buf=new char[15535];  
	
	int nLenth = 0;   
	int nRecv;
	nRecv = recv(sServer, buf, 15534, 0);
	if (nRecv > 0)
	{
		buf[nRecv]=0;
		char* pStatue = strchr(buf,' ');
		char* headend = strstr(buf,"\r\n\r\n");
		*headend = '\0';
		headend += 4;
		nHttpCode = atoi(pStatue);
		if (nHttpCode != 200)
		{
			closesocket(sServer);
			delete [] buf;
			return nHttpCode;
		}
		
		pStatue = strstr(buf, "Content-Length:");
		pStatue += 15;
		nLenth = atoi(pStatue);  
		
		string strNewURL;
		char* pNewURL= strstr(buf, "Last-Modified:");
		if(pNewURL != NULL)
		{
			pNewURL+=14;
			while((*pNewURL)==' ')
			{
				pNewURL++;
			}
			char* pEnd=strchr(pNewURL,'\n'); 
			if(pEnd) 
				*pEnd='\0';
			pEnd = strchr(pNewURL,'\r');
			if(pEnd) 
				*pEnd='\0';
			//  LastModify = pNewURL;
		}
		
		int headlen=headend-buf;
		body = string(headend, nRecv - headlen); 
		
		nLenth = nLenth - nRecv + headlen;
		while(nLenth > 0)
		{
			nRecv = recv(sServer, buf, 15534, 0);
			if (nRecv<=0)
			{
				break;
			}
			buf[nRecv]=0;
			body += string(buf, nRecv);
			nLenth -= nRecv;
		}
		if (nLenth>0) 
		{
			nHttpCode=404;
		}
	}
	
	
	closesocket(sServer);
	delete [] buf;
    return nHttpCode;
}

string GetModuleFileDir()
{
	DWORD	dwLength, dwSize;
	TCHAR	szFileName [MAX_PATH];
	string	strFileName;
	int		nPos;
	
	dwSize = sizeof (szFileName) / sizeof (szFileName [0]);
	dwLength = ::GetModuleFileName (NULL, szFileName, dwSize);
	if (dwLength <= 0) 
	{
		return _T("");
	}

	strFileName = szFileName;
	nPos = strFileName.ReverseFind( '\\' );
	return strFileName.Left( nPos );
}

void GetHostInfo(const string& strHostInfo, string& strHost, UINT& nPort)
{
	int pos = strHostInfo.Find(_T(":"));

	strHost = strHostInfo;
	nPort = 80;

	if (pos != -1)
	{
		strHost = strHostInfo.Left(pos);
		string strPort = strHostInfo.Right(strHostInfo.GetLength() - pos - 1);
		nPort = (UINT)_ttoi(strPort);
	}
}

void GetRequestInfoFromUrl(const string& strUrl,
	string& strHost,
	UINT& nPort,
	string& strRequest)
{
	int pos, pos1;

	string strHttp = _T("http://");
	string strSp = _T("/");

	pos = strUrl.Find(strHttp);
	int httplen = strHttp.GetLength();
	if (pos != -1)
	{
		string strHostInfo;
		pos1 = strUrl.Find(strSp, pos + httplen);
		if (pos1 != -1)
		{
			strHostInfo = strUrl.Mid(pos + httplen, pos1 - pos - httplen);
			GetHostInfo(strHostInfo, strHost, nPort);
			strRequest = strUrl.Right(strUrl.GetLength() - pos1);
		}
	}
}


#define HTTP_RECV_DATA_BUF 10240
DWORD HttpDownloadFile(int nIndex,
	char* lpszUrl,
	char* lpszLocalName,
	char* lpszHeader,
	IHttpFileEvent* pEvent)
{
	DWORD dwRet = 0;

	string strUrl = lpszUrl;
	string strHost;
	string strRequest;
	UINT nPort;

	GetRequestInfoFromUrl(strUrl, strHost, nPort, strRequest);


	HINTERNET hInternet = InternetOpen(strHost,
		INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

	HINTERNET hConn = InternetConnect(hInternet,
		strHost,
		nPort,
		NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);

	static char* accept[2] = { _T("*/*"), NULL };
	HINTERNET hRequest = HttpOpenRequest(hConn, _T("GET"),
		strRequest, NULL, NULL,
		(char**)accept,
		INTERNET_FLAG_RELOAD,
		0);

	if (lpszHeader)
	{
		HttpAddRequestHeaders(hRequest, lpszHeader, -1L, HTTP_ADDREQ_FLAG_ADD);
	}

	BOOL bRet = HttpSendRequest(hRequest, NULL, 0, NULL, 0);

	TCHAR databuf[HTTP_RECV_DATA_BUF + 1] = { 0 };
	DWORD dwSize = HTTP_RECV_DATA_BUF;
	bRet = HttpQueryInfo(hRequest, HTTP_QUERY_STATUS_CODE, databuf, &dwSize, NULL);

	if (_tcscmp(databuf, _T("200")) == 0)
	{

		HANDLE hFile = CreateFile(lpszLocalName,
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			dwRet = 2;
			goto CleanUp;
		}

		dwSize = HTTP_RECV_DATA_BUF;
		bRet = HttpQueryInfo(hRequest, HTTP_QUERY_CONTENT_LENGTH, databuf, &dwSize, NULL);

		BOOL bKnowSize = FALSE;

		DWORD cbFileSize = HTTP_RECV_DATA_BUF;

		if (bRet != 0)
		{
			bKnowSize = TRUE;
			cbFileSize = (DWORD)_ttol(databuf);
		}

		DWORD dwFinishLen = 0;
		DWORD dwBytesRead = 0;

		do
		{
			dwBytesRead = 0;

			bRet = InternetReadFile(hRequest, databuf, HTTP_RECV_DATA_BUF, &dwBytesRead);

			if (dwBytesRead > 0)
			{
				DWORD dwBytesWritten;
				WriteFile(hFile, databuf, dwBytesRead, &dwBytesWritten, NULL);

				dwFinishLen += dwBytesWritten;

				if (pEvent)
				{
					pEvent->OnProgress(nIndex, dwFinishLen, cbFileSize);
				}
			}

		} while (dwBytesRead > 0);

		CloseHandle(hFile);

		if (pEvent)
		{
			pEvent->OnReturn(nIndex, lpszLocalName);
		}

	}
	else
	{
		dwRet = 1;
	}

CleanUp:
	InternetCloseHandle(hRequest);
	InternetCloseHandle(hConn);
	InternetCloseHandle(hInternet);

	if (dwRet != 0 && pEvent != NULL)
	{
		pEvent->OnError(nIndex, dwRet);
	}

	return dwRet;
}
