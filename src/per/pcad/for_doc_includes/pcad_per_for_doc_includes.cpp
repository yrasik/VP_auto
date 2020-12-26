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
#include "pcad_per_for_doc_includes.h"

#define LOG ""

void pcad_per_for_doc_includes::page_setup ( void )
{
  QString filename( PathToEtcDir + codec->toUnicode("/per_for_doc_settings.lua"));
  int       err;

  delete l_ctrl;

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
  lines_in_first_page = 28;
  lines_in_others_pages = 28;

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


bool pcad_per_for_doc_includes::open_files ( void )
{

 // *plog << LOG << PathToOutDir << endl;

  FullFileName = PathToOutDir  + "/" + el->begin()->get_Detimal_Number() + codec->toUnicode( "ПЭ3_И1.sch" );

 // *plog << LOG << FullFileName << endl;

  FullCanvasFileName = QCoreApplication::applicationDirPath() + "/../canvas/per_canvas_i1.sch";

  canvas_file.open( FullCanvasFileName.toLatin1(), std::ios::in );
  if ( !canvas_file )
  {
    *plog << LOG << codec->toUnicode( "ERROR: Не могу открыть \"per_canvas_i1.sch\" файл" ) << endl;
    return false;
  }
  else
  {
    *plog << codec->toUnicode( "Открываем \"" ) << FullCanvasFileName << codec->toUnicode( "\" файл;" ) << endl;
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

