#ifndef TPLYTKA_SPECTRUM_H
#define TPLYTKA_SPECTRUM_H

#include "ui_tplytka_spectrum.h"

#include <string>


class Tplytka_spectrum : public QWidget
{
    Q_OBJECT
    
public:
    explicit Tplytka_spectrum(std::string name, QWidget *parent = 0);

    std::string nazwa_widma;
    
protected:
    void changeEvent(QEvent *e);
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::Tplytka_spectrum ui;
};

#endif // TPLYTKA_SPECTRUM_H
