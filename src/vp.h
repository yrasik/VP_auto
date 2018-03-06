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
#ifndef VP_H
  #define VP_H

  #include "element.h"

class vp
{ public:
    vp ( QString &PathToOutDir, QVector<element> *el_, const QString &detimal_number_, signed int Number_of_Copy_ );
    ~     vp ();
    void  generate ( void );
    void  create_file_for_excel ( void );
    void  create_file_for_order ( void ); private:
    enum Group_Title_state_enum { GROUP_TITLE_ONLY_FIRST, GROUP_TITLE_ONLY_NEXT, GROUP_TITLE_EMPTY };

    float x_Name_mm;
    float x_Code_mm;
    float x_Doc_for_order_mm;
    float x_Provider_mm;
    float x_Assembly_mm;
    float x_Count1_mm;
    float x_Count2_mm;
    float x_Count3_mm;
    float x_Count_summ_mm;
    float x_Note_mm;

    float x_PageNumber_mm;
    float x_PageNumber_end_mm;
    float y_PageNumber_mm;

    float x_DetimalNumber_mm;
    float y_DetimalNumber_mm;
    float y_DetimalNumber_first_mm;

    bool long_record;
    signed int Number_of_Copy;

    void page_setup ( void );

    QString FullCanvasFileName;
    QString FullFileName_txt;
    QString FullFileName_order_txt;

    QVector<QString> *document; //Для хранения страниц документа
    QVector<element> *el;
    QString Page_content;       //Формируемая страница документа
    QString detimal_number;
    QString PathToOutDir;
    QString FullFileName;

    std::ifstream canvas_file;
    std::string canvas_string;
    std::ofstream target_file;

    bool open_files ( void );
    void close_files ( void );

    QVector<QVector<element> >::iterator current_table; //Указатель на текущую таблицу из массива tables
    QVector<element>::iterator current_element;         //Указатель на текущий элемент в текущей таблице
    QVector<element>::iterator end_in_last_table;

    class line_control *l_ctrl;
    void sorting ( void );

    void try_create_page ( int req_lines );
    void put_group_title ( void );
    void put_record ( void );
    void try_put_record ( QString &tmp_str );
    void create_first_page ( void );

    QVector<QVector<QVector<element> > > bom;           //Ведомость покупных изделий. Отсортировано по Ref, Value, Detimal_Number.
    QVector<QVector<QVector<element> > >::iterator iter_same_Ref;
    QVector<QVector<element> >::iterator iter_same_Ref_Value;
    QVector<element>::iterator iter_same_Ref_Value_Detimal;

    float shift_text_mm;

    enum Group_Title_state_enum Group_Title_state;
};
#endif // VP_H

