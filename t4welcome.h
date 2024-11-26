#ifndef T4WELCOME_H
#define T4WELCOME_H

#include <QDialog>
#include <vector>

namespace Ui {
class T4welcome;
}

class T4welcome : public QDialog
{
    Q_OBJECT
    
public:
    explicit T4welcome(QWidget *parent = 0);
    ~T4welcome();
    
private slots:
    void on_push_next_tip_clicked();

    void on_push_begin_clicked();

private:
    Ui::T4welcome *ui;
    void load_tips();
    std::string give_tip();
    std::vector<std::string> tip;
};
/////////////////////////////////////////////////////////////////
extern int general_font_size ;

#endif // T4WELCOME_H
