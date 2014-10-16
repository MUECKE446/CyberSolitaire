// Do.h: Schnittstelle für die Klasse CDo.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DO_H__1EB5C4B3_46DC_11D6_976B_00A0CC562BEB__INCLUDED_)
#define AFX_DO_H__1EB5C4B3_46DC_11D6_976B_00A0CC562BEB__INCLUDED_

#if !defined(AFX_CARDS_H__741739C5_7F1D_11D4_A917_0000B48B263B__INCLUDED_)
#include "Card.h"
#endif

#if !defined(PILES_H_INCLUDED_)
#include "Piles.h"
#endif


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// typedefs
//

// welche Aktionen werden ausgeführt
enum DO_ACTIONS {
	NO_ACTION,
	START,
	END,
	PUT,
	GET,
	MIX,
	PUT_DEAL,
	GET_DEAL,
	CHANGE_CARD_FACEUP,
	REVERT,
	REDEAL,
	START_AUTOPLAY,
	END_AUTOPLAY
};


class CDo : public CObject  
{
    DECLARE_SERIAL(CDo)
public:
	CDo();
	CDo(DO_ACTIONS Action, CPile* pSrcPile, CPile* pDstPile, CCard* pCard, BOOL ChangeLastCardFaceUp);
	virtual ~CDo();

public:

	virtual void Serialize(CArchive& ar);
	
	DO_ACTIONS	m_Action;
	CPile*		m_pSrcPile;
	CPile*		m_pDstPile;
	CCard*		m_pCard;
	BOOL		m_ChangeLastCardFaceUp;
	// die Action als String
	CString m_strAction;
	// liefert die Action als String
	CString& GetActionString(void);
};


class CDoControl : public CObject  
{
    DECLARE_SERIAL(CDoControl)
public:
	void UndoAll();
	void InsertStartDo();
	CDoControl();
	CDoControl(CCyberSolitaireDoc* pDoc);
	virtual ~CDoControl();

	BOOL				m_CanRedo;
	BOOL				m_StopLoop;
	BOOL				m_InAutoPlay;
	int					m_DoIndex;
	CDoArray			m_DoArr;
	CCyberSolitaireDoc* m_pDoc;
	int					m_StartCnt;
	int					m_EndCnt;
	
	void CreateDo(DO_ACTIONS Action, CPile* pSrcPile, CPile* pDstPile, CCard* pCard, BOOL ChangeLastCardFaceUp);
	void Undo();
	void Redo();

	virtual void Serialize(CArchive& ar);

	// unterdrückt während einer laufenden START END Klammer neue START END Klammern
	void SuppressNewStartEnd(void);
};

#endif // !defined(AFX_DO_H__1EB5C4B3_46DC_11D6_976B_00A0CC562BEB__INCLUDED_)
