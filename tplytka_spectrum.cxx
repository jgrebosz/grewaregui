#include "tplytka_spectrum.h"

#include <iostream>
using namespace std;


Tplytka_spectrum::Tplytka_spectrum(std::string name, QWidget *parent) :
    QWidget(parent), nazwa_widma(name)
{
    ui.setupUi(this);
}

void Tplytka_spectrum::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui.retranslateUi(this);
        break;
    default:
        break;
    }
}

void Tplytka_spectrum::on_pushButton_clicked()
{
    cout << "klikniecie" << endl;
}
