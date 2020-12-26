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
#ifndef ELEMENT_H
  #define ELEMENT_H

  #include <QtCore/QTextCodec>
  #include <QtCore/QString>
  #include <QtCore/QTextStream>
  #include <QtCore/QFile>
  #include <QtCore/QIODevice>
  #include <QtCore/QVector>
  #include <QtCore/QDir>
  #include <QtCore/QFileInfo>
  #include <QCoreApplication>
  #include <QApplication>
  #include <QFontMetrics>
  #include <QRegExp>

  #include <cstdlib>
  #include <string>
  #include <iostream>
  #include <sstream>
  #include <vector>
  #include <fstream>
  #include <vector>
  #include <algorithm>

  #include "do_element.h"
  #include "dot_export.h"
  #include "similarity.h"

  #include "firm_name.h"
  #include "text_name.h"
  #include "text_width.h"

  #include "lua/lua.hpp"

extern QTextStream  *plog;
extern QTextCodec   *codec;
extern QFontMetrics *FontMetric;
extern lua_State *L;



enum process { EXEC, RESTART, STOP };

enum pcad_put_record_state { PROCESSING, CURRENT_TABLE_END, CURRENT_PAGE_END, CURRENT_TABLE_END_AND_CURRENT_PAGE_END, SP_END };


extern QVector<text_name> Text_Names;
extern QVector<firm_name> Firm_Names;
extern text_width *Text_Width;






class element
{ public:

  enum sr
  {
    SR_SAME = 0,
    SR_SAME_CODE_FIRM______SIMILAR_DECODING_______DIFF_,
    SR_SAME_CODE_DECODING__SIMILAR_FIRM___________DIFF_,
    SR_SAME_DECODING_FIRM__SIMILAR_CODE___________DIFF_,
    SR_SAME_CODE___________SIMILAR_FIRM_DECODING_______DIFF_,

    SR_SAME_CODE___________SIMILAR_DECODING_______DIFF_FIRM,
    SR_SAME_CODE___________SIMILAR_FIRM___________DIFF_DECODING,
    SR_SAME_DECODING_______SIMILAR_CODE___________DIFF_FIRM,

    SR_SAME_CODE_DECODING__SIMILAR________________DIFF_FIRM,
    SR_SAME_CODE___________SIMILAR________________DIFF_FIRM_DECODING,
    SR_SAME________________SIMILAR_CODE___________DIFF_FIRM_DECODING,

    SR_SAME________________SIMILAR_CODE_DECODING__DIFF_FIRM,
    SR_SAME_CODE_FIRM______SIMILAR________________DIFF_DECODING,

    SR_SAME________________SIMILAR_DECODING_______DIFF_CODE_FIRM,
    SR_SAME_DECODING_______SIMILAR________________DIFF_CODE_FIRM,

    SR_SAME_FIRM___________SIMILAR_DECODING_______DIFF_CODE,
    SR_SAME_FIRM_DECODING__SIMILAR________________DIFF_CODE,
    SR_SAME_DECODING_______SIMILAR_FIRM___________DIFF_CODE,

    SR_SAME________________SIMILAR_DECODING_FIRM__DIFF_CODE,

    SR_SAME_FIRM___________SIMILAR_CODE_DECODING__DIFF_,
    SR_SAME_FIRM___________SIMILAR_CODE___________DIFF_DECODING,

    SR_SAME_DECODING_______SIMILAR_CODE_FIRM______DIFF_,

    SR_SAME________________SIMILAR_CODE_FIRM______DIFF_DECODING,
    SR_SAME________________SIMILAR_CODE_FIRM_DECODING__DIFF_,

    SR_DIFFERENT
  };


  struct sr_ss
  {
    enum sr  SR;
    enum similarity::ss SS;
  };


    element ();
    element ( QString &Value, QString &Ref, int file_line_num );
    element ( QString *source, QString *detimal, enum bom_type *bom_type_ );
    element ( const element &el2 );                 //Конструктор копирования
    friend bool operator    < ( const element &el1, const element &el2 );
    friend bool operator    > ( const element &el1, const element &el2 ); ////
    friend bool operator    == ( const element &el1, const element &el2 );
    friend bool operator    != ( const element &el1, const element &el2 );
    friend element operator + ( element el2 );
    element operator        = ( const element &el2 );                     // Оператор присваивания
    bool removable ( void )
    {
      return am_removable;
    }

    void set_removable(void)
    {
      am_removable = true;
    }

    friend bool by_Value_Code ( const element &el1, const element &el2 );

    friend bool by_RefDes_Count ( const element &el1, const element &el2 );

    friend bool by_Ref_Value_Code_Value_Decoding ( const element &el1, const element &el2 );

    friend bool by_Ref_Value_Code_Value_Decoding_Value_Firm_Detimal_Number ( const element &el1, const element &el2 );

    friend bool by_RefDes_RefDes_count ( const element &el1, const element &el2 );

    friend bool by_Type_from_Value_and_Code_from_Value ( const element &el1, const element &el2 );

    friend bool by_Value_Firm_Ref ( const QVector<element> &el1, const QVector<element> &el2 );

    QString get_record ( void );
    QString get_full_record ( void );

    QString get_full_record_empty_firm ( void );

    QString get_record1 ( void );
    QString get_record2 ( void );
    QString get_record3 ( void );
    QString get_record4 ( void );
    QString get_record5 ( void );
    QString get_record6 ( void );
    QString get_record7 ( void );
    QString get_record8 ( void );
    QString get_record9 ( void );

    QString get_RefDes ( void )
    {
      return RefDes;
    }

    void set_RefDes ( QString str )
    {
      RefDes = str;
    }

    unsigned int get_RefDes_Count ( void )
    {
      return RefDes_count;
    }

    QString get_RefDes_and_Count ( void )
    {
      return RefDes + QString::number( RefDes_count );
    }

    QString get_Value_Code ( void )
    {
      return Value_Code;
    }

    QString get_Value_Decoding ( void )
    {
      return Value_Decoding;
    }

    QString get_Value_Firm ( void )
    {
      return Value_Firm;
    }

    QString get_Detimal_Number ( void )
    {
      return Detimal_Number;
    }

    QString get_Type_from_Value ( void )
    {
      return Type_from_Value;
    }

    QString get_Type_from_Value_if_not_equal_Group_Name ( void )
    {
      if ( Types_from_Value != Group_Name )
      {
        return Type_from_Value + " ";
      }
      else
      {
        return "";
      }
    }

    QString get_Types_from_Value ( void )
    {
      return Types_from_Value;
    }

    QString get_Code_from_Value ( void )
    {
      return Code_from_Value;
    }

    unsigned int get_Count ( void )
    {
      return Count;
    }

    void set_Count ( unsigned int cnt )
    {
      Count = cnt;
    }

    QString get_Group_Name ( void )
    {
      return Group_Name;
    }

    void        analyse_Value_Code ( void );

    static bool by_Value_Code ( const element &el1, const element &el2 );
    static bool by_RefDes_Count ( const element &el1, const element &el2 );

    static bool by_Ref_Value_Code_Value_Decoding ( const element &el1, const element &el2 );

    static bool by_Ref_Value_Code_Value_Decoding_Value_Firm_Detimal_Number ( const element &el1, const element &el2 );

    static bool by_RefDes_RefDes_count ( const element &el1, const element &el2 );

    static bool by_Type_from_Value_and_Code_from_Value ( const element &el1, const element &el2 );

    static bool by_Value_Firm_Ref ( const QVector<element> &el1, const QVector<element> &el2 );

    static void sorting_by_refdes ( QVector<element> &ls1 );

    static void sorting_by_value_code ( QVector<element> &ls1 );

    static void sorting_by_count ( QVector<element> &ls1 );

    static void similarity_elements ( QVector<element> &ls1 );

    static void similarity_firm ( QVector<element> &ls1, QVector<firm_name> &Firm_Names );

    static bool eq_Ref_Type_Code_Decoding_Firm ( const element &el1, const element &el2 );

    static void remove_blocks_and_adding_content ( QString PathToOutDir, QVector<QVector<element> > &tables, QVector<element> &ls1 );

    friend struct sr_ss is_Similarity ( const element &el1, const element &el2 );
    static struct sr_ss is_Similarity ( const element &el1, const element &el2 );

    friend QString Decoding_Similarity_result ( struct sr_ss SR_SS );
    static QString Decoding_Similarity_result ( struct sr_ss SR_SS );

  private:
    bool         am_removable;
    int           Count;
    QString       Value_Firm;
    QString       Detimal_Number;
    QString       RefDes;           //Это вообщето Ref
    int           RefDes_count;
    QString       Value_Code;
    QString       Value_Decoding;

    QString       Code_from_Value;
    QString       Type_from_Value;  //Type_from_Value=Терморезистор
    QString       Types_from_Value; //Types_from_Value=Терморезисторы
    QString       Group_Name;       //Type_from_Value=Терморезистор входит в Group_Name=Резисторы
    bool          element_pcad ( QString *source, QString *detimal );
    bool          element_ad ( QString *source, QString *detimal );
    int           file_line_num;
};


// Попробуем применить Lua для продвинутой сортировки с учетом тонких особенностей написяния отечественных обозначений.
bool by_Value_Firm_Ref ( const QVector<element> &el1, const QVector<element> &el2 );

bool by_Type_from_Value_and_Code_from_Value ( const element &el1, const element &el2 );




#endif // ELEMENT_H

