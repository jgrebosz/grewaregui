#include "Tfile_line_det_cal_lookup.h"
using namespace std;
extern istream & zjedz(istream & plik) ;
#include "appl_form.h"
#include "paths.h"
#include <cctype>
//*********************************************************************************
bool Tfile_line_det_cal_lookup::read_in(ifstream &infile)
{
    calib_factors.clear();
    infile >> zjedz >> channel;
    cout << "Read in a channel =>>>>>  " << channel << endl;

    if(infile.eof()) return true; // success
    if(!infile)
        throw Texcept_read{"signal name", "error while reading a name of a signal"};

    infile >> enable ;
    if(!infile)  throw Texcept_read( channel, "enable");

    infile >> name_of_detector;
    if(!infile) { throw Texcept_read(channel, "name_of_detector"); }

    bool flag_try_anyway = false;

    if(enable == false)
    {
            infile >> zjedz ;
            char c = infile.peek();
            if(infile.eof() ||  isalpha(c))
            {
                cal_numbers = 1;
                calib_factors = { 0, 1};\
                first_thresh = 0;
                theta = 0;
                phi = 0;

                if(second_calibration_present) cal2_numbers = 1;
                calib2_factors = { 0, 1};
            }
            else // it is numerical, so continue
            {

                flag_try_anyway = true;
            }

    }

    if(enable == true || flag_try_anyway)
    {
        infile >> zjedz ;
        if(infile.eof() || !infile) return true;

        infile >> first_thresh;

        if(!infile) {
            throw Texcept_read(channel, "first threshold");
        }

        if(second_threshold_present) {
            infile >> second_threshold;
            if(!infile) { throw Texcept_read(channel, "second threshold"); }
        }

        if(geometry_present )
        {
            infile >> theta;
            if(!infile) { throw Texcept_read(channel, "theta"); }
            infile >> phi;
            if(!infile) { throw Texcept_read(channel, "phi"); }
        }

        infile >> cal_numbers;
        if(!infile) { throw Texcept_read(channel, "cal_numbers"); }
        for(int i = 0 ; i < cal_numbers ; ++i)
        {
            double tmp;
            infile >> tmp;
            if(!infile) { throw Texcept_read(channel, "calibration factor - nr: "
                                             + to_string(i)); }
            calib_factors.push_back(tmp);
        }

        if(second_calibration_present)
        {
            infile >> cal2_numbers;
            if(!infile) { throw Texcept_read(channel, "cal2_numbers"); }
            for(int i = 0 ; i < cal2_numbers ; ++i)
            {
                double tmp;
                infile >> tmp;
                if(!infile) { throw Texcept_read(channel, "calibration2 factor - nr.: "
                                                 + to_string(i)); }
                calib2_factors.push_back(tmp);
            }
        }
    }

    infile >> zjedz;

    return 1;
}
//**************************************************************************************
void Tfile_line_det_cal_lookup::save_line(ofstream &sav,
                                          [[__maybe_unused__]] appl_form *ptr,
                                          bool & flag_really_replace)
{
    sav << channel << "\t"
        << enable << "\t"
        << name_of_detector ;

    if(true /*enable*/)
    {
        sav << "\t" << first_thresh ;
        if(second_threshold_present)
            sav << "\t" << second_threshold ;

        if(geometry_present)
        {
            sav << "\t"<< theta
                << "\t"<< phi ;
        }

        sav << "\t" << (cal_numbers) ;// because it is not an order, but how many cal words to read.
        for(auto c : calib_factors)
        {
            sav << "\t" << c ;
        }

        if(second_calibration_present)
        {
            sav << "\t" << (cal2_numbers) ;// because it is not an order, but how many cal words to read.
            for(auto c : calib2_factors)
            {
                sav << "\t" << c ;
            }
        }

    }else{

        string mess =
                "Your non-zero information for " +
                channel + ", will be lost now\n (Ask Alain why he suggests this)\n\n";

        string what ;
        if(first_thresh) what += "\n   threshold = " + to_string(first_thresh);
        if(second_threshold_present && second_threshold > 0)
            what += "\n   second threshold = " + to_string(second_threshold);


//        cout << channel << ", geom:" << geometry_present << ", th:" << theta << ", phi " << phi << endl;

        if(geometry_present && (theta || phi))  // theta and phi are non zero, so it is probably a valid information
        {
            what += "\n   non-zero geometry information: (" ;
            what += to_string(theta) + "/" + to_string(phi) + ") ";
        }



        if(calib_factors.size() == 2 &&calib_factors[0] == 0 && calib_factors[1] == 1)
        {
            // OK
        }
        else
        {
            what += "\n   calibration factors ";
        }

        if(second_calibration_present)
        {
            if(calib2_factors.size() == 2 &&calib2_factors[0] == 0 && calib2_factors[1] == 1)
            {
                // OK
            }
            else
            {
                what += "\n   second calibration factors ";
            }
        }

        if(what.size())
        {
            auto odp = askYesNoCancel(
                                             " Risk of loosing the information",
                                             (mess + what + "\n\nAre you sure?").c_str()
                                             );
            switch ( odp )
            {
                default:
                    flag_really_replace = false;
                break;

                case QMessageBox::Yes:    // Yes
                break;
            }
        } // end if th&phi

    } // end if enable
    sav << endl;
}
