//===========================================================================
#ifndef M_TargetMoveH
#define M_TargetMoveH
//===========================================================================
#ifndef mpiElementH
#include "mpiElement.h"
#endif
class TTargetTraektory ;
//===========================================================================
class TTargetMove : public TElement {
private:
     // ----------------  ������� ��������� ------------
        TFloat H0;
        TFloat Betta0;
        TFloat V0;
        TFloat R0;
        TFloat K0;
        TFloat t1; // --- ����� �� ��������� -----
        TFloat GLoad;       // ---- ���������� �� ��������� ------
        TFloat fi; // ---- ���� ��������� ----
        TFloat radius;      // ---- ������ ��������� *-----
        TFloat a1;      // ---- ��������� �� 1 ������� ------
        TFloat a3;      // ---- ��������� �� 3 ������� ------

        TFloat sigma1;   // ---- ��� � ������ ��������� ----
        TFloat sigma2;   // ---- ��� � ������������ ������ ----
        TFloat sigma3;   // ---- ��� � ����������� ������ ---
     // ----------------
        TTargetTraektory *TargetTraektory;
public:
        TTargetMove();
        virtual ~TTargetMove();



        virtual void     SaveToStream(TStream *aStream);
        virtual void     LoadFromStream(TStream *aStream);
        virtual void     DrawElementFace();
        virtual void     Edit();
        virtual void     Copy(void *aElement);
        virtual void     Init();
        virtual void     InitBeforeRun();
        virtual void     Run();
        virtual void     DoneRun(){};
        static  TTargetMove *Create() { return new TTargetMove(); };
};
//===========================================================================
#endif
