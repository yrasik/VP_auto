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
#include "dot_export.h"

#define LOG ""

dot_export::dot_export()
{
  created = false;
}


int dot_export::create_dot(QString File_Name)
{
  dot_file = new QFile(File_Name);
  if (! dot_file->open(QIODevice::WriteOnly | QIODevice::Text))
  {
    *plog << codec->toUnicode("ERROR: ÕÂ ÏÓ„Û ÒÓÁ‰‡Ú¸ \"") << File_Name << codec->toUnicode("\" Ù‡ÈÎ") << endl;
    return -1;
  }
  dot = new QTextStream(dot_file);

  if( dot == NULL )
  {
    *plog << codec->toUnicode("ERROR: if( dot == NULL )") << endl;
    dot_file->close();
    return -2;
  }

  created = true;
  dot->setCodec("UTF-8");

  *dot << "digraph G{" << endl;
  *dot << "charset=\"utf8\";" << endl;
  //*dot << "size=\"3,4\";" << endl;
//# ratio="fill";
// size="8.3,11.7!";
//# margin=0;


  *dot << "graph [ dpi = 400 ];" << endl;
  *dot << "node [shape = box];" << endl;
  *dot << "rankdir=\"BT\";" << endl;
  *dot << "/* The rest of your graph here. */" << endl;


 // "¿¡¬√ƒ.123456.473" [label="¿¡¬√ƒ.123456.473\n143254"];


  return 0;
}


void dot_export::set_record(QString bot, QString top)
{
  if(created == true)
  {
    *dot << "\"" << bot << "\"" << "->" << "\"" << top << "\"" << ";" << endl;
  }
}


void dot_export::set_name_record(QString box, QString name)
{
// "¿¡¬√ƒ.123456.473" [label="¿¡¬√ƒ.123456.473\n143254"];
*dot << "\"" << box << "\"" << "[label=\"" << name << "\\n" << box << "\"]" << endl;

}



void dot_export::close_dot(void)
{
  if(created == true)
  {
    *dot << "}" << endl;
    dot_file->close();
    delete dot;
    delete dot_file;

    created = false;
  }
}


bool dot_export::is_created(void)
{
  return created;
}




  
