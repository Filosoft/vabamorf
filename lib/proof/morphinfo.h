#if !defined _MORPHINFO_H_DFGAVADFGERFSVSDGERRWEFSFFG_
#define _MORPHINFO_H_DFGAVADFGERFSVSDGERRWEFSFFG_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMorphInfo {
public:
	CMorphInfo() : m_cPOS(0) { }

	CFSWString m_szRoot;
	CFSWString m_szEnding;
	CFSWString m_szClitic;
	wchar_t m_cPOS;
	CFSWString m_szForm;
};

#endif // _MORPHINFO_H_DFGAVADFGERFSVSDGERRWEFSFFG_
