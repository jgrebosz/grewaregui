#ifndef SPECTRUM_1D_H
#define SPECTRUM_1D_H

//#include <qworkspace.h>
#include <qevent.h>
#include <qcursor.h>
#include <qapplication.h>

#include <QGridLayout>
#include <QShortcut>
#include <QKeySequence>

#include "tplate_spectrum.h"
#include <string>
#include <cstdlib>

//#include <ui_results_of_integration.h>
#include <t4results_of_integration.h>

using namespace std ;

#include "box_of_spectrum.h"
#include "box_of_channels.h"
#include "box_of_counts.h"
#include "gate_1D.h"
#include "Tpinup.h"
#include "tundo.h"



//////////////////////////////////////////////////////////////////////
class spectrum_1D : public Tplate_spectrum
{
    Q_OBJECT            // macrodef

    friend class box_of_spectrum;

    int fake[4500];

    list<Tundo> undo_list ;
    QGridLayout * layout;
    bool flag_impossible_to_draw_scales; // because widget is too small

    bool flag_spectrum1D_needs_update = true;

public:
    vector <Tpinup> nalepka ;
    /** to make the rebining on screen */
    int rebin_factor;
    Tpinup nalepka_notice ;
    Tpinup rebining_notice ;

    spectrum_1D ( QMdiArea *ws, QString nazwa ) :  Tplate_spectrum ( ws ) //  , nazwa)
    {
        //cout << " this is the constructor of the 1D specrum" << endl ;
        name_of_spectrum = nazwa.toStdString() ;

        // the init below must be called manually from here !
        init() ;

        connect(this, &spectrum_1D::hovered, this, [this]() {
            // cout<< "hovered over 1D spectrum " << name_of_spectrum << endl;
            hoveredDocument = this;
        });
    }
    //---------------------
    ~spectrum_1D()
    {
        delete layout;
        destroy();
    }
    //---------------------
protected:
    void enterEvent(QEnterEvent *event) //override
    {
        emit hovered();
        QWidget::enterEvent(event);
    }
    //---------------------
signals:
    void hovered();
public:
    void showContextMenu() override {
        // QMenu menu;
        // menu.addAction("Opcja A1");
        // menu.addAction("Opcja A2");
        // menu.exec(QCursor::pos());
        b_spectrum-> showContextMenu(QCursor::pos() );
    }

#if 0
    // Obsługa zdarzenia menu kontekstowego (kliknięcie prawym przyciskiem myszy)
    void contextMenuEvent(QContextMenuEvent *event) override
    {
       // auto pos = event->globalPos();
         auto pos = event->pos();
        cout << "Ctrl+M  -->Przed mapowaniem x = " << pos.x() << ", y = " << pos.y() << endl;

        b_spectrum->remember_Shortcut_position(pos);
        moj_showContextMenu(pos);
    }
#endif

private slots:
#if 0
    void showContextMenu(QPoint pos);

#endif

protected:

    void moj_showContextMenu(QPoint pos)
    {
        // cout << "moj_showContextMenu -> Przysłana pozycja  [" << pos.x() << ",  " << pos.y() << "]"
        //      << endl;


        show_context_1d_menu(pos, false);
        update();

    }

public:
    /** there can be many gates on one spectrum. We need this function
		for the class box of spectrum to draw the gate on the screen */
    vector<gate_1D>&  give_vector_of_gates();


    bool find_gates_with_names_as_this_spectrum();
    /** You will be asked name by name - should it be removed */

    void remove_the_gate_1D();
    /** No descriptions */

    void black_white_spectrum ( bool on );
    /** when somebody press X to close spectrum */
    void closeEvent ( QCloseEvent * e ) ;
    /** with dialog precisely setting the marker */
    void set_integr_marker();

    // copy integration markers and background markers from the other spectrum
    void copy_markers ( Tplate_spectrum * doc );
    void set_new_refreshing_times();  // virtual
    void zero_spectrum();

    void name_overlay_spec_nr();
    /** storing the parameters in case of later undo command */
    void remember_for_undo ( string s = "No name" );
    /** To display on the menu bar  */
    string give_undo_text ( int nr );
    /** No descriptions */
    void print_postscript();
    /** See description in  the class spectrum widget */
    void give_Cx_Cy ( double *x, double *y );
    /** No descriptions */
    int giveCurrentMinCounts();

    void rebin_on_screen();
    /** No descriptions */
    int give_rebin_factor()
    {
        return rebin_factor ;
    }
    void freezing_photo_of_spectrum();
    void show_list_of_incrementers() ;
    bool give_flag_draw_scales()    // virtual
    {
        return (flag_draw_scales &&   ! flag_impossible_to_draw_scales);
    }

    void show_context_1d_menu(QPoint pos, bool flaga_myszki);


protected:
    typ_x recent_bgr_marker ;
    vector<typ_x> backgr_markers;
    bool flag_bgr_valid ;

    vector<gate_1D> my_gates_1D;
    string prefix_of_gate_1D_names;

    int min_counts ;   // was int
    int max_counts;   // was int

    typ_x min_x;
    typ_x max_x;

    box_of_channels* b_channels ;
    box_of_counts * b_counts ;
    box_of_spectrum * b_spectrum ;

    //#ifdef RAINBOW_SCALE
    //	box_of_counts * b_scale ;
    //#endif


    //int spectrum_length_skas;

    vector<int> spectrum_table;

    // for overplot
    struct sover
    {
        spectrum_descr sd;
        vector<int> spec_table;
        int color ;
        string spec_name ;

        sover ( spectrum_descr des, vector<int> tab, string name, int col ) :
            sd ( des ), spec_table ( tab ), spec_name ( name )
        {
            if ( col ==0 )
            {
                color = ( ( rand() % 155 )  << 16 )
                + ( ( rand() % 155 )  << 8 ) + ( rand() % 155 );
            }
            else
            {
                color = col;
            }
        }
    } ;

    vector< sover > overplot_vector ;

    // for frozen picture of the spectrum-------------------------------
    struct frozen_one_spectrum
    {
        vector<int> spec_frozen;
        string spec_name;
        int color;

        frozen_one_spectrum ( const vector<int> & orig_spec, string name, int col ) : spec_name ( name )
        {
            if ( col ==0 )
            {
                // we need a random color
                // random();

                srand(time( nullptr) );
                int rrr = ( rand() % 8 ) ;
                if ( rrr == 7 )
                    rrr = 3; // read instead white
                color =  //0xffffff;
                    ( rrr & 1 ? 0xff0000 : 0 )
                    + ( rrr & 2 ? 0x00ff00 : 0 )
                    + ( rrr & 4 ? 0x0000ff : 0 )  ;
            }
            else
            {
                color = col;
            }
            // copying into the memory of the frozen
            spec_frozen = orig_spec;

            /*
				  for(unsigned int i = 0 ; i < spec_frozen.size(); i++)  {
					spec_frozen[i] /= 2;  // dla jaj na razie
				  }
			*/
        }

    };

    vector< frozen_one_spectrum > frozen_photo_vector ;

    //-------------------------------

    enum typ_spec { normal, sum, overplot } kind_of_spectrum;

    //	int max_channel_z;
    //	int min_channel_z;

    double Bx;
    double Cx;
    double Dx;
    double Ax;
    double Ay;
    double By;
    double Cy;
    double Dy;
    double bgr_slope ;

    typ_x marker_older;
    typ_x marker_younger;
    double bgr_offset;

public slots:

    void init();
    void destroy();
    void mousePressEvent ( QMouseEvent *e );
    void wheelEvent ( QWheelEvent * e );
    void mouseMoveEvent ( QMouseEvent * e );

    void  focusInEvent ( QFocusEvent * e ) ;

    void paintEvent ( QPaintEvent * );
    // void init();
    void resizeEvent ( QResizeEvent * );
    void recalculate_my_geometry();
    // int give_max_channel_skas();
    void expand();
    // void mousePressEvent( QMouseEvent * e );
    void remember_as_marker ( typ_x k );
    void full();
    void shift_right ( int divider );
    void shift_left ( int divider );
    void scale_to_maxY();
    void scaleY_by_2();
    void scaleY_by_05();
    void integrate ( T4results_of_integration * ptr );
    void give_parameters ( typ_x * min_ch, typ_x * max_co,
                         typ_x * max_ch, typ_x * min_co,
                         spectrum_descr *sd );
    void scrollbar_horizontal_moved ( int int_left_edge );
    void scaleY_by_factor ( double value );
    void slider_horizontal ( int value );
    void scroller_vertical_moved ( int value_bottom );
    int giveCurrentMaxCounts();
    void set_parameters ( typ_x min_ch, typ_x max_co, typ_x max_ch, typ_x min_co );
    void save();
    void save_as ( string prefix );
    void set_the_name ( QString & name );
    void print_it();
    void draw_all_on_screen();
    void read_in_file ( const char * name, bool this_is_first_time );
    // bool find_x_description ( const char * name );
    void log_linear ( bool stan );
    void draw_scale_or_not ( bool stan );
    void set_backgr_marker ( typ_x dana );

    vector<typ_x>* give_bg_markers();

    typ_x justify_to_bin_center ( typ_x dana );
    typ_x justify_to_bin_left_edge ( typ_x dana );
    int typ_x_to_bin ( typ_x value );
    bool give_bg_info ( double * sl, double * off );
    void draw_all ( QPainter * piorko );
    bool give_flag_log_scale();
    void erase_nearest_bgr_marker ( typ_x place );
    void real_save_ascii ( QString & nazwa, bool flag_radware_header /*= false*/ );
    void expand_double_click ( typ_x pos_x,  typ_x  pos_y,  bool shift ) ;


    bool undo();
    void re_read_spectrum();
    void slot_set_bg_marker();
    void slot_erase_all_bg_markers();
    void slot_erase_recent_bg_marker() ;
    void slot_erase_nearest_bg_marker();
    void slot_save_bg_markers_as_default_gate();
    void slot_remove_default_gate();
    void slot_remember_all_markers();
    void slot_paste_all_markers();
    void slot_place_integ_marker_by_value() ;
    void slot_remove_all_tags();
    void slot_add_a_tag();
    void slot_add_nalepka();
    void slot_name_overlay_spec_nr() ;
    void slot_rebin_on_screen();
    void slot_make_foto();
    void slot_show_list_of_incrementers();
    void slot_times_of_last_zeroing() ;
    void slot_help_about_1D_mouse_clicking();


protected:
    void save_bg_markers_as_gate();


    // ********************************************************************
    // this is a function to shift the spectrum (matrix) on the picture up
    // or down
    //the argument is telling if we are going higher (plus) or lower (minus)
    //by the definition devider = 1 means:
    //      move as much as this part of the picture
    // which you see now.
    // if the argument will be 10 - means:
    //      move by 1/10 of the current picture
    //																						*/
    void shift_up_down ( int divider ) ;
    /** reading many spectra, which names are stored in "@sum_" file */
    bool read_in_sum_spectra ( string fname );
    bool save_sum_spectrum ( string fname );
    /** User defined comments, to make hardcopy nicer */
    void add_tag_with_comment ( double x, double y );
    /** No descriptions */
    void erase_nearest_tag_with_comment ( double x, double y );
    /** No descriptions */
    void remove_all_tags();
    bool read_in_overplot_spectra ( string fname, bool this_is_first_time );
    /** No descriptions */
    void make_foto();
    void show_time_of_last_zeroing();

    void  read_tags_from_disk();
    void  save_tags_to_disk();

    void set_layout_normal();
    void set_layout_without_scale();
    void paste_integration_and_bgr_markers();
public:
    static std::vector<std::vector<typ_x>> set_of_markers;
    static void read_from_disk_sets_of_markers();
    static void store_all_marker_on_a_disk();

    void remember_integration_and_bgr_markers();
    void paste_integration_and_bgr_markers(int set_nr);
    string take_integration_and_bgr_markers();

};
#endif  // SPECTRUM_1D_H
/** reading many spectra, which names are stored in "@over_" file */

