/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

//*****************************************************************************
void Tsum_spectra_dialog::set_parameters( QStringList *ptr_names, QStringList *ptr_factors )
{
  names = ptr_names ;
  factors = ptr_factors;
  int nr = 0 ;
  table1->setNumRows(0);
  QStringList::Iterator itfac =  factors->begin(); 
  for ( QStringList::Iterator it = names->begin() ; it != names->end() ;  ++it, nr++, ++itfac ) 
  {
    //cout << *it << "\n";
    if(nr+1 > table1->numRows())
    {
			 table1->insertRows(nr, 1);
    }
    table1->setText(nr, 0, *it);
    table1->setText(nr, 1, *itfac);   
  }
  
  table1->adjustColumn(0);
  table1->adjustColumn(1);
  
}
//****************************************************************************
void Tsum_spectra_dialog::factor_double_clicked()
{
  // which row is selected
  int nr = table1->currentRow();
  
  // Take the text from the table
  
  
  double old_value = table1->text(nr, 1).toDouble() ;
  
  // display the dialog
  bool ok;
  double res = QInputDialog::getDouble("Adding factor",
    QString("Enter a new value of the factor for spectrum %1").arg(table1->text(nr, 0)),
    old_value, -10000,
    10000, 8, &ok, this );
  if ( ok ) {
    // user entered something and pressed OK
    // store this back into the table
    QString new_text;
    new_text.setNum(res);
    table1->setText(nr, 1, new_text);
  } else {
    // user pressed Cancel
  }  
}
//****************************************************************************
void Tsum_spectra_dialog::remove_row()
{
  
  // which row is selected
  int nr = table1->currentRow();
  
  // are you sure?
  
  if(QMessageBox::warning(this,
                          "Removing the spectrum from the table",
                          QString(
                            "You are going to remove the row nr %1\n which is: %2  factor %3\n"
                            "Are you sure?").arg(nr+1).
                          arg(table1->text(nr, 0)).
                          arg(table1->text(nr, 1))
                          ,
                          QMessageBox::Yes,
                          QMessageBox::No,
                          QMessageBox::Cancel)
    == QMessageBox::Yes)
    {
    
    // remove it
    
    table1->removeRow(nr);
  }
  
}
//*******************************************

void Tsum_spectra_dialog::accept()
{
  
  
  names->clear();
  factors->clear();
  for (int nr = 0; nr < table1->numRows() ;  nr++ )  
  {
    *names += table1->text(nr, 0);
    *factors += table1->text(nr, 1);   
  }
  
  QDialog::accept();
}
