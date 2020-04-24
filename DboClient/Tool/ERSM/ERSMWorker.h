#pragma once

/// 에러 리포트 파일 구조체
struct SErrorReportFile
{
	std::string m_strFolderName;	///< 파일이 위치한 폴더명
	std::string	m_strFileName;		///< 파일 이름 (폴더 제외)
	std::string m_strUserID;		///< 유저 ID
	std::string m_strFileDate;		///< 파일 생성 날짜 (20070130 형식)
};

/// 실제 스레드로 작동하면서 작업을 처리하는 클래스
class CERSMWorker
{
public:
	CERSMWorker(void);
	virtual ~CERSMWorker(void);

public:
	bool ProcessErrReports();										///< 에러 리포트 파일들의 처리를 수행한다.
	
protected:
	bool GetListZipFiles(const std::string& strFolderName);			///< 폴더로부터 Zip파일을 가져와서 벡터를 생성한다.
	std::string GetUserIDFromZipFile(const std::string& strZipFileName);	///< Zip파일로부터 유저 ID를 가져온다.
	bool RenameZipFiles();											///< ZipFile들의 이름을 변경한다.	

	void Clear();													///< 벡터내의 내용을 비운다.
	string GetTime();												///< 현재 시간을 문자열로 반환한다.

protected:
	std::vector<SErrorReportFile*>	m_vecErrorReportFiles;			///< 에러 리포트 파일 구조체들의 벡터
};
