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
#include "do_element.h"

#define LOG ""


do_element::do_element()
{

}

do_element::do_element(const do_element &el2)
{
  codec = QTextCodec::codecForName("Windows-1251");

  File_Name = el2.File_Name;
  Detimal_Number = el2.Detimal_Number;
  File_info = el2.File_info ;
  bom_type_ =el2.bom_type_;

}

do_element do_element::operator=(const do_element &el2)
{
  File_Name = el2.File_Name;
  Detimal_Number = el2.Detimal_Number;
  File_info = el2.File_info ;
  bom_type_ =el2.bom_type_;
  return *this;
}

QString do_element::get_record(void)
{
  return (File_Name + " = " + Detimal_Number);
}

bool  do_element::is_empty(void)
{
   // if( File_Name. )

return true;
}


bool do_element::do_element_soft_check(QString &path_to_do, QString &source)
{
  QString::iterator iter;
  QString type;


  File_Name = path_to_do + "/";
  iter = source.begin();

  while (*iter != '"')
  {
    iter++;
    if (iter ==  source.end())
    {
      *plog << LOG  << "ERROR: " << codec->toUnicode("Неожиданное окончание записи : \"") << source << codec->toUnicode("\" Проверьте знаки '\"' ") << endl;
      return false;
    }
  }

  iter++;
  if (iter ==  source.end())
  {
    *plog << LOG  << "ERROR: " << codec->toUnicode("Неожиданное окончание записи : \"") << source << codec->toUnicode("\" Проверьте знаки '\"' ") << endl;
    return false;
  }


  while (*iter != '"')
  {
    File_Name += *iter;
    iter++;
    if (iter ==  source.end())
    {
      *plog << LOG  << "ERROR: " << codec->toUnicode("Неожиданное окончание записи : \"") << source << codec->toUnicode("\" Проверьте знаки '\"' ") << endl;
      return false;
    }

  }

  File_info.setFile(File_Name);


  if( File_info.suffix() != "bom" )
  {
      *plog << LOG  << "ERROR: " << codec->toUnicode("Файлы с расширением отличным от .bom : \"") << File_Name << codec->toUnicode("\" не поддерживаются") << endl;
      return false;
  }

  if( (File_info.baseName()).isEmpty() == true )
  {
      *plog << "ERROR: " << codec->toUnicode("Файлы : \"") << File_Name << codec->toUnicode("\" не содержит имени") << endl;
      return false;
  }
//-----------------------------------------------------------------
  iter++;
  if (iter ==  source.end())
  {
    *plog << LOG  << "ERROR: " << codec->toUnicode("Неожиданное окончание записи : \"") << source << codec->toUnicode("\" Проверьте знаки '\"' ") << endl;
    return false;
  }


  while (*iter != '"')
  {
    iter++;
    if (iter ==  source.end())
    {
      *plog << LOG  << "ERROR: " << codec->toUnicode("Неожиданное окончание записи : \"") << source << codec->toUnicode("\" Проверьте знаки '\"' ") << endl;
      return false;
    }
  }

  iter++;
  if (iter ==  source.end())
  {
    *plog << LOG  << "ERROR: " << codec->toUnicode("Неожиданное окончание записи : \"") << source << codec->toUnicode("\" Проверьте знаки '\"' ") << endl;
    return false;
  }


  while (*iter != '"')
  {
    Detimal_Number += *iter;
    iter++;
    if (iter ==  source.end())
    {
      *plog << LOG  << "ERROR: " << codec->toUnicode("Неожиданное окончание записи : \"") << source << codec->toUnicode("\" Проверьте знаки '\"' ") << endl;
      return false;
    }
  }

  if( Detimal_Number.isEmpty() == true )
  {
      *plog << LOG  << "ERROR: " << codec->toUnicode("В записи : \"") << source << codec->toUnicode("\" нет децимального номера") << endl;
      return false;
  }

  if( Detimal_Number.indexOf(QRegExp("[, ~!@$%^&*()+|\{}:<>?]"), 0) != -1 )
  {
      *plog << LOG  << "ERROR: " << codec->toUnicode("Децимальный номер : \"") << Detimal_Number << codec->toUnicode("\" содержит недопустимые символы") << endl;
      return false;
  }
//------------------------------------------------------------------------
  iter++;
  if (iter ==  source.end())
  {
    *plog << LOG  << "ERROR: " << codec->toUnicode("Неожиданное окончание записи : \"") << source << codec->toUnicode("\" Проверьте знаки '\"' ") << endl;
    return false;
  }

  while (*iter != '"')
  {
    iter++;
    if (iter ==  source.end())
    {
      *plog << LOG  << "ERROR: " << codec->toUnicode("Неожиданное окончание записи : \"") << source << codec->toUnicode("\" Проверьте знаки '\"' ") << endl;
      return false;
    }
  }

  iter++;
  if (iter ==  source.end())
  {
    *plog << LOG  << "ERROR: " << codec->toUnicode("Неожиданное окончание записи : \"") << source << codec->toUnicode("\" Проверьте знаки '\"' ") << endl;
    return false;
  }

  while (*iter != '"')
  {
    type += *iter;
    iter++;
    if (iter ==  source.end())
    {
      *plog << LOG  << "ERROR: " << codec->toUnicode("Неожиданное окончание записи : \"") << source << codec->toUnicode("\" Проверьте знаки '\"' ") << endl;
      return false;
    }
  }

  if( type == "pcad" )
  {
     bom_type_ = BOM_PCAD;
  }
  else
    if( type == "ad" )
    {
      bom_type_ = BOM_AD;
    }
    else
    {
      bom_type_ = BOM_UNKNOWN;
      *plog << LOG  << "ERROR: " << codec->toUnicode("Неизвестный формат файла : \"") << type << codec->toUnicode("\" в строчке : \"") << source << "\"" << endl;
      return false;
    }


  return true;
}





