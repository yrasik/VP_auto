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

#ifndef PILOT_H
#define PILOT_H

#include "element.h"
#include <QtCore/qmath.h>

class pilot
{
public:
  pilot(QString &PathToOutDir, QVector<element> *el_, signed int Number_of_Copy_);
  ~pilot();
  void generate(void);
private:
  signed int y_First_line;
  signed int y_step;
  signed int last_line;
  signed int lines_in_first_page;
  signed int lines_in_others_pages;
  unsigned int x_RefDes;
  unsigned int x_Group;
  unsigned int x_Code;
  unsigned int x_Firm;
  unsigned int x_Count;
  unsigned int x_Decoding;

  unsigned int PageNumber;
  bool long_record;

  signed int Current_line;
  unsigned int Current_page;
  unsigned int lines_in_page;

  unsigned int y_object_height;

  signed int Number_of_Copy;

  QVector<element> *el;
  std::ifstream canvas_file;
  std::ofstream per_file;
  QVector<element>::iterator iter;
  QString PathToOutDir;
  QString FullFileName;
  QString detimal_number;
  std::string canvas_string;
  std::ofstream target_file;
  QString FullCanvasFileName;
  QVector<QString> *document; //Для хранения страниц документа
  QVector<element>::iterator current_element; //Указатель на текущий элемент в текущей таблице
  QVector<element>::iterator end_in_last_table;


  QVector<QString> records;
  QVector<QString>::iterator current_record;



  unsigned int x_PageNumber;
  unsigned int y_PageNumber;

  unsigned int x_DetimalNumber;
  unsigned int y_DetimalNumber;

  unsigned int x_Count_summ;

  void pcad_page_setup(void);

  void pcad_create_next_page(void);
  void pcad_create_records(void);
  void pcad_put_record(QString &str);
  void set_normal_record(QString &str);
  bool pcad_open_files(void);
  void pcad_close_files(void);


  void
  by_value_code(void);
  void
  by_refdes(void);
  void
  by_count(void);

};

#endif // PILOT_H
