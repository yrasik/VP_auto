/* Copyright (C) 2018 Yuri Stepanenko stepanenkoyra@gmail.com

This file is part of 'VP_auto'.

'VP_auto' is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

'VP_auto' is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with 'VP_auto'.  If not, see <http://www.gnu.org/licenses/>. */

/* CP1251*/

#ifndef DO_ELEMENT_H
#define DO_ELEMENT_H

#include <QtCore/QString>
#include <QFileInfo>
#include <QtCore/QTextStream>
#include <QtCore/QTextCodec>

#include <QRegExp>


#define _DEBUG_ 1


#if(_DEBUG_ == 1)
  #define LOG __FILE__ << ": " << __LINE__  << ": " << __FUNCTION__ << ": "
#else
  #define LOG ""
#endif



extern QTextStream *plog;
extern QTextCodec *codec;











class line_control
{ public:
  line_control(int y_First_line, int lines_in_first_page, int lines_in_others_pages)
  {
    this->lines_in_first_page = lines_in_first_page;
    this->lines_in_others_pages = lines_in_others_pages;

    this->y_First_line = y_First_line;
    y_step = 8;

    PageNumber = 1;

    clear_y_Line_counter();
  }

  void clear_y_Line_counter ( void )
  {
    if ( PageNumber == 1 )
    {
      y_Last_line = y_First_line - ( lines_in_first_page * y_step );
    }
    else
    {
      y_Last_line = y_First_line - ( lines_in_others_pages * y_step );
    }

    y_Current_line = y_First_line;
  }

  int get_y_Current_line ( void )
  {
    return y_Current_line;
  }

  void dec_y_Current_line ( void )
  {
    y_Current_line -= 1 * y_step;
  }

  void inc_y_Current_line ( void )
  {
    y_Current_line += 1 * y_step;
  }

  int get_free_lines ( void )
  {
    //Предполагается, что y_Current_line - ещё пустая линия
    int  tmp = ( (y_Current_line + 1 * y_step) - y_Last_line ) / y_step;
    return tmp;
  }

  void increment_page_num ( void )
  {
    PageNumber++;
  }

  int get_lines_in_others_pages( void )
  {
    return lines_in_others_pages;
  }

  int get_y_First_line( void )
  {
    return y_First_line;
  }

  int get_y_step( void )
  {
    return y_step;
  }

  int get_Page_num( void )
  {
    return PageNumber;
  }


  private:
    int      PageNumber;
    int      lines_in_first_page;
    int      lines_in_others_pages;
    int      y_First_line;
    int      y_Last_line;
    int      y_Current_line;
    int      y_step;
};











enum bom_type
{
  BOM_PCAD,
  BOM_AD,
  BOM_UNKNOWN
};



class do_element
{
public:
  do_element();
  do_element(const do_element &el2); //Конструктор копирования
  do_element operator=(const do_element &el2); // Оператор присваивания
  QString get_record(void);
  bool do_element_soft_check(QString &path_to_do, QString &source);
  bool is_empty(void);

  QString get_File_Name(void)
  {
    return File_Name;
  }
  QString get_Detimal_Number(void)
  {
    return Detimal_Number;
  }
  enum bom_type get_bom_type(void)
  {
    return bom_type_;
  }


private:
  QString File_Name;
  QString Detimal_Number;
  QFileInfo File_info;
  enum bom_type bom_type_;
};

#endif // DO_ELEMENT_H
