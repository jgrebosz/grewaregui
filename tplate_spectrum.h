#ifndef TPLATE_SPECTRUM_H
#define TPLATE_SPECTRUM_H

#include <QWidget>
#include <QMdiSubWindow>
#include <QMdiArea>

#include "swiat.h"
class T4results_of_integration;


namespace Ui
{
class Tplate_spectrum;
}

class QMdiArea;
///////////////////////////////////////////////////////////////////////////////////////
class Tplate_spectrum :
        // public  QMdiSubWindow //
        public  QWidget
{
    Q_OBJECT

public:
    // explicit Tplate_spectrum ( QMdiArea *parent = nullptr );
    //explicit Tplate_spectrum(QMdiArea *ws, QString nazwa);
    explicit Tplate_spectrum ( QWidget *parent = nullptr );


    ~Tplate_spectrum();
    Tplate_spectrum(const Tplate_spectrum&) = delete;
    Tplate_spectrum& operator=(const Tplate_spectrum&) = delete;


    static bool flag_white_black;

     static bool flag_repainting_allowed;

	 // To not to repaint all the time, we make flags which allow to update
     bool flag_repaint_spectrum_box = true;
	 bool flag_repaint_counts_box= true;
	 bool flag_repaint_channels_box = true;
	 bool flag_repaint_bscale_box = true;

    virtual string give_spectrum_name();
    virtual void set_new_refreshing_times() = 0;
    virtual void re_read_counter_after_zeroing ( int count );
    virtual string projection ( int );
    virtual void find_notice_in_description();
    virtual string give_notice();
    virtual void zero_spectrum();
    virtual string give_undo_text ( int );
    //virtual bool printing_mode();
//    int give_paper_width();
//    int give_paper_height();
    virtual bool give_flag_draw_scales();
    int give_dimension()
    {
        return dimension;
    }

    void init();
public slots:
    virtual void paintEvent ( QPaintEvent * ) = 0;
    //virtual void resizeEvent ( QResizeEvent * ) = 0;
    virtual void full() = 0;
    virtual void shift_right ( int ) = 0;
    virtual void shift_left ( int ) = 0;
    virtual void scale_to_maxY() = 0;
    virtual void scaleY_by_2() = 0;
    virtual void scaleY_by_05() = 0;
    virtual void integrate ( T4results_of_integration * );
    virtual void give_parameters ( typ_x *, typ_x *, typ_x *, typ_x *, spectrum_descr * );
    virtual void scrollbar_horizontal_moved ( int ) = 0;
    virtual void scaleY_by_factor ( double ) = 0;
    virtual void slider_horizontal ( int ) = 0;
    virtual void scroller_vertical_moved ( int ) = 0;
    virtual int giveCurrentMaxCounts() = 0;
    virtual void Tplate_spectrum_destroyed ( QObject * );
    virtual void set_parameters ( typ_x, typ_x, typ_x, typ_x );
    virtual void save() = 0;
    virtual void save_as ( string prefix ) = 0;
    virtual void real_save_ascii ( QString &, bool );
    virtual void set_the_name ( QString & );
//    virtual void print_it() = 0;
    virtual void draw_all_on_screen() = 0;
    virtual void read_in_file ( const char *, bool );
	// virtual bool find_x_description ( const char * );
    virtual void log_linear ( bool );
    virtual void auto_scale( bool );
    virtual void draw_all ( QPainter * ) = 0;
    virtual bool give_flag_log_scale();
    virtual void expand() = 0;
    virtual void show_polygons ( bool );
    virtual void lupa_button_pressed();
    virtual bool undo();
    virtual void shift_up_down ( int );
    virtual spectrum_descr give_specif();
    virtual void show_disk_names_of_gates ( bool );
    virtual void get_focus ( QObject * );
    virtual bool give_flag_black_white();
    virtual void re_read_spectrum() = 0;
    virtual void black_white_spectrum ( bool ) = 0;
    virtual void destroy();
    virtual void closeEvent_ ( QCloseEvent * e );
    virtual void copy_markers ( Tplate_spectrum * );
    virtual void print_postscript();
    virtual void give_Cx_Cy ( double * x, double * y );
    virtual void freezing_photo_of_spectrum();
    virtual void draw_scale_or_not ( bool stan );
    virtual void mouseMoveEvent_ ( QMouseEvent * e );
    virtual int give_minZthreshold () { return 0; }
	string give_incrementers() { return list_of_incrementers_of_this_spectrum;}

signals:
    void closing_the_spectrum();
    void spec_update_scrolls_and_sliders();
    void enable_selected_actions ( int typ_of_spectrum );

protected:
    spectrum_descr specif;
//    QPrinter *printer ;
    bool flag_log_scale;
    bool flag_auto_maximum = false; // auto rescaling to the highest peak (in region?)
    QTimer * timer_refresh;
    string name_of_spectrum;
    int re_read_counter;
    string notice;
//    int paper_width;
//    int paper_height;
    string list_of_incrementers_of_this_spectrum;
    bool flag_draw_scales;

    int dimension;



    virtual void read_list_of_incrementers ( ifstream & plik );

    // for MAC
public:
    virtual void showContextMenu() = 0;
    static Tplate_spectrum* getHoveredDocument() {
        return hoveredDocument;
    }

protected:
    // ColorWidget *colorWidget;
    static inline Tplate_spectrum *hoveredDocument = nullptr;

private:
    Ui::Tplate_spectrum *ui;
};

#endif // TPLATE_SPECTRUM_H
