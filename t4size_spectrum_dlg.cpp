#include "t4size_spectrum_dlg.h"
#include "ui_t4size_spectrum_dlg.h"

#include <QDoubleValidator>
//*****************************************************************************
T4size_spectrum_dlg::T4size_spectrum_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::T4size_spectrum_dlg)
{
    ui->setupUi(this);
}
//*****************************************************************************
T4size_spectrum_dlg::~T4size_spectrum_dlg()
{
    delete ui;
}


//*****************************************************************************
void T4size_spectrum_dlg::set_parameters( vector<typ_x>  tab, spectrum_descr sd, int dim)    // int tab[4]
{

//<p align="center"><font size="+1">%D spectrum</font></p>

   // ui->textLabel_green->setForegroundColor(QColor(0,100,100) );

#define    SPOSOB_PALETTE false

#if SPOSOB_PALETTE
    QPalette Pal(palette());
    // set black background
    Pal.setColor(QPalette::Background, Qt::darkGreen);
     Pal.setColor(QPalette::Foreground, Qt::yellow);
    ui->textLabel_green->setAutoFillBackground(true);
    ui->textLabel_green->setPalette(Pal);
#endif

    // way to change the background color for widget   -- good, working !
    ui->textLabel_green->setStyleSheet(dim == 1 ? "background-color:darkGreen;" : "background-color:black;");



 ui->textLabel_green->setText(QString("<p align=\"center\"><font size=\"+1\">%1D spectrum</font></p>").arg(dim));
// pushButton_apply_all->setText(QString("Apply to all %1D spectra").arg(dim));

    ui->lineEdit_min_x->setValidator( new QDoubleValidator( sd.beg, sd.end, 6,  ui->lineEdit_min_x ) );
    ui->lineEdit_min_x->setText(QString::number(tab[0], 'f', 2));

     ui->lineEdit_max_x->setValidator( new QDoubleValidator( sd.beg, sd.end, 6,  ui->lineEdit_max_x ) );
    ui->lineEdit_max_x->setText(QString::number(tab[1], 'f', 2));

    ui->lineEdit_min_y->setValidator( new QDoubleValidator( sd.beg_y, sd.end_y, 6,  ui->lineEdit_min_y ) );
    ui->lineEdit_min_y->setText(QString::number(tab[2], 'f', dim == 1 ? 0 : 2));

     ui->lineEdit_max_y->setValidator( new QDoubleValidator( sd.beg_y, sd.end_y, 6,  ui->lineEdit_max_y ) );
    ui->lineEdit_max_y->setText(QString::number(tab[3], 'f', dim == 1 ? 0 : 2));

    enable_2D( dim == 2 );
}

//******************************************************************************
vector<typ_x> T4size_spectrum_dlg::give_parameters()
{
    //typ_x *result = new typ_x[4] ;
    vector<typ_x> result;

    result.push_back( (ui->lineEdit_min_x->text()).toDouble()) ;
    result.push_back( (ui->lineEdit_max_x->text()).toDouble());

    result.push_back( (ui->lineEdit_min_y->text()).toDouble());
    result.push_back( (ui->lineEdit_max_y->text()).toDouble());

    return result ;  // the caller will have to delete the array
}
//******************************************************************************
void T4size_spectrum_dlg::apply_all_this_kind()
{

}
//*****************************************************************************
void T4size_spectrum_dlg::enable_2D( bool two_D )
{
  ui->textLabel_zet_thresh->setVisible(two_D);
}
