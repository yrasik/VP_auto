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

#ifndef per_H
  #define per_H

  #include "element.h"


enum Group_Title_state_enum { GROUP_TITLE_ONLY, GROUP_TITLE_AND_FIRM, GROUP_TITLE_EMPTY };

class per
{ public:
    per ( QString &PathToEtcDir, QString &PathToOutDir, QVector<element> *el_ );
    ~     per ();
    void  generate ( void );
  protected:
    float      x_Format;
    float      x_Pozition;
    float      x_Oboznachen;

    float      shift_text_mm;
    float      shift_Code_mm;
    float      x_Group_mm;
    float      x_RefDes_mm;
    float      x_Code_mm;
    float      x_Firm_mm;
    float      x_Count_mm;
    float      x_Decoding_mm;
    float      x_Addon_mm;
    float      x_Decoding_after_code_mm;

    float      Column_RefDes_size_mm;
    float      Column_Naimenovanie_size_mm;
    float      Column_Quantity_size_mm;
    float      Column_Note_size_mm;

    float x_PageNumber_mm;
    float x_PageNumber_end_mm;
    float y_PageNumber_mm;

    float x_DetimalNumber_mm;
    float y_DetimalNumber_mm;
    float y_DetimalNumber_first_mm;

    int y_First_line;
    int lines_in_first_page;
    int lines_in_others_pages;

    QString         PathToEtcDir;
    QString         PathToOutDir;
    QString         FullFileName;
    QString         FullCanvasFileName;

    QVector<element>  *el;        //Первоначально копируется перечень элементов
    QString         Page_content; //Формируемая страница документа
    QVector<QString>  *document;  //Для хранения страниц документа
    QVector<QVector<QVector<element> > >  perechen;

    std::ifstream canvas_file;
    std::ofstream target_file;

    void          by_refdes_and_value_code ( void );
    virtual void  page_setup ( void );
    virtual bool  open_files ( void );
    void          close_files ( void );
    void          create_first_stamp ( void );
    void          try_create_page ( int req_lines );
    void          put_group_title ( void );
    void          put_record ( void );
    void          try_put_record ( QString &tmp_str );
    void create_first_page ( void );

    QVector<QVector<QVector<element> > >::iterator  iter_same_Ref_before;
    QVector<QVector<QVector<element> > >::iterator  iter_same_Ref;
    QVector<QVector<element> >::iterator          iter_same_Ref_Firm;
    QVector<element>::iterator  iter_same_Ref_Firm_Name;

    enum Group_Title_state_enum Group_Title_state;
    class line_control *l_ctrl;

    float pixel_width_mm;         //Для расчета длины строки
};
#endif // per_H

