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
  delete l_ctrl;
  l_ctrl = new line_control(270, 28, 28);

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

  Page_content = "";
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

