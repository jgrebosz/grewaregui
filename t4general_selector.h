#ifndef T4general_selector_H
#define T4general_selector_H

#include <QDialog>
#include <vector>
using namespace  std;

namespace Ui {
class T4general_selector;
}
/////////////////////////////////////////////////////////////////////////////
class T4general_selector : public QDialog
{
    Q_OBJECT
    
public:
    explicit T4general_selector(QWidget *parent = 0);
    ~T4general_selector();
    QStringList give_seleced_items();
    void set_multi_selection( bool multi );
    void make_incremeter_ALL_impossible();
    void add_all_user_incrementers_to_the_list();
    
private slots:
    //    void on_buttonOk_clicked();
    void on_lineEdit_filter_textChanged(const QString &filter);

private:
    Ui::T4general_selector *ui;
    static QString last_filter;

    // QStringList list_of_all_increm ;
    QStringList list_of_all_increm;

    void filter_changed();
    void change_title( QString str );
};

#endif // T4general_selector_H
