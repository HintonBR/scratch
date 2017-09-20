// test2Doc.h : interface of the CTest2Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEST2DOC_H__1F8D9BC7_7F53_40E8_A7B6_E2C01C35526F__INCLUDED_)
#define AFX_TEST2DOC_H__1F8D9BC7_7F53_40E8_A7B6_E2C01C35526F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTest2Doc : public CDocument
{
protected: // create from serialization only
	CTest2Doc();
	DECLARE_DYNCREATE(CTest2Doc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTest2Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTest2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTest2Doc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEST2DOC_H__1F8D9BC7_7F53_40E8_A7B6_E2C01C35526F__INCLUDED_)
