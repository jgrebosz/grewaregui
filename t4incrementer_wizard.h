#ifndef T4INCREMENTER_WIZARD_H
#define T4INCREMENTER_WIZARD_H

#include <QWizard>
#include "Tincrementer_description_cracow.h"

namespace Ui {
class T4user_incrementer_wizard;
}

class T4incrementer_wizard : public QWizard
{
    Q_OBJECT

public:
    explicit T4incrementer_wizard(QWidget *parent = 0);
    ~T4incrementer_wizard();
    void set_parameters( Tincrementer_description_cracow *increm_ptr );

protected:
    void init();

    void nextId();
    void accept();
    void name_changed();
    void choosing_expressionA_incrementer();
    void choosing_expressionB_incrementer();
    void radio_changed( );

    enum  { page__name_of_incrementer = 10,
           page__expression_green = 20,
           page__operation = 30,
           page__expression_blue = 40,
           page__result = 50 } ;


private slots:
    void on_push_increm_green_clicked();

    void on_push_increm_blue_clicked();

private:
    Ui::T4user_incrementer_wizard *ui;
    Tincrementer_description_cracow *increm ;
};

#endif // T4INCREMENTER_WIZARD_H
