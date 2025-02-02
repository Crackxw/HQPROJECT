//==================================================================
//   CellRange.h
//  Control Cell in GridCtrl
//
// Produced   by    Written by Chris Maunder  (Copyright (c) 1998.)
//==================================================================

#ifndef CELL_RANGE_H
#define CELL_RANGE_H


///==============================================================================
// CCellID Class
//	 class for Cell ID (Identifying Cell)
//==============================================================================
class CCellID
{    
// Attributes
public:
    int row, col;

// Operations
public:
    CCellID(int nRow = -1, int nCol = -1) : row(nRow), col(nCol) {}

    int IsValid() const { return (row >= 0 && col >= 0); }
    int operator==(const CCellID& rhs)    { return (row == rhs.row && col == rhs.col); }
    int operator!=(const CCellID& rhs)    { return !operator==(rhs); }
};

///==============================================================================
// CCellRange Class
//	 class for Ranging Cell
//==============================================================================
class CCellRange
{ 
public:
    
    CCellRange(int nMinRow = -1, int nMinCol = -1, int nMaxRow = -1, int nMaxCol = -1)
    {
        Set(nMinRow, nMinCol, nMaxRow, nMaxCol);
    }

    void Set(int nMinRow = -1, int nMinCol = -1, int nMaxRow = -1, int nMaxCol = -1);
    
    int  IsValid() const;
    int  InRange(int row, int col) const;
    int  InRange(const CCellID& cellID) const;
    int  Count() { return (m_nMaxRow - m_nMinRow + 1) * (m_nMaxCol - m_nMinCol + 1); }
    
    CCellID  GetTopLeft() const;
    CCellRange  Intersect(const CCellRange& rhs) const;
    
    int GetMinRow() const {return m_nMinRow;}
    void SetMinRow(int minRow) {m_nMinRow = minRow;}
    
    int GetMinCol() const {return m_nMinCol;}
    void SetMinCol(int minCol) {m_nMinCol = minCol;}
    
    int GetMaxRow() const {return m_nMaxRow;}
    void SetMaxRow(int maxRow) {m_nMaxRow = maxRow;}
    
    int GetMaxCol() const {return m_nMaxCol;}
    void SetMaxCol(int maxCol) {m_nMaxCol = maxCol;}

    int GetRowSpan() const {return m_nMaxRow - m_nMinRow + 1;}
    int GetColSpan() const {return m_nMaxCol - m_nMinCol + 1;}
    
    int  operator==(const CCellRange& rhs);
    int  operator!=(const CCellRange& rhs);
    
protected:
    int m_nMinRow;
    int m_nMinCol;
    int m_nMaxRow;
    int m_nMaxCol;
};

inline void CCellRange::Set(int minRow, int minCol, int maxRow, int maxCol)
{
     m_nMinRow = minRow;
     m_nMinCol = minCol;
     m_nMaxRow = maxRow;
     m_nMaxCol = maxCol;
}

inline int CCellRange::operator==(const CCellRange& rhs)
{
     return ((m_nMinRow == rhs.m_nMinRow) && (m_nMinCol == rhs.m_nMinCol) &&
             (m_nMaxRow == rhs.m_nMaxRow) && (m_nMaxCol == rhs.m_nMaxCol));
}

inline int CCellRange::operator!=(const CCellRange& rhs)
{
     return !operator==(rhs);
}

inline int CCellRange::IsValid() const
{
     return (m_nMinRow >= 0 && m_nMinCol >= 0 && m_nMaxRow >= 0 && m_nMaxCol >= 0 &&
             m_nMinRow <= m_nMaxRow && m_nMinCol <= m_nMaxCol);
}

inline int CCellRange::InRange(int row, int col) const
{
     return (row >= m_nMinRow && row <= m_nMaxRow && col >= m_nMinCol && col <= m_nMaxCol);
}

inline int CCellRange::InRange(const CCellID& cellID) const
{
     return InRange(cellID.row, cellID.col);
}

inline CCellID CCellRange::GetTopLeft() const
{
     return CCellID(m_nMinRow, m_nMinCol);
}

inline CCellRange CCellRange::Intersect(const CCellRange& rhs) const
{
     return CCellRange(max(m_nMinRow,rhs.m_nMinRow), max(m_nMinCol,rhs.m_nMinCol),
                       min(m_nMaxRow,rhs.m_nMaxRow), min(m_nMaxCol,rhs.m_nMaxCol));
}

#endif 