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
#include "element.h"

#define LOG ""

element::element ()
{
  Count = 0;
  RefDes_count = 0;
}


element::element ( QString &Value, QString &Ref, int file_line_num )
{
  RefDes = Ref;
  am_removable = false;
  Count = 1;
  Detimal_Number = "EMPTY";
  RefDes_count = 1;
  this->file_line_num = file_line_num;

  QStringList lst = Value.split( "\\", QString::KeepEmptyParts );
  if ( lst.size() >= 3 )
  {
    Value_Decoding = lst.at(0);
    Value_Code = lst.at(1);
    Value_Firm = lst.at(2);
  }
  else
    if( lst.size() >= 2 )
    {
      Value_Code = lst.at(0);
      Value_Firm = lst.at(1);
    }

   analyse_Value_Code();
}



element::element ( QString *source, QString *detimal, enum bom_type *bom_type_ )
{
  if ( *bom_type_ == BOM_PCAD )
  {
    element_pcad( source, detimal );
  }
  else if ( *bom_type_ == BOM_AD )
  {
    element_ad( source, detimal );
  }
  else
  {
    *plog << LOG << "ERROR: " << codec->toUnicode( "Данный тип записи не поддерживается : {" ) << source->trimmed() << "}" << endl;
  }

  analyse_Value_Code();
}

bool element::element_pcad ( QString *source, QString *detimal )
{
  QString         temp;
  QString         value;
  QString         str_count;
  QString         str_refdes_count;
  QString::iterator iter;
  am_removable = false;
  Count = 0;

  Detimal_Number = *detimal;

  if ( source->indexOf(QRegExp("\""), 0) == -1 )
  {
    *plog << LOG << "ERROR: " << codec->toUnicode( "Неверный формат строки : {" ) << ( *source ).trimmed() << "}" << endl;
    am_removable = true;
    return false;
  }

  for ( iter = source->begin(); iter < source->end(); ++iter )  // Избавляемся от знаков '"'
  {
    if ( (*iter != '"') && (*iter != '\n') )
    {
      temp += *iter;
    }
  }

  for ( iter = temp.begin(); *iter != '@' && iter < temp.end(); ++iter )
  { // Рассовываем текст по специальным полям
    if ( *iter >= '0' && *iter <= '9' )
    {
      str_refdes_count += *iter;
    }
    else
    {
      RefDes += *iter;
    }
  }

  if ( iter == temp.end() )
  {
    *plog << LOG << "ERROR: " << codec->toUnicode( "Неожиданное окончание записи (1) : {" ) << ( *source ).trimmed() << "}" << endl;
    return false;
  }

  ++iter;

  if ( iter == temp.end() )
  {
    *plog << LOG << "ERROR: " << codec->toUnicode( "Неожиданное окончание записи (2) : {" ) << ( *source ).trimmed() << "}" << endl;
    return false;
  }

  for ( ; *iter != '@' && iter < temp.end(); ++iter )
  {
    str_count += *iter;
  }

  if ( iter == temp.end() )
  {
    *plog << LOG << "ERROR: " << codec->toUnicode( "Неожиданное окончание записи (3) : {" ) << ( *source ).trimmed() << "}" << endl;
    return false;
  }

  ++iter;

  for ( ; iter < temp.end(); ++iter )
  {
    value += *iter;
  }

  for ( iter = value.begin(); *iter != '\\' && iter < value.end(); ++iter )
  {
    Value_Decoding += *iter;
  }

  if ( iter == temp.end() )
  {
    *plog << LOG << "ERROR: " << codec->toUnicode( "Неожиданное окончание записи (4) : {" ) << ( *source ).trimmed() << "}" << endl;
    return false;
  }

  ++iter;

  if ( iter == temp.end() )
  {
    *plog << LOG << "ERROR: " << codec->toUnicode( "Неожиданное окончание записи (5) : {" ) << ( *source ).trimmed() << "}" << endl;
    return false;
  }

  for ( ; *iter != '\\' && iter < value.end(); ++iter )
  {
    Value_Code += *iter;
  }

  if ( iter == temp.end() )
  {
    *plog << LOG << "ERROR: " << codec->toUnicode( "Неожиданное окончание записи (6) : {" ) << ( *source ).trimmed() << "}" << endl;
    return false;
  }

  ++iter;

  if ( iter == temp.end() )
  {
    *plog << LOG << "ERROR: " << codec->toUnicode( "Неожиданное окончание записи (7) : {" ) << ( *source ).trimmed() << "}" << endl;
    return false;
  }

  for ( ; iter < value.end(); ++iter )
  {
    Value_Firm += *iter;
  }

  if ( RefDes.indexOf("KT") != -1 )
  { // Контрольные точки не покупаем...
    am_removable = true;
  }

  if ( RefDes.indexOf("REF") != -1 )
  { // Реперные знаки не покупаем...
    am_removable = true;
  }

  if ( RefDes.indexOf("REF_P") != -1 )
  { // Реперные знаки не покупаем...
    am_removable = true;
  }

  if ( RefDes.indexOf("REF_G") != -1 )
  { // Реперные знаки не покупаем...
    am_removable = true;
  }

  if ( RefDes.indexOf("UNUZED") != -1 )
  { // Помеченные элементы не покупаем
    am_removable = true;
  }

  if ( RefDes.size() == 0 )
  { // Паяные контакты не покупаем
    am_removable = true;
  }

  Count = str_count.toInt();  // Переводим str_count в unsigned int - предстваление
  RefDes_count = str_refdes_count.toInt();

  return true;
}

bool element::element_ad ( QString *source, QString *detimal )
{
  QString         temp;
  QString         value;
  QString         str_count;
  QString         str_refdes_count;
  QString::iterator iter;
  am_removable = false;
  Count = 0;

  Detimal_Number = *detimal;

  if ( source->indexOf(QRegExp("\""), 0) == -1 )
  {
    *plog << LOG << "ERROR: " << codec->toUnicode( "Неверный формат строки : {" ) << ( *source ).trimmed() << "}" << endl;
    am_removable = true;
    return false;
  }

  for ( iter = source->begin(); iter < source->end(); ++iter )            // Избавляемся от знаков '"'
  {
    if ( (*iter != '"') && (*iter != '\n') )
    {
      temp += *iter;
    }
  }

  for ( iter = temp.begin(); *iter != '\t' && iter < temp.end(); ++iter ) // Рассовываем текст по специальным полям
  {
    if ( *iter >= '0' && *iter <= '9' )
    {
      str_refdes_count += *iter;
    }
    else
    {
      RefDes += *iter;
    }
  }

  if ( iter == temp.end() )
  {
    *plog << LOG << "ERROR: " << codec->toUnicode( "Неожиданное окончание записи (1) : {" ) << ( *source ).trimmed() << "}" << endl;
    return false;
  }

  ++iter;
  if ( iter == temp.end() )
  {
    *plog << LOG << "ERROR: " << codec->toUnicode( "Неожиданное окончание записи (2) : {" ) << ( *source ).trimmed() << "}" << endl;
    return false;
  }

  for ( ; *iter != '\t' && iter < temp.end(); ++iter )
  {
    str_count += *iter;
  }

  if ( iter == temp.end() )
  {
    *plog << LOG << "ERROR: " << codec->toUnicode( "Неожиданное окончание записи (3) : {" ) << ( *source ).trimmed() << "}" << endl;
    return false;
  }

  ++iter;

  for ( ; iter < temp.end(); ++iter )
  {
    value += *iter;
  }

  for ( iter = value.begin(); *iter != '\\' && iter < value.end(); ++iter )
  {
    Value_Decoding += *iter;
  }

  if ( iter == temp.end() )
  {
    *plog << LOG << "ERROR: " << codec->toUnicode( "Неожиданное окончание записи (4) : {" ) << ( *source ).trimmed() << "}" << endl;
    return false;
  }

  ++iter;

  if ( iter == temp.end() )
  {
    *plog << LOG << "ERROR: " << codec->toUnicode( "Неожиданное окончание записи (5) : {" ) << ( *source ).trimmed() << "}" << endl;
    return false;
  }

  for ( ; *iter != '\\' && iter < value.end(); ++iter )
  {
    Value_Code += *iter;
  }

  if ( iter == temp.end() )
  {
    *plog << LOG << "ERROR: " << codec->toUnicode( "Неожиданное окончание записи (6) : {" ) << ( *source ).trimmed() << "}" << endl;
    return false;
  }

  ++iter;

  if ( iter == temp.end() )
  {
    *plog << LOG << "ERROR: " << codec->toUnicode( "Неожиданное окончание записи (7) : {" ) << ( *source ).trimmed() << "}" << endl;
    return false;
  }

  for ( ; iter < value.end(); ++iter )
  {
    Value_Firm += *iter;
  }

  if ( RefDes.indexOf("KT") != -1 )
  { // Контрольные точки не покупаем...
    am_removable = true;
  }

  if ( RefDes.indexOf("REF") != -1 )
  { // Реперные знаки не покупаем...
    am_removable = true;
  }

  if ( RefDes.indexOf("REF_P") != -1 )
  { // Реперные знаки не покупаем...
    am_removable = true;
  }

  if ( RefDes.indexOf("REF_G") != -1 )
  { // Реперные знаки не покупаем...
    am_removable = true;
  }

  if ( RefDes.indexOf("UNUZED") != -1 )
  { // Помеченные элементы не покупаем
    am_removable = true;
  }

  if ( RefDes.size() == 0 )
  { // Паяные контакты не покупаем
    am_removable = true;
  }

  Count = str_count.toInt();  // Переводим str_count в unsigned int - предстваление
  RefDes_count = str_refdes_count.toInt();

  return true;
}

element::element ( const element &el2 )
{
  am_removable = el2.am_removable;
  Detimal_Number = el2.Detimal_Number;
  Count = el2.Count;
  RefDes = el2.RefDes;
  RefDes_count = el2.RefDes_count;
  Value_Code = el2.Value_Code;
  Value_Decoding = el2.Value_Decoding;
  Value_Firm = el2.Value_Firm;

  Code_from_Value = el2.Code_from_Value;
  Type_from_Value = el2.Type_from_Value;
  Types_from_Value = el2.Types_from_Value;
  Group_Name = el2.Group_Name;

  file_line_num = el2.file_line_num;
}

bool operator < (const element &el1, const element &el2)
{
  if ( el1.RefDes < el2.RefDes )
  {
    return true;
  }
  else
  {
    return false;
  }
}
bool element::by_Value_Code ( const element &el1, const element &el2 )
{
  if ( el1.Value_Code < el2.Value_Code )
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool element::by_Type_from_Value_and_Code_from_Value ( const element &el1, const element &el2 )
{
  if ( el1.Type_from_Value < el2.Type_from_Value )
  {
    return true;
  }

  if ( el1.Type_from_Value == el2.Type_from_Value )
  {
    if ( el1.Code_from_Value < el2.Code_from_Value )
    {
      return true;
    }
  }

  return false;
}

bool element::by_RefDes_Count ( const element &el1, const element &el2 )
{
  if ( el1.RefDes_count < el2.RefDes_count )
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool element::by_Ref_Value_Code_Value_Decoding ( const element &el1, const element &el2 )
{
  if ( el1.RefDes == el2.RefDes )
  {
    if ( el1.Value_Code == el2.Value_Code )
    {
      if ( el1.Value_Decoding == el2.Value_Decoding )
      {
        return true;
      }
    }
  }

  return false;
}

bool element::by_Ref_Value_Code_Value_Decoding_Value_Firm_Detimal_Number ( const element &el1, const element &el2 )
{
  bool  result = false;

  if ( el1.RefDes == el2.RefDes )
  {
    if ( el1.Value_Code == el2.Value_Code )
    {
      if ( el1.Value_Decoding == el2.Value_Decoding )
      {
        if ( el1.Value_Firm == el2.Value_Firm )
        {
          if ( el1.Detimal_Number == el2.Detimal_Number )
          {
            result = true;
          }
        }
      }
    }
  }

  return result;
}



bool element::eq_Ref_Type_Code_Decoding_Firm ( const element &el1, const element &el2 )
{
  bool  result = false;

  if ( el1.RefDes == el2.RefDes )
  {
    if ( el1.Type_from_Value == el2.Type_from_Value )
    {
      if ( el1.Code_from_Value == el2.Code_from_Value )
      {
        if ( el1.Value_Decoding == el2.Value_Decoding )
        {
          if ( el1.Value_Firm == el2.Value_Firm )
          {
            result = true;
          }
        }
      }
    }
  }

  return result;
}






bool element::by_RefDes_RefDes_count ( const element &el1, const element &el2 )
{
  if ( el1.RefDes < el2.RefDes )
  {
    return true;
  }

  if ( (el1.RefDes == el2.RefDes) && (el1.RefDes_count < el2.RefDes_count) )
  {
    return true;
  }

  return false;
}


bool element::by_Value_Firm_Ref ( const QVector<element> &el1, const QVector<element> &el2 )
{
  class element el1_ = ( *el1.begin() );
  class element el2_ = ( *el2.begin() );

  if ( el1_.get_RefDes() < el2_.get_RefDes() )
  {
    return true;
  }
  else
  {
    if ( el1_.get_RefDes() == el2_.get_RefDes() )
    {
      if ( el1_.get_Value_Firm() < el2_.get_Value_Firm() )
      {
        return true;
      }
      else
      {
        if ( el1_.get_Value_Firm() == el2_.get_Value_Firm() )
        {
          if ( el1_.get_Value_Code() <= el2_.get_Value_Code() )
          {
            return true;
          }
          else
          {
            return false;
          }
        }
        else
        {
          return false;
        }
      }
    }
    else
    {
      return false;
    }
  }

  return false;
}


void element::similarity_elements ( QVector<element> &ls1 )
{
  *plog << endl;
  *plog << codec->toUnicode( "#########################################################################" ) << endl;
  *plog << codec->toUnicode( "###                        Сравнение элементов                        ###" ) << endl;
  *plog << codec->toUnicode( "#########################################################################" ) << endl;
  *plog << codec->toUnicode( "### Справочная информация :                                           ###" ) << endl;
  *plog << codec->toUnicode( "###                                                                   ###" ) << endl;
  *plog << codec->toUnicode( "###      Проверка на различную запись одного элемента                 ###" ) << endl;
  *plog << codec->toUnicode( "### Скорее всего один и тот-же элемент записан по разному             ###" ) << endl;
  *plog << codec->toUnicode( "###   Результат - лишние вопросы в отделе комплектации,               ###" ) << endl;
  *plog << codec->toUnicode( "###  коррекция перечня, спецификации, ведомости покупных              ###" ) << endl;
  *plog << codec->toUnicode( "#########################################################################" ) << endl;
  *plog << LOG << "START {" << endl;

  QVector<element>::iterator  i;
  QVector<element>::iterator  j;

  for ( i = ls1.begin(); i < ls1.end(); i++ )
  {
    j = i;
    for ( ; j < ls1.end(); j++ )
    {
      class element::sr_ss result = element::is_Similarity( *i, *j );

      if ( (result.SR != element::SR_SAME) && (result.SR != element::SR_DIFFERENT) )
      {
        *plog << "WARNING : ";
        *plog << element::Decoding_Similarity_result ( result );
        *plog << " :" << endl;
        *plog << "    ";
        *plog << i->get_record2() << endl;
        *plog << "    ";
        *plog << j->get_record2() << endl << endl;
      }
    }
  }
  *plog << LOG << "} END " << endl;
}


bool operator== ( const element &el1, const element &el2 )
{
  bool  temp = false;
  if ( (el1.RefDes == el2.RefDes) &&
       (el1.Code_from_Value == el2.Code_from_Value) &&
       (el1.Type_from_Value == el2.Type_from_Value) &&
       (el1.Value_Decoding == el2.Value_Decoding) && //Добавил расшифровку. Больше несоответствий вылезет
       (el1.Detimal_Number == el2.Detimal_Number) &&
       (el1.Value_Firm == el2.Value_Firm) ) //надо по хорошему сравнить фирму...
  {
    temp = true;
  }

  return temp;
}

bool operator!= ( const element &el1, const element &el2 )
{
  bool  temp = false;
  if ( (el1.RefDes == el2.RefDes) &&
       (el1.Code_from_Value == el2.Code_from_Value) &&
       (el1.Type_from_Value == el2.Type_from_Value) &&
       (el1.Value_Decoding == el2.Value_Decoding) && //Добавил расшифровку. Больше несоответствий вылезет
       (el1.Detimal_Number != el2.Detimal_Number) &&
       (el1.Value_Firm == el2.Value_Firm) ) //надо по хорошему сравнить фирму...
  {
    temp = true;
  }

  return temp;
}

element operator+ ( element el2 )
{
  element temp = el2;

  temp.Count += el2.Count;
  return temp;
}

element element::operator= ( const element &el2 )
{
  am_removable = el2.am_removable;
  Detimal_Number = el2.Detimal_Number;
  Count = el2.Count;
  RefDes = el2.RefDes;
  RefDes_count = el2.RefDes_count;
  Value_Code = el2.Value_Code;
  Value_Decoding = el2.Value_Decoding;
  Value_Firm = el2.Value_Firm;

  Code_from_Value = el2.Code_from_Value;
  Type_from_Value = el2.Type_from_Value;
  Types_from_Value = el2.Types_from_Value;
  Group_Name = el2.Group_Name;

  file_line_num = el2.file_line_num;

  return *this;
}

QString element::get_record ( void )
{
  return ( RefDes + '\t' + Value_Code + "   " + Value_Decoding + "   " + Value_Firm + "   " + Detimal_Number + "   " + QString::number(Count) + "   " + Group_Name );
}

QString element::get_full_record ( void )
{
  return ( RefDes + QString::number(RefDes_count) + '\t' + Value_Code + "   " + Value_Decoding + "   " + Value_Firm + "   " + Detimal_Number + "   " + QString::number(Count) + "   " + Group_Name );
}

QString element::get_full_record_empty_firm ( void )
{
  return ( RefDes + QString::number(RefDes_count) + '\t' + Value_Code + "   " + Value_Decoding + "   " + Detimal_Number + "   " + QString::number(Count) + "   " + Group_Name );
}

QString element::get_record1 ( void )
{
  return ( RefDes + QString::number(RefDes_count) + '\t' + Value_Code + "   " + Value_Decoding + "   " + Value_Firm + "   " + QString::number(Count) + "   " + Group_Name );
}

QString element::get_record2 ( void )
{
  return ( "'" + RefDes + QString::number(RefDes_count) + "'\t'" + Type_from_Value + "'\t'" + Code_from_Value + "'\t'" + Value_Decoding + "'\t'" + Value_Firm + "'\t'" + QString::number(Count) + "'\t'" + Group_Name + "'\t'" + Detimal_Number + "'" );
}

QString element::get_record3 ( void )
{
  return ( "'" + Type_from_Value + "'\t'" + Code_from_Value + "'\t'" + Value_Decoding + "'\t'" + Value_Firm + "'\t'" + QString::number(Count) + "'\t'" + Group_Name + "'\t'" + Detimal_Number + "'" );
}

QString element::get_record4 ( void )
{
  //order << codec->toUnicode("Наименование\tОбозначение\tФирма/ТУ\tКод группы\tКоличество\tПримечание\tЦена") << endl;
  return ( Type_from_Value + "\t" + Code_from_Value + " " + Value_Decoding + "\t" + Value_Firm );
}

QString element::get_record5 ( void )
{
  //order << codec->toUnicode("Наименование\tОбозначение\tФирма/ТУ\tКуда входит\tКоличество\tПримечание\tЦена") << endl;
  return ( Type_from_Value + "\t" + Code_from_Value + " " + Value_Decoding + "\t" + Value_Firm + "\t" + Detimal_Number + "\t" + QString::number(Count) + "\t" + "\t" );
}

QString element::get_record6 ( void )
{
  return ( Value_Decoding + "\\" + get_Type_from_Value_if_not_equal_Group_Name() + Code_from_Value + "\\" + Value_Firm + "@" + RefDes);
}


QString element::get_record7 ( void )
{
  return ( "LINE " + QString::number(file_line_num) + ": '" + RefDes + "'\t'" + Type_from_Value + "'\t'" + Code_from_Value + "'\t'" + Value_Decoding + "'\t'" + Value_Firm + "'" );
}




void element::analyse_Value_Code ( void )
{
  for ( QVector<text_name>::iterator i = Text_Names.begin(); i < Text_Names.end(); i++ )
  {
    if ( Value_Code.toLower().indexOf(i->get_Name().toLower(), Qt::CaseInsensitive) != -1 ) //Нашли подстроку в строке
    {
      if ( RefDes == i->get_Ref() )
      {
        Type_from_Value = i->get_Name();
        Types_from_Value = i->get_Names();
        Group_Name = i->get_Group_Name();
        Code_from_Value = Value_Code;
        Code_from_Value.remove( i->get_Name(), Qt::CaseInsensitive );
        Code_from_Value = Code_from_Value.trimmed();  //Удаляем пробелы с начала и с конца

        //     *plog << codec->toUnicode("============== ") << (*i).get_Name() << endl;
       // *plog << LOG << "          '" << Group_Name << "'       '" << Code_from_Value << "'       '" << Type_from_Value << "'       '" << Types_from_Value << endl;
        break;
      }
    }
  }

  if ( Type_from_Value.isEmpty() )                    //В наименовании элемента нет слова 'Резистор'
  {
    Code_from_Value = Value_Code.trimmed();           //Удаляем пробелы с начала и с конца
    for ( QVector<text_name>::iterator i = Text_Names.begin(); i < Text_Names.end(); i++ )
    {
      if ( RefDes == i->get_Ref() )
      {
        Types_from_Value = i->get_Group_Name();
        Type_from_Value = i->get_Group_Name_one();
        Group_Name = i->get_Group_Name();
      //  *plog << LOG << "          '" << Group_Name << "'       '" << Code_from_Value << "'       '" << Type_from_Value << "'       '" << Types_from_Value << endl;
        break;
      }
    }
  }
}

void element::remove_blocks_and_adding_content ( QString PathToOutDir, QVector<QVector<element> > &tables, QVector<element> &ls1 )
{
  *plog << endl;
  *plog << endl;
  *plog << codec->toUnicode( "#########################################################################" ) << endl;
  *plog << codec->toUnicode( "###   В список включается содержимое блоков (для ведомости покупных)  ###" ) << endl;
  *plog << codec->toUnicode( "#########################################################################" ) << endl;
  *plog << codec->toUnicode( "### Справочная информация :                                           ###" ) << endl;
  *plog << codec->toUnicode( "###                                                                   ###" ) << endl;
  *plog << codec->toUnicode( "### Предполагается, что нужно составить ведомость покупных, в которую ###" ) << endl;
  *plog << codec->toUnicode( "### войдут все покупные элементы, перечисленные во всех     *.bom -   ###" ) << endl;
  *plog << codec->toUnicode( "### файлах, подключенных к проекту, в соответствующем количестве.     ###" ) << endl;
  *plog << codec->toUnicode( "#########################################################################" ) << endl;
  *plog << endl;
  *plog << LOG << "START {" << endl;

  QVector<QVector<element> >::iterator  top_level_table;

  QVector<QVector<element> >::iterator  current_table;
  QVector<element>::iterator          top_level_iter;
  QVector<element>::iterator          temp_iter;

  enum process  processing = EXEC;

  top_level_table = tables.begin();

  for ( QVector<element>::iterator iter = top_level_table->begin(); iter < top_level_table->end(); iter++ )
  {
    ls1.push_back( *iter );
  }

  top_level_iter = ls1.begin();

  class dot_export  *dot = new dot_export();

  dot->create_dot( PathToOutDir + "/sch.dot" );
  *plog << codec->toUnicode( "PathToOutDir:" ) << PathToOutDir << "\"" << endl;

  while ( processing != STOP )
  {
    processing = EXEC;
    current_table = tables.begin();                   //Таблица верхнего уровня
    while ( processing != RESTART )                   //Сравниваем фирму в записи таблицы top_level_table с децимальными номерами блоков
    {
      current_table++;  //Опять начинаем со второй в списке таблицы
      if ( current_table == tables.end() )
      {                 //Проверяем есть-ли таблица вообще
        break;
      }

      temp_iter = current_table->begin(); //Указатель на первый элемент таблицы current_table
      if ( top_level_iter->get_Value_Firm() == temp_iter->get_Detimal_Number() )  //блок current_table оказался включенным в блок top_level_table
      {
        unsigned int  Count = top_level_iter->get_Count();  //Определяем сколько раз блок current_table оказался включенным в блок top_level_table
        *plog << codec->toUnicode( "В сборку \"" ) << top_level_iter->get_Detimal_Number() << "\"" << endl;
        *plog << codec->toUnicode( "   входит блок \"" ) << top_level_iter->get_Value_Code() << "   " << top_level_iter->get_Value_Firm() << "\"" << codec->toUnicode( "   в количестве " ) << ( *top_level_iter ).get_Count() << codec->toUnicode( " экз." ) << endl;
        *plog << codec->toUnicode( "   Содержимое блока будет включено в соответствующем количестве." ) << endl;

        dot->set_name_record( top_level_iter->get_Value_Firm(), top_level_iter->get_Value_Code() );
        dot->set_record( /*(*top_level_iter).get_Value_Code() + "\n" +*/ top_level_iter->get_Value_Firm(), top_level_iter->get_Detimal_Number() );

        ls1.erase( top_level_iter );  //Выкмдываем запись о блоке из top_level_table
        if ( Count == 0 )
        {

        }
        else
        {
          for ( QVector<element>::iterator iter = current_table->begin(); iter < current_table->end(); iter++ )  //Копируем все элементы из current_table в top_level_table с учётом количества
          {
            element temp_el = ( *iter );
            temp_el.set_Count( temp_el.get_Count() * Count );
            ls1.push_back( temp_el );
          }
        }

        *plog << endl;
        processing = RESTART; //Так как число элементов в top_level_table изменилось, нужно дать команду на перезапуск алгоритма
        top_level_iter = ls1.begin();
      }
    }

    if ( processing != RESTART )
    {
      top_level_iter++;       //Переводим указатель на следующий элемент в таблице top_level_table
      if ( top_level_iter == ls1.end() )
      { //Контролируем не кончилась ли таблица top_level_table
        processing = STOP;
      }
    }
  }

  dot->close_dot();
  delete dot;

  *plog << LOG << "} END " << endl;
}

void element::similarity_firm ( QVector<element> &ls1, QVector<firm_name> &Firm_Names )
{
  QVector<element> el;

  for ( QVector<element>::iterator i = ls1.begin(); i < ls1.end(); i++ )
  {
    el.push_back( *i );
  }

  *plog << endl;
  *plog << endl;
  *plog << codec->toUnicode( "#########################################################################" ) << endl;
  *plog << codec->toUnicode( "###       Проверка на правильность записи фирмы-производителя         ###" ) << endl;
  *plog << codec->toUnicode( "#########################################################################" ) << endl;
  *plog << codec->toUnicode( "### Справочная информация :                                           ###" ) << endl;
  *plog << codec->toUnicode( "###                                                                   ###" ) << endl;
  *plog << codec->toUnicode( "###    В файле /res/firms.txt содержится список названий фирм -       ###" ) << endl;
  *plog << codec->toUnicode( "### производителей элементов. Если фирмы нет в этом списке - даётся   ###" ) << endl;
  *plog << codec->toUnicode( "### предупреждение. Если название записано с ошибкой - то тоже        ###" ) << endl;
  *plog << codec->toUnicode( "### выдаётся предупреждение. Цель этого - минимизировать вопросы      ###" ) << endl;
  *plog << codec->toUnicode( "### в отделе комплектации, и нормоконторле, уменьшить ошибки в        ###" ) << endl;
  *plog << codec->toUnicode( "### перечне, спецификации, ведомости покупных.                        ###" ) << endl;
  *plog << codec->toUnicode( "#########################################################################" ) << endl;
  *plog << LOG << "START {" << endl;

  QVector<element>::iterator    elem;
  QVector<firm_name>::iterator  firm;

  for ( firm = Firm_Names.begin(); firm < Firm_Names.end(); firm++ )
  {
    elem = el.begin();
    while ( elem < el.end() )
    {
      enum similarity::ss result = similarity::is_Similarity( elem->get_Value_Firm(), firm->get_Firm() );
      if ( (result != similarity::ss::SS_SAME) && (result != similarity::ss::SS_DIFFERENT) )
      {
        *plog << "WARNING : ";
        *plog << similarity::Decoding_Similarity_result ( result );
        *plog << " :" << endl;
        *plog << "    ";
        *plog << elem->get_record2() << endl;
        *plog << "    ";
        *plog << firm->get_Firm() << endl << endl;
      }

      if (result == similarity::ss::SS_SAME)
      {
        el.erase(elem);
      }
      else
      {
        elem++;
      }

    }
  }


  for ( elem = el.begin(); elem < el.end(); elem++ )
  {
    *plog << "WARNING : ";
    *plog << codec->toUnicode("Фирмы нет в списке");
    *plog << " :" << endl;
    *plog << "    ";
    *plog << elem->get_record2() << endl << endl;
  }

  *plog << LOG << "} END " << endl;
}





















struct element::sr_ss element::is_Similarity ( const element &el1, const element &el2 )
{
  struct sr_ss SR_SS;

  SR_SS.SR = element::SR_DIFFERENT;
  SR_SS.SS = similarity::SS_DIFFERENT;

  element temp_el1 = el1;
  element temp_el2 = el2;

  enum similarity::ss sim_Code = similarity::is_Similarity( temp_el1.Code_from_Value, temp_el2.Code_from_Value );
  enum similarity::ss sim_Decoding = similarity::is_Similarity( temp_el1.Value_Decoding, temp_el2.Value_Decoding );
  enum similarity::ss sim_Firm = similarity::is_Similarity( temp_el1.Value_Firm, temp_el2.Value_Firm );

bool Decoding_1_isNull = false;
bool Decoding_2_isNull = false;

if ( temp_el1.Value_Decoding.size() == 0 )
{
  Decoding_1_isNull = true;
}

if ( temp_el2.Value_Decoding.size() == 0 )
{
  Decoding_2_isNull = true;
}


  switch ( sim_Code )
  {
    case similarity::SS_SAME:
      switch ( sim_Firm )
      {
        case similarity::SS_SAME:
          if ( (Decoding_1_isNull == false) && (Decoding_2_isNull == false) )
          {
            switch ( sim_Decoding )
            {
              case similarity::SS_SAME:
                SR_SS.SR = element::SR_SAME;
                SR_SS.SS = similarity::SS_SAME;
                return SR_SS;  //Одно и тоже
              case similarity::SS_DIFFERENT:
                SR_SS.SR = element::SR_SAME_CODE_FIRM______SIMILAR________________DIFF_DECODING;
                SR_SS.SS = similarity::SS_DIFFERENT;
                return SR_SS;
              default:
                SR_SS.SR = element::  SR_SAME_CODE_FIRM______SIMILAR_DECODING_______DIFF_;
                SR_SS.SS = sim_Decoding;
                return SR_SS;
            }
          }
          else
          {
            if ( ((Decoding_1_isNull == true) && (Decoding_2_isNull == false)) ||
                 ((Decoding_1_isNull == false) && (Decoding_2_isNull == true))
               )
            {
              SR_SS.SR = element::SR_SAME_CODE_FIRM______SIMILAR________________DIFF_DECODING;
              SR_SS.SS = similarity::SS_DIFFERENT;
              return SR_SS;
            }
            else
            {
              SR_SS.SR = element::SR_SAME;
              SR_SS.SS = similarity::SS_SAME;
              return SR_SS;  //Одно и тоже
            }
          }
          break;
        case similarity::SS_DIFFERENT:
          if ( (Decoding_1_isNull == false) && (Decoding_2_isNull == false) )
          {
            switch ( sim_Decoding )
            {
              case similarity::SS_SAME:
                SR_SS.SR = element::SR_SAME_CODE_DECODING__SIMILAR________________DIFF_FIRM;
                SR_SS.SS = similarity::SS_DIFFERENT;
                return SR_SS;
              case similarity::SS_DIFFERENT:
                SR_SS.SR = element::SR_SAME_CODE___________SIMILAR________________DIFF_FIRM_DECODING;
                SR_SS.SS = similarity::SS_DIFFERENT;
                return SR_SS;
              default:
                SR_SS.SR = element::SR_SAME_CODE___________SIMILAR_DECODING_______DIFF_FIRM;
                SR_SS.SS = sim_Decoding;
                return SR_SS;
            }
          }
          else
          {
             if ( ((Decoding_1_isNull == true) && (Decoding_2_isNull == false)) ||
                  ((Decoding_1_isNull == false) && (Decoding_2_isNull == true))
                )
             {
                SR_SS.SR = element::SR_SAME_CODE___________SIMILAR________________DIFF_FIRM_DECODING;
                SR_SS.SS = similarity::SS_DIFFERENT;
                return SR_SS;
             }
             else
             {
                SR_SS.SR = element::SR_SAME_CODE_DECODING__SIMILAR________________DIFF_FIRM;
                SR_SS.SS = similarity::SS_DIFFERENT;
                return SR_SS;
             }
          }
          break;
        default:
          if ( (Decoding_1_isNull == false) && (Decoding_2_isNull == false) )
          {
            switch ( sim_Decoding )
            {
              case similarity::SS_SAME:
                SR_SS.SR = element::SR_SAME_CODE_DECODING__SIMILAR_FIRM___________DIFF_;
                SR_SS.SS = sim_Firm;
                return SR_SS;
              case similarity::SS_DIFFERENT:
                SR_SS.SR = element::SR_SAME_CODE___________SIMILAR_FIRM___________DIFF_DECODING;
                SR_SS.SS = sim_Firm;
                return SR_SS;
              default:
                SR_SS.SR = element::SR_SAME_CODE___________SIMILAR_FIRM_DECODING_______DIFF_;
                SR_SS.SS = sim_Firm;
                return SR_SS;
            }
          }
          else
          {
            if ( ((Decoding_1_isNull == true) && (Decoding_2_isNull == false)) ||
                 ((Decoding_1_isNull == false) && (Decoding_2_isNull == true))
               )
            {
              SR_SS.SR = element::SR_SAME_CODE___________SIMILAR_FIRM___________DIFF_DECODING;
              SR_SS.SS = sim_Firm;
              return SR_SS;
            }
            else
            {
              SR_SS.SR = element::SR_SAME_CODE_DECODING__SIMILAR_FIRM___________DIFF_,
              SR_SS.SS = sim_Firm;
              return SR_SS;
            }
          }
      }
      break;
    case similarity::SS_DIFFERENT:
      switch ( sim_Firm )
      {
        case similarity::SS_SAME:
          if ( (Decoding_1_isNull == false) && (Decoding_2_isNull == false) )
          {
            switch ( sim_Decoding )
            {
              case similarity::SS_SAME:
                SR_SS.SR = element::SR_SAME_FIRM_DECODING__SIMILAR________________DIFF_CODE;
                SR_SS.SS = sim_Code;
                return SR_SS;
              case similarity::SS_DIFFERENT:
                SR_SS.SR = element::SR_DIFFERENT;
                SR_SS.SS = similarity::SS_DIFFERENT;
                return SR_SS;
              default:
                SR_SS.SR = element::SR_SAME_FIRM___________SIMILAR_DECODING_______DIFF_CODE;
                SR_SS.SS = sim_Decoding;
                return SR_SS;
            }
          }
          else
          {
            if ( ((Decoding_1_isNull == true) && (Decoding_2_isNull == false)) ||
                 ((Decoding_1_isNull == false) && (Decoding_2_isNull == true))
               )
            {
              SR_SS.SR = element::SR_DIFFERENT;
              SR_SS.SS = similarity::SS_DIFFERENT;
              return SR_SS;
            }
            else
            {
              SR_SS.SR = element::SR_DIFFERENT;
              SR_SS.SS = similarity::SS_DIFFERENT;
              return SR_SS;
            }
          }
          break;
        case similarity::SS_DIFFERENT:  //Код и фирма не совпадают..
          if ( (Decoding_1_isNull == false) && (Decoding_2_isNull == false) )
          {
            switch ( sim_Decoding )
            {
              case similarity::SS_SAME:
                SR_SS.SR = element::SR_SAME_DECODING_______SIMILAR________________DIFF_CODE_FIRM;
                SR_SS.SS = similarity::SS_DIFFERENT;
                return SR_SS;
              case similarity::SS_DIFFERENT:
                SR_SS.SR = element::SR_DIFFERENT;
                SR_SS.SS = similarity::SS_DIFFERENT;
                return SR_SS;
              default:
                SR_SS.SR = element::SR_SAME________________SIMILAR_DECODING_______DIFF_CODE_FIRM;
                SR_SS.SS = sim_Decoding;
                return SR_SS;
            }
          }
          else
          {
            if ( ((Decoding_1_isNull == true) && (Decoding_2_isNull == false)) ||
                 ((Decoding_1_isNull == false) && (Decoding_2_isNull == true))
               )
            {
              SR_SS.SR = element::SR_DIFFERENT;
              SR_SS.SS = similarity::SS_DIFFERENT;
              return SR_SS;
            }
            else
            {
              SR_SS.SR = element::SR_DIFFERENT;
              SR_SS.SS = similarity::SS_DIFFERENT;
              return SR_SS;
            }
          }
          break;
        default:
          if ( (Decoding_1_isNull == false) && (Decoding_2_isNull == false) )
          {
            switch ( sim_Decoding )
            {
              case similarity::SS_SAME:
                SR_SS.SR = element::SR_SAME_DECODING_______SIMILAR_FIRM___________DIFF_CODE;
                SR_SS.SS = sim_Firm;
                return SR_SS;
              case similarity::SS_DIFFERENT:
                SR_SS.SR = element::SR_DIFFERENT;
                SR_SS.SS = similarity::SS_DIFFERENT;
                return SR_SS;
              default:
                SR_SS.SR = element::SR_SAME________________SIMILAR_DECODING_FIRM__DIFF_CODE;
                SR_SS.SS = sim_Decoding;
                return SR_SS;
            }
          }
          else
          {
            if ( ((Decoding_1_isNull == true) && (Decoding_2_isNull == false)) ||
                 ((Decoding_1_isNull == false) && (Decoding_2_isNull == true))
               )
            {
              SR_SS.SR = element::SR_DIFFERENT;
              SR_SS.SS = similarity::SS_DIFFERENT;
              return SR_SS;
            }
            else
            {
              SR_SS.SR = element::SR_DIFFERENT;
              SR_SS.SS = similarity::SS_DIFFERENT;
              return SR_SS;
            }
          }
      }
      break;
    default:
      switch ( sim_Firm )
      {
        case similarity::SS_SAME:
          if ( (Decoding_1_isNull == false) && (Decoding_2_isNull == false) )
          {
            switch ( sim_Decoding )
            {
              case similarity::SS_SAME:
                SR_SS.SR = element::SR_SAME_DECODING_FIRM__SIMILAR_CODE___________DIFF_;
                SR_SS.SS = sim_Code;
                return SR_SS;
              case similarity::SS_DIFFERENT:
                SR_SS.SR = element::SR_SAME_FIRM___________SIMILAR_CODE___________DIFF_DECODING;
                SR_SS.SS = sim_Code;
                return SR_SS;
              default:
                SR_SS.SR = element::SR_SAME_FIRM___________SIMILAR_CODE_DECODING__DIFF_;
                SR_SS.SS = sim_Code;
                return SR_SS;
            }
          }
          else
          {
            if ( ((Decoding_1_isNull == true) && (Decoding_2_isNull == false)) ||
                 ((Decoding_1_isNull == false) && (Decoding_2_isNull == true))
               )
            {
              SR_SS.SR = element::SR_SAME_FIRM___________SIMILAR_CODE___________DIFF_DECODING;
              SR_SS.SS = sim_Code;
              return SR_SS;
            }
            else
            {
              SR_SS.SR = element::SR_SAME_DECODING_FIRM__SIMILAR_CODE___________DIFF_;
              SR_SS.SS = sim_Code;
              return SR_SS;
            }
          }
          break;
        case similarity::SS_DIFFERENT:
          if ( (Decoding_1_isNull == false) && (Decoding_2_isNull == false) )
          {
            switch ( sim_Decoding )
            {
              case similarity::SS_SAME:
                SR_SS.SR = element::SR_SAME_DECODING_______SIMILAR_CODE___________DIFF_FIRM;
                SR_SS.SS = sim_Code;
                return SR_SS;
              case similarity::SS_DIFFERENT:
                SR_SS.SR = element::SR_SAME________________SIMILAR_CODE___________DIFF_FIRM_DECODING;
                SR_SS.SS = sim_Code;
                return SR_SS;
              default:
                SR_SS.SR = element::SR_SAME________________SIMILAR_CODE_DECODING__DIFF_FIRM;
                SR_SS.SS = sim_Code;
                return SR_SS;
            }
          }
          else
          {
            if ( ((Decoding_1_isNull == true) && (Decoding_2_isNull == false)) ||
                 ((Decoding_1_isNull == false) && (Decoding_2_isNull == true))
               )
            {
              SR_SS.SR = element::SR_SAME________________SIMILAR_CODE___________DIFF_FIRM_DECODING;
              SR_SS.SS = sim_Code;
              return SR_SS;
            }
            else
            {
              SR_SS.SR = element::SR_SAME_DECODING_______SIMILAR_CODE___________DIFF_FIRM;
              SR_SS.SS = sim_Code;
              return SR_SS;
            }
          }
          break;
        default:
          if ( (Decoding_1_isNull == false) && (Decoding_2_isNull == false) )
          {
            switch ( sim_Decoding )
            {
              case similarity::SS_SAME:
                SR_SS.SR = element::SR_SAME_DECODING_______SIMILAR_CODE_FIRM______DIFF_;
                SR_SS.SS = sim_Code;
                return SR_SS;
              case similarity::SS_DIFFERENT:
                SR_SS.SR = element::SR_SAME________________SIMILAR_CODE_FIRM______DIFF_DECODING;
                SR_SS.SS = sim_Code;
                return SR_SS;
              default:
                SR_SS.SR = element::SR_SAME________________SIMILAR_CODE_FIRM_DECODING__DIFF_;
                SR_SS.SS = sim_Code;
                return SR_SS;
            }
          }
          else
          {
            if ( ((Decoding_1_isNull == true) && (Decoding_2_isNull == false)) ||
                 ((Decoding_1_isNull == false) && (Decoding_2_isNull == true))
               )
            {
              SR_SS.SR = element::SR_SAME________________SIMILAR_CODE_FIRM______DIFF_DECODING;
              SR_SS.SS = sim_Code;
              return SR_SS;
            }
            else
            {
              SR_SS.SR = element::    SR_SAME_DECODING_______SIMILAR_CODE_FIRM______DIFF_;
              SR_SS.SS = sim_Code;
              return SR_SS;
            }
          }
      }

  }

  return SR_SS; //Очень большие различия
}


QString element::Decoding_Similarity_result ( struct sr_ss SR_SS )
{
  QString result;

    switch( SR_SS.SR )
    {
      case SR_SAME :
        result = codec->toUnicode( "Код, расшифровка, фирма совпадаю" );
        break;
      case SR_SAME_CODE_FIRM______SIMILAR_DECODING_______DIFF_:
        result = codec->toUnicode( "Код и фирма совпадают, найдено совпадение в расшифровке " );
        break;
      case SR_SAME_CODE_DECODING__SIMILAR_FIRM___________DIFF_:
        result = codec->toUnicode( "Код и расшифровка совпадают, найдено совпадение в фирме " );
        break;
      case SR_SAME_DECODING_FIRM__SIMILAR_CODE___________DIFF_:
        result = codec->toUnicode( "Дешифровка и фирма совпадают, найдено совпадение в коде " );
        break;
      case SR_SAME_CODE___________SIMILAR_FIRM_DECODING_______DIFF_:
        result = codec->toUnicode( "Код совпадает, найдено совпадение в фирме и дешифровке " );
        break;
      case SR_SAME_CODE___________SIMILAR_DECODING_______DIFF_FIRM :
        result = codec->toUnicode( "Код совпадает, фирма не совпадает, найдено совпадение в дешифровке " );
        break;
      case SR_SAME_CODE___________SIMILAR_FIRM___________DIFF_DECODING :
        result = codec->toUnicode( "Код совпадает, дешифровка не совпадает, найдено совпадение в фирме " );
        break;
      case SR_SAME_DECODING_______SIMILAR_CODE___________DIFF_FIRM :
        result = codec->toUnicode( "Дешифровка совпадает, фирма не совпадает, найдено совпадение в коде " );
        break;
      case SR_SAME_CODE_DECODING__SIMILAR________________DIFF_FIRM :
        result = codec->toUnicode( "Код и дешифровка совпадают, фирма не совпадает " );
        break;
      case SR_SAME_CODE___________SIMILAR________________DIFF_FIRM_DECODING :
        result = codec->toUnicode( "Код совпадает, фирма и дешифровка не совпадают " );
        break;
      case SR_SAME________________SIMILAR_CODE___________DIFF_FIRM_DECODING :
        result = codec->toUnicode( "Фирма и дешифровка не совпадают, найдено совпадение в коде " );
        break;
      case SR_SAME________________SIMILAR_CODE_DECODING__DIFF_FIRM :
        result = codec->toUnicode( "Фирма не совпадает, найдено совпадение с коде и расшифровке " );
        break;
      case SR_SAME_CODE_FIRM______SIMILAR________________DIFF_DECODING :
        result = codec->toUnicode( "Код и фирма совпадают, дешифровка не совпадает " );
        break;
      case SR_SAME________________SIMILAR_DECODING_______DIFF_CODE_FIRM :
        result = codec->toUnicode( "Код и фирма не совпадают, найдено совпадение в дешифровке " );
        break;
      case SR_SAME_DECODING_______SIMILAR________________DIFF_CODE_FIRM :
        result = codec->toUnicode( "Дешифровка совпадает, код и фирма не совпадают " );
        break;
      case SR_SAME_FIRM___________SIMILAR_DECODING_______DIFF_CODE :
        result = codec->toUnicode( "Фирма совпадает, код не совпадает, найдено совпадение в дешифровке " );
        break;
      case SR_SAME_FIRM_DECODING__SIMILAR________________DIFF_CODE :
        result = codec->toUnicode( "Фирма и дешифровка совпадают, код не совпадает " );
        break;
      case SR_SAME_DECODING_______SIMILAR_FIRM___________DIFF_CODE :
        result = codec->toUnicode( "Дешифровка совпадает, код не совпадает, найдено совпадение в фирме " );
        break;
      case SR_SAME________________SIMILAR_DECODING_FIRM__DIFF_CODE :
        result = codec->toUnicode( "Код не совпадает, найдено совпадение в фирме и дешифровке " );
        break;
      case SR_SAME_FIRM___________SIMILAR_CODE_DECODING__DIFF_:
        result = codec->toUnicode( "Фирма совпадает, найдено совпадение в коде и расшифровке " );
        break;
      case SR_SAME_FIRM___________SIMILAR_CODE___________DIFF_DECODING :
        result = codec->toUnicode( "Фирма совпадает, дешифровка не совпадает, найдено совпадение в коде " );
        break;
      case SR_SAME_DECODING_______SIMILAR_CODE_FIRM______DIFF_:
        result = codec->toUnicode( "Дешифровка совпадает, найдено совпадение в коде и фирме " );
        break;
      case SR_SAME________________SIMILAR_CODE_FIRM______DIFF_DECODING :
        result = codec->toUnicode( "Дешифровка не совпадает, найдено совпадение в коде и фирме " );
        break;
      case SR_SAME________________SIMILAR_CODE_FIRM_DECODING__DIFF_ :
        result = codec->toUnicode( "Найдено совпадение в коде, фирме, дешифровке " );
        break;
      case SR_DIFFERENT :
        result = codec->toUnicode( "Сходство не найдено" );
        break;
      default :
        result = codec->toUnicode( "UNSUPPORTED");
    };

  result += "\n" + similarity::Decoding_Similarity_result ( SR_SS.SS );

  return result;
}

