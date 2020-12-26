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
#include "pcad_per_for_sch.h"

#define LOG ""

per::per ( QString &PathToEtcDir,  QString &PathToOutDir, QVector<element> *el_ )
{

  this->PathToEtcDir = PathToEtcDir;
  this->PathToOutDir = PathToOutDir;

  page_setup();


  el = new QVector<element>;
  document = new QVector<QString>;

  for ( QVector<element>::iterator i = el_->begin(); i != el_->end(); i++ )
  {
    el->push_back( *i );
  }

  FullFileName = this->PathToOutDir + "/" + el->begin()->get_Detimal_Number() + codec->toUnicode( "ПЭ3.sch" );

  FullCanvasFileName = QCoreApplication::applicationDirPath() + "/../canvas/per_canvas.sch";
}

per::~per ()
{
  delete el;
  delete l_ctrl;
  delete document;
}

void per::generate ( void )
{
  *plog << endl;
  *plog << "#########################################################################" << endl;
  *plog << codec->toUnicode( "###                  Генерация перечня элементов.                     ###" ) << endl;
  *plog << "#########################################################################" << endl;
  *plog << endl;
  *plog << "START {" << endl;

  by_refdes_and_value_code(); //////////////////////////////
  create_first_page();


  std::string canvas_string;

  iter_same_Ref_before = NULL;
  for ( iter_same_Ref = perechen.begin(); iter_same_Ref < perechen.end(); iter_same_Ref++ )
  {
    if ( (iter_same_Ref_before == NULL) || (iter_same_Ref_before->begin()->begin()->get_RefDes() < iter_same_Ref->begin()->begin()->get_RefDes()) )  //Меняется Ref группы
    {
      put_group_title();
    }

    for ( iter_same_Ref_Firm = iter_same_Ref->begin(); iter_same_Ref_Firm < iter_same_Ref->end(); iter_same_Ref_Firm++ )
    {
      try_create_page( 0 );
      put_record();
    }

    iter_same_Ref_before = iter_same_Ref;
  }

  try_create_page( l_ctrl->get_lines_in_others_pages() ); //Формируем страницу из всего что осталось

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

void per::page_setup ( void )
{
  QString filename( PathToEtcDir + codec->toUnicode("/per_for_sch_settings.lua"));
  int       err;

  x_PageNumber_mm = 195.5;
  x_PageNumber_end_mm = 205.5;
  y_PageNumber_mm = 7;

  x_DetimalNumber_mm = 85.5;
  y_DetimalNumber_mm = 10;
  y_DetimalNumber_first_mm = 35;

  shift_text_mm = 2;
  shift_Code_mm = 10;
  x_RefDes_mm = 20;
  Column_RefDes_size_mm = 20;
  Column_Naimenovanie_size_mm = 110;
  Column_Quantity_size_mm = 10;
  Column_Note_size_mm = 45;

  x_Code_mm = x_RefDes_mm + Column_RefDes_size_mm;
  x_Group_mm = x_Code_mm + shift_Code_mm;
  x_Firm_mm = x_Code_mm + 70;
  x_Decoding_mm = x_Code_mm + 20;
  x_Decoding_after_code_mm = x_Code_mm + 60;
  x_Count_mm = x_Code_mm + Column_Naimenovanie_size_mm;

  y_First_line = 270;
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

  lua_getglobal(L, "shift_Code_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    shift_Code_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "x_RefDes_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    x_RefDes_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "Column_RefDes_size_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    Column_RefDes_size_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "Column_Naimenovanie_size_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    Column_Naimenovanie_size_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "Column_Quantity_size_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    Column_Quantity_size_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "Column_Note_size_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    Column_Note_size_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "x_Code_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    x_Code_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "x_Group_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    x_Group_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "x_Firm_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    x_Firm_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "x_Decoding_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    x_Decoding_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "x_Decoding_after_code_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    x_Decoding_after_code_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "x_Count_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    x_Count_mm = (float)(lua_tonumberx) (L, -1, NULL);


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

bool per::open_files ( void )
{
  canvas_file.open( FullCanvasFileName.toLatin1(), std::ios::in );
  if ( !canvas_file )
  {
    *plog << LOG << codec->toUnicode( "ERROR: Не могу открыть \"per_canvas.sch\" файл" ) << endl;
    return false;
  }
  else
  {
    *plog << LOG << codec->toUnicode( "Открываем \"" ) << FullCanvasFileName << codec->toUnicode( "\" файл;" ) << endl;
    target_file.open( codec->fromUnicode(FullFileName).constData(), std::ios::out );
    if ( !target_file )
    {
      *plog << LOG << codec->toUnicode( "ERROR: Не могу создать \"" ) << FullFileName << codec->toUnicode( "\" файл;" ) << endl;
      return false;
    }

    *plog << LOG << codec->toUnicode( "Создаём \"" ) << FullFileName << codec->toUnicode( "\" файл" ) << endl;
  }

  return true;
}

void per::close_files ( void )
{
  target_file.close();
  *plog << LOG << codec->toUnicode( "Закрываем \"" ) << FullFileName << codec->toUnicode( "\" файл" ) << endl;
  canvas_file.close();
  *plog << LOG << codec->toUnicode( "Закрываем \"" ) << FullCanvasFileName << codec->toUnicode( "\" файл" ) << endl;
}

void per::create_first_page ( void )
{
  QTextStream temp ( &Page_content );

  QString Detimal_Number = perechen.begin()->begin()->begin()->get_Detimal_Number() + codec->toUnicode("ПЭ3");
  float   Detimal_Number_width_mm = Text_Width->mm( Detimal_Number );
  float   x_Detimal_Number_center_mm = x_DetimalNumber_mm + ( (x_PageNumber_mm - x_DetimalNumber_mm) / 2 ) - Detimal_Number_width_mm / 2;

  temp << "(text (pt " << x_Detimal_Number_center_mm << " " << y_DetimalNumber_first_mm << ") \"" << Detimal_Number << "\" (textStyleRef \"GostB-16\"))" << endl;
}

void per::put_group_title ()
{
  QTextStream temp ( &Page_content );
  QString     Ref = iter_same_Ref->begin()->begin()->get_RefDes();

  Group_Title_state = GROUP_TITLE_ONLY;

  if ( (Ref == codec->toUnicode("A")) || (Ref == codec->toUnicode("B")) ) //Под этими Ref могут скрываться разные штуки....
  {
    *plog << LOG << endl;
    Group_Title_state = GROUP_TITLE_EMPTY;
  }
  else
  {
    try_create_page( 4 );

    QString Ref_current = iter_same_Ref->begin()->begin()->get_RefDes();

    QVector<QVector<QVector<element> > >::iterator  iter1 = ( iter_same_Ref + 1 );

    if ( iter1 < perechen.end() )
    {
      QString Ref = iter1->begin()->begin()->get_RefDes();

      if ( Ref_current == Ref )
      {
        //В группе "Переключатели" несколько видов кнопок разных фирм
        if ( l_ctrl->get_y_Current_line() != l_ctrl->get_y_First_line() )   //Вверху страницы строчку не пропускаем...
        {
          l_ctrl->dec_y_Current_line();
        }

        QString Group_Name = iter_same_Ref->begin()->begin()->get_Group_Name();
        float   Group_Name_width_mm = Text_Width->mm( Group_Name );

        temp << "(text (pt " << ( x_Group_mm + shift_text_mm ) << " " << l_ctrl->get_y_Current_line() << ") \"" << Group_Name << "\" (textStyleRef \"GostB-12\"))" << endl;
        temp << "(line (pt " << x_Group_mm << " " << ( l_ctrl->get_y_Current_line() - 0.3 ) << ") (pt " << ( x_Group_mm + Group_Name_width_mm + shift_text_mm ) << " " << ( l_ctrl->get_y_Current_line() - 0.3 ) << ") )" << endl;

        l_ctrl->dec_y_Current_line();
        Group_Title_state = GROUP_TITLE_ONLY;
      }
      else
      {
        QVector<QVector<element> >::iterator  iter2 = iter_same_Ref->begin();
        QVector<QVector<element> >::iterator  iter2_second = iter2 + 1;

        if ( iter2_second < iter_same_Ref->end() )
        {
          //В группе "Переключатели" несколько видов кнопок одной фирмы
          if ( l_ctrl->get_y_Current_line() != l_ctrl->get_y_First_line() ) //Вверху страницы строчку не пропускаем...
          {
            l_ctrl->dec_y_Current_line();
          }

          QString Group_and_Firm = iter_same_Ref->begin()->begin()->get_Group_Name() + "   " + iter_same_Ref->begin()->begin()->get_Value_Firm();
          float   Group_and_Firm_width_mm = Text_Width->mm( Group_and_Firm );

          temp << "(text (pt " << ( x_Group_mm + shift_text_mm ) << " " << l_ctrl->get_y_Current_line() << ") \"" << Group_and_Firm << "\" (textStyleRef \"GostB-12\"))" << endl;
          temp << "(line (pt " << x_Group_mm << " " << ( l_ctrl->get_y_Current_line() - 0.3 ) << ") (pt " << ( x_Group_mm + Group_and_Firm_width_mm + shift_text_mm ) << " " << ( l_ctrl->get_y_Current_line() - 0.3 ) << ") )" << endl;

          l_ctrl->dec_y_Current_line();
          Group_Title_state = GROUP_TITLE_AND_FIRM;
        }
        else
        {
          //В группе "Переключатели" только один вид кнопок
          l_ctrl->dec_y_Current_line();
          Group_Title_state = GROUP_TITLE_EMPTY;
        }
      }
    }
    else
    {
      //iter_same_Ref - Последний элемент в списке
      l_ctrl->dec_y_Current_line();
      Group_Title_state = GROUP_TITLE_EMPTY;
    }
  }
}

void per::put_record ( void )
{
  QString tmp_str;

  try_put_record( tmp_str );

  if ( l_ctrl->get_free_lines () <= 0 )                 //Не уместилось на стриницу. Переделаем...
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

void per::try_put_record ( QString &tmp_str )
{
  QTextStream temp ( &tmp_str );

  bool        as_group_title = false;

  iter_same_Ref_Firm_Name = iter_same_Ref_Firm->begin();

  QString Type_or_Group_Name = iter_same_Ref_Firm_Name->get_Type_from_Value_if_not_equal_Group_Name();
  float   Type_or_Group_Name_width_mm = Text_Width->mm( Type_or_Group_Name );

  if ( Group_Title_state == GROUP_TITLE_EMPTY )       //Печатаем в строчке всё т.к. заголовка нет...
  {
    Type_or_Group_Name = iter_same_Ref_Firm_Name->get_Type_from_Value() + "  ";
    Type_or_Group_Name_width_mm = Text_Width->mm( Type_or_Group_Name );

    as_group_title = true;

    QString Ref = iter_same_Ref_Firm_Name->get_RefDes();

    if ( (Ref == codec->toUnicode("A")) )             //Под этими Ref могут скрываться разные штуки....
    {
      Type_or_Group_Name.clear();
      Type_or_Group_Name_width_mm = Text_Width->mm( Type_or_Group_Name );
    }
  }

  QString Type = iter_same_Ref_Firm_Name->get_Type_from_Value();
  float   Type_width_mm = Text_Width->mm( Type );

  QString Code = iter_same_Ref_Firm_Name->get_Code_from_Value();
  float   Code_width_mm = Text_Width->mm( Code );

  QString Firm = iter_same_Ref_Firm_Name->get_Value_Firm();
  float   Firm_width_mm = Text_Width->mm( Firm );

  QString Decoding = iter_same_Ref_Firm_Name->get_Value_Decoding();
  float   Decoding_width_mm = Text_Width->mm( Decoding );

  QString RefDeses = iter_same_Ref_Firm_Name->get_RefDes_and_Count();

  switch ( iter_same_Ref_Firm_Name->get_Count() )
  {
    case 1:
      break;
    case 2:
      RefDeses += ", " + iter_same_Ref_Firm_Name->get_RefDes() + QString::number( (iter_same_Ref_Firm_Name->get_RefDes_Count() + iter_same_Ref_Firm_Name->get_Count() - 1) );
      break;
    default:
      RefDeses += "- " + iter_same_Ref_Firm_Name->get_RefDes() + QString::number( (iter_same_Ref_Firm_Name->get_RefDes_Count() + iter_same_Ref_Firm_Name->get_Count() - 1) );
  }

  float RefDes_width_mm = Text_Width->mm( RefDeses );
  float shift_mm = ( (float) (Column_RefDes_size_mm) - RefDes_width_mm ) / 2;
  shift_mm = ( shift_mm < shift_text_mm ) ? shift_mm : shift_text_mm;
  temp << "(text (pt " << ( shift_mm + x_RefDes_mm ) << " " << l_ctrl->get_y_Current_line() << ") \"" << RefDeses << "\" (textStyleRef \"GostB-12\"))" << endl;

  QString Count = QString::number( iter_same_Ref_Firm_Name->get_Count() );
  float   Count_width_mm = Text_Width->mm( Count );
  shift_mm = ( (float) (Column_Quantity_size_mm) - Count_width_mm ) / 2;
  temp << "(text (pt " << ( shift_mm + x_Count_mm ) << " " << l_ctrl->get_y_Current_line() << ") \"" << Count << "\" (textStyleRef \"GostB-12\"))" << endl;

  if ( (Group_Title_state == GROUP_TITLE_ONLY) || (as_group_title == true) )
  {
    if ( (x_Code_mm + shift_Code_mm + Type_or_Group_Name_width_mm + Code_width_mm + shift_text_mm) < x_Firm_mm )  //Название вписывается в длину
    {
      if ( (x_Firm_mm + Firm_width_mm + shift_text_mm) < x_Count_mm ) //Фирма писывается в длину
      {
        temp << "(text (pt " << x_Code_mm + shift_Code_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Type_or_Group_Name << Code << "\" (textStyleRef \"GostB-12\"))" << endl;
        temp << "(text (pt " << x_Firm_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Firm << "\" (textStyleRef \"GostB-12\"))" << endl;
        if ( Decoding_width_mm != 0 )
        {
          l_ctrl->dec_y_Current_line();
          temp << "(text (pt " << x_Decoding_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Decoding << "\" (textStyleRef \"GostB-12\"))" << endl;
        }
      }
      else  //Фирма не вписывается в длину.
      {
        if ( (x_Code_mm + shift_Code_mm + Type_or_Group_Name_width_mm + Code_width_mm + shift_text_mm + Firm_width_mm + shift_text_mm) < x_Count_mm ) //Так красивее будет смотреться
        {
          temp << "(text (pt " << x_Code_mm + shift_Code_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Type_or_Group_Name << Code << "\" (textStyleRef \"GostB-12\"))" << endl;
          temp << "(text (pt " << x_Count_mm - shift_text_mm - Firm_width_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Firm << "\" (textStyleRef \"GostB-12\"))" << endl;
          if ( Decoding_width_mm != 0 )
          {
            l_ctrl->dec_y_Current_line();
            temp << "(text (pt " << x_Decoding_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Decoding << "\" (textStyleRef \"GostB-12\"))" << endl;
          }
        }
        else  //Фирма не вписывается в длину. Надо на другую строчку перенести...
        {
          temp << "(text (pt " << x_Code_mm + shift_Code_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Type_or_Group_Name << Code << "\" (textStyleRef \"GostB-12\"))" << endl;
          l_ctrl->dec_y_Current_line();
          if ( Decoding_width_mm != 0 )
          {
            temp << "(text (pt " << x_Count_mm - shift_text_mm - Firm_width_mm - shift_text_mm - Decoding_width_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Decoding << "\" (textStyleRef \"GostB-12\"))" << endl;
          }

          temp << "(text (pt " << x_Count_mm - shift_text_mm - Firm_width_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Firm << "\" (textStyleRef \"GostB-12\"))" << endl;
        }
      }
    }
    else
    {
      if ( (x_Code_mm + shift_text_mm + Type_or_Group_Name_width_mm + Code_width_mm + shift_text_mm) < x_Firm_mm )  //Подвинем название вправо и проверим
      {
        if ( (x_Firm_mm + Firm_width_mm + shift_text_mm) < x_Count_mm ) //Фирма вписывается в длину
        {
          temp << "(text (pt " << x_Code_mm + shift_text_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Type_or_Group_Name << Code << "\" (textStyleRef \"GostB-12\"))" << endl;
          temp << "(text (pt " << x_Firm_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Firm << "\" (textStyleRef \"GostB-12\"))" << endl;
          if ( Decoding_width_mm != 0 )
          {
            l_ctrl->dec_y_Current_line();
            temp << "(text (pt " << x_Decoding_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Decoding << "\" (textStyleRef \"GostB-12\"))" << endl;
          }
        }
        else  //Фирма не вписывается в отведённую длину. Нужно её а другую строчку перенести.
        {
          if ( (x_Code_mm + shift_Code_mm + Type_or_Group_Name_width_mm + Code_width_mm + shift_text_mm + Firm_width_mm + shift_text_mm) < x_Count_mm ) //Так красивее будет смотреться
          {
            temp << "(text (pt " << x_Code_mm + shift_Code_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Type_or_Group_Name << Code << "\" (textStyleRef \"GostB-12\"))" << endl;
            temp << "(text (pt " << x_Count_mm - shift_text_mm - Firm_width_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Firm << "\" (textStyleRef \"GostB-12\"))" << endl;
            if ( Decoding_width_mm != 0 )
            {
              l_ctrl->dec_y_Current_line();
              temp << "(text (pt " << x_Decoding_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Decoding << "\" (textStyleRef \"GostB-12\"))" << endl;
            }
          }
          else
          {
            if ( (x_Code_mm + shift_Code_mm + Type_or_Group_Name_width_mm + Code_width_mm + shift_text_mm) < x_Count_mm ) //Так красивее будет смотреться
            {
              temp << "(text (pt " << x_Code_mm + shift_Code_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Type_or_Group_Name << Code << "\" (textStyleRef \"GostB-12\"))" << endl;
              l_ctrl->dec_y_Current_line();
              if ( Decoding_width_mm != 0 )
              {
                temp << "(text (pt " << x_Count_mm - shift_text_mm - Firm_width_mm - shift_text_mm - Decoding_width_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Decoding << "\" (textStyleRef \"GostB-12\"))" << endl;
              }

              temp << "(text (pt " << x_Count_mm - shift_text_mm - Firm_width_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Firm << "\" (textStyleRef \"GostB-12\"))" << endl;
            }
            else
            {
              temp << "(text (pt " << x_Code_mm + shift_text_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Type_or_Group_Name << Code << "\" (textStyleRef \"GostB-12\"))" << endl;
              l_ctrl->dec_y_Current_line();
              if ( Decoding_width_mm != 0 )
              {
                temp << "(text (pt " << x_Count_mm - shift_text_mm - Firm_width_mm - shift_text_mm - Decoding_width_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Decoding << "\" (textStyleRef \"GostB-12\"))" << endl;
              }

              temp << "(text (pt " << x_Count_mm - shift_text_mm - Firm_width_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Firm << "\" (textStyleRef \"GostB-12\"))" << endl;
            }
          }
        }
      }
      else  //Название заползает на поле для фирмы
      {
        if ( (x_Code_mm + shift_Code_mm + Type_or_Group_Name_width_mm + Code_width_mm + shift_text_mm + Firm_width_mm + shift_text_mm) < x_Count_mm )   //Можно сё-таки вписать название и фирму в строчку...
        {
          temp << "(text (pt " << x_Code_mm + shift_Code_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Type_or_Group_Name << Code << "\" (textStyleRef \"GostB-12\"))" << endl;
          temp << "(text (pt " << x_Count_mm - shift_text_mm - Firm_width_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Firm << "\" (textStyleRef \"GostB-12\"))" << endl;
          if ( Decoding_width_mm != 0 )
          {
            l_ctrl->dec_y_Current_line();
            temp << "(text (pt " << x_Decoding_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Decoding << "\" (textStyleRef \"GostB-12\"))" << endl;
          }
        }
        else
        {
          if ( (x_Code_mm + shift_text_mm + Type_or_Group_Name_width_mm + Code_width_mm + shift_text_mm + Firm_width_mm + shift_text_mm) < x_Count_mm ) //Можно сё-таки вписать название и фирму в строчку...
          {
            temp << "(text (pt " << x_Code_mm + shift_text_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Type_or_Group_Name << Code << "\" (textStyleRef \"GostB-12\"))" << endl;
            temp << "(text (pt " << x_Count_mm - shift_text_mm - Firm_width_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Firm << "\" (textStyleRef \"GostB-12\"))" << endl;
            if ( Decoding_width_mm != 0 )
            {
              l_ctrl->dec_y_Current_line();
              temp << "(text (pt " << x_Decoding_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Decoding << "\" (textStyleRef \"GostB-12\"))" << endl;
            }
          }
          else
          {
            temp << "(text (pt " << x_Code_mm + shift_text_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Type_or_Group_Name << Code << "\" (textStyleRef \"GostB-12\"))" << endl;
            l_ctrl->dec_y_Current_line();
            if ( Decoding_width_mm != 0 )
            {
              temp << "(text (pt " << x_Count_mm - shift_text_mm - Firm_width_mm - shift_text_mm - Decoding_width_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Decoding << "\" (textStyleRef \"GostB-12\"))" << endl;
            }

            temp << "(text (pt " << x_Count_mm - shift_text_mm - Firm_width_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Firm << "\" (textStyleRef \"GostB-12\"))" << endl;
          }
        }
      }
    }
  }

  if ( Group_Title_state == GROUP_TITLE_AND_FIRM )  //Название фирмы не печатаем в строчке
  {
    if ( 0 /*(x_Code_mm + shift_Code_mm + Type_or_Group_Name_width_mm + Code_width_mm + shift_text_mm) < x_Decoding_after_code_mm*/ ) //Название вписывается в длину
    {
      if ( 0/*(x_Decoding_after_code_mm + Decoding_width_mm + shift_text_mm) < x_Count_mm*/ )  //Дешифровка вписывается в длину
      {
        temp << "(text (pt " << x_Code_mm + shift_Code_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Type_or_Group_Name << Code << "\" (textStyleRef \"GostB-12\"))" << endl;
        if ( Decoding_width_mm != 0 )
        {
          temp << "(text (pt " << x_Decoding_after_code_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Decoding << "\" (textStyleRef \"GostB-12\"))" << endl;
        }
      }
      else
      {
        temp << "(text (pt " << x_Code_mm + shift_Code_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Type_or_Group_Name << Code << "\" (textStyleRef \"GostB-12\"))" << endl;
        if ( Decoding_width_mm != 0 )
        {
          temp << "(text (pt " << x_Count_mm - shift_text_mm - Decoding_width_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Decoding << "\" (textStyleRef \"GostB-12\"))" << endl;
        }
      }
    }
    else
    {
      if ( 0/*(x_Code_mm + shift_text_mm + Type_or_Group_Name_width_mm + Code_width_mm + shift_text_mm + Decoding_width_mm + shift_text_mm) < x_Code_mm*/ )  //Подвинем название вправо и проверим
      {
        temp << "(text (pt " << x_Code_mm + shift_Code_mm + shift_text_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Type_or_Group_Name << Code << "\" (textStyleRef \"GostB-12\"))" << endl;
        if ( Decoding_width_mm != 0 )
        {
          temp << "(text (pt " << x_Count_mm - shift_text_mm - Decoding_width_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Decoding << "\" (textStyleRef \"GostB-12\"))" << endl;
        }
      }
      else
      {
        if ( (x_Code_mm + shift_Code_mm + Type_or_Group_Name_width_mm + Code_width_mm + shift_text_mm) < x_Code_mm )
        {
          temp << "(text (pt " << x_Code_mm + shift_Code_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Type_or_Group_Name << Code << "\" (textStyleRef \"GostB-12\"))" << endl;
          if ( Decoding_width_mm != 0 )
          {
            l_ctrl->dec_y_Current_line();
            temp << "(text (pt " << x_Decoding_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Decoding << "\" (textStyleRef \"GostB-12\"))" << endl;
          }
        }
        else  //Название вообще длинное...
        {
          temp << "(text (pt " << x_Code_mm + shift_Code_mm + shift_text_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Type_or_Group_Name << Code << "\" (textStyleRef \"GostB-12\"))" << endl;
          if ( Decoding_width_mm != 0 )
          {
            l_ctrl->dec_y_Current_line();
            temp << "(text (pt " << x_Decoding_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Decoding << "\" (textStyleRef \"GostB-12\"))" << endl;
          }
        }
      }
    }
  }

  l_ctrl->dec_y_Current_line();
}

void per::try_create_page ( int req_lines )
{
  QTextStream temp ( &Page_content );

  bool        create_page = false;

  if ( ((int) (l_ctrl->get_free_lines()) - req_lines) <= 0 )
  {
//    *plog << LOG << "if( ( (int)(get_free_lines()) - req_lines ) < 0 ) =" << (int) ( l_ctrl->get_free_lines() ) - req_lines << endl;
    create_page = true;
  }

  if ( create_page == true )
  {
    document->push_back( Page_content );
    l_ctrl->increment_page_num();
    Page_content = "";
    l_ctrl->clear_y_Line_counter();

    QString Page_num = QString::number(l_ctrl->get_Page_num());
    float   Page_num_width_mm = Text_Width->mm( Page_num );
    float x_Page_num_center_mm = x_PageNumber_mm + ( (x_PageNumber_end_mm - x_PageNumber_mm) / 2 ) - Page_num_width_mm / 2;

    temp << "(text (pt " << x_Page_num_center_mm << " " << y_PageNumber_mm << ") \"" << Page_num << "\" (textStyleRef \"GostB-12\"))" << endl;


    QString Detimal_Number = perechen.begin()->begin()->begin()->get_Detimal_Number() + codec->toUnicode("ПЭ3");
    float   Detimal_Number_width_mm = Text_Width->mm( Detimal_Number );
    float   x_Detimal_Number_center_mm = x_DetimalNumber_mm + ( (x_PageNumber_mm - x_DetimalNumber_mm) / 2 ) - Detimal_Number_width_mm / 2;

    temp << "(text (pt " << x_Detimal_Number_center_mm << " " << y_DetimalNumber_mm << ") \"" << Detimal_Number << "\" (textStyleRef \"GostB-16\"))" << endl;
  }
}

void per::create_first_stamp ( void )
{

}

void per::by_refdes_and_value_code ( void )
{
  QVector<QVector<element> >  tables; //Для хранения таблиц с одинаковыми элементами
  QVector<QVector<element> >  groups; //Для хранения групп однотипного элемента c одинаковой фирмой
  QVector<element>          temp_table;
  QVector<element>::iterator  ele;

  tables.clear();
  std::sort( el->begin(), el->end(), element::by_RefDes_RefDes_count ); //Упорядочили по Ref  и по Des

  /*
    *plog << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
        *plog << "           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
        for ( QVector<element>::iterator iter3 =  el->begin(); iter3 != el->end(); iter3++ )
        {
          *plog << "               " << ( *iter3 ).get_full_record() << endl;
        }
    *plog << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl << endl;
*/
  for ( QVector<element>::iterator iter3 = el->begin(); iter3 < el->end(); ++iter3 )
  {
    if ( iter3->get_RefDes() == codec->toUnicode("COMPO") )
    {
      iter3 = el->erase( iter3 );
      iter3--;
    }
    
    if ( iter3->get_RefDes() == codec->toUnicode("ZCOMPO") )
    {
      iter3 = el->erase( iter3 );
      iter3--;
    }
  }

  while ( el->empty() == false )
  {
    ele = el->begin();
    temp_table.push_back( *ele );     //Помещаем оригинальный элемент в таблицу
    ele = el->erase( ele );           //Удаляем запись из таблицы чтоб не мешалась
    QVector<element>::iterator  temp = temp_table.begin();

    for ( ; ele < el->end(); ele++ ) //Перекидываем в таблицу temp_table все одинаковые элементы по порядку, до первого неодинакового
    {
      if ( *temp == *ele )
      {
        temp_table.push_back( *ele );
        ele = el->erase( ele );
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

  /*
    *plog << "**************************************************************************************" << endl;
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
*/
  perechen.clear();

  groups.push_back( *(tables.begin()) );
  tables.erase( tables.begin() );

  while ( tables.empty() == false )
  {
    if ( (tables.begin()->begin()->get_Value_Firm() == groups.begin()->begin()->get_Value_Firm()) && (tables.begin()->begin()->get_RefDes() == groups.begin()->begin()->get_RefDes()) )
    {
      groups.push_back( *(tables.begin()) );
      tables.erase( tables.begin() );
    }
    else
    {
      perechen.push_back( groups );
      groups.clear();
      groups.push_back( *(tables.begin()) );
      tables.erase( tables.begin() );
    }
  }

  if ( groups.empty() == false )
  {
    perechen.push_back( groups );
  }

  /*
       *plog << "**************************************************************************************" << endl;
       for ( QVector < QVector < QVector<element> > >::iterator iter1 = perechen.begin(); iter1 != perechen.end(); iter1++ )
       {
         *plog << "     ================================================================================" << endl;
         for ( QVector < QVector<element> >::iterator iter2 = (*iter1).begin(); iter2 != (*iter1).end(); iter2++ )
         {
           *plog << "           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
           for ( QVector<element>::iterator iter3 = (*iter2).begin(); iter3 != (*iter2).end(); iter3++ )
           {
             *plog << "               " << ( *iter3 ).get_full_record() << endl;
           }
         }
       }
       *plog << "***************************************************************************************" << endl << endl;
*/
  for ( QVector < QVector < QVector<element> > >::iterator iter1 = perechen.begin(); iter1 < perechen.end(); iter1++ ) //посчитаем все однотипные элементы, стоящие попорядку.
  {
    for ( QVector < QVector<element> >::iterator iter2 = iter1->begin(); iter2 < iter1->end(); iter2++ )
    {
      unsigned int  count = 0;
      for ( QVector<element>::iterator iter3 = iter2->begin(); iter3 < iter2->end(); iter3++ )
      {
        count += iter3->get_Count();
        iter3->set_Count( 0 );
      }
      iter2->begin()->set_Count( count );
    }
  }

  /*
       *plog << LOG  << "***************************************************************************************************" << endl;
      QVector < QVector < QVector<element> > >::iterator iter1_before = NULL;
      for ( QVector < QVector < QVector<element> > >::iterator iter1 = perechen.begin(); iter1 != perechen.end(); iter1++ )
       {
         if( (iter1_before == NULL ) || ( (*(*( *iter1_before ).begin()).begin()).get_RefDes() != (*(*( *iter1 ).begin()).begin()).get_RefDes() ) ) //Меняется Ref группы
           {
             QString Ref = (*(*( *iter1 ).begin()).begin()).get_RefDes();
             if( (Ref == "A") || (Ref == "B") )
               {

               }
              else
               *plog << LOG << "### " << (*(*( *iter1 ).begin()).begin()).get_Group_Name() << " ###" << endl;
           }
         *plog << LOG  << "   $$$$$$$$$$$ " << codec->toUnicode("Элементы, принадлежащие к одной фирме") << " $$$$$$$$$$$" << endl;
         if( (*iter1).size() > 1  )
         {
       //    *plog << LOG << "    ======= "<< (*(*( *iter1 ).begin()).begin()).get_Group_Name() << "\t" << (*(*( *iter1 ).begin()).begin()).get_Types_from_Value() << " " << (*(*( *iter1 ).begin()).begin()).get_Value_Firm() << endl;
         }
         else
         {
       //      *plog << LOG  << "    ======= "<<  (*(*( *iter1 ).begin()).begin()).get_Group_Name() << "\t" << (*(*( *iter1 ).begin()).begin()).get_Type_from_Value() << " " << (*(*( *iter1 ).begin()).begin()).get_Value_Firm() << endl;
         }

         for ( QVector < QVector<element> >::iterator iter2 = (*iter1).begin(); iter2 != (*iter1).end(); iter2++ )
         {
           *plog << LOG  << codec->toUnicode("          %%%%%%%%%%%%%%%% Однотипные подряд идущие элементы %%%%%%%%%%%%%%%%%")  << endl;
           for ( QVector<element>::iterator iter3 = (*iter2).begin(); iter3 != (*iter2).end(); iter3++ )
           {
             *plog << LOG  << "               " << ( *iter3 ).get_full_record() << endl;
           }
         }

         iter1_before = iter1;
       }
       *plog << "***************************************************************************************" << endl << endl;
*/
}

