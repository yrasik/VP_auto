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
#ifndef SP_H
  #define SP_H

  #include "element.h"

class sp
{ public:
    sp ( QString &PathToOutDir, QVector<element> *el_ );
    ~     sp ();
    void  generate ( void ); private:

    float x_Format_mm;
    float x_Pozition_mm;
    float x_Oboznachen_mm;
    float x_Code_mm;
    float x_Count_mm;
    float x_Addon_mm;
    float x_Addon_end_mm;

    float shift_text_mm;

    int Poz_Oboznachenie;

    float x_PageNumber_mm;
    float x_PageNumber_end_mm;
    float y_PageNumber_mm;

    float x_DetimalNumber_mm;
    float y_DetimalNumber_mm;
    float y_DetimalNumber_first_mm;

    bool long_record;

    QString PathToOutDir;
    QString FullFileName;
    QString FullCanvasFileName;

    bool put_group_title_and_firm;
    bool old_element_in_groupe;

    QVector<element> *el;       //Первоначально копируется перечень элементов
    QVector<QString> *document; //Для хранения страниц документа
    std::ifstream canvas_file;
    std::ofstream target_file;

    bool processing;

    void sorting ( void );
    void page_setup ( void );
    bool open_files ( void );
    void close_files ( void );

    bool try_create_page ( int req_lines );
    void put_group_title ( void );
    void put_record ( void );
    void try_put_record ( QString &tmp_str );

    void create_first_page ( void );

    class line_control *l_ctrl;

    QString Page_content;       //Формируемая страница документа

    QVector<QVector<QVector<QVector<element> > > > spec;

    QVector<QVector<QVector<QVector<element> > > >::iterator iter_same_Ref_Firm;
    QVector<QVector<QVector<element> > >::iterator iter_same_Ref_Firm_Code;
    QVector<QVector<element> >::iterator iter_same_Ref_Firm_Code_Addon;
};
#endif // SP_H

