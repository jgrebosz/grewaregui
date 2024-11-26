#ifndef SPECTRUM_2D_H
#define SPECTRUM_2D_H

//#include <qworkspace.h>
#include <qevent.h>
#include <QGridLayout>

#include "tplate_spectrum.h"

#include <box_of_matrix.h>
#include <box_of_channels.h>
#include <box_of_counts.h>

#include "polygon_gate.h"
#include "Tpinup.h"

#include "t4results_of_integration.h"


enum refresh_big_matrices_policy { not_decided, skip_one =1, skip_all, refresh_one, refresh_all};

#ifdef NIGDY
/*
    class Tundo_2D
    {
    typ_x recent_bgr_marker ;
    vector<typ_x> backgr_markers;
    bool flag_bgr_valid ;

    typ_x max_x;
    typ_x min_x;

    typ_x max_y;
    typ_x min_y;

    int max_counts_on_map;
    int min_counts_on_map ;

    double bgr_slope ;
    double bgr_offset;

    typ_x marker_older;
    typ_x marker_younger;
    //spectrum_descr specif ;

    vector<polygon_gate> banana ;
    };
*/
#endif


////////////////////////////////////////////////////////////////////
class spectrum_2D : public Tplate_spectrum
{
    Q_OBJECT

    //const string   name_of_matrix ;
    //	  list<Tundo_2D> undo_list ;
    static bool flag_ask_if_refresh_the_huge_matrices;
    bool flag_he_wants_to_suffer_so_do_not_ask_more;
    QGridLayout * layout;
    bool flag_error_while_reading;

public:
    spectrum_2D ( QMdiArea *ws, QString nazwa ) : Tplate_spectrum ( ws ) // , nazwa)
    {
        //cout << " this is the constructor of the 2D specrum" << endl ;

        name_of_spectrum = nazwa.toStdString() ;
        // the init below must be called manually from here !
        init() ;

    }
    ~spectrum_2D()
    {
        spectrum_table.clear() ;
        //			cout << "Memory freed " << endl;
        destroy();
    }

    vector <Tpinup> nalepka ;  // user texts
    Tpinup nalepka_notice ;     // info about the spectrum
    Tpinup z_threshold_notice ;


    typ_x recent_bgr_marker ;
    vector<typ_x> backgr_markers;
    bool flag_bgr_valid ;

    box_of_channels* b_channels ;
    box_of_counts * b_counts ;
    box_of_matrix * b_matrix ;
    box_of_counts * b_scale ; // scale on the right side, reusing a class box_of_counts

    typ_x max_x;
    typ_x min_x;

    typ_x max_y;
    typ_x min_y;

    typ_x min_z_threshold;


    int spectrum_length_skas;
    vector<int> spectrum_table;

    //int max_channel_z;
    //int min_channel_z;
    int max_counts_on_map;
    int min_counts_on_map ;


//    double Bx;
//    double Cx;
//    double Dx;
//    double Ax;
//    double Ay;
//    double By;
//    double Cy;
//    double Dy;
    double bgr_slope ;


    typ_x marker_older;
    typ_x marker_younger;
    double bgr_offset;
    //spectrum_descr specif ;

    vector<polygon_gate> banana ;
    bool flag_show_polygons ;
    bool flag_show_disknames_of_polygons ;

    static polygon_gate *clipboard_polygon  ;
    static bool flag_polygon_in_clipboard;



    bool flag_lupa_mode ; // when somebody pressed lupa button on the toolbox
    string  prefix_of_polygon_names ; // what in the disk filename introduces screen_name (= "_polygon_) ;


    bool is_flag_error_while_reading() { return flag_error_while_reading;}
    void set_new_refreshing_times();  // virtual

    // void white_black( bool on );
    void black_white_spectrum ( bool on );
    /** No descriptions */
    void closeEvent ( QCloseEvent * e );
    /** No descriptions */
    void integrate_polygon ( polygon_gate &g );
    /** No descriptions */
    void zero_spectrum();
    /** To display on the menu bar */
    string give_undo_text ( int nr );
    /** No descriptions */
    void adjust_range_to_legal_values();
    /** No descriptions */
    void ask_z_threshold();
    bool is_possible_to_erase_this_polygon ( string );
    int give_minZthreshold() { return min_z_threshold; }
    //---------------

public slots:

    void init();
    void destroy();
    void mousePressEvent ( QMouseEvent *e );
    void keyPressEvent ( QKeyEvent *e );

    void wheelEvent ( QWheelEvent * e );
	void mouseMoveEvent ( QMouseEvent * e );

    // ---------
    void paintEvent ( QPaintEvent * );
    // void init();
    void resizeEvent ( QResizeEvent * );


    void focusInEvent ( QFocusEvent * ) ;


    void recalculate_my_geometry();
    //int give_max_channel_skas();

    //----------------------------------
    // two overloaded function for expansion
    void expand();
    void expand ( typ_x mleft, typ_x mtop, typ_x mright, typ_x mbottom );
    //----------------------------------

    // void mousePressEvent( QMouseEvent * e );
    void remember_as_marker ( typ_x k );
    void full();
    void auto_maximum();
    void shift_right ( int divider );
    void shift_left ( int divider );
    void scale_to_maxY();
    void scaleY_by_2();
    void scaleY_by_05();
    void integrate ( T4results_of_integration *ptr );
    void give_parameters ( typ_x * min_ch, typ_x * max_co,
                           typ_x * max_ch, typ_x * min_co,
                           spectrum_descr *sd );
    //typ_x * sp_beg, typ_x * sp_end );
    void scrollbar_horizontal_moved ( int int_left_edge );
    void scaleY_by_factor ( double value );
    void slider_horizontal ( int value );
    void scroller_vertical_moved ( int value_bottom );
    int giveCurrentMaxCounts();
    // void spectrum__destroyed( QObject * );
    void set_parameters ( typ_x min_ch, typ_x max_yy,
                          typ_x max_ch, typ_x min_yy );
    void save();
    void save_as ( string prefix );
    void set_the_name ( QString & name );
    void print_it();
    void paint_all_on_screen();
    // 2 below are not needed, but they are pure virtuali in a base class
    void draw_all_on_screen()
    {
        cout << "  spectrum_2D::draw_all_on_screen - is just update " << endl;
        //update() ;
    }
    void draw_all ( QPainter * )
    {
        cout << "  spectrum_2D::draw_all - is just update " << endl;
        // update() ;
    }

    void read_in_file ( const char * name, bool this_is_first_time );
//    bool find_x_description ( const char * name );
    void log_linear ( bool stan );
    void auto_scale( bool state)
    { // cout << "Wirtualna " << __PRETTY_FUNCTION__ << endl;
        flag_auto_maximum = state;
    }
    void set_backgr_marker ( typ_x dana );
    vector<typ_x>* give_bg_markers();

    vector<polygon_gate>&  give_vector_of_polygons();

    typ_x justify_to_bin_center ( typ_x dana );
    int typ_x_to_bin ( typ_x value );
    bool give_bg_info ( double * sl, double * off );
    void paint_all ( QPainter * piorko );
    bool give_flag_log_scale();

    void real_save_ascii ( QString & nazwa, bool flag_radware );
    void show_polygons ( bool stan ) ; // { } ;
    bool give_flag_show_polygons()
    {
        return  flag_show_polygons ;
    }

    void show_disk_names_of_gates ( bool flag )
    {
        //cout <<  "now flag_show_disknames_of_polygons = " <<  flag  << endl ;
        flag_show_disknames_of_polygons = flag ;
        //update() ;
    }
    bool give_flag_show_disk_names_of_gates( )
    {
        return flag_show_disknames_of_polygons ;
    }
    void lupa_button_pressed();

    void error_more_than_one_polygon_selected() ;
    void erase_polygon ( int nr );
    int give_max_counts()
    {
        return max_counts_on_map ;
    }
    int give_value_of_pixel ( typ_x x, typ_x y ) ;
    void shift_up_down ( int divider ) ;

    bool find_polygons_with_names_as_this_marix();
    void add_new_polygon() ;
    //     void add_horizontal_polygon();

    void re_read_spectrum();
    string projection ( int axis_x ); // <---- virtual
    bool is_polygon_name_unique ( string name ) ;
    void paste_polygon() ;
    /** storing the parameters in case of later undo command */
    // void remember_for_undo(string s);

    void draw_scale_or_not ( bool stan );


    // slots for popup Qmenu..........................................
    void slot_no_operation() {  }
    void slot_add_new_polygon()
    {
        add_new_polygon();
        //        update() ;
    }

    void slot_crosshair_mode()
    {
        // ( "Add horizontal line", 25 );
        // add_horizontal_polygon();
        b_matrix->crosshair_mode ( ! b_matrix->is_crosshair_mode() );
		b_matrix->update();
    }
	//-----------------------------------------
    void slot_add_horizontal_obsolate()
    {
        //
        //                 // ( "Add horizontal line", 25 );
        //                 // add_horizontal_polygon();
        //                 b_matrix->crosshair_mode ( false /*! b_matrix->is_crosshair_mode()*/ );
    }

    void slot_delete_polygon()    ;


#ifdef NA_RAZIE_NIE
    void slot_clone_polygon_obsolate()
    {
        // ----------------------clone new polygon

        if ( more_than_one_polygon_selected )
        {
            error_more_than_one_polygon_selected() ;
            return;
        }

        for ( unsigned int i = 0 ; i < banana.size() ; i++ )
        {
            if ( banana[i].is_selected() )
            {
                polygon_gate klon = banana[i] ;

                // dialog to choose the name and the format
                QFileDialog* fd = new QFileDialog ( this,
                                                    "Select the name for cloned polygon gate",
                                                    true );
                fd->setMode ( QFileDialog::AnyFile );
                fd->setFilter ( "polygon gates ( *.poly ) " );
                QString fileName;
                if ( fd->exec() == QDialog::Accepted )
                {
                    QString fileName = fd->selectedFile();
                    klon.name = fileName.toStdString();
                }
                klon.move_vertices_of_kloned_polygon();
                banana.push_back ( klon ) ;
                break ;
            }
        }

        // update() ;


    }
#endif

#ifdef NIGDY

    void slot_change_name_of_polygon_obsolate()
    {
        //--------------------  changing the name of the polygon
        // warning - no selected vertices
        if ( more_than_one_polygon_selected )
        {
            error_more_than_one_polygon_selected() ;
            return;
        }
        // else = no break here
        // changing the name
        for ( unsigned int i = 0 ; i < banana.size() ; i++ )
        {
            if ( banana[i].is_selected() )
            {
                // dialog to choose the name and the format
                QFileDialog* fd = new QFileDialog ( this, "Select the name for current polygon gate", true );
                fd->setMode ( QFileDialog::AnyFile );
                fd->setFilter ( "polygon gates ( *.poly ) " );
                QString fileName;
                if ( fd->exec() == QDialog::Accepted )
                {
                    QString fileName = fd->selectedFile();
                    banana[i].name = fileName.toStdString();
                }
                //cout << "Nazwa " << fileName << endl ;
            }
        }
    }
#endif

    void slot_deselect_all_vertices()    ;
    void slot_remove_vertex()    ;
    void slot_add_vertices() ;
    void slot_find_polygons_belonging_to_this_matrix()
    {
        // find
        find_polygons_with_names_as_this_marix();
    }

    void slot_integrate_polygon()  ;
    void slot_copy_polygon();
    void slot_paste_polygon()
    {
        // paste ---------------------

        if ( flag_polygon_in_clipboard )
        {
            paste_polygon();
        }
    }
	void slot_help_about_moving_vertices();
	void slot_help_about_selecting_vertices();
	void slot_help_about_mouse_clicking();

    void slot_undo_banana_change()
    {
        // undo the banana change
        b_matrix->make_undo_banana();
        find_polygons_with_names_as_this_marix();
    }

	void slot_create_grid_of_polygons()   // never used?
    {
        // create the grid of polygon gates
        grid_of_polygons() ;
        find_polygons_with_names_as_this_marix();
    }

    void slot_ask_for_z_threshold()
    {
        ask_z_threshold();
    }

    void slot_remove_all_tags()
    {
        remove_all_tags();
    }

    void slot_add_tag()
    {
        add_tag_with_comment (
                    b_matrix->give_x_mouse_click(),
                    b_matrix->give_y_mouse_click() );
        // update() ;
    }

    void slot_erase_nearest_tag()
    {
        erase_nearest_tag_with_comment (
                    b_matrix->give_x_mouse_click(),
                    b_matrix->give_y_mouse_click() );
        //        update() ;
    }

    void slot_show_incrementers()
    {
        // show the list of incrementers
        show_list_of_incrementers() ;
    }

    void slot_show_time_of_last_zeroing()
    {
        show_time_of_last_zeroing();
    }



protected: // Protected methods
    /** creating many polygons, which cover the full view */
    void grid_of_polygons();

    /** User defined comments, to make hardcopy nicer */
    void add_tag_with_comment ( double x, double y );
    /** No descriptions */
    void erase_nearest_tag_with_comment ( double x, double y );
    /** No descriptions */
    void remove_all_tags();
    void show_list_of_incrementers() ;
    void show_time_of_last_zeroing();

    void  read_tags_from_disk();
    void  save_tags_to_disk();

    void set_layout_normal();
    void set_layout_without_scale();

    bool more_than_one_polygon_selected ;
    int total_vertices_selected ;
    bool flag_impossible_to_draw_scales;


};
////////////////////////////////////////////////////////////////////

// extern istream & zjedz ( istream & plik );

#endif   // SPECTRUM_2D_H


