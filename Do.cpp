// Do.cpp: Implementierung der Klasse CDo.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CyberSolitaire.h"
#include "CyberSolitaireDoc.h"
#include "CyberSolitaireView.h"
#include "Do.h"
#include ".\do.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CDo, CObject, VERSIONABLE_SCHEMA | 1)

CDo::CDo()
: m_strAction(_T(""))
{
	m_Action   = NO_ACTION;
	m_pSrcPile = NULL;
	m_pDstPile = NULL;
	m_pCard    = NULL;
}

CDo::CDo(DO_ACTIONS Action, CPile* pSrcPile, CPile* pDstPile, CCard* pCard, BOOL ChangeLastCardFaceUp)
{
	m_Action   = Action;
	m_pSrcPile = pSrcPile;
	m_pDstPile = pDstPile;
	m_pCard    = pCard;
	m_ChangeLastCardFaceUp = ChangeLastCardFaceUp;
	GetActionString();
}

CDo::~CDo()
{

}


void CDo::Serialize(CArchive &ar)
{
	if (ar.IsStoring()) 
	{
		ar << m_ChangeLastCardFaceUp;
		ar.Write(&m_Action,sizeof(m_Action));
		ar.WriteObject(m_pSrcPile);
		ar.WriteObject(m_pDstPile);
		ar.WriteObject(m_pCard);
	}
	else 
	{
		ar >> m_ChangeLastCardFaceUp;
		ar.Read(&m_Action,sizeof(m_Action));
		m_pSrcPile = (CPile*)ar.ReadObject(RUNTIME_CLASS(CPile));
		m_pDstPile = (CPile*)ar.ReadObject(RUNTIME_CLASS(CPile));
		m_pCard    = (CCard*)ar.ReadObject(RUNTIME_CLASS(CCard));
	}
}


//////////////////////////////////////////////////////////////////////
// CDoControl Klasse
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CDoControl, CObject, VERSIONABLE_SCHEMA | 1)

CDoControl::CDoControl()
{
	m_CanRedo	= FALSE;
	m_StopLoop	= TRUE;
	m_InAutoPlay= FALSE;
	m_DoIndex	= -1;
	m_StartCnt  = 0;
	m_EndCnt	= 0;
}

CDoControl::CDoControl(CCyberSolitaireDoc* pDoc)
{
	m_CanRedo	= FALSE;
	m_StopLoop	= TRUE;
	m_InAutoPlay= FALSE;
	m_DoIndex	= -1;
	m_pDoc		= pDoc;
	m_StartCnt  = 0;
	m_EndCnt	= 0;
}

CDoControl::~CDoControl()
{
	// beim Mischen von Karten entstehen für das Redo/Undo neue Piles
	// diese werden jetzt entsorgt
	// alle eventuell vorhandenen Einträge löschen
	while (m_DoArr.GetUpperBound() >= 0)
	{
		CDo* pDo = m_DoArr.GetAt(m_DoArr.GetUpperBound());
		if (pDo->m_Action == MIX)
			delete pDo->m_pSrcPile;
		delete pDo;
		m_DoArr.RemoveAt(m_DoArr.GetUpperBound());
	}

}



void CDoControl::CreateDo(DO_ACTIONS Action, CPile* pSrcPile, CPile* pDstPile, CCard* pCard, BOOL ChangeLastCardFaceUp)
{
	// geschachtelte START/END Klammern werden ignoriert
	if (Action == START)
	{
		m_StartCnt++;
		if ((m_StartCnt - m_EndCnt) > 1)
			return;		// ignoriere START
	}
	if (Action == END)
	{
		m_EndCnt++;
		if ( m_StartCnt > m_EndCnt)
			return;		// ignoriere END
		if (m_EndCnt && m_StartCnt && m_EndCnt == m_StartCnt)
			m_StartCnt = m_EndCnt = 0;	// Zähler zurücksetzen
	}

	CDo* pDoNew = new CDo(Action,pSrcPile,pDstPile,pCard,ChangeLastCardFaceUp);
	if (m_CanRedo)
	{
		// ab jetzt kein Redo mehr möglich
		m_CanRedo = FALSE;
		// alle eventuell vorhandenen Einträge löschen
		while ((m_DoIndex+1) <= m_DoArr.GetUpperBound())
		{
			int index =	m_DoArr.GetUpperBound();
			CDo* pDo = m_DoArr.GetAt(m_DoArr.GetUpperBound());
			TRACE("Do Eintrag %s gelöscht Index %d UpperBound %d\n"
				,m_DoArr[index]->GetActionString(),index,m_DoArr.GetUpperBound());
			if (pDo->m_Action == MIX)
				delete pDo->m_pSrcPile;
			delete pDo;
			m_DoArr.RemoveAt(m_DoArr.GetUpperBound());
		}
	}
	m_DoArr.Add(pDoNew);
	m_DoIndex++;
	m_pDoc->SetModifiedFlag();
	TRACE("Do Eintrag %s zugefügt Index %d UpperBound %d\n",pDoNew->GetActionString(),m_DoIndex,m_DoArr.GetUpperBound());
}

void CDoControl::Undo()
{
	if (m_DoIndex < 0)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	CDo* pDo = m_DoArr[m_DoIndex];
	switch (pDo->m_Action) 
	{
	case START:
			if (!m_InAutoPlay)
				m_StopLoop = TRUE;
			break;
	case END:
		{
			if (!m_InAutoPlay)
			{
				m_StopLoop = FALSE;
				m_CanRedo = TRUE;
				m_DoIndex--;
				if (!m_pDoc->IsModified()) m_pDoc->SetModifiedFlag();
				TRACE("Undo durchgeführt Index %d UpperBound %d\n",m_DoIndex,m_DoArr.GetUpperBound());
				do
				{
					Undo();
				} 
				while (!m_StopLoop);
				// neu zeichnen
				if (m_StopLoop)
					m_pDoc->UpdateAllViews(NULL,HINT_UPDATE_PILES,NULL);
				return;
			}
			break;
		}
	case START_AUTOPLAY:
			m_InAutoPlay = FALSE;
			break;
	case END_AUTOPLAY:
		{
			m_InAutoPlay = TRUE;
			m_CanRedo = TRUE;
			m_DoIndex--;
			if (!m_pDoc->IsModified()) m_pDoc->SetModifiedFlag();
			TRACE("Undo durchgeführt Index %d UpperBound %d\n",m_DoIndex,m_DoArr.GetUpperBound());
			do
			{
				Undo();
			} 
			while (m_InAutoPlay);
			// neu zeichnen
			if (!m_InAutoPlay)
				m_pDoc->UpdateAllViews(NULL,HINT_UPDATE_PILES,NULL);
			return;
		}
	case MIX:
		{
			CCard* pSrcCard;
			CCard* pDstCard;
			// tauschen die Karten, um ein Redo möglich zu machen
			POSITION pos1 = pDo->m_pSrcPile->m_Cards.GetHeadPosition();
			POSITION pos2 = pDo->m_pDstPile->m_Cards.GetHeadPosition();
			while (pos1)
			{
				POSITION lastpos1 = pos1;
				POSITION lastpos2 = pos2;
				pSrcCard = pDo->m_pSrcPile->m_Cards.GetNext(pos1);
				pDstCard = pDo->m_pDstPile->m_Cards.GetNext(pos2);
				pDo->m_pDstPile->m_Cards.SetAt(lastpos2,pSrcCard);
				pDo->m_pSrcPile->m_Cards.SetAt(lastpos1,pDstCard);
			}
			m_pDoc->GetView()->m_UpdateRect.UnionRect(m_pDoc->GetView()->m_UpdateRect,pDo->m_pDstPile->GetRect());
			break;
		}
	/*
	case PUT_DEAL:
	case PUT:
		{
			if (!pDo->m_pDstPile->IsEmpty() && (pDo->m_Action == PUT))
				pDo->m_pDstPile->m_Cards.GetTail()->SetFaceUp(pDo->m_LastCardFaceUp);
			pDo->m_pDstPile->Get(pDo->m_pSrcPile,pDo->m_pCard,m_pDoc->GetView()->m_UpdateRect);
			if (!pDo->m_pDstPile->IsEmpty() && (pDo->m_Action == PUT_STOCK))
				pDo->m_pDstPile->m_Cards.GetTail()->SetFaceUp(pDo->m_LastCardFaceUp);
			m_pDoc->GetView()->m_UpdateRect.UnionRect(m_pDoc->GetView()->m_UpdateRect,pDo->m_pSrcPile->GetRect());
			m_pDoc->GetView()->m_UpdateRect.UnionRect(m_pDoc->GetView()->m_UpdateRect,pDo->m_pDstPile->GetRect());
			break;
		}
	*/
	case GET_DEAL:
	case GET:
		{
			if (pDo->m_Action == GET) 
				ASSERT(pDo->m_pSrcPile != pDo->m_pDstPile);
			if (!pDo->m_pSrcPile->IsEmpty() && (pDo->m_Action == GET) && pDo->m_ChangeLastCardFaceUp)
				pDo->m_pSrcPile->m_Cards.GetTail()->SetFaceUp(!pDo->m_pSrcPile->m_Cards.GetTail()->GetFaceUp());
			pDo->m_pDstPile->Put(pDo->m_pSrcPile,pDo->m_pCard,m_pDoc->GetView()->m_UpdateRect);
			if (!pDo->m_pSrcPile->IsEmpty() && (pDo->m_Action == GET_DEAL) && pDo->m_ChangeLastCardFaceUp)
				pDo->m_pSrcPile->m_Cards.GetTail()->SetFaceUp(!pDo->m_pSrcPile->m_Cards.GetTail()->GetFaceUp());
			m_pDoc->GetView()->m_UpdateRect.UnionRect(m_pDoc->GetView()->m_UpdateRect,pDo->m_pSrcPile->GetRect());
			m_pDoc->GetView()->m_UpdateRect.UnionRect(m_pDoc->GetView()->m_UpdateRect,pDo->m_pDstPile->GetRect());
			break;
		}
	case CHANGE_CARD_FACEUP:
		{
			// eine Karte wurde umgedreht
			// m_ChangeLastCardFaceUp ist hier der neue Wert nach dem umdrehen
			pDo->m_pCard->SetFaceUp(!pDo->m_ChangeLastCardFaceUp);
			m_pDoc->GetView()->m_UpdateRect.UnionRect(m_pDoc->GetView()->m_UpdateRect,pDo->m_pSrcPile->GetRect());
			break;
		}
	case REVERT:
		{
			pDo->m_pSrcPile->Revert();
			break;
		}
	case REDEAL:
		{
			m_pDoc->m_CountOfRedeals--;
			break;
		}
	default:
		break;
	}
	// neu zeichnen
	if (m_StopLoop || !m_InAutoPlay)
		m_pDoc->UpdateAllViews(NULL,HINT_UPDATE_PILES,NULL);
	m_CanRedo = TRUE;
	m_DoIndex--;
	if (!m_pDoc->IsModified()) m_pDoc->SetModifiedFlag();
	TRACE("Undo durchgeführt Index %d UpperBound %d\n",m_DoIndex,m_DoArr.GetUpperBound());
}

void CDoControl::Redo()
{
	if (!m_CanRedo)
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	if (m_DoIndex >= m_DoArr.GetUpperBound())
	{
		MessageBeep(MB_ICONHAND);
		return;
	}
	m_DoIndex++;
	m_pDoc->SetModifiedFlag();
	CDo* pDo = m_DoArr[m_DoIndex];
	BOOL Repeat = FALSE;
	switch (pDo->m_Action) 
	{
	case START:
		{
			if (!m_InAutoPlay)
			{
				m_StopLoop = FALSE;
				do
				{
					Redo();
				} 
				while (!m_StopLoop);
			}
			break;
		}
	case END:
			if (!m_InAutoPlay)
				m_StopLoop = TRUE;
			break;
	case START_AUTOPLAY:
		{
			m_InAutoPlay = TRUE;
			do
			{
				Redo();
			} 
			while (m_InAutoPlay);
			break;
		}
	case END_AUTOPLAY:
			m_InAutoPlay = FALSE;
			break;
	case MIX:
		{
			CCard* pSrcCard;
			CCard* pDstCard;
			// tauschen die Karten, um ein Undo möglich zu machen
			POSITION pos1 = pDo->m_pSrcPile->m_Cards.GetHeadPosition();
			POSITION pos2 = pDo->m_pDstPile->m_Cards.GetHeadPosition();
			while (pos1)
			{
				POSITION lastpos1 = pos1;
				POSITION lastpos2 = pos2;
				pSrcCard = pDo->m_pSrcPile->m_Cards.GetNext(pos1);
				pDstCard = pDo->m_pDstPile->m_Cards.GetNext(pos2);
				pDo->m_pDstPile->m_Cards.SetAt(lastpos2,pSrcCard);
				pDo->m_pSrcPile->m_Cards.SetAt(lastpos1,pDstCard);
			}
			m_pDoc->GetView()->m_UpdateRect.UnionRect(m_pDoc->GetView()->m_UpdateRect,pDo->m_pDstPile->GetRect());
			break;
		}
	/*
	case PUT_DEAL:
	case PUT:
		{
			if (!pDo->m_pDstPile->IsEmpty() && (pDo->m_Action == PUT_DEAL))
				pDo->m_pDstPile->m_Cards.GetTail()->SetFaceUp(!pDo->m_LastCardFaceUp);
			pDo->m_pSrcPile->Get(pDo->m_pDstPile,pDo->m_pCard,m_pDoc->GetView()->m_UpdateRect);
			if (!pDo->m_pDstPile->IsEmpty() && (pDo->m_Action == PUT))
				pDo->m_pDstPile->m_Cards.GetTail()->SetFaceUp(!pDo->m_LastCardFaceUp);
			m_pDoc->GetView()->m_UpdateRect.UnionRect(m_pDoc->GetView()->m_UpdateRect,pDo->m_pSrcPile->GetRect());
			m_pDoc->GetView()->m_UpdateRect.UnionRect(m_pDoc->GetView()->m_UpdateRect,pDo->m_pDstPile->GetRect());
			break;
		}
	*/
	case GET_DEAL:
	case GET:
		{
			if (!pDo->m_pSrcPile->IsEmpty() && (pDo->m_Action == GET_DEAL) && pDo->m_ChangeLastCardFaceUp)
				pDo->m_pSrcPile->m_Cards.GetTail()->SetFaceUp(!pDo->m_pSrcPile->m_Cards.GetTail()->GetFaceUp());
			pDo->m_pSrcPile->Put(pDo->m_pDstPile,pDo->m_pCard,m_pDoc->GetView()->m_UpdateRect);
			if (!pDo->m_pSrcPile->IsEmpty() && (pDo->m_Action == GET) && pDo->m_ChangeLastCardFaceUp)
				pDo->m_pSrcPile->m_Cards.GetTail()->SetFaceUp(!pDo->m_pSrcPile->m_Cards.GetTail()->GetFaceUp());
			m_pDoc->GetView()->m_UpdateRect.UnionRect(m_pDoc->GetView()->m_UpdateRect,pDo->m_pSrcPile->GetRect());
			m_pDoc->GetView()->m_UpdateRect.UnionRect(m_pDoc->GetView()->m_UpdateRect,pDo->m_pDstPile->GetRect());
			break;
		}
	case CHANGE_CARD_FACEUP:
		{
			// eine Karte wurde umgedreht
			// m_ChangeLastCardFaceUp ist hier der neue Wert nach dem umdrehen
			pDo->m_pCard->SetFaceUp(pDo->m_ChangeLastCardFaceUp);
			m_pDoc->GetView()->m_UpdateRect.UnionRect(m_pDoc->GetView()->m_UpdateRect,pDo->m_pSrcPile->GetRect());
			break;
		}
	case REVERT:
		{
			pDo->m_pSrcPile->Revert();
			break;
		}
	case REDEAL:
		{
			m_pDoc->m_CountOfRedeals++;
			break;
		}
	default:
		break;
	}
	// neu zeichnen
	if (m_StopLoop)
		m_pDoc->UpdateAllViews(NULL,HINT_UPDATE_PILES,NULL);
	//TRACE("Redo durchgeführt Index %d UpperBound %d\n",m_DoIndex,m_DoArr.GetUpperBound());
}

void CDoControl::Serialize(CArchive &ar)
{
	m_DoArr.Serialize(ar);
	if (ar.IsStoring()) 
	{
		ar << m_CanRedo << m_StopLoop << m_DoIndex;
	}
	else 
	{
		ar >> m_CanRedo >> m_StopLoop >> m_DoIndex;
	}
}


//////////////////////////////////////////////////////////////////////////
// es wird ein Start Do vor dem letzten Eintrag eingefügt
//
void CDoControl::InsertStartDo()
{
	// geschachtelte START/END Klammern werden ignoriert
	m_StartCnt++;
	if (m_StartCnt > m_EndCnt)
		return;		// ignoriere START
	CDo* pDoNew = new CDo(START,NULL,NULL,NULL,NULL);
	int Index =	m_DoArr.GetUpperBound();
	if (Index == -1) Index = 0;
	m_DoArr.InsertAt(Index,pDoNew);
	m_DoIndex++;
	m_pDoc->SetModifiedFlag();
	TRACE("Start Do Eintrag %s zugefügt Index %d UpperBound %d\n",pDoNew->GetActionString(),m_DoIndex-1,m_DoArr.GetUpperBound());

}

void CDoControl::UndoAll()
{
	while (m_DoIndex >= 0)
		Undo();
}

// liefert die Action als String
CString& CDo::GetActionString(void)
{
	switch (m_Action)
	{	
	case NO_ACTION:
		m_strAction = "NO_ACTION";
		break;
	case START:
		m_strAction = "START";
		break;
	case END:
		m_strAction = "END";
		break;
	case PUT:
		m_strAction = "PUT";
		break;
	case GET:
		m_strAction = "GET";
		break;
	case MIX:
		m_strAction = "MIX";
		break;
	case PUT_DEAL:
		m_strAction = "PUT_DEAL";
		break;
	case GET_DEAL:
		m_strAction = "GET_DEAL";
		break;
	case CHANGE_CARD_FACEUP:
		m_strAction = "CHANGE_CARD_FACEUP";
		break;
	case REVERT:
		m_strAction = "REVERT";
		break;
	case REDEAL:
		m_strAction = "REDEAL";
		break;
	case START_AUTOPLAY:
		m_strAction = "START_AUTOPLAY";
		break;
	case END_AUTOPLAY:
		m_strAction = "END_AUTOPLAY";
		break;
	default:
		m_strAction = "unbekannt";
		break;
	}

	return m_strAction;
}

// unterdrückt während einer laufenden START END Klammer neue START END Klammern
void CDoControl::SuppressNewStartEnd(void)
{
}
