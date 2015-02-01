#ifndef MOVE_H
#define MOVE_H

class Move
{
    public:
        void SetFromX(int nX) { m_nFromX = nX; }
        void SetFromY(int nY) { m_nFromX = nY; }
        void SetToX(int nX)   { m_nFromX = nX; }
        void SetToY(int nY)   { m_nFromX = nY; }

        int FromX() { return m_nFromX; }
        int FromY() { return m_nFromY; }
        int ToX()   { return m_nToX; }
        int ToY()   { return m_nToY; }

        bool ValidMove() { return m_bValidMove; }

    private:
        int m_nFromX {0};
        int m_nFromY {0};
        int m_nToX   {0};
        int m_nToY   {0};

        bool m_bValidMove {false};
};

#endif // MOVE_H
