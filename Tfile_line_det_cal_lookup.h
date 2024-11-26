#ifndef TFILE_LINE_DET_CAL_LOOKUP_H
#define TFILE_LINE_DET_CAL_LOOKUP_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <QMessageBox>
using namespace std;
class appl_form;
////////////////////////////////////////////////////////////////
struct  Texcept_read
{
    string channel;
    string item;
    Texcept_read(string ch_, string item_): channel{ch_}, item{item_} {}
};

///////////////////////////////////////////////////////////////
struct  Tfile_line_det_cal_lookup
{
    std::string channel ;
    bool enable;
    std::string name_of_detector;
    double first_thresh;
    bool second_threshold_present;
    double second_threshold;
    bool geometry_present;
    double theta;
    double phi;

    int cal_numbers;
    std::vector<double> calib_factors;
    bool second_calibration_present;
    int cal2_numbers;
    std::vector<double> calib2_factors;


    Tfile_line_det_cal_lookup()
    {
        enable = true;
        first_thresh = 0;
        second_threshold_present = false;
        second_threshold = 0 ;
        geometry_present = false;
        theta = 0 ; phi = 0;
        cal_numbers = 0 ;
        cal2_numbers = 0 ;
        second_calibration_present = false;
    };

    Tfile_line_det_cal_lookup(bool second_th, bool geometry, bool cal2_present):
        second_threshold_present{second_th},
        geometry_present{ geometry},
        second_calibration_present(cal2_present)
    {
        enable = true;
        first_thresh = 0;
        second_threshold = 0 ;
        theta = 0 ; phi = 0;
        cal_numbers = 0 ;
        cal2_numbers = 0 ;
    }

    // reading from the stream (file)
    bool read_in(ifstream &infile);

    // saving into stream
    void save_line(ofstream &sav, appl_form * ptr, bool &flag_really_replace);





    // -------------------------------
    friend ostream &  operator<< (ostream & s, Tfile_line_det_cal_lookup & obj)
    {
        s   << "channel="  << obj.channel
            << ", enable=" << obj.enable
            << ", name=" << obj.name_of_detector
            << ", first thr=" << obj.first_thresh ;
        if(obj.second_threshold_present)
            s << "sec thr=" << obj.second_threshold ;

        if(obj.geometry_present)
        {    s << ", theta=" << obj.theta << ", phi=" << obj.phi ;}

        s << ", cal order=" << obj.cal_numbers ;
        for(auto k: obj.calib_factors) s << ", " << k ;

        if(obj.second_calibration_present){
            s << ", cal2 order=" << obj.cal2_numbers ;
            for(auto k: obj.calib2_factors) s << ", " << k ;
        }
        return s;
    }
} ;

#endif // TFILE_LINE_DET_CAL_LOOKUP_H
