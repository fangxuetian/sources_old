//---------------------------------------------------------------------------
void __fastcall TfrmMain::Edit1Change(TObject *Sender)
{
  if (tv2->Selected       == NULL) return;
  if (tv2->Selected->Data == NULL) return;

  ((TElement*)tv2->Selected->Data)->Name = Edit1->Text;
  tv2->Selected->Text = Edit1->Text;

}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::tv2KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
   if (tv2->Selected == NULL) return;
   if (Key           != 13  ) return;

   frmElementParam->mpiShowModal( tv2->Selected);
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::aElementParametersExecute(TObject *Sender)
{
     if (tv2->Selected == NULL) return;

     frmElementParam->mpiShowModal( tv2->Selected);

     UpdateMemos();
}