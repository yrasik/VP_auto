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
#include "vp.h"

#define LOG ""

vp::vp ( QString &PathToEtcDir, QString &PathToOutDir, QVector<element> *el_, const QString &detimal_number_, signed int Number_of_Copy_ )
{
  this->PathToEtcDir = PathToEtcDir;
  this->PathToOutDir = PathToOutDir;

  page_setup();

  Number_of_Copy = Number_of_Copy_;
  codec = QTextCodec::codecForName( "Windows-1251" );

  detimal_number = detimal_number_;
  el = el_;

  document = new QVector<QString>;

  FullFileName = this->PathToOutDir + "/" + detimal_number + codec->toUnicode( "ВП.sch" );
  FullFileName_txt = this->PathToOutDir + "/" + detimal_number + codec->toUnicode( "ВП.txt" );
  FullFileName_order_txt = this->PathToOutDir + "/" + detimal_number + codec->toUnicode( "ВП_для_заказа.txt" );

  FullCanvasFileName = QCoreApplication::applicationDirPath() + "/../canvas/vp_canvas.sch";
}

vp::~vp ()
{
  delete l_ctrl;
  delete document;
}

void vp::page_setup ( void )
{
  QString filename( PathToEtcDir + codec->toUnicode("/vp_settings.lua"));
  int       err;

  x_Name_mm = 29;
  x_Code_mm = x_Name_mm + 60;
  x_Doc_for_order_mm = x_Code_mm + 43;
  x_Provider_mm = x_Doc_for_order_mm + 70;
  x_Assembly_mm = x_Provider_mm + 55;
  x_Count1_mm = 327;
  x_Count2_mm = 343;
  x_Count3_mm = 359;
  x_Count_summ_mm = 375;
  x_Note_mm = 391;

  x_PageNumber_mm = 405;
  x_PageNumber_end_mm = 415;
  y_PageNumber_mm = 7;

  x_DetimalNumber_mm = 295;
  y_DetimalNumber_mm = 10;
  y_DetimalNumber_first_mm = 35;

  shift_text_mm = 1;

  y_First_line = 259;
  lines_in_first_page = 24;
  lines_in_others_pages = 30;

  Page_content = "";


  lua_State *L = luaL_newstate();
  luaL_openlibs( L );

  err = luaL_loadfile( L, filename.toLocal8Bit().data() );
  if ( err != LUA_OK )
  {
    QString err = codec->toUnicode("WARNING: Ошибка в файле '") +
                  filename + QObject::tr("' :") +
                  codec->toUnicode( lua_tostring(L, -1) );
    *plog << err << endl;
    lua_close( L );
    return;
  }
  lua_pcall(L, 0, 0, 0);

  lua_getglobal(L, "x_Name_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    x_Name_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "x_Code_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    x_Code_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "x_Doc_for_order_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    x_Doc_for_order_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "x_Provider_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    x_Provider_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "x_Assembly_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    x_Assembly_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "x_Count1_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    x_Count1_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "x_Count2_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    x_Count2_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "x_Count3_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    x_Count3_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "x_Count_summ_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    x_Count_summ_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "x_Note_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    x_Note_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "x_PageNumber_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    x_PageNumber_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "x_PageNumber_end_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    x_PageNumber_end_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "y_PageNumber_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    y_PageNumber_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "x_DetimalNumber_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    x_DetimalNumber_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "y_DetimalNumber_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    y_DetimalNumber_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "y_DetimalNumber_first_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    y_DetimalNumber_first_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "shift_text_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    shift_text_mm = (float)(lua_tonumberx) (L, -1, NULL);


  lua_getglobal(L, "y_First_line");
  if( lua_type(L, -1) == LUA_TNUMBER )
    y_First_line = (int)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "lines_in_first_page");
  if( lua_type(L, -1) == LUA_TNUMBER )
    lines_in_first_page = (int)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "lines_in_others_pages");
  if( lua_type(L, -1) == LUA_TNUMBER )
    lines_in_others_pages = (int)(lua_tonumberx) (L, -1, NULL);

  l_ctrl = new line_control( y_First_line, lines_in_first_page, lines_in_others_pages );

  lua_close( L );
}

void vp::generate ( void )
{
  *plog << endl;
  *plog << "#########################################################################" << endl;
  *plog << codec->toUnicode( "###                   Генерация ведомости покупных                    ###" ) << endl;
  *plog << "#########################################################################" << endl;
  *plog << endl;
  *plog << "START {" << endl;

  sorting();

  create_first_page();



  for ( iter_same_Ref = bom.begin(); iter_same_Ref < bom.end(); iter_same_Ref++ )
  {
    put_group_title();
    for ( iter_same_Ref_Value = iter_same_Ref->begin(); iter_same_Ref_Value < iter_same_Ref->end(); iter_same_Ref_Value++ )
    {
      try_create_page( 0 );
      put_record();
    }
  }

  try_create_page( l_ctrl->get_lines_in_others_pages() );                 //Формируем страницу из всего что осталось

  //Тут уже сформирован перечень
  QVector<QString>::iterator  current_page = document->begin();

  if ( open_files() )
  {
    while ( !getline(canvas_file, canvas_string, '\n').eof() )
    {
      target_file << canvas_string << std::endl;
      if ( canvas_string.find("fieldSetRef") != std::string::npos )
      {
        if ( current_page != document->end() )
        {
          target_file << codec->fromUnicode( *current_page ).constData();
          current_page++;
        }
      }
    }

    close_files();
  }

  *plog << "} END " << endl;
}

bool vp::open_files ( void )
{
  canvas_file.open( FullCanvasFileName.toLatin1(), std::ios::in );
  if ( !canvas_file )
  {
    *plog << codec->toUnicode( "ERROR: Не могу открыть \"vp_canvas.sch\" файл" ) << endl;
    return false;
  }
  else
  {
    *plog << codec->toUnicode( "Открываем \"" ) << FullCanvasFileName << codec->toUnicode( "\" файл;" ) << endl;
    target_file.open( codec->fromUnicode(FullFileName).constData(), std::ios::out );
    if ( !target_file )
    {
      *plog << codec->toUnicode( "ERROR: Не могу создать \"" ) << FullFileName << codec->toUnicode( "\" файл;" ) << endl;
      return false;
    }

    *plog << codec->toUnicode( "Создаём \"" ) << FullFileName << codec->toUnicode( "\" файл" ) << endl;
  }

  return true;
}

void vp::close_files ( void )
{
  target_file.close();
  *plog << codec->toUnicode( "Закрываем \"" ) << FullFileName << codec->toUnicode( "\" файл" ) << endl;
  canvas_file.close();
  *plog << codec->toUnicode( "Закрываем \"" ) << FullCanvasFileName << codec->toUnicode( "\" файл" ) << endl;
}

void vp::create_first_page ( void )
{
  QTextStream temp ( &Page_content );

  QString Detimal_Number = bom.begin()->begin()->begin()->get_Detimal_Number() + codec->toUnicode("ВП");;
  float   Detimal_Number_width_mm = Text_Width->mm( Detimal_Number );
  float   x_Detimal_Number_center_mm = x_DetimalNumber_mm + ( (x_PageNumber_mm - x_DetimalNumber_mm) / 2 ) - Detimal_Number_width_mm / 2;

  temp << "(text (pt " << x_Detimal_Number_center_mm << " " << y_DetimalNumber_first_mm << ") \"" << Detimal_Number << "\" (textStyleRef \"GostB-16\"))" << endl;
}

void vp::put_group_title ( void )
{
  QTextStream temp ( &Page_content );
  QString     Ref = iter_same_Ref->begin()->begin()->get_RefDes();

  Group_Title_state = GROUP_TITLE_ONLY_FIRST;

  if ( (Ref == codec->toUnicode("A")) || (Ref == codec->toUnicode("B")) ) //Под этими Ref могут скрываться разные штуки....
  {
//    *plog << LOG << endl;
    Group_Title_state = GROUP_TITLE_EMPTY;
  }
  else
  {
    int record_Ref_size = iter_same_Ref->size();

    if ( record_Ref_size == 1 )
    {
      Group_Title_state = GROUP_TITLE_EMPTY;
    }
    else
    {
      int record_size = iter_same_Ref->begin()->size();

      if ( record_size == 1 )
      {
        record_size = 1 + 1 + 1;  //заг.группы + назв.элемента + код
        if ( iter_same_Ref->begin()->begin()->get_Value_Decoding().size() != 0 )          //3 строки минимум на странице должны оставаться + 1 строка с названием группы
        {
          record_size++;
        }
      }
      else
      {
        record_size = 1 + 1 + record_size + 1;
      }

      try_create_page( record_size );

      //  QString Ref_current = iter_same_Ref->begin()->begin()->get_RefDes();
      QVector<QVector<QVector<element> > >::iterator  iter1 = ( iter_same_Ref /*+ 1 */ ); //FIXME
      if ( iter1 < bom.end() )
      {
        QString Ref = iter1->begin()->begin()->get_RefDes();

        QString Group_Name = iter_same_Ref->begin()->begin()->get_Group_Name();
        if ( Ref == codec->toUnicode("D") )
        {
          Group_Name = codec->toUnicode( "Схемы интегральные, микросборки" );
        }

        float Group_Name_width_mm = Text_Width->mm( Group_Name );

        temp << "(text (pt " << ( x_Name_mm + shift_text_mm + shift_text_mm ) << " " << l_ctrl->get_y_Current_line() << ") \"" << Group_Name << "\" (textStyleRef \"GostB-12\"))" << endl;
        temp << "(line (pt " << ( x_Name_mm + shift_text_mm ) << " " << ( l_ctrl->get_y_Current_line() - 0.3 ) << ") (pt " << ( x_Name_mm + Group_Name_width_mm + shift_text_mm ) << " " << ( l_ctrl->get_y_Current_line() - 0.3 ) << ") )" << endl;

        l_ctrl->dec_y_Current_line();
        Group_Title_state = GROUP_TITLE_ONLY_FIRST;
      }
      else
      {
        //iter_same_Ref - Последний элемент в списке
        l_ctrl->dec_y_Current_line();
        Group_Title_state = GROUP_TITLE_EMPTY;
      }
    }
  }
}

void vp::put_record ( void )
{
  QString tmp_str;

  try_put_record( tmp_str );

  if ( l_ctrl->get_free_lines() <= 0 )          //Не уместилось на стриницу. Переделаем...
  {
    tmp_str.clear();
    try_create_page( 2 );
    try_put_record( tmp_str );
    Page_content += tmp_str;
  }
  else
  {
    Page_content += tmp_str;
  }
}

void vp::try_put_record ( QString &tmp_str )
{
  QTextStream temp ( &tmp_str );

  bool        as_group_title = false;
  bool        group_title_empty = false;

  iter_same_Ref_Value_Detimal = iter_same_Ref_Value->begin();

  QString Type = iter_same_Ref_Value_Detimal->get_Type_from_Value();
  float   Type_width_mm = Text_Width->mm( Type );

  QString Code = iter_same_Ref_Value_Detimal->get_Code_from_Value();
  float   Code_width_mm = Text_Width->mm( Code );

  QString Firm = iter_same_Ref_Value_Detimal->get_Value_Firm();
  float   Firm_width_mm = Text_Width->mm( Firm );

  QString Decoding = iter_same_Ref_Value_Detimal->get_Value_Decoding();
  float   Decoding_width_mm = Text_Width->mm( Decoding );

  QString Detimal_Number = iter_same_Ref_Value_Detimal->get_Detimal_Number();
  float   Detimal_Number_width_mm = Text_Width->mm( Detimal_Number );

  QString Count = QString::number( iter_same_Ref_Value_Detimal->get_Count() );
  float   Count_width_mm = Text_Width->mm( Count );

  if ( Group_Title_state == GROUP_TITLE_EMPTY ) //Печатаем в строчке всё т.к. заголовка нет...
  {
    group_title_empty = true;
  }

  if ( Group_Title_state == GROUP_TITLE_ONLY_NEXT )
  {
    temp << "(text (pt " << ( x_Name_mm + shift_text_mm + shift_text_mm ) << " " << l_ctrl->get_y_Current_line() << ") \"" << Code << "\" (textStyleRef \"GostB-12\"))" << endl;
    temp << "(text (pt " << ( x_Doc_for_order_mm + shift_text_mm + shift_text_mm ) << " " << l_ctrl->get_y_Current_line() << ") \"" << Firm << "\" (textStyleRef \"GostB-12\"))" << endl;
    temp << "(text (pt " << ( x_Assembly_mm + shift_text_mm + shift_text_mm ) << " " << l_ctrl->get_y_Current_line() << ") \"" << Detimal_Number << "\" (textStyleRef \"GostB-12\"))" << endl;

    float x_Count1_center_mm = x_Count1_mm + ( (x_Count2_mm - x_Count1_mm) / 2 ) - Count_width_mm / 2;
    temp << "(text (pt " << x_Count1_center_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Count << "\" (textStyleRef \"GostB-12\"))" << endl;

    float x_Count_summ_center_mm = x_Count_summ_mm + ( (x_Note_mm - x_Count_summ_mm) / 2 ) - Count_width_mm / 2;
    temp << "(text (pt " << x_Count_summ_center_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Count << "\" (textStyleRef \"GostB-12\"))" << endl;

    int Count_summ = iter_same_Ref_Value_Detimal->get_Count();

    if ( Decoding_width_mm > 0 )
    {
      temp << "(text (pt " << ( x_Name_mm + 3 * shift_text_mm ) << " " << l_ctrl->get_y_Current_line() - 1 * l_ctrl->get_y_step() << ") \"" << Decoding << "\" (textStyleRef \"GostB-12\"))" << endl;
    }

    iter_same_Ref_Value_Detimal++;
    l_ctrl->dec_y_Current_line();

    while ( iter_same_Ref_Value_Detimal < iter_same_Ref_Value->end() )
    {
      QString Count = QString::number( iter_same_Ref_Value_Detimal->get_Count() );
      float   Count_width_mm = Text_Width->mm( Count );

      QString Detimal_Number = iter_same_Ref_Value_Detimal->get_Detimal_Number();
      float   Detimal_Number_width_mm = Text_Width->mm( Detimal_Number );
      temp << "(text (pt " << ( x_Assembly_mm + shift_text_mm + shift_text_mm ) << " " << l_ctrl->get_y_Current_line() << ") \"" << Detimal_Number << "\" (textStyleRef \"GostB-12\"))" << endl;

      float x_Count1_center_mm = x_Count1_mm + ( (x_Count2_mm - x_Count1_mm) / 2 ) - Count_width_mm / 2;
      temp << "(text (pt " << x_Count1_center_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Count << "\" (textStyleRef \"GostB-12\"))" << endl;

      float x_Count_summ_center_mm = x_Count_summ_mm + ( (x_Note_mm - x_Count_summ_mm) / 2 ) - Count_width_mm / 2;
      temp << "(text (pt " << x_Count_summ_center_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Count << "\" (textStyleRef \"GostB-12\"))" << endl;

      Count_summ += iter_same_Ref_Value_Detimal->get_Count();

      iter_same_Ref_Value_Detimal++;
      l_ctrl->dec_y_Current_line();
    }

    if ( iter_same_Ref_Value->size() > 1 )
    {
      temp << "(line (pt " << ( x_Count_summ_mm + shift_text_mm ) << " " << ( l_ctrl->get_y_Current_line() + 5.0 ) << ") (pt " << ( x_Note_mm - shift_text_mm ) << " " << ( l_ctrl->get_y_Current_line() + 5.0 ) << ") )" << endl;

      QString Count_summ_str = QString::number( Count_summ );
      float   Count_summ_str_width_mm = Text_Width->mm( Count_summ_str );

      float   x_Count_summ_center_mm = x_Count_summ_mm + ( (x_Note_mm - x_Count_summ_mm) / 2 ) - Count_summ_str_width_mm / 2;
      temp << "(text (pt " << x_Count_summ_center_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Count_summ_str << "\" (textStyleRef \"GostB-12\"))" << endl;
      l_ctrl->dec_y_Current_line();
    }
    else
    {
      if ( Decoding_width_mm > 0 )
      {
        l_ctrl->dec_y_Current_line();
      }
    }
  }

  if ( (Group_Title_state == GROUP_TITLE_ONLY_FIRST) || (as_group_title == true) || (group_title_empty == true) )
  {
    temp << "(text (pt " << ( x_Name_mm + shift_text_mm + shift_text_mm ) << " " << l_ctrl->get_y_Current_line() << ") \"" << Type << "\" (textStyleRef \"GostB-12\"))" << endl;
    temp << "(text (pt " << ( x_Doc_for_order_mm + shift_text_mm + shift_text_mm ) << " " << l_ctrl->get_y_Current_line() << ") \"" << Firm << "\" (textStyleRef \"GostB-12\"))" << endl;
    temp << "(text (pt " << ( x_Assembly_mm + shift_text_mm + shift_text_mm ) << " " << l_ctrl->get_y_Current_line() << ") \"" << Detimal_Number << "\" (textStyleRef \"GostB-12\"))" << endl;

    float x_Count1_center_mm = x_Count1_mm + ( (x_Count2_mm - x_Count1_mm) / 2 ) - Count_width_mm / 2;
    temp << "(text (pt " << x_Count1_center_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Count << "\" (textStyleRef \"GostB-12\"))" << endl;

    float x_Count_summ_center_mm = x_Count_summ_mm + ( (x_Note_mm - x_Count_summ_mm) / 2 ) - Count_width_mm / 2;
    temp << "(text (pt " << x_Count_summ_center_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Count << "\" (textStyleRef \"GostB-12\"))" << endl;

    int Count_summ = iter_same_Ref_Value_Detimal->get_Count();

    temp << "(text (pt " << ( x_Name_mm + shift_text_mm + shift_text_mm ) << " " << l_ctrl->get_y_Current_line() - l_ctrl->get_y_step() << ") \"" << Code << "\" (textStyleRef \"GostB-12\"))" << endl;

    if ( Decoding_width_mm > 0 )
    {
      temp << "(text (pt " << ( x_Name_mm + 3 * shift_text_mm ) << " " << l_ctrl->get_y_Current_line() - 2 * l_ctrl->get_y_step() << ") \"" << Decoding << "\" (textStyleRef \"GostB-12\"))" << endl;
    }

    iter_same_Ref_Value_Detimal++;
    l_ctrl->dec_y_Current_line();

    while ( iter_same_Ref_Value_Detimal < iter_same_Ref_Value->end() )
    {
      QString Count = QString::number( iter_same_Ref_Value_Detimal->get_Count() );
      float   Count_width_mm = Text_Width->mm( Count );

      QString Detimal_Number = iter_same_Ref_Value_Detimal->get_Detimal_Number();
      float   Detimal_Number_width_mm = Text_Width->mm( Detimal_Number );
      temp << "(text (pt " << ( x_Assembly_mm + shift_text_mm + shift_text_mm ) << " " << l_ctrl->get_y_Current_line() << ") \"" << Detimal_Number << "\" (textStyleRef \"GostB-12\"))" << endl;

      float x_Count1_center_mm = x_Count1_mm + ( (x_Count2_mm - x_Count1_mm) / 2 ) - Count_width_mm / 2;
      temp << "(text (pt " << x_Count1_center_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Count << "\" (textStyleRef \"GostB-12\"))" << endl;

      float x_Count_summ_center_mm = x_Count_summ_mm + ( (x_Note_mm - x_Count_summ_mm) / 2 ) - Count_width_mm / 2;
      temp << "(text (pt " << x_Count_summ_center_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Count << "\" (textStyleRef \"GostB-12\"))" << endl;

      Count_summ += iter_same_Ref_Value_Detimal->get_Count();

      iter_same_Ref_Value_Detimal++;
      l_ctrl->dec_y_Current_line();
    }

    if ( iter_same_Ref_Value->size() > 1 )
    {
      temp << "(line (pt " << ( x_Count_summ_mm + shift_text_mm ) << " " << ( l_ctrl->get_y_Current_line() + 5.0 ) << ") (pt " << ( x_Note_mm - shift_text_mm ) << " " << ( l_ctrl->get_y_Current_line() + 5.0 ) << ") )" << endl;

      QString Count_summ_str = QString::number( Count_summ );
      float   Count_summ_str_width_mm = Text_Width->mm( Count_summ_str );

      float   x_Count_summ_center_mm = x_Count_summ_mm + ( (x_Note_mm - x_Count_summ_mm) / 2 ) - Count_summ_str_width_mm / 2;
      temp << "(text (pt " << x_Count_summ_center_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Count_summ_str << "\" (textStyleRef \"GostB-12\"))" << endl;
      l_ctrl->dec_y_Current_line();
    }
    else
    {
      if ( Decoding_width_mm > 0 )
      {
        l_ctrl->dec_y_Current_line();
        l_ctrl->dec_y_Current_line();
      }
      else
      {
        l_ctrl->dec_y_Current_line();
      }
    }

    if ( group_title_empty == false )
    {
      Group_Title_state = GROUP_TITLE_ONLY_NEXT;
    }
  }

  l_ctrl->dec_y_Current_line();
}

void vp::try_create_page ( int req_lines )
{
  QTextStream temp ( &Page_content );

  bool        create_page = false;

  if ( ((int) (l_ctrl->get_free_lines()) - req_lines) <= 0 )
  {
    create_page = true;
  }

  if ( create_page == true )
  {
    document->push_back( Page_content );
    l_ctrl->increment_page_num();
    Page_content = "";
    l_ctrl->clear_y_Line_counter();

    if ( Group_Title_state == GROUP_TITLE_ONLY_NEXT )
    {
      Group_Title_state = GROUP_TITLE_ONLY_FIRST;
    }

    QString Page_num = QString::number(l_ctrl->get_Page_num());
    float   Page_num_width_mm = Text_Width->mm( Page_num );
    float x_Page_num_center_mm = x_PageNumber_mm + ( (x_PageNumber_end_mm - x_PageNumber_mm) / 2 ) - Page_num_width_mm / 2;

    temp << "(text (pt " << x_Page_num_center_mm << " " << y_PageNumber_mm << ") \"" << Page_num << "\" (textStyleRef \"GostB-12\"))" << endl;


    QString Detimal_Number = bom.begin()->begin()->begin()->get_Detimal_Number() + codec->toUnicode("ВП");;
    float   Detimal_Number_width_mm = Text_Width->mm( Detimal_Number );
    float   x_Detimal_Number_center_mm = x_DetimalNumber_mm + ( (x_PageNumber_mm - x_DetimalNumber_mm) / 2 ) - Detimal_Number_width_mm / 2;

    temp << "(text (pt " << x_Detimal_Number_center_mm << " " << y_DetimalNumber_mm << ") \"" << Detimal_Number << "\" (textStyleRef \"GostB-16\"))" << endl;
  }
}

void vp::sorting ( void )
{
  QVector<QVector<element> >  tables;           //Для хранения таблиц с одинаковыми элементами
  QVector<element>          temp_table;
  QVector<element>::iterator  ele;

  QVector<element>          el_local;

  tables.clear();

  for ( QVector<element>::iterator i = el->begin(); i != el->end(); i++ )
  {
    if ( i->get_RefDes() == codec->toUnicode("COMPO") )
    {
      i->set_RefDes( codec->toUnicode("ZCOMPO") );
    }
  }


  std::sort( el->begin(), el->end(), element::by_RefDes_RefDes_count ); //Упорядочили по Ref  и по Des
  for ( ele = el->begin(); ele < el->end(); ele++ )
  {
    el_local.push_back( *ele );
  }

#if 0
  * plog << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
  *plog << "           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
  for ( QVector<element>::iterator iter3 = el->begin(); iter3 != el->end(); iter3++ )
  {
    *plog << "               " << ( *iter3 ).get_record2() << endl;
  }

  *plog << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl << endl;
#endif
  while ( el_local.empty() == false )
  {
    ele = el_local.begin();
    temp_table.push_back( *ele );         //Помещаем оригинальный элемент в таблицу
    ele = el_local.erase( ele );          //Удаляем запись из таблицы чтоб не мешалась
    QVector<element>::iterator  temp = temp_table.begin();
    for ( ; ele < el_local.end(); ele++ ) //Перекидываем в таблицу элементы с одинаковым Ref
    {
      if ( (*temp).get_RefDes() == (*ele).get_RefDes() )
      {
        temp_table.push_back( *ele );
        ele = el_local.erase( ele );
        ele--;
      }
      else
      {
        break;
      }
    }

    tables.push_back( temp_table );
    temp_table.clear();
  }

#if 0
  * plog << "**************************************************************************************" << endl;
  *plog << "     ================================================================================" << endl;
  for ( QVector < QVector<element> >::iterator iter2 = tables.begin(); iter2 != tables.end(); iter2++ )
  {
    *plog << "           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
    for ( QVector<element>::iterator iter3 = (*iter2).begin(); iter3 != (*iter2).end(); iter3++ )
    {
      *plog << "               " << ( *iter3 ).get_full_record() << endl;
    }
  }

  *plog << "***************************************************************************************" << endl << endl;
#endif
  for ( QVector < QVector<element> >::iterator iter2 = tables.begin(); iter2 < tables.end(); iter2++ )
  {
    for ( QVector<element>::iterator iter3 = iter2->begin(); iter3 < iter2->end(); iter3++ )
    {
      QVector<element>::iterator  iter4 = ( iter3 + 1 );
      while ( iter4 < iter2->end() )
      {
        if ( true == element::by_Ref_Value_Code_Value_Decoding_Value_Firm_Detimal_Number(*iter3, *iter4) )
        {
          iter3->set_Count( iter3->get_Count() + iter4->get_Count() );
          iter4 = iter2->erase( iter4 );
          iter4--;
        }

        iter4++;
      }
    }
  }

  for ( QVector < QVector<element> >::iterator iter2 = tables.begin(); iter2 < tables.end(); iter2++ )
  {
    std::sort( iter2->begin(), iter2->end(), by_Type_from_Value_and_Code_from_Value );
  }

#if 0
  * plog << "**************************************************************************************" << endl;
  *plog << "     ================================================================================" << endl;
  for ( QVector < QVector<element> >::iterator iter2 = tables.begin(); iter2 != tables.end(); iter2++ )
  {
    *plog << "           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
    for ( QVector<element>::iterator iter3 = iter2->begin(); iter3 < iter2->end(); iter3++ )
    {
      *plog << "               " << iter3->get_record3() << endl;
    }
  }

  *plog << "***************************************************************************************" << endl << endl;
#endif
  for ( QVector < QVector<element> >::iterator iter2 = tables.begin(); iter2 != tables.end(); iter2++ )
  {
    QVector<QVector<element> >  bom_Ref;
    for ( QVector<element>::iterator iter3 = iter2->begin(); iter3 < iter2->end(); iter3++ )
    {
      QVector<element>  bom_Ref_Value;
      bom_Ref_Value.push_back( *iter3 );

      QVector<element>::iterator  iter4 = ( iter3 + 1 );
      while ( iter4 < iter2->end() )
      {
        if ( (iter3->get_Code_from_Value() == iter4->get_Code_from_Value()) && (iter3->get_Type_from_Value() == iter4->get_Type_from_Value()) && (iter3->get_Value_Decoding() == iter4->get_Value_Decoding()) && (iter3->get_Value_Firm() == iter4->get_Value_Firm()) )
        {
          bom_Ref_Value.push_back( *iter4 );
          iter4 = iter2->erase( iter4 );
          iter4--;
        }

        iter4++;
      }

      bom_Ref.push_back( bom_Ref_Value );
    }

    bom.push_back( bom_Ref );
  }

#if 0
  * plog << "**************************************************************************************" << endl;
  for ( QVector < QVector < QVector<element> > >::iterator iter1 = bom.begin(); iter1 < bom.end(); iter1++ )
  {
    *plog << "     ================================================================================" << endl;
    for ( QVector < QVector<element> >::iterator iter2 = iter1->begin(); iter2 < iter1->end(); iter2++ )
    {
      *plog << "           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
      for ( QVector<element>::iterator iter3 = iter2->begin(); iter3 < iter2->end(); iter3++ )
      {
        *plog << "               " << iter3->get_record3() << endl;
      }
    }
  }

  *plog << "***************************************************************************************" << endl << endl;
#endif
}

void vp::create_file_for_order ( void )
{
  QFile *order_file;

  order_file = new QFile( FullFileName_order_txt );
  if ( !order_file->open(QIODevice::WriteOnly | QIODevice::Text) )
  {
    *plog << codec->toUnicode( "ERROR: Не могу создать \"" ) << FullFileName_order_txt << codec->toUnicode( "\" файл;" ) << endl;
    return;
  }

  QTextStream order ( order_file );
  order.setCodec( "UTF-8" );

  order << codec->toUnicode( "#Наименование\tОбозначение\tФирма/ТУ\tКод группы\tКоличество\tПримечание\tЦена" ) << endl;

  for ( QVector < QVector < QVector<element> > >::iterator iter1 = bom.begin(); iter1 < bom.end(); iter1++ )
  {
    for ( QVector < QVector<element> >::iterator iter2 = iter1->begin(); iter2 < iter1->end(); iter2++ )
    {
      int summ = 0;
      QVector<element>::iterator  iter3;
      for ( iter3 = iter2->begin(); iter3 < iter2->end(); iter3++ )
      {
        summ += iter3->get_Count();

        //order << iter3->get_record() << endl;
      }

      iter3 = iter2->begin();
      order << iter3->get_record4() << "\t" << "\t" << QString::number( summ * Number_of_Copy ) << "\t" << "\t" << endl;
    }
  }

  *plog << codec->toUnicode( "Создаём \"" ) << FullFileName_order_txt << codec->toUnicode( "\" файл" ) << endl;
  order_file->close();
  *plog << codec->toUnicode( "Закрываем \"" ) << FullFileName_order_txt << codec->toUnicode( "\" файл" ) << endl;
  return;
}

void vp::create_file_for_excel ( void )
{
  QFile *order_file;

  order_file = new QFile( FullFileName_txt );
  if ( !order_file->open(QIODevice::WriteOnly | QIODevice::Text) )
  {
    *plog << codec->toUnicode( "ERROR: Не могу создать \"" ) << FullFileName_txt << codec->toUnicode( "\" файл;" ) << endl;
    return;
  }

  QTextStream order ( order_file );
  order.setCodec( "UTF-8" );

  order << codec->toUnicode( "#Наименование\tОбозначение\tФирма/ТУ\tКуда входит\tКоличество\tПримечание\tЦена" ) << endl;

  for ( QVector < QVector < QVector<element> > >::iterator iter1 = bom.begin(); iter1 < bom.end(); iter1++ )
  {
    for ( QVector < QVector<element> >::iterator iter2 = iter1->begin(); iter2 < iter1->end(); iter2++ )
    {
      QVector<element>::iterator  iter3;
      for ( iter3 = iter2->begin(); iter3 < iter2->end(); iter3++ )
      {
        order << iter3->get_record5() << endl;
      }
    }
  }

  *plog << codec->toUnicode( "Создаём \"" ) << FullFileName_txt << codec->toUnicode( "\" файл" ) << endl;
  order_file->close();
  *plog << codec->toUnicode( "Закрываем \"" ) << FullFileName_txt << codec->toUnicode( "\" файл" ) << endl;
  return;
}

