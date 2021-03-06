//===========================================================================
#ifndef M_TimeKeyH
#define M_TimeKeyH
//===========================================================================
#ifndef mpiElementH
#include "mpiElement.h"
#endif
//===========================================================================

class TTimeKey : public TElement {
private:
        TFloat t1, t2, t3, a, b;
public:
        TTimeKey();
       ~TTimeKey();

        //virtual void     Draw(TCanvas *aCanvas, int aParentId);
        virtual void     SaveToStream(TStream *aStream);
        virtual void     LoadFromStream(TStream *aStream);
        virtual void     DrawElementFace();
        virtual void     Edit();
        virtual void     Copy(void *aElement);
        virtual void     Init();
        virtual void     InitBeforeRun(){};
        virtual void     Run();
        virtual void     DoneRun(){};
        static  TTimeKey *Create() { return new TTimeKey(); };
};
//===========================================================================
#endif
