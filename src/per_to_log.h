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

#ifndef PER_TO_LOG_H
#define PER_TO_LOG_H

#include "element.h"

class per_to_log
{

public:
  per_to_log(QString &PathToOutDir, QVector<element> *el);
  ~per_to_log();
  void generate(void);

private:
  signed int y_First_line;
  signed int y_step;
  signed int lines_in_first_page;
  signed int lines_in_others_pages;
  signed int last_line;
  unsigned int x_RefDes;

  float x_GOSTB12_symbol_width;
  unsigned int x_Format;
  unsigned int x_Pozition;
  unsigned int x_Oboznachen;
  unsigned int x_Code;
  unsigned int x_Decoding;
  unsigned int x_Firm;
  unsigned int x_Count;
  unsigned int x_Addon;
  unsigned int x_Group;
  unsigned int Poz_Oboznachenie;

  unsigned int x_PageNumber;
  unsigned int y_PageNumber;

  unsigned int x_DetimalNumber;
  unsigned int y_DetimalNumber;

  unsigned int PageNumber;

  signed int Current_line;

  bool long_record;

  QString PathToOutDir;
  QString FullFileName;
  QString FullCanvasFileName;


  bool old_element_in_groupe;

  QVector<element> *el; //Первоначально копируется перечень элементов
  QVector<QString> *document; //Для хранения страниц документа
  QVector<QVector<QVector<element> > > perechen;

  std::ifstream canvas_file;
  std::ofstream target_file;


  enum pcad_put_record_state state;
  bool processing;

  void by_refdes_and_value_code(void);
  void page_setup(void);
  bool open_files(void);
  void close_files(void);
  void create_first_stamp(void);
  int  create_first_page(void);
  int  create_next_page(void);
  void put_group_title(void);
  void put_record(void);
  void create_perechen_image(void);




private:
  QVector < QVector < QVector<element> > >::iterator iter_same_Ref_before;
  QVector < QVector < QVector<element> > >::iterator iter_same_Ref;
  QVector < QVector<element> >::iterator iter_same_Ref_Firm;
  QVector<element>::iterator iter_same_Ref_Firm_Name;

  bool put_group_title_and_firm;

};

#endif // PER_TO_LOG_H
