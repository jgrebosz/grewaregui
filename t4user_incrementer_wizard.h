#ifndef T4INCREMENTER_WIZARD_H
#define T4INCREMENTER_WIZARD_H

#include <QWizard>
#include "Tincrementer_description_cracow.h"

namespace Ui {
class T4user_incrementer_wizard;
}

class T4user_incrementer_wizard : public QWizard
{
    Q_OBJECT

public:
    explicit T4user_incrementer_wizard(QWidget *parent = 0);
    ~T4user_incrementer_wizard();
    void set_parameters( Tincrementer_description_cracow *increm_ptr,
                         string spectra, string conditions, string incrementers);

protected:
    void init();

    using QWizard::nextId;
    int nextId();
    void accept();

    void choosing_expressionA_incrementer();
    void choosing_expressionB_incrementer();
    void radio_changed( );
    bool validateCurrentPage();

    enum  { page__name_of_incrementer = 10,
            page__expression_green = 20,
            page__operation = 30,
            page__expression_blue = 40,
            page__result = 50 } ;


private slots:
    void on_push_increm_green_clicked();

    void on_push_increm_blue_clicked();

    void on_lineEdit_incrementer_name_textChanged(const QString &arg1);

    void on_lineEdit_incrementer_name_cursorPositionChanged(int, int arg2);

private:
    Ui::T4user_incrementer_wizard *ui;
    Tincrementer_description_cracow *increm ;
    std::string PressToChoose;

    std::string green;
    std::string oper;
    std::string blue;
    std::string donnor;

};

#endif // T4INCREMENTER_WIZARD_H
