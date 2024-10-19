#pragma once

class COPClientDlg;
class UnDo
{
public:
	UnDo(COPClientDlg *pDlg) :m_pDlg(pDlg)	{}
	~UnDo()	{}

	void	AnalysisUndo(char *pData);
	void	AnalysisRedo(char *pData);


	void	RedoText(char *pData);
	void    UndoText(char *pData);

public:
	COPClientDlg	*m_pDlg;
};