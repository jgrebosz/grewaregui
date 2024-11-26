//*************************************************************************************
void T4manager_user_conditions::update_review()
{
  QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );

  ui->review->setRowCount (0);
  unsigned int nr_rows =vec_cond_descr.size();
  ui->review->setRowCount ( nr_rows );

  unsigned int current_line = 0 ;
  for (unsigned int j = 0; j < nr_rows; ++j, current_line++ )
    {

      COTO ;
      ui->review->setRowCount(current_line+1);
      for(int i = 0 ; i< ui->review->columnCount() ; i++)
        {
          ui->review->setItem(current_line, i, new QTableWidgetItem() );
        }


      ui->review->item(current_line, 0)->setText(vec_cond_descr[j].name.c_str());


      cout << "==========Review =============preparing condition named "
           << vec_cond_descr[j].name << "  in the row nr " << current_line << endl;


      unsigned int extra_lines = 0 ;
      // loop over 1D AND
      //#ifdef NIGDY

      //    cout << "    item_1D_AND.size() = " << vec_cond_descr[j].item_1D_AND.size() << endl;
      for (unsigned int r = 0; r < vec_cond_descr[j].item_1D_AND.size(); ++r)
        {
          COTO ;
          if(ui->review->rowCount() <= (signed) (current_line+r) )
            {
              ui->review->setRowCount(current_line+r+1);
              for(int i = 0 ; i< ui->review->columnCount() ; i++)
                {
                  ui->review->setItem(current_line+r, i, new QTableWidgetItem() );
                }

            }

          if(r)
            {
              ui->review->item(current_line+r, 0)->setText( "          (cont.) ");
            }
          COTO ;
          ui->review->item(current_line+r, 1)->setText( (vec_cond_descr[j]. item_1D_AND[r].obligatory_in_event  == 1 ?
                                                           "consider this line as: FALSE" : " consider this line as: TRUE"));
          ui->review->item(current_line+r, 2)->setText( (vec_cond_descr[j]. item_1D_AND[r].var_name.c_str() ));
          ui->review->item(current_line+r, 3)->setText( QString::number(vec_cond_descr[j]. item_1D_AND[r].min_value));
          ui->review->item(current_line+r, 4)->setText( QString::number(vec_cond_descr[j]. item_1D_AND[r].max_value));

          extra_lines = max(extra_lines, r) ;

        }
      COTO ;

      cout << "    item_1D_OR.size() = " << vec_cond_descr[j].item_1D_OR.size() << endl;
      for (unsigned int r = 0; r < vec_cond_descr[j].item_1D_OR.size(); ++r)
        {

          cout << "r = " << r << endl;
          if(ui->review->rowCount() <= (int) (current_line + r))
            {
              cout << "Need to add a row  because ui->review->rowCount()"<< ui->review->rowCount()
                   << " and current_line + 4 = " << (current_line + r)<< endl;
              ui->review->setRowCount(current_line+r+1); // always add 1 !
              cout << " Now row count = " << ui->review->rowCount() << endl;

              for(int i = 0 ; i< ui->review->columnCount() ; i++)
                {
                  ui->review->setItem(current_line+r, i, new QTableWidgetItem() );
                }
            }
          else cout << "No need to add row " << endl;

          if(r)
            {
              if(!ui->review->item(current_line+r, 0)) ui->review->setItem(current_line+r, 0, new QTableWidgetItem);
            }

          //          ui->review->item()->setText(current_line+r, 1, (vec_cond_descr[j]. item_1D_OR[r].obligatory_in_event  == 1 ?
          //            "obligatory" : " consider this line as: TRUE"));


//          if( ui->review->item(current_line+r, 5) == NULL)
//            {
//              for(int col = 5 ; col <= 7 ; col++)
//                {
//                  cout << "Creating item row = " << (current_line+r)  << ", col=" << col << endl;
//                  ui->review->setItem(current_line+r, col, new QTableWidgetItem() );
//                  if (ui->review->item(current_line+r, col) == NULL)
//                    {
//                      {
//                        cout << "error while creating item " << (current_line+r) << " column = " << col << endl;
//                        cout << " ui->review->rowCount() = " << ui->review->rowCount() << endl;
//                      }
//                    }
//                }

//            }
          if(!ui->review->item(current_line+r, 5) )
            cout << "No such item !!!!!!!!!!!  row" << (current_line+r)
                 << ", column=" << 5 << endl;

          ui->review->item(current_line+r, 5)->setText( vec_cond_descr[j]. item_1D_OR[r].var_name.c_str() );
          ui->review->item(current_line+r, 6)->setText( QString::number(vec_cond_descr[j]. item_1D_OR[r].min_value));
          ui->review->item(current_line+r, 7)->setText( QString::number(vec_cond_descr[j]. item_1D_OR[r].max_value));

          extra_lines = max(extra_lines, r) ;

        } //
      COTO ;
      //   cout << "    item_2D_AND.size() = " << vec_cond_descr[j].item_2D_AND.size() << endl;
      for (unsigned int r = 0; r < vec_cond_descr[j].item_2D_AND.size(); ++r)
        {
          //          COTO ;
          if(ui->review->rowCount() <= (signed) (current_line+r))
            {
              ui->review->setRowCount(current_line+r);
            }
          //          COTO ;

          //      cout << "setting text in the line " << current_line+r
          //      << " while available are = " << ui->review->rowCount()
          //      << endl;

          if(r)
            {
              ui->review->item(current_line+r, 0)->setText( "          (continuation) ");
            }

          COTO ;

          ui->review->item(current_line+r, 8)->setText( (vec_cond_descr[j]. item_2D_AND[r].obligatory_in_event  == 1 ?
                                                           "consider this line as: FALSE" : " consider this line as: TRUE"));
          ui->review->item(current_line+r, 9)->setText( vec_cond_descr[j]. item_2D_AND[r].var_x_name.c_str()  );

          ui->review->item(current_line+r, 10)->setText( vec_cond_descr[j]. item_2D_AND[r].var_y_name.c_str()) ;

          ui->review->item(current_line+r, 11)->setText( vec_cond_descr[j]. item_2D_AND[r].polygon_name.c_str());

          extra_lines = max(extra_lines, r) ;

        }


      //-----------
      //        COTO ;
      //   cout << "    item_2D_OR.size() = " << vec_cond_descr[j].item_2D_OR.size() << endl;
      for (unsigned int r = 0; r < vec_cond_descr[j].item_2D_OR.size(); ++r)
        {
          if(ui->review->rowCount() <= (signed)(current_line+r) )
            {
              ui->review->setRowCount(current_line+r+1);
            }

          if(r)
            {
              ui->review->item(current_line+r, 0)->setText( "          (continuation) ");
            }

          //          ui->review->item()->setText(current_line+r, 1, (vec_cond_descr[j]. item_2D_OR[r].obligatory_in_event  == 1 ?
          //            "obligatory" : " consider this line as: TRUE"));
          ui->review->item(current_line+r, 12)->setText(  vec_cond_descr[j]. item_2D_OR[r].var_x_name.c_str() );
          ui->review->item(current_line+r, 13)->setText(  vec_cond_descr[j]. item_2D_OR[r].var_y_name.c_str());
          ui->review->item(current_line+r, 14)->setText(  vec_cond_descr[j]. item_2D_OR[r].polygon_name.c_str());
          extra_lines = max(extra_lines, r) ;

        }


      //-----------
      //    COTO ;

      //   cout << "    nodes other_condition_AND.size() = " << vec_cond_descr[j].other_condition_AND.size() << endl;
      for (unsigned int r = 0; r < vec_cond_descr[j].other_condition_AND.size(); ++r)
        {
          if(ui->review->rowCount() <= (signed)(current_line+r))
            {
              ui->review->setRowCount(current_line+r+1);

              for(int c = 0 ; c < ui->review->columnCount() ; c++)
                ui->review->setItem(current_line+r, c, new QTableWidgetItem() );
            }
          if(r)
            {
              ui->review->item(current_line+r, 0)->setText( "          (continuation) ");
            }
          ui->review->item(current_line+r, 15)->setText(  vec_cond_descr[j]. other_condition_AND[r].c_str() );
          extra_lines = max(extra_lines, r) ;
        }
      //    COTO ;
      for (unsigned int r = 0; r < vec_cond_descr[j].other_condition_OR.size(); ++r)
        {
          if(ui->review->rowCount() <= (signed)(current_line+r) )
            {
              ui->review->setRowCount(current_line+r);
            }
          if(r)
            {
              ui->review->item(current_line+r, 0)->setText( "          (continuation) ");
            }
          ui->review->item(current_line+r, 16)->setText( vec_cond_descr[j]. other_condition_OR[r].c_str() );
          extra_lines = max(extra_lines, r) ;
        }
      //        COTO ;
      for (unsigned int r = 0; r < vec_cond_descr[j].other_condition_NAND.size(); ++r)
        {
          if(ui->review->rowCount() <= (signed)(current_line+r) )
            {
              ui->review->setRowCount(current_line+r);
            }
          if(r)
            {
              ui->review->item(current_line+r, 0)->setText( "          (continuation) ");
            }
          ui->review->item(current_line+r, 17)->setText( vec_cond_descr[j]. other_condition_NAND[r].c_str() );
          extra_lines = max(extra_lines, r) ;
        }
      COTO ;
      for (unsigned int r = 0; r < vec_cond_descr[j].other_condition_NOR.size(); ++r)
        {
          if(ui->review->rowCount() <= (signed) (current_line+r) )
            {
              ui->review->setRowCount(current_line+r);
            }
          if(r)
            {
              ui->review->item(current_line+r, 0)->setText( "          (continuation) ");
            }
          ui->review->item(current_line+r, 18)->setText( vec_cond_descr[j]. other_condition_NOR[r].c_str() );
          extra_lines = max(extra_lines, r) ;
        }


      //    COTO ;
      // FINALE - of One conditon definition row of stars-----------

      //        ui->review->setRowCount(current_line+extra_lines+1);


      // one row more
      ui->review->setRowCount(ui->review->rowCount() + 1 );
      for(unsigned i = 0 ; i < 19 ; i++)   // <--- 19 columns of the text
        {
          ui->review->setItem(current_line+ extra_lines+1, i, new QTableWidgetItem( "***" ) );
        }

      current_line += extra_lines +1 ;

    }// end of loop over conditons
  //    ui->review->setRowCount (current_line-1);


  //------------------------------------------
  for(unsigned i = 0 ; i < 19 ; i++)
    {
      // make them all not editable !!!!!!!!!!!
    }
  ui->review->resizeColumnsToContents();

  QApplication::restoreOverrideCursor();
}
//*************************************************************************************
