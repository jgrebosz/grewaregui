#include "t4results_of_integration.h"
#include "ui_t4results_of_integration.h"

#include <sstream>

#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>

#include <QTextStream>
#include <QClipboard>

#include "tplate_spectrum.h"
#include "spectrum_1D.h"

extern T4results_of_integration * integr_resul_ptr;

//*********************************************************************************
T4results_of_integration::T4results_of_integration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::T4results_of_integration)
{
    ui->setupUi(this);
    integr_resul_ptr = this;

    ui->tabWidget->setCurrentIndex(0);
}
//*********************************************************************************
T4results_of_integration::~T4results_of_integration()
{
    delete ui;
}
//*********************************************************************************

void T4results_of_integration::on_button_clear_clicked()
{
    ui->textEdit1->setPlainText("");

}
//*********************************************************************************
void T4results_of_integration::on_pushButton_save_to_file_clicked()
{

    // storing on the disk the whole contents of the textEdit
    static QString last_file = "" ;

    Tplate_spectrum::flag_repainting_allowed = false;
    QString fileName;
    QString filter;

    fileName = QFileDialog::getSaveFileName(this,
                                            tr("Save current matrix as a file "),
                                            QString("./"),
                                            tr( "Any files (*);; Text files (*.txt)"),
                                            &filter
                                            );
    Tplate_spectrum::flag_repainting_allowed = true;

    //     if ( fd->exec() == QDialog::Accepted )
    //     {
    //         fileName = fd->selectedFile();
    //     }
    //cout << "Nazwa " << fileName << endl ;
    if(fileName.endsWith ( ".txt") == false)
    {
        fileName.append(".txt");
    }

    if(fileName.size() )
    {
        // saving everything to the disk

        QFile file( fileName ); // Write the text to a file

        QIODevice::OpenMode   flags_to_open = QIODevice::WriteOnly ;
        if(file.exists() )
        {
            switch(QMessageBox::information ( this, "File exists",
                                              "File exists, Overwrite it or append ?",
                                              "Overwrite",
                                              "Append",
                                              "Cancel",
                                              2)
                   ) // int defaultButtonNumber = 0,
            {
            case 0:
                break ;
            case 1:
                flags_to_open = (QIODevice::WriteOnly | QIODevice::Append);
                break ;
            default:
                return ;

            }
        }

        if ( file.open( flags_to_open ) )
        {
            QTextStream stream( &file );   // bylo (&file)
            stream << ui->textEdit1->toPlainText();
            //ui->textEdit1->setModified( false );    // ?????????????????? w Qt3
            last_file = fileName ;
        }


    }
}
//*********************************************************************************
void T4results_of_integration::add_results(const char *ss,
                                           std::string name_of_spectrum__,
                                           double centroid_channel__,
                                           double real_channel_of_local_maximum__)
{
    name_of_spectrum = name_of_spectrum__;
    centroid_channel = centroid_channel__;
    real_channel_of_local_maximum = real_channel_of_local_maximum__;
    ui->  textEdit1->append(ss);
    ui->tabWidget->setCurrentIndex(0);

    QString txt =
            QString("1. Insert results of last integration: CENTROID = %1,  MAXIMUM = %2").
            arg(centroid_channel).arg(real_channel_of_local_maximum);

    ui->label__centroid_and_max->setText(txt);
}
//*********************************************************************************
bool T4results_of_integration::multi_line_mode()
{
    return ! ui->checkBox_one_line->isChecked() ;
}
//*********************************************************************************
void T4results_of_integration::on_pushButton_copy_centroid_clicked()  // to A
{

    QString tmp = QString("%1").arg(centroid_channel);
    ui->lineEdit_ch_a->setText(tmp);

}
//*********************************************************************************
void T4results_of_integration::on_pushButton_copy_maximum_clicked()  // to A
{
    QString tmp = QString("%1").arg( real_channel_of_local_maximum);
    ui->lineEdit_ch_a->setText(tmp);


}
//*********************************************************************************
void T4results_of_integration::on_pushButton_calculate_clicked()
{
    // procedure possible when all 4 edit boxes have non zero entries
    double ch_a = ui->lineEdit_ch_a->text().toDouble();
    double ch_b = ui->lineEdit_ch_b->text().toDouble();

    double en_a = ui->lineEdit_en_a->text().toDouble();
    double en_b = ui->lineEdit_en_b->text().toDouble();

    // rearange order a < b   ???

    // gain

    double gain = (en_b - en_a) / (ch_b - ch_a);
    double offset  = en_b - (gain * ch_b);

    string keyword = name_of_spectrum;
    auto where = name_of_spectrum.rfind("_raw");
    if(where != string::npos)
    {
        keyword = name_of_spectrum.substr(0, where) + "_cal_factors";
    }

    QString   wynik = QString(" %1       %2     %3     // offset  gain").arg(keyword.c_str()).arg(offset).arg(gain);

    ui->plainTextEdit_coefficients->setPlainText(wynik);

}
//*********************************************************************************
void T4results_of_integration::on_pushButton_remember_clicked()
{
    ui->plainTextEdit_remember->appendPlainText(
                ui->plainTextEdit_coefficients->toPlainText()
                );
}
//*********************************************************************************
void T4results_of_integration::on_pushButton_copy_centroid_B_clicked()
{
    QString tmp = QString("%1").arg(centroid_channel);
    ui->lineEdit_ch_b->setText(tmp);
}
//*********************************************************************************
void T4results_of_integration::on_pushButton_copy_maximum_B_clicked()
{
    QString tmp = QString("%1").arg( real_channel_of_local_maximum);
    ui->lineEdit_ch_b->setText(tmp);
}
//*********************************************************************************


//*********************************************************************************
//void T4results_of_integration::on_pushButton_grab_clicked()
//{
//    ui->textEdit_result->setText( name_of_spectrum.c_str());
//    cout << "Widmo name " << name_of_spectrum << endl;


//    //    Tplate_spectrum * doc = ( Tplate_spectrum * ) area-> currentSubWindow()->widget();
//    //    if(doc)
//    ostringstream plik;
//    plik << "Widmo: " << name_of_spectrum;
//    plik << "zolte markery  = " << left << " " << right << endl;
//    for(auto n : background_markers)
//    {

//        plik << "BG "  << n << endl;
//    }
//    // take current markers from current spectrum
//    //doc->

//    // take all bg markers

//    // take 2 integration markers

//    // display them on the screen as text
//    ui->textEdit_result->setText( plik.str().c_str() );
//    //}
//}
//*********************************************************************************
void T4results_of_integration::on_pushButton_drugi_clicked()
{
    ostringstream plik;
    //-------------------------
    uint how_many = spectrum_1D::set_of_markers.size();
    ui->pushButton_remove_line->setEnabled(how_many);

    for(uint nr = 0 ; nr < how_many ; nr++)
    {
        plik << "set nr [" << nr << "]  " ;

        for(uint n = 0; n < spectrum_1D::set_of_markers[nr].size(); n++)
        {
            if(n == 0) plik << "  Integration markers : " ;
            if(n == 2) plik << "  Background markers : " ;
            plik << " "
                << spectrum_1D::set_of_markers[nr][n]
                   << "  ";
        }
        plik << endl;  ;
    }
    // display them on the screen as text
    ui->textEdit_result->setText(  plik.str().c_str() );
}
//*********************************************************************************
void T4results_of_integration::on_pushButton_remove_line_clicked()
{
    // which nr of set
    uint how_many = spectrum_1D::set_of_markers.size() -1;

    bool ok;
        int i = QInputDialog::getInt(this, tr("Type a number to remove"),
                                     tr("Which set number "),
                                     how_many, 0, how_many, 1, &ok);
        if (ok){
//           cout << "Sukces, przyjete = " << i << endl;
            if(spectrum_1D::set_of_markers.size())
            {
           auto iter= spectrum_1D::set_of_markers.begin() +i;
           spectrum_1D::set_of_markers.erase(iter );

            on_pushButton_drugi_clicked();
            }
//           for(uint el = 0;  el < spectrum_1D::set_of_markers.size() ; el++)
//           {
//               for(uint m = 0;  m < spectrum_1D::set_of_markers[el].size() ; m++)
//               {
//                   cout << spectrum_1D::set_of_markers[el][m] << "  ";
//               }
//               cout << endl;

//           }
        }


   spectrum_1D::store_all_marker_on_a_disk();
}
//*********************************************************************************
void T4results_of_integration::on_tabWidget_tabBarClicked([[ maybe_unused]] int index)
{
    on_pushButton_drugi_clicked();
}
