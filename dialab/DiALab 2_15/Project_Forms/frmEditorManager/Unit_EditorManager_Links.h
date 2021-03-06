void TfrmEditorManager::CreateLink (int aParentId)
{
     LinkList = (TLink**) realloc (LinkList, sizeof(TLink*) * (++LinkList_Count));

     LinkList[LinkList_Count - 1] = new TLink();
     LinkList[LinkList_Count - 1]->ParentId        = aParentId;
     LinkList[LinkList_Count - 1]->ClassName       = typeid(*LinkList[LinkList_Count - 1]).name();
     LinkList[LinkList_Count - 1]->Id              = ProjectParam.GlobalElementCounter++;
     LinkList[LinkList_Count - 1]->OffsetX         = sbHorizontal->Position;
     LinkList[LinkList_Count - 1]->OffsetY         = sbVertical->Position;
     LinkList[LinkList_Count - 1]->Conecting       = true;

     ProjectModified = true;
}

//===========================================================================
void TfrmEditorManager::CreateLink_SetFirstElement (int aNumberOut, TElement *aFirstElement)
{
     LinkList[LinkList_Count - 1]->FirstElement    = aFirstElement;
     LinkList[LinkList_Count - 1]->IdFirstElement  = aFirstElement->Id;
     LinkList[LinkList_Count - 1]->NumberOutput    = aNumberOut;

     aFirstElement->oPoints->Items[aNumberOut]->CurrentLink = LinkList[LinkList_Count - 1];

     AddLinkPoint(aFirstElement->VisualElementParam->sLeft + aFirstElement->oPoints->Items[aNumberOut]->xPos,
                  aFirstElement->VisualElementParam->sTop  + aFirstElement->oPoints->Items[aNumberOut]->yPos);

}
//===========================================================================
void TfrmEditorManager::CreateLink_SetSecondElement(int aNumberIn , TElement *aSecondElement)
{
     LinkList[LinkList_Count - 1]->SecondElement   = aSecondElement;
     LinkList[LinkList_Count - 1]->IdSecondElement = aSecondElement->Id;
     LinkList[LinkList_Count - 1]->NumberInput     = aNumberIn;
     LinkList[LinkList_Count - 1]->Conecting       = false;
     AddLinkPoint(aSecondElement->VisualElementParam->sLeft + aSecondElement->iPoints->Items[aNumberIn]->xPos,
                  aSecondElement->VisualElementParam->sTop  + aSecondElement->iPoints->Items[aNumberIn]->yPos);

     aSecondElement->iPoints->Items[aNumberIn]->CurrentLink = LinkList[LinkList_Count - 1];

     aSecondElement->iPoints->Items[aNumberIn]->Conected = true;
     aSecondElement->NeedRedraw                          = true;

   // ---- ���� ������ ������� "�����" --- ����� ������ ����� ����� -------
      if (aSecondElement->ClassName == "TOutElementStandart") {
         TOutElementStandart *oes = (TOutElementStandart*)aSecondElement;
         LinkList[LinkList_Count - 1]->LinkPenColor = oes->mpiSeries[aNumberIn]->Color;
      }

}
//===========================================================================
void TfrmEditorManager::BeginLink(TElement *aMouseElement)
{
  // ------ ������ ����� ����� -----
     CreateLink(aMouseElement->ParentId);
     CreateLink_SetFirstElement (mp.NumberOut, aMouseElement);

  // ---------- �������� �������� ������ �������� , ��� �� ��� ��������� ----
     aMouseElement->oPoints->Items[mp.NumberOut]->Conected = true;

  // ----
     WhatDo = wdLink;
}

//===========================================================================
void TfrmEditorManager::AddLinkPoint(int aSx, int aSy)
{
     LinkList[LinkList_Count - 1]->AddPoint(aSx + sbHorizontal->Position,
                                            aSy + sbVertical->Position  );
}

//===========================================================================
void TfrmEditorManager::EndLink(TElement *aMouseElement)
{
     CreateLink_SetSecondElement(mp.NumberIn, aMouseElement);
  // ---------------- ��������� --------
     Draw();
  // ----
     WhatDo = wdNone;
}

//===========================================================================
void TfrmEditorManager::EndLinkWhenMouseOnElement(TElement *aMouseElement)
{
   // --- ������ ������ ���������� ����� -----
      int index = -1;

      for (int i = 0; i < aMouseElement->iPoints->Items_Count; i++)
         if (!aMouseElement->iPoints->Items[i]->Conected && aMouseElement->iPoints->Items[i]->Visible) {
            index = i;
            break;
         }

      if (index == -1) return;

      mp.NumberIn = index;

   // --- "������ �����" ----
      EndLink(aMouseElement);
      WhatDo = wdNone;
}

//===========================================================================
void TfrmEditorManager::DeleteCurrenetLink()
{
     bool finding = false;

   // ---- ������ ���� Conected � �����a �������� ���� ���� �������  ------
     if (LinkList[LinkList_Count - 1]->FirstElement != NULL) {
             TElement  *el = ((TElement*)LinkList[LinkList_Count - 1]->FirstElement);
           // ---- ���� ����� ����������� ----
             el->NeedRedraw = true;
           // ----

             el->oPoints->Items[LinkList[LinkList_Count - 1]->NumberOutput]->CurrentLink = NULL;
           // --- ���� �� ���� ������ ---
             for (int i = 0; i < LinkList_Count - 1; i++)
                 if (el == LinkList[i]->FirstElement) {finding = true; break;}

           // --- ���� ������� �� ������� ���� � ������� ������ �������� ��� �� � ����� ��������� ---------
             if (!finding) el->oPoints->Items[LinkList[LinkList_Count - 1]->NumberOutput]->Conected = false;
     }

     finding = false;

   // ---- ������ ���� Conected �o ����a �������� ���� ��� ����� ------
     if (LinkList[LinkList_Count - 1]->SecondElement != NULL){
             TElement  *el = (TElement*)LinkList[LinkList_Count - 1]->SecondElement;
           // ---- ���� ����� ����������� ----
             el->NeedRedraw = true;
           // ----

             el->iPoints->Items[LinkList[LinkList_Count - 1]->NumberInput]->CurrentLink = NULL;
           // --- ���� �� ���� ������ ---
             for (int i = 0; i < LinkList_Count - 1; i++)
                 if (el == LinkList[i]->SecondElement) {finding = true; break;}

           // --- ���� ������� �� ������� ���� � ������� ������ �������� ��� �� � ����� ��������� ---------
             if (!finding) (el->iPoints->Items[LinkList[LinkList_Count - 1]->NumberInput])->Conected = false;
     }


 // -------------- �� �������-�� ������� ����� ----
     delete LinkList[LinkList_Count - 1];
     LinkList = (TLink**) realloc (LinkList, sizeof(TLink*) * (--LinkList_Count));

  // ---------------- ��������� --------
     Draw();
 // -----
     WhatDo = wdNone;
 // -----
     ProjectModified = true;
}

