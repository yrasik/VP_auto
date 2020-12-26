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
#include "sp.h"

//#define LOG ""
sp::sp ( QString &PathToEtcDir, QString &PathToOutDir, QVector<element> *el_ )
{
  this->PathToEtcDir = PathToEtcDir;
  this->PathToOutDir = PathToOutDir;

  page_setup();


  processing = true;
  el = new QVector<element>;
  document = new QVector<QString>;

  for ( QVector<element>::iterator i = el_->begin(); i != el_->end(); i++ )
  {
    el->push_back( *i );
  }

  FullFileName = this->PathToOutDir + "/" + ( *((*el).begin()) ).get_Detimal_Number() + codec->toUnicode( ".sch" );

  FullCanvasFileName = QCoreApplication::applicationDirPath() + "/../canvas/sp_canvas.sch";
}

sp::~sp ()
{
  delete el;
  delete l_ctrl;
  delete document;
}

void sp::page_setup ( void )
{
  QString filename( PathToEtcDir + codec->toUnicode("/sp_settings.lua"));
  int       err;

  x_Format_mm = 20.5;
  x_Pozition_mm = 32.5;
  x_Oboznachen_mm = 40.5;
  x_Code_mm = 110.5;
  x_Count_mm = 173.5;
  x_Addon_mm = 183.5;
  x_Addon_end_mm = 205.5;
  shift_text_mm = 2;
  x_PageNumber_mm = 195.5;
  x_PageNumber_end_mm = 205.5;
  y_PageNumber_mm = 7;
  x_DetimalNumber_mm = 85.5;
  y_DetimalNumber_mm = 10;
  y_DetimalNumber_first_mm = 35;

  Poz_Oboznachenie = 2;

  y_First_line = 270;
  lines_in_first_page = 24;
  lines_in_others_pages = 30;


  put_group_title_and_firm = false;
  old_element_in_groupe = false;

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

  lua_getglobal(L, "x_Format_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    x_Format_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "x_Pozition_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    x_Pozition_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "x_Oboznachen_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    x_Oboznachen_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "x_Code_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    x_Code_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "x_Count_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    x_Count_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "x_Addon_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    x_Addon_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "x_Addon_end_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    x_Addon_end_mm = (float)(lua_tonumberx) (L, -1, NULL);

  lua_getglobal(L, "shift_text_mm");
  if( lua_type(L, -1) == LUA_TNUMBER )
    shift_text_mm = (float)(lua_tonumberx) (L, -1, NULL);

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

  lua_getglobal(L, "Poz_Oboznachenie");
  if( lua_type(L, -1) == LUA_TNUMBER )
    Poz_Oboznachenie = (float)(lua_tonumberx) (L, -1, NULL);


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

bool sp::open_files ( void )
{
  canvas_file.open( FullCanvasFileName.toLatin1(), std::ios::in );
  if ( !canvas_file )
  {
    *plog << codec->toUnicode( "ERROR: Не могу открыть \"sp_canvas.sch\" файл" ) << endl;
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

void sp::close_files ( void )
{
  target_file.close();
  *plog << codec->toUnicode( "Закрываем \"" ) << FullFileName << codec->toUnicode( "\" файл" ) << endl;
  canvas_file.close();
  *plog << codec->toUnicode( "Закрываем \"" ) << FullCanvasFileName << codec->toUnicode( "\" файл" ) << endl;
}


void sp::create_first_page ( void )
{
  QTextStream temp ( &Page_content );

  QString Detimal_Number = spec.begin()->begin()->begin()->begin()->get_Detimal_Number();
  float   Detimal_Number_width_mm = Text_Width->mm( Detimal_Number );
  float   x_Detimal_Number_center_mm = x_DetimalNumber_mm + ( (x_PageNumber_mm - x_DetimalNumber_mm) / 2 ) - Detimal_Number_width_mm / 2;

  temp << "(text (pt " << x_Detimal_Number_center_mm << " " << y_DetimalNumber_first_mm << ") \"" << Detimal_Number << "\" (textStyleRef \"GostB-16\"))" << endl;

  try_create_page( l_ctrl->get_lines_in_others_pages() );
  l_ctrl->dec_y_Current_line();
  l_ctrl->dec_y_Current_line();

  for(int i = 0; i < 12 ; i++)
  {
    l_ctrl->dec_y_Current_line();
  }
}


/*
void sp::create_first_stamp ( void )
{

}
*/

void sp::generate ( void )
{
  *plog << endl;
  *plog << codec->toUnicode( "#########################################################################" ) << endl;
  *plog << codec->toUnicode( "###                   Генерация спецификации                          ###" ) << endl;
  *plog << codec->toUnicode( "### для сборки: '" ) <<  ( *((*el).begin()) ).get_Detimal_Number() << "' ###" << endl;
  *plog << codec->toUnicode( "#########################################################################" ) << endl;
  *plog << endl;
  *plog << "START {" << endl;

  sorting();


  std::string canvas_string;

  create_first_page();

  for ( iter_same_Ref_Firm = spec.begin(); iter_same_Ref_Firm < spec.end(); iter_same_Ref_Firm++ )
  {
    put_group_title();
    for ( iter_same_Ref_Firm_Code = iter_same_Ref_Firm->begin(); iter_same_Ref_Firm_Code < iter_same_Ref_Firm->end(); iter_same_Ref_Firm_Code++ )
    {
      put_record();
    }
  }

  try_create_page( l_ctrl->get_lines_in_others_pages() );

  //Тут уже сформирована спецификация
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

bool sp::try_create_page ( int req_lines )
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
    l_ctrl->dec_y_Current_line();

    QString Page_num = QString::number(l_ctrl->get_Page_num());
    float   Page_num_width_mm = Text_Width->mm( Page_num );
    float   x_Page_num_center_mm = x_PageNumber_mm + ( (x_PageNumber_end_mm - x_PageNumber_mm) / 2 ) - Page_num_width_mm / 2;

    temp << "(text (pt " << x_Page_num_center_mm << " " << y_PageNumber_mm << ") \"" << Page_num << "\" (textStyleRef \"GostB-12\"))" << endl;


    QString Detimal_Number = spec.begin()->begin()->begin()->begin()->get_Detimal_Number();
    float   Detimal_Number_width_mm = Text_Width->mm( Detimal_Number );
    float   x_Detimal_Number_center_mm = x_DetimalNumber_mm + ( (x_PageNumber_mm - x_DetimalNumber_mm) / 2 ) - Detimal_Number_width_mm / 2;

    temp << "(text (pt " << x_Detimal_Number_center_mm << " " << y_DetimalNumber_mm << ") \"" << Detimal_Number << "\" (textStyleRef \"GostB-16\"))" << endl;
  }

  return create_page;
}

void sp::put_group_title ( void )
{
  QTextStream temp ( &Page_content );

  int         record_Ref_size = iter_same_Ref_Firm->size();


  if ( record_Ref_size == 1 )
  {
    try_create_page( 3 );

    QString Group = iter_same_Ref_Firm->begin()->begin()->begin()->get_Type_from_Value();
    float   Group_width_mm = Text_Width->mm( Group );

    temp << "(text (pt " << ( x_Code_mm + shift_text_mm ) << " " << l_ctrl->get_y_Current_line() << ") \"" << Group << "\" (textStyleRef \"GostB-12\"))" << endl;
    l_ctrl->dec_y_Current_line();
  }
  else
  {
    QString Group = iter_same_Ref_Firm->begin()->begin()->begin()->get_Group_Name();
    float   Group_width_mm = Text_Width->mm( Group );

    QString Firm = iter_same_Ref_Firm->begin()->begin()->begin()->get_Value_Firm();
    float   Firm_width_mm = Text_Width->mm( Firm );

    QString Group_Firm = Group + "  " + Firm;
    float   Group_Firm_width_mm = Text_Width->mm( Group_Firm );

    if( Group_Firm_width_mm < (x_Count_mm - x_Code_mm) )
    {
      try_create_page( 4 );
      temp << "(text (pt " << ( x_Code_mm + shift_text_mm ) << " " << l_ctrl->get_y_Current_line() << ") \"" << Group_Firm << "\" (textStyleRef \"GostB-12\"))" << endl;
    }
    else
    {
      try_create_page( 5 );
      temp << "(text (pt " << ( x_Code_mm + shift_text_mm ) << " " << l_ctrl->get_y_Current_line() << ") \"" << Group << "\" (textStyleRef \"GostB-12\"))" << endl;
      l_ctrl->dec_y_Current_line();
      temp << "(text (pt " << ( x_Count_mm - shift_text_mm - Firm_width_mm) << " " << l_ctrl->get_y_Current_line() << ") \"" << Firm << "\" (textStyleRef \"GostB-12\"))" << endl;
    }

    l_ctrl->dec_y_Current_line();
    l_ctrl->dec_y_Current_line();
  }
}

void sp::put_record ( void )
{
  QString     tmp_str;
  QTextStream temp ( &tmp_str );
  bool        put_Code = true;
  bool        first_Code = true;

  //  try_put_record( tmp_str );
  QString     Code = iter_same_Ref_Firm_Code->begin()->begin()->get_Code_from_Value();
  float       Code_width_mm = Text_Width->mm( Code );

  QString     Decoding = iter_same_Ref_Firm_Code->begin()->begin()->get_Value_Decoding();
  float       Decoding_width_mm = Text_Width->mm( Decoding );

  QString     Firm = iter_same_Ref_Firm_Code->begin()->begin()->get_Value_Firm();
  float       Firm_width_mm = Text_Width->mm( Firm );

  iter_same_Ref_Firm_Code_Addon = iter_same_Ref_Firm_Code->begin();

  int Count_int = 0;


  for ( QVector < QVector<element> >::iterator iter2 = iter_same_Ref_Firm_Code->begin(); iter2 < iter_same_Ref_Firm_Code->end(); iter2++ )
  {
    for ( QVector<element>::iterator iter3 = iter2->begin(); iter3 < iter2->end(); iter3++ )
    {
      Count_int += iter3->get_Count();
    }
  }

  QString Count = QString::number( Count_int );

  float Count_width_mm = Text_Width->mm( Count );
  float x_Count_center_mm = x_Count_mm + ( (x_Addon_mm - x_Count_mm) / 2 ) - Count_width_mm / 2;



  QString Poz_Oboznachen = QString::number( Poz_Oboznachenie );

  float Poz_Oboznachen_width_mm = Text_Width->mm( Poz_Oboznachen );
  float x_Pozition_center_mm = x_Pozition_mm + ( (x_Oboznachen_mm - x_Pozition_mm) / 2 ) - Poz_Oboznachen_width_mm / 2;




  int count_line = 0;

  while ( iter_same_Ref_Firm_Code_Addon < iter_same_Ref_Firm_Code->end() )
  {
    if ( iter_same_Ref_Firm->size() == 1 )  //Алгоритм переносов записи на другую страницу...
    {
      if ( try_create_page(1) )
      {
        put_group_title();
        put_Code = true;
      }

      if ( put_Code == true )
      {

        if( first_Code == true)
        {
          temp << "(text (pt " << x_Count_center_mm << " " << l_ctrl->get_y_Current_line() + l_ctrl->get_y_step() << ") \"" << Count << "\" (textStyleRef \"GostB-12\"))" << endl;
          temp << "(text (pt " << x_Pozition_center_mm << " " << l_ctrl->get_y_Current_line() + l_ctrl->get_y_step() << ") \"" << Poz_Oboznachen << "\" (textStyleRef \"GostB-12\"))" << endl;

          Poz_Oboznachenie++;
          first_Code = false;
        }

        temp << "(text (pt " << ( x_Code_mm + shift_text_mm ) << " " << l_ctrl->get_y_Current_line() << ") \"" << Code << "\" (textStyleRef \"GostB-12\"))" << endl;

        if ( Decoding_width_mm != 0 )
        {
          temp << "(text (pt " << ( x_Code_mm + shift_text_mm + shift_text_mm ) << " " << l_ctrl->get_y_Current_line() - l_ctrl->get_y_step() << ") \"" << Decoding << "\" (textStyleRef \"GostB-12\"))" << endl;
          temp << "(text (pt " << ( x_Code_mm + shift_text_mm + shift_text_mm ) << " " << l_ctrl->get_y_Current_line() - 2 * l_ctrl->get_y_step() << ") \"" << Firm << "\" (textStyleRef \"GostB-12\"))" << endl;
        }
        else
        {
          temp << "(text (pt " << ( x_Code_mm + shift_text_mm + shift_text_mm ) << " " << l_ctrl->get_y_Current_line() - l_ctrl->get_y_step() << ") \"" << Firm << "\" (textStyleRef \"GostB-12\"))" << endl;
        }

        l_ctrl->inc_y_Current_line();
        put_Code = false;
      }

      QString RefDes_RefDes = iter_same_Ref_Firm_Code_Addon->begin()->get_RefDes_and_Count();

      QVector<element>::iterator  iter_same_Ref_Firm_Code_Addon_End;
      iter_same_Ref_Firm_Code_Addon_End = ( iter_same_Ref_Firm_Code_Addon->end() ) - 1;

      switch ( iter_same_Ref_Firm_Code_Addon->size() )
      {
        case 0: //ERROR
          break;
        case 1:
          if ( (iter_same_Ref_Firm_Code_Addon + 1) < iter_same_Ref_Firm_Code->end() )
          {
            if ( (iter_same_Ref_Firm_Code_Addon + 1)->size() == 1 )
            {
              iter_same_Ref_Firm_Code_Addon++;
              RefDes_RefDes += ", " + iter_same_Ref_Firm_Code_Addon->begin()->get_RefDes_and_Count();
            }
          }
          break;
        case 2:
          RefDes_RefDes += ", " + iter_same_Ref_Firm_Code_Addon_End->get_RefDes_and_Count();
          break;
        default:
          RefDes_RefDes += "..." + iter_same_Ref_Firm_Code_Addon_End->get_RefDes_and_Count();
      }

      if( iter_same_Ref_Firm_Code_Addon < (iter_same_Ref_Firm_Code->end() - 1) )
      {
        RefDes_RefDes += ",";
      }


      float RefDes_RefDes_width_mm = Text_Width->mm( RefDes_RefDes );
      float x_Addon_center_mm = x_Addon_mm + ( (x_Addon_end_mm - x_Addon_mm) / 2 ) - RefDes_RefDes_width_mm / 2;
      temp << "(text (pt " << x_Addon_center_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << RefDes_RefDes << "\" (textStyleRef \"GostB-12\"))" << endl;
      count_line++;
      l_ctrl->dec_y_Current_line();
    }
    else
    {
      if ( Decoding_width_mm != 0 )
      {
        switch ( iter_same_Ref_Firm_Code->size() )
        {
          case 1:
            if ( try_create_page(2) )
            {
              put_group_title();
            }
            break;
          case 2:
            if ( iter_same_Ref_Firm_Code->begin()->size() == 1 )
            {
              if ( (iter_same_Ref_Firm_Code->begin() + 1)->size() == 1 )
              {
                if ( try_create_page(2) )
                {
                  put_group_title();
                }
              }
            }
            break;
          default:
            if ( put_Code == true )
            {
              if ( try_create_page(2) )
              {
                put_group_title();
              }
            }
            else
            {
              if ( try_create_page(1) )
              {
                put_group_title();
                put_Code = true;
              }
            }
        }
      }

      if ( try_create_page(1) )
      {
        put_group_title();
        put_Code = true;
      }

      if ( put_Code == true )
      {
        if( first_Code == true)
        {
          temp << "(text (pt " << x_Count_center_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Count << "\" (textStyleRef \"GostB-12\"))" << endl;
          temp << "(text (pt " << x_Pozition_center_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << Poz_Oboznachen << "\" (textStyleRef \"GostB-12\"))" << endl;

          Poz_Oboznachenie++;
          first_Code = false;
        }

        temp << "(text (pt " << ( x_Code_mm + shift_text_mm ) << " " << l_ctrl->get_y_Current_line() << ") \"" << Code << "\" (textStyleRef \"GostB-12\"))" << endl;

        if ( Decoding_width_mm != 0 )
        {
          temp << "(text (pt " << ( x_Code_mm + shift_text_mm + shift_text_mm ) << " " << l_ctrl->get_y_Current_line() - l_ctrl->get_y_step() << ") \"" << Decoding << "\" (textStyleRef \"GostB-12\"))" << endl;
        }

        put_Code = false;
      }

      QString RefDes_RefDes = iter_same_Ref_Firm_Code_Addon->begin()->get_RefDes_and_Count();

      QVector<element>::iterator  iter_same_Ref_Firm_Code_Addon_End;
      iter_same_Ref_Firm_Code_Addon_End = ( iter_same_Ref_Firm_Code_Addon->end() ) - 1;

      switch ( iter_same_Ref_Firm_Code_Addon->size() )
      {
        case 0: //ERROR
          break;
        case 1:
          if ( (iter_same_Ref_Firm_Code_Addon + 1) < iter_same_Ref_Firm_Code->end() )
          {
            if ( (iter_same_Ref_Firm_Code_Addon + 1)->size() == 1 )
            {
              iter_same_Ref_Firm_Code_Addon++;
              RefDes_RefDes += ", " + iter_same_Ref_Firm_Code_Addon->begin()->get_RefDes_and_Count();
            }
          }
          break;
        case 2:
          RefDes_RefDes += ", " + iter_same_Ref_Firm_Code_Addon_End->get_RefDes_and_Count();
          break;
        default:
          RefDes_RefDes += "..." + iter_same_Ref_Firm_Code_Addon_End->get_RefDes_and_Count();
      }


      if( iter_same_Ref_Firm_Code_Addon < (iter_same_Ref_Firm_Code->end() - 1) )
      {
        RefDes_RefDes += ",";
      }


      float RefDes_RefDes_width_mm = Text_Width->mm( RefDes_RefDes );
      float x_Addon_center_mm = x_Addon_mm + ( (x_Addon_end_mm - x_Addon_mm) / 2 ) - RefDes_RefDes_width_mm / 2;
      temp << "(text (pt " << x_Addon_center_mm << " " << l_ctrl->get_y_Current_line() << ") \"" << RefDes_RefDes << "\" (textStyleRef \"GostB-12\"))" << endl;
      l_ctrl->dec_y_Current_line();

      if ( Decoding_width_mm != 0 )
      {
        switch ( iter_same_Ref_Firm_Code->size() )
        {
          case 1:
            l_ctrl->dec_y_Current_line();
            break;
          case 2:
            if ( iter_same_Ref_Firm_Code->begin()->size() == 1 )
            {
              if ( (iter_same_Ref_Firm_Code->begin() + 1)->size() == 1 )
              {
                l_ctrl->dec_y_Current_line();
              }
            }
            break;
        }
      }
    }

    Page_content += tmp_str;
    tmp_str.clear();

    iter_same_Ref_Firm_Code_Addon++;
  }

  l_ctrl->dec_y_Current_line();

  if ( iter_same_Ref_Firm->size() == 1 )
  {
    if ( Decoding_width_mm != 0 )
    {
      switch ( count_line )
      {
        case 1:
          l_ctrl->dec_y_Current_line();
          l_ctrl->dec_y_Current_line();
          l_ctrl->dec_y_Current_line();
          break;
        case 2:
          l_ctrl->dec_y_Current_line();
          l_ctrl->dec_y_Current_line();
          break;
        case 3:
          l_ctrl->dec_y_Current_line();
          break;
      }
    }
    else
    {
      switch ( count_line )
      {
        case 1:
          l_ctrl->dec_y_Current_line();
          l_ctrl->dec_y_Current_line();
          break;
        case 2:
          l_ctrl->dec_y_Current_line();
          break;
      }
    }
  }
}

void sp::try_put_record ( QString &tmp_str )
{

}


void sp::sorting ()
{
  QVector<QVector<element> >          tables; //Для хранения таблиц с одинаковыми элементами
  QVector<element>          temp_table;
  QVector<element>::iterator          ele;
  QVector<QVector<QVector<element> > >  specification;

  tables.clear();

  while ( el->empty() == false )
  {
    ele = el->begin();
    temp_table.push_back( *ele );             //Помещаем оригинальный элемент в таблицу
    ele = el->erase( ele );                   //Удаляем запись из таблицы чтоб не мешалась
    QVector<element>::iterator  temp = temp_table.begin();
    for ( ; ele != el->end(); ele++ )         //Перекидываем в таблицу temp_table все одинаковые элементы
    {
      if ( *temp == *ele )
      {
        temp_table.push_back( *ele );
        ele = el->erase( ele );
        ele--;
      }
    }

    tables.push_back( temp_table );
    temp_table.clear();
  }

  // Сортировка по by_RefDes_Count
  for ( QVector < QVector<element> >::iterator current_table = tables.begin(); current_table != tables.end(); current_table++ )
  {
    std::sort( (*current_table).begin(), (*current_table).end(), element::by_RefDes_Count );
  }

  //Теперь список tables представляет собой совокупность таблиц с одинаковыми элементами /////
  std::sort( tables.begin(), tables.end(), by_Value_Firm_Ref );

  // Для полного счастья нужно, каждую такую таблицу представить тоже в виде таблиц, рефдесы в которых инкрементируются на единицу (для облегчения формирования спецификации)
  specification.clear();
  for ( QVector < QVector<element> >::iterator current_table = tables.begin(); current_table != tables.end(); current_table++ )
  {
    QVector<element>          temp_part_in_refdes;
    QVector<QVector<element> >  temp_refdeses_in_group;
    QVector<element>::iterator  iter_part_in_refdes = ( *current_table ).begin();

    temp_part_in_refdes.push_back( *iter_part_in_refdes );
    temp_refdeses_in_group.push_back( temp_part_in_refdes );
    specification.push_back( temp_refdeses_in_group );

    QVector<QVector<QVector<element> > >::iterator  group_specification = ( specification.end() - 1 );
    QVector<QVector<element> >::iterator          refdes_in_group_specification = ( *group_specification ).begin();

    if ( current_table->size() == 1 )         // В таблице всего один элемент. Никакой группировки не требуется
    {

    }
    else
    {
      for ( QVector<element>::iterator iter = (current_table->begin() + 1); iter < current_table->end(); iter++ )
      {
        if ( iter->get_RefDes_Count() == ((iter - 1)->get_RefDes_Count() + 1) ) //Рефдесы отличаются на единицу
        {
          refdes_in_group_specification->push_back( *iter );
        }
        else
        {
          temp_part_in_refdes.clear();
          temp_part_in_refdes.push_back( *iter );
          group_specification->push_back( temp_part_in_refdes );
          refdes_in_group_specification = ( group_specification->end() - 1 );
        }
      }
    }
  }

#if 0
  * plog << "**************************************************************************************" << endl;
  for ( QVector < QVector < QVector<element> > >::iterator iter1 = specification.begin(); iter1 < specification.end(); iter1++ )
  {
    *plog << "     ================================================================================" << endl;
    for ( QVector < QVector<element> >::iterator iter2 = iter1->begin(); iter2 < iter1->end(); iter2++ )
    {
      *plog << "           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
      for ( QVector<element>::iterator iter3 = iter2->begin(); iter3 < iter2->end(); iter3++ )
      {
        *plog << "               " << iter3->get_full_record() << endl;
      }
    }
  }

  *plog << "***************************************************************************************" << endl << endl;
#endif
  spec.clear();

  while ( specification.size() > 0 )
  {
    QVector<QVector<QVector<element> > >          same_Firm;
    QVector<QVector<QVector<element> > >::iterator  iter1 = specification.begin();
    QString Ref = iter1->begin()->begin()->get_RefDes();
    QString Firm = iter1->begin()->begin()->get_Value_Firm();

    same_Firm.push_back( *iter1 );
    specification.erase( iter1 );

    while ( iter1 < specification.end() )
    {
      if ( (Ref == iter1->begin()->begin()->get_RefDes()) && (Firm == iter1->begin()->begin()->get_Value_Firm()) )
      {
        same_Firm.push_back( *iter1 );
        specification.erase( iter1 );
      }
      else
      {
        iter1++;
      }
    }

    spec.push_back( same_Firm );
  }

#if 0
  * plog << "**************************************************************************************" << endl;
  for ( QVector < QVector < QVector < QVector<element> > > >::iterator iter0 = spec.begin(); iter0 < spec.end(); iter0++ )
  {
    *plog << "   ~~~ Same Ref Firm ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    for ( QVector < QVector < QVector<element> > >::iterator iter1 = iter0->begin(); iter1 < iter0->end(); iter1++ )
    {
      *plog << "     ==== Same Code ============================================================================" << endl;
      for ( QVector < QVector<element> >::iterator iter2 = iter1->begin(); iter2 < iter1->end(); iter2++ )
      {
        *plog << "           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
        for ( QVector<element>::iterator iter3 = iter2->begin(); iter3 < iter2->end(); iter3++ )
        {
          *plog << "               " << iter3->get_full_record() << endl;
        }
      }
    }
  }

  *plog << "***************************************************************************************" << endl << endl;
#endif

#if 1
  * plog << "**************************************************************************************" << endl;
  for ( QVector < QVector < QVector < QVector<element> > > >::iterator iter0 = spec.begin(); iter0 < spec.end(); iter0++ )
  {
    //*plog << "   ~~~ Same Ref Firm ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    *plog << "   ~~~ " << codec->toUnicode("Фирма '") << iter0->begin()->begin()->begin()->get_Value_Firm() << "' ~~~~~~~~~" << endl;
    for ( QVector < QVector < QVector<element> > >::iterator iter_same_Ref_Firm_Code = iter0->begin(); iter_same_Ref_Firm_Code < iter0->end(); iter_same_Ref_Firm_Code++ )
    {
     // *plog << "     ==== Same Code ============================================================================" << endl;

      int Count_int = 0;

      for ( QVector < QVector<element> >::iterator iter2 = iter_same_Ref_Firm_Code->begin(); iter2 < iter_same_Ref_Firm_Code->end(); iter2++ )
      {
        for ( QVector<element>::iterator iter3 = iter2->begin(); iter3 < iter2->end(); iter3++ )
        {
          Count_int += iter3->get_Count();
        }
      }

      *plog << "   " << iter_same_Ref_Firm_Code->begin()->begin()->get_record8()  << "     " << QString::number( Count_int ) << endl;
      *plog << "               ";

       for ( QVector < QVector<element> >::iterator iter_same_Ref_Firm_Code_Addon = iter_same_Ref_Firm_Code->begin(); iter_same_Ref_Firm_Code_Addon < iter_same_Ref_Firm_Code->end(); iter_same_Ref_Firm_Code_Addon++ )
       {
           QString RefDes_RefDes = iter_same_Ref_Firm_Code_Addon->begin()->get_RefDes_and_Count();

           QVector<element>::iterator  iter_same_Ref_Firm_Code_Addon_End;
           iter_same_Ref_Firm_Code_Addon_End = ( iter_same_Ref_Firm_Code_Addon->end() ) - 1;

           switch ( iter_same_Ref_Firm_Code_Addon->size() )
           {
             case 0: //ERROR
               break;
             case 1:
               if ( (iter_same_Ref_Firm_Code_Addon + 1) < iter_same_Ref_Firm_Code->end() )
               {
                 if ( (iter_same_Ref_Firm_Code_Addon + 1)->size() == 1 )
                 {
                   iter_same_Ref_Firm_Code_Addon++;
                   RefDes_RefDes += "," + iter_same_Ref_Firm_Code_Addon->begin()->get_RefDes_and_Count();
                 }
               }
               break;
             case 2:
               RefDes_RefDes += "," + iter_same_Ref_Firm_Code_Addon_End->get_RefDes_and_Count();
               break;
             default:
               RefDes_RefDes += "..." + iter_same_Ref_Firm_Code_Addon_End->get_RefDes_and_Count();
           }

           if( iter_same_Ref_Firm_Code_Addon < (iter_same_Ref_Firm_Code->end() - 1) )
           {
             RefDes_RefDes += ",";
           }

           *plog <<  /*"  " <<*/ RefDes_RefDes;
       }

       *plog << endl;
    }
  }

  *plog << "***************************************************************************************" << endl << endl;
#endif



}

