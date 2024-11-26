#ifndef T4INCREMENTER_SELECTOR_H
#define T4INCREMENTER_SELECTOR_H

#include <QDialog>
#include <vector>
using namespace  std;

namespace Ui {
class T4incrementer_selector;
}
/////////////////////////////////////////////////////////////////////////////
class T4incrementer_selector : public QDialog
{
    Q_OBJECT
    
public:
    explicit T4incrementer_selector(QWidget *parent = 0);
    ~T4incrementer_selector();
    QStringList give_seleced_items();
	   QStringList give_filtered_items();
    void set_multi_selection( bool multi );
    void make_incremeter_ALL_impossible();
    void add_all_user_incrementers_to_the_list();
	void set_filter(string name) { on_lineEdit_filter_textChanged(name.c_str()); }
    
private slots:
    //    void on_buttonOk_clicked();
    void on_lineEdit_filter_textChanged(const QString &filter);

private:
    Ui::T4incrementer_selector *ui;
    static QString last_filter;

    // QStringList list_of_all_increm ;
    QStringList list_of_all_increm;

    void filter_changed();
    void change_title( QString str );
};

#endif // T4INCREMENTER_SELECTOR_H
