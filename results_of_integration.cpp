/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/
void results_of_integration::save_to_diskfile()
{
	// storing on the disk the whole contents of the textEdit
	static QString last_file = "" ;   
	// file selection dialog
	// dialog to choose the name and the format
	QFileDialog* fd = new  //QFileDialog::
  QFileDialog ( QString("./"),
																										QString::null,  // filter
																										this,
																										last_file,
																										TRUE ) ;
	
	//    QFileDialog* fd = new QFileDialog( this, "Save current text as a file ",  TRUE );
	fd->setMode( QFileDialog::AnyFile );
	
	//    QString types("Text files (*.txt)"
	//                  "Any files (*)");
	fd->setFilter(QString("Any files (*)")  );
	fd->addFilter( QString("Text files (*.txt)") );
	fd->setSelection(last_file);
	QString fileName;
	if ( fd->exec() == QDialog::Accepted )
	{
		fileName = fd->selectedFile();
	}
	//cout << "Nazwa " << fileName << endl ;
	if(fileName.findRev ( ".txt") == -1)
	{
		fileName.append(".txt");
	}
	
	if(fileName)
	{
		// saving everything to the disk
		
		QFile file( fileName ); // Write the text to a file
		
		int flags_to_open = IO_WriteOnly ;
		if(file.exists() )
		{
			
			switch(QMessageBox::information ( this, "File exists",
																				"File exists, Overwrite it or append ?",
																				"Overwrite",
																				"Append",
																				"Cancel",
																				2)) // int defaultButtonNumber = 0,
			{
										case 0:
				break ;
										case 1:
				flags_to_open = (IO_WriteOnly | IO_Append) ;
				break ;
										default:
				return ;
				
			}
		}
		
		if ( file.open( flags_to_open ) )
		{
			QTextStream stream( &file );
			stream << textEdit1->text();
			textEdit1->setModified( FALSE );
			last_file = fileName ;
		}
		
		
	}
	
}
//*********************************************************************************
void results_of_integration::clear_this_log()
{
	textEdit1->setText("");
}
//*********************************************************************************
void results_of_integration::add_results(const char *ss )
{
	textEdit1->append(ss);
}
//*********************************************************************************
bool results_of_integration::multi_line_mode()
{
	return ! checkBox_one_line->isChecked() ;
}
