#include <GSL.h>

#include "Graphics16.h"
#include "MemMgr.h"



MemMgr::MemMgr(UI32 MBMemSize, UI16 uiListSize)
{
	UI32 MemSize = MBMemSize*1000000;
	m_pMemBuffer = new BYTE[MemSize];
	ZeroMemory(m_pMemBuffer, sizeof(BYTE)*MemSize);

	m_MemList.Create(uiListSize);
	m_EmptyList.Create(uiListSize);
	MemInfo TempMemInfo;
	TempMemInfo.uiMemPos		= m_pMemBuffer;
	TempMemInfo.uiAllocLength	= 0;
	TempMemInfo.uiEmptyLength	= MemSize;
	TempMemInfo.bMgrAlloc		= TRUE;
	m_MemList.Add(&TempMemInfo);

	SI32 Index = 0;
	m_EmptyList.Add(&Index);
}

MemMgr::~MemMgr()
{
	if(m_pMemBuffer)
	{
		delete [] m_pMemBuffer;
		m_pMemBuffer = NULL;
	}
}

BOOL MemMgr::AllocMem(char* filename, XSPR& xspr)
{
	int version;
	FILE *fp;

	fp=fopen(filename, "rb");
	if(!fp)
	{
		MessageBox(NULL, "File Open Failure", filename, MB_OK);
		return FALSE;
	}

	fread(&version, 4, 1, fp);
	if(version!=XSPR_VERSION)
	{
		MessageBox(NULL, "Version Mismatch", "", MB_OK);
		return FALSE;
	}

    fseek(fp, 0, SEEK_SET);
    fread(&xspr.Header, sizeof(XSPR_HEADER), 1, fp);

	// 데이타를 할당할 공간을 찾는다.
	SI16 siEmptyIndex;
	MemInfo *pEmptyMemInfo = FindEmptyMemInfo(xspr.Header.TotalLength, siEmptyIndex);

	if(pEmptyMemInfo != NULL)
	{
		xspr.Image = pEmptyMemInfo->uiMemPos + pEmptyMemInfo->uiAllocLength;
		fread(xspr.Image, xspr.Header.TotalLength, 1, fp);

		if(pEmptyMemInfo->uiAllocLength == 0) 
		{
			pEmptyMemInfo->uiAllocLength = xspr.Header.TotalLength;
			pEmptyMemInfo->uiEmptyLength -= xspr.Header.TotalLength;

			if(!pEmptyMemInfo->uiEmptyLength)
			{
				SI32 Index = siEmptyIndex;
				m_EmptyList.Delete(&Index);
			}
		}
		else
		{
			MemInfo TempMemInfo;
			TempMemInfo.uiMemPos		= xspr.Image;
			TempMemInfo.uiAllocLength	= xspr.Header.TotalLength;
			TempMemInfo.uiEmptyLength	= pEmptyMemInfo->uiEmptyLength - xspr.Header.TotalLength;
			TempMemInfo.bMgrAlloc		= TRUE;

			pEmptyMemInfo->uiEmptyLength= 0;
			m_MemList.Insert(siEmptyIndex, &TempMemInfo);

			int *TempIndex;
			m_EmptyList.ResetPos();
			while((TempIndex = m_EmptyList.GetNextData()) != NULL)
			{
				if(*TempIndex > siEmptyIndex)
					*TempIndex += 1;
			}
				
			SI32 Index = siEmptyIndex;
			m_EmptyList.Delete(&Index);

			if(TempMemInfo.uiEmptyLength)
			{
				Index = siEmptyIndex + 1;
				m_EmptyList.Add(&Index);
			}
		}
	}
	else
	{
		xspr.Image = new unsigned char[xspr.Header.TotalLength];
		fread(xspr.Image, xspr.Header.TotalLength, 1, fp);

		MemInfo TempMemInfo;
		TempMemInfo.uiMemPos		= xspr.Image;
		TempMemInfo.uiAllocLength	= xspr.Header.TotalLength;
		TempMemInfo.uiEmptyLength	= 0;
		TempMemInfo.bMgrAlloc		= FALSE;
		m_MemList.Add(&TempMemInfo);
	}

    fclose(fp);
	xspr.bLoadComplete = true;

	return TRUE;
}


MemInfo* MemMgr::FindEmptyMemInfo(UI32 uiLength, SI16 &index)
{
	int *TempIndex;
	MemInfo *pEmptyMemInfo;
	m_EmptyList.ResetPos();
	while((TempIndex = m_EmptyList.GetNextData()) != NULL)
	{
		pEmptyMemInfo = m_MemList.GetData(*TempIndex);
		if(pEmptyMemInfo->uiEmptyLength >= uiLength)
		{
			index = *TempIndex;
			return pEmptyMemInfo;
		}
	}

	return NULL;
}

BOOL MemMgr::Free(XSPR& xspr)
{
	if(xspr.Image==NULL)	return FALSE;

	MemInfo *pEmptyMemInfo;
	m_MemList.ResetPos();
	while((pEmptyMemInfo = m_MemList.GetNextData()) != NULL)
	{
		if(pEmptyMemInfo->uiMemPos == xspr.Image)
		{
			if(pEmptyMemInfo->bMgrAlloc)
			{
				MemInfo *pPrevMemInfo = m_MemList.GetPrevData();
				if(pPrevMemInfo)
				{
					if(pEmptyMemInfo->uiEmptyLength)
					{
						SI32 Index = m_MemList.GetCurPos();
						m_EmptyList.Delete(&Index);
					}

					if(pPrevMemInfo->uiEmptyLength == 0)
					{
						SI32 Index = m_MemList.GetCurPos();
						m_EmptyList.Add(&Index);
					}

					int *TempIndex;
					m_EmptyList.ResetPos();
					while((TempIndex = m_EmptyList.GetNextData()) != NULL)
					{
						if(*TempIndex > (m_MemList.GetCurPos()+1))
							*TempIndex -= 1;
					}

					ZeroMemory(pEmptyMemInfo->uiMemPos, pEmptyMemInfo->uiAllocLength);
					pPrevMemInfo->uiEmptyLength += (pEmptyMemInfo->uiAllocLength + pEmptyMemInfo->uiEmptyLength);
					m_MemList.Delete(pEmptyMemInfo);
				}
				else
				{
					if(pEmptyMemInfo->uiEmptyLength == 0)
					{
						SI32 Index = 0;
						m_EmptyList.Add(&Index);
					}

					ZeroMemory(pEmptyMemInfo->uiMemPos, pEmptyMemInfo->uiAllocLength);
					pEmptyMemInfo->uiEmptyLength += pEmptyMemInfo->uiAllocLength;
					pEmptyMemInfo->uiAllocLength = 0;
				}
				xspr.Image = NULL;
				xspr.bLoadComplete = false;
			}
			else
				clGrp.FreeXspr(xspr);
			return TRUE;
		}
	}

	return FALSE;
}