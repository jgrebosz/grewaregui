#include "tcalibration_factors_dlg.h"
#include "ui_tcalibration_factors_dlg.h"

#include <QMessageBox>
#include <iostream>
using namespace std;

//********************************************************************
Tcalibration_factors_dlg::Tcalibration_factors_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Tcalibration_factors_dlg )
{
    ui->setupUi(this);
}
//********************************************************************
Tcalibration_factors_dlg::~Tcalibration_factors_dlg()
{
    delete ui;
}
//********************************************************************
void Tcalibration_factors_dlg::setup(Tfile_line_det_cal_lookup line,
                                     string first_thr_label, string second_thr_label,
                                     string first_cal_txt, string second_cal_txt)
{
    line_local = line;

    ui->label_data_track->setText(("The signals from the data track: <span style=\" color:#b20000;\">"
                                  + line_local.channel +
                                  "</span><br/>will be assigned to the detector called: ").c_str() );
    ui->label_second_threshold->setText( second_thr_label.c_str());
    ui->label_first_threshold->setText(first_thr_label.c_str());

    ui->LineEdit_det_name->setText(line.name_of_detector.c_str());
    ui->doubleSpinBox_phi->setValue(line.phi);
    ui->doubleSpinBox_theta->setValue(line.theta);

    ui->spinBox_first_threshold->setValue(line.first_thresh);

    bool flag_visible = line.second_threshold_present;
    ui->spinBox_second_threshold->setVisible(flag_visible);
    ui->label_second_threshold->setVisible(flag_visible);
    if(flag_visible)ui->spinBox_second_threshold->setValue(line.second_threshold);


    if(!line.geometry_present)
    {
        ui->label_theta->setVisible(false);
        ui->label_phi->setVisible(false);
        ui->doubleSpinBox_theta->setVisible(false);
        ui->doubleSpinBox_phi->setVisible(false);
    }


    int degree = line.calib_factors.size()-1;

    ui->spinBox_degree ->setValue(degree);
    ui->tableWidget->setColumnCount(degree+1);


    for(unsigned int col = 0 ; col < line.calib_factors.size(); ++col)
    {
        QTableWidgetItem *newItem = new QTableWidgetItem
                (tr("%1").arg(line.calib_factors[col]) );
        ui->tableWidget->setItem(0, col, newItem);
    }


    if(line.second_calibration_present)
    {
        ui->label_degree_1->setText((first_cal_txt+ "  degree").c_str());
        ui->label_degree_2->setText((second_cal_txt+ "  degree").c_str());


        int degree2 = line.calib2_factors.size()-1;
        ui->spinBox_degree_2 ->setValue(degree2);
        ui->tableWidget_2->setColumnCount(degree2+1);

        for(unsigned int col = 0 ; col < line.calib2_factors.size(); ++col)
        {
            QTableWidgetItem *newItem = new QTableWidgetItem
                    (tr("%1").arg(line.calib2_factors[col]) );
            ui->tableWidget_2->setItem(0, col, newItem);
        }

    }
    else ui->frame_calibration_2->hide();

    adjustSize();

}
//***********************************************************************
void Tcalibration_factors_dlg::on_spinBox_degree_valueChanged(int degree)
{
    //cout << "arg1 = " << degree << endl;
    // so we have to change the nr of columns
    int old_degree = ui->tableWidget->columnCount()-1;

    ui->tableWidget->setColumnCount( degree+1);

    //    if previously was more - no problem
    if(degree > old_degree)
    {
        // if it is bigger - we need new items
        // adding items
//        cout << "adding column" << endl;

        //        QTableWidgetItem *newItem = new QTableWidgetItem
        //                (tr("%1").arg(4.44) );
        //        ui->tableWidget->setItem(0, arg1-1, newItem);

        double new_value = ((unsigned int) degree >= line_local.calib_factors.size()   )? 0 : line_local.calib_factors[degree];
        ui->tableWidget->setItem(0, degree,
                                 new QTableWidgetItem (tr("%1").arg(new_value) )) ;

        // add title of the column
        QStringList labels;
        for(int i = 0 ; i<= degree ; ++i) labels+= ("a" + to_string(i)).c_str();
        ui->tableWidget->setHorizontalHeaderLabels (labels );
    }
    else{

    }
}
//********************************************************************************************
void Tcalibration_factors_dlg::on_spinBox_degree_2_valueChanged(int degree)
{
    //cout << "arg1 = " << degree << endl;
    // so we have to change the nr of columns
    int old_degree = ui->tableWidget_2->columnCount()-1;

    ui->tableWidget_2->setColumnCount( degree+1);

    //    if previously was more - no problem
    if(degree > old_degree)
    {
        // if it is bigger - we need new items
        // adding items
        //cout << "adding column" << endl;

        //        QTableWidgetItem *newItem = new QTableWidgetItem
        //                (tr("%1").arg(4.44) );
        //        ui->tableWidget->setItem(0, arg1-1, newItem);

        double new_value = ((unsigned int) degree >= line_local.calib2_factors.size()   )? 0 : line_local.calib2_factors[degree];
        ui->tableWidget_2->setItem(0, degree,
                                 new QTableWidgetItem (tr("%1").arg(new_value) )) ;

        // add title of the column
        QStringList labels;
        for(int i = 0 ; i<= degree ; ++i) labels+= ("a" + to_string(i)).c_str();
        ui->tableWidget_2->setHorizontalHeaderLabels (labels );
    }
    else{

    }
}

//********************************************************************************************
void Tcalibration_factors_dlg::on_tableWidget_cellChanged(int row, int column)
{

    bool success = false;
    //double val =
            ui->tableWidget->item(row, column)->text().toDouble(&success);

    if(success)
    {
        //cout << "tvalue = " << val << endl;
    }
    else
    {
        // cant open file warning
        QMessageBox::warning
                (0, "Wrong entry" ,
                 QString("The string \"" ) +
                 ui->tableWidget->item(row, column)->text() +
                 QString("\" can not be converted to a 'Double' number"),

                 QMessageBox::Yes | QMessageBox::Default ,  QMessageBox::NoButton );
    }

}
//*********************************************************************************************
void Tcalibration_factors_dlg::on_tableWidget_2_cellChanged(int row, int column)
{
    bool success = false;
    //double val =
            ui->tableWidget_2->item(row, column)->text().toDouble(&success);

    if(success)
    {
        //cout << "tvalue = " << val << endl;
    }
    else
    {
        // cant open file warning
        QMessageBox::warning
                (0, "Wrong entry in the second calibration table" ,
                 QString("The string \"" ) +
                 ui->tableWidget_2->item(row, column)->text() +
                 QString("\" can not be converted to a 'Double' number"),

                 QMessageBox::Yes | QMessageBox::Default ,  QMessageBox::NoButton );
    }

}

//********************************************************************************************
void Tcalibration_factors_dlg::on_buttonBox_accepted()
{
    // read all table into fact_local

    bool success = true;

    Tfile_line_det_cal_lookup f;

    f.channel = line_local.channel;
    f.name_of_detector = ui->LineEdit_det_name->text().toStdString();
    f.first_thresh = ui->spinBox_first_threshold ->value();

    f.geometry_present = line_local.geometry_present;
    f.theta = ui->doubleSpinBox_theta->value();
    f.phi = ui->doubleSpinBox_phi->value();
    f.cal_numbers = ui->spinBox_degree->value()+1;
    f.cal2_numbers = ui->spinBox_degree_2->value()+1;
     f.second_threshold_present = ui->spinBox_second_threshold->isVisible();
     f.second_calibration_present = ui->tableWidget_2->isVisible();

    if(f.second_threshold_present)
    {
        f.second_threshold = ui->spinBox_second_threshold->value();
    }



    for(int col = 0 ; col <= ui->spinBox_degree->value()  /* tableWidget->columnCount()*/ ; ++col )
    {
        double tmp = ui->tableWidget->item(0, col)->text().toDouble(&success);
        if(success) f.calib_factors.push_back(tmp);
        else {
            QMessageBox::warning
                    (0, "Wrong entry" ,
                     QString("The string \"" ) +
                     ui->tableWidget->item(0, col)->text() +
                     QString("\" can not be converted to a 'Double' number"),

                     QMessageBox::Yes | QMessageBox::Default ,  QMessageBox::NoButton );

        }
    }

    if(f.second_calibration_present)
    {
        f.cal2_numbers = ui->spinBox_degree_2->value()+1;
        for(int col = 0 ; col <= ui->spinBox_degree_2->value()  /* tableWidget->columnCount()*/ ; ++col )
        {
            double tmp = ui->tableWidget_2->item(0, col)->text().toDouble(&success);
            if(success) f.calib2_factors.push_back(tmp);
            else {
                QMessageBox::warning
                        (0, "Wrong entr in the second calibration tabley" ,
                         QString("The string \"" ) +
                         ui->tableWidget_2->item(0, col)->text() +
                         QString("\" can not be converted to a 'Double' number"),

                         QMessageBox::Yes | QMessageBox::Default ,  QMessageBox::NoButton );

            }
        }
    }


    if(success) line_local = f;
    return;
}


