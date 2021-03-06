//===========================================================================
#ifndef M_MultiplierH
#define M_MultiplierH
//===========================================================================
#ifndef mpiElementH
#include "mpiElement.h"
#endif
//===========================================================================

class TMultiplier : public TElement {
private:
        TFloat  NullValue;
        int     fCountInput;

        void __fastcall Set_CountInput(int aCountInput);
public:
        TMultiplier();
       ~TMultiplier();

        virtual void     Draw(TCanvas *aCanvas, int aParentId);
        virtual void     SaveToStream(TStream *aStream);
        virtual void     LoadFromStream(TStream *aStream);
        virtual void     DrawElementFace();
        virtual void     Edit();
        virtual void     Copy(void *aElement);
        virtual void     Init();
        virtual void     InitBeforeRun(){};
        virtual void     Run();
        virtual void     DoneRun(){};
        static  TMultiplier *Create() { return new TMultiplier(); };

        __property int     CountInput      = { read = fCountInput     , write = Set_CountInput   };
};
//===========================================================================
#endif
