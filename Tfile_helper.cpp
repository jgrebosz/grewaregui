/***************************************************************************
                          tfile_helper.cpp  -  description
                             -------------------
    begin                : Sat May 17 2003
    copyright            : (C) 2003 by Jurek Grebosz (IFJ Krakow, Poland)
    email                : jerzy.grebosz@ifj.edu.pl
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 ***************************************************************************/

#include "Tfile_helper.h"
#include <algorithm>
#include <cctype>

#include <iostream>


//****************************************************************************
void File_helper::spot_in_file(ifstream& s, string slowo)
{
  s.seekg(0) ; // rewind

  string arch_s = slowo ; // to have oryginal case for error message

  // to be independent of lower/upper case of keywords...
  for(unsigned i = 0 ; i < slowo.size() ; i++)
      slowo[i] = (char) (std::tolower(slowo[i]));
  
  //cout << "Keyword to find is " << slowo << endl ;


  string word ;
  //-----------------
  while(s)
    {

      s >> zjedz >> word ;
      for(unsigned i = 0 ; i < word.size() ; i++)
          word[i] = (char) tolower(word[i]);
  
      // cout << "found word " << word << endl ;
      if(word == slowo) break ;
    }
  //----------------

  if(!s)
    {
      string mess =
	   "I can't find keyword '"
	   + arch_s
	   + "' in the file.(The search was case UNsensitive)" ;

      // the file is now in the error state
      if(s.eof())
      {
        s.clear(s.rdstate() & ~ios::eofbit) ;  // clearing the eof flag
      }
      // the file is now in the error state badbit
      if(!s)
      {
        s.clear(s.rdstate() & ~ios::badbit) ;  // clearing the eof flag
      }
      if(s.fail())
      {
        s.clear(s.rdstate() & ~ios::failbit) ;  // clearing the eof flag
      }
        
      Tno_keyword_exception capsule ;
      capsule.message= mess ;
      throw capsule ;
    }
  
}
//****************************************************************************
/** This function searches (in the given file) a keyword and then reads the
    value which follows it. Then returns this value as the result.  */
double File_helper::find_in_file(ifstream& s, string slowo)
{
  s.seekg(0) ; // rewind
  string arch_s = slowo ; // to have oryginal case for error message

  // to be independent of lower/upper case of keywords...
  for(unsigned i = 0 ; i < slowo.size() ; i++)
      slowo[i] = (char) tolower(slowo[i]);
  
  //cout << "Keyword to find is " << slowo << endl ;
  string word ;
  //-----------------
  while(s)
    {
      s >> zjedz >> word ;
      for(unsigned i = 0 ; i < word.size() ; i++)
          word[i] = (char) tolower(word[i]);
      //cout << "found word " << word << endl ;
      if(word == slowo) break ;
    }
  //----------------

  if(!s)
    {

	    Tno_keyword_exception capsule ;
	    capsule.message=  "I can't find keyword '"
			    + arch_s
			    + "' in the file.(The search was case UNsensitive)" ;
	    throw capsule ;
    }

  // otherwise we read the data
  double value ;
  s >> value ;

  if(!s) {

  
/*
  if(s.eof()) cout << "Error eof" << endl;
  if(s.fail()) cout << "Error fail" << endl;
  if(s.bad()) cout << "Error bad" << endl;
*/
  
	  Treading_value_exception capsule;

	  capsule.message = "I found keyword '"
				  + slowo
				  + "' but error was in reading its value." ;

	  throw capsule ;

  }
  return value ;  

}
//*************************************************************************
//*****************************************************************************
namespace File_helper
{
void repair_the_stream(ifstream& s)
{
// repair the stream
    s.clear(s.rdstate() & ~(ios::eofbit | ios::failbit));
}

}
//***********************************************************************
istream & zjedz ( istream & plik )
{
    char c = 'x' ;
    do
    {
        plik >> c  ;
        if ( (c == '/' && ( plik.peek() == '/')) || c == '#' )
        {
            std::string schowek;
            getline ( plik, schowek );
        }
        else
        {
            plik.putback ( c ) ;
            return plik ;
        }
    }
    while ( plik ) ;
    return plik ;
}
