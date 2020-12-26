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
#include "true_elements.h"

#define LOG ""



int true_element::reading_true_element_file_lua ( QVector<QVector<element> > &true_tables, QString &PathToResDir, QString &PathToExecDir )
{
  QFile   *true_file;
  int result;

  *plog << endl;
  *plog << endl;

  // Qstring file_name_extended = this->PathToResDir + "/elements_full_attr.lua";
  //QString script_name = PathToResDir + "true_elements_reader.lua";
  QString script_name = PathToExecDir + "/true_elements_reader.lua";
  int       err;
  lua_State *L = luaL_newstate();
  luaL_openlibs( L );
  
  err = luaL_loadfile( L, script_name.toLocal8Bit().data() );
  if ( err != LUA_OK )
  {
    *plog << "ERROR in file '" << script_name << "' :" << endl;
    *plog << codec->toUnicode( lua_tostring(L, -1) ) << endl;
    lua_close( L );
    return -1;
  }
  
  err = lua_pcall( L, 0, 0, 0 );
  if ( err != LUA_OK )
  {
    *plog << "ERROR in file '" << script_name << "' :" << endl;
    *plog << codec->toUnicode( lua_tostring(L, -1) ) << endl;
    lua_close( L );
    return -3;
  }




  lua_getglobal(L, "exec");
  lua_pushstring( L, PathToResDir.toLocal8Bit().data() );
  lua_pushstring( L, " " );
  if( lua_pcall(L, 2, 2, 0) != LUA_OK )
  {
    *plog << "ERROR : in 'exec' \n" << endl;
    *plog << codec->toUnicode( lua_tostring(L, -1) ) << endl;
    return -2;
  }


  if(! lua_isinteger(L, -2))
  {
    // printf("ERROR : in return type from 'get_CAD()'  '%s'\n", lua_tostring(L, -1));
     return -3;
  }
  result = (int)lua_tointeger(L, -2);

/*
  if(! lua_isinteger(L, -1))
  {
     printf("ERROR : in return type from 'get_CAD()'  '%s'\n", lua_tostring(L, -1));
     return -4;
  }
*/






  lua_getglobal(L, "get_cell");
  lua_pushinteger(L, 1);
  if( lua_pcall(L, 1, 1, 0) != LUA_OK )
  {
    *plog << "ERROR : in 'get_cell' \n" << endl;
    *plog << codec->toUnicode( lua_tostring(L, -1) ) << endl;
    return -2;
  }


  if(! lua_isstring(L, -1))
  {
    // printf("ERROR : in return type from 'get_CAD()'  '%s'\n", lua_tostring(L, -1));
     return -3;
  }
  const char  *log = lua_tostring( L, -1 );

 *plog << ">>>>>>>>>>>>>>3>>>>>>>>>>>>>>>>" << log << endl;




  
  lua_close( L );
  
  
  return 0;
}


int true_element::reading_true_element_file ( QVector<QVector<element> > &true_tables, QString &PathToResDir )
{
  QFile   *true_file;

  *plog << endl;
  *plog << endl;

  QString file_name = PathToResDir + "/true_elements.txt";

  true_file = new QFile( file_name );

  if ( !true_file->open(QIODevice::ReadOnly | QIODevice::Text) )
  {
    *plog << codec->toUnicode( "ERROR: Не могу открыть \"" ) << file_name << codec->toUnicode( "\" файл;" ) << endl;
    return -1;
  }

  *plog << codec->toUnicode( "Открываем \"" ) << file_name << codec->toUnicode( "\" файл;" ) << endl;


  QString true_file_record;

  true_tables.clear();

  QVector<element>  support_table;
  QVector<element>  temp_table;
  int file_line_num = 1;

  while ( !true_file->atEnd() )
  {
    true_file_record = true_file->readLine();
    true_file_record = true_file_record.trimmed();

    if ( true_file_record.indexOf("#") != 0 ) //прокидываем незначащие строчки (начинающиеся с '#')
    {
      if ( true_file_record.indexOf("@") == 0 )
      {
        if ( temp_table.size() > 0 )
        {
          true_tables.push_back( temp_table );
          temp_table.clear();
        }
      }
      else
      {
        QStringList lst = true_file_record.split( "@", QString::SkipEmptyParts );

        if ( lst.size() >= 2 )
        {
          QString Value = lst.at( 0 );
          QString Ref = lst.at( 1 );
          Value.remove( QRegExp("^[-|+]?") );

          element temp ( Value, Ref, file_line_num );
          temp_table.push_back( temp );
          support_table.push_back( temp );
        }
      }
    }
    file_line_num++;
  }

  if ( temp_table.size() > 0 )
  {
    true_tables.push_back( temp_table );
    temp_table.clear();
  }

  true_file->close();

  *plog << codec->toUnicode( "#########################################################################" ) << endl;
  *plog << codec->toUnicode( "###  Прочитаны  из файла '") << file_name << "'" << endl;
  *plog << codec->toUnicode( "#########################################################################" ) << endl;
  *plog << endl;
  *plog << "START {" << endl;

  QVector<QVector<element> >::iterator  iter_true_tables;

  for ( iter_true_tables = true_tables.begin(); iter_true_tables < true_tables.end(); iter_true_tables++ )
  {
    *plog << "-------------------------------------------" << endl;
    QVector<element>::iterator  iter;

     iter = iter_true_tables->begin();
     *plog << "+" << iter->get_record2() << endl;
     iter++;
    for ( ; iter < iter_true_tables->end(); iter++ )
    {
      *plog << "-" << iter->get_record2() << endl;
    }
  }

  *plog << "} END " << endl;



    //QString get_record7 ( void );


  *plog << endl;
  *plog << endl;
  *plog << codec->toUnicode( "#########################################################################" ) << endl;
  *plog << codec->toUnicode( "###   Проверка эталонного файла '") << file_name << "'" << endl;
  *plog << codec->toUnicode( "###   на схожие записи (на ошибку, вкравшуюся в эталонный файл)       ###" ) << endl;
  *plog << codec->toUnicode( "#########################################################################" ) << endl;
  *plog << endl;
  *plog << "START {" << endl;


  QVector<QVector<element> >::iterator  iter_tables_first;
  QVector<QVector<element> >::iterator  iter_tables_second;

  for ( iter_tables_first = true_tables.begin(); iter_tables_first < true_tables.end(); iter_tables_first++ )
  {
    iter_tables_second = iter_tables_first + 1;
    while( iter_tables_second < true_tables.end() )
    {
      for (QVector<element>::iterator  iter_first = iter_tables_first->begin(); iter_first < iter_tables_first->end(); iter_first++ )
      {
        for (QVector<element>::iterator  iter_second = iter_tables_second->begin(); iter_second < iter_tables_second->end(); iter_second++ )
        {
            class element::sr_ss result = element::is_Similarity( *iter_first, *iter_second );

            if ( (result.SR != element::SR_SAME) && (result.SR != element::SR_DIFFERENT) )
            {
              *plog << "WARNING : ";
              *plog << element::Decoding_Similarity_result ( result );
              *plog << " :" << endl;
              *plog << "    ";
              *plog << iter_first->get_record7() << endl;
              *plog << "    ";
              *plog << iter_second->get_record7() << endl << endl;
            }
          }
        }
        iter_tables_second++;
      }
    }

  *plog << "} END " << endl;


  delete true_file;
  return 0;
}




void true_element::similarity_elements ( QVector<element> &ls1,  QVector<QVector<element> > &etalon)
{
  QVector<element> target;

  for ( QVector<element>::iterator iter = ls1.begin(); iter < ls1.end(); iter++ )
  {
    target.push_back(*iter);
  }


  QVector<element>::iterator  iter_target;
  QVector<QVector<element> >::iterator  iter_etalon;
  QVector<element>::iterator  iter_etalon_element;

  for ( iter_etalon = etalon.begin(); iter_etalon < etalon.end(); iter_etalon++ )
  {
    iter_target = target.begin();

    while (iter_target < target.end() )
    {
      if (element::eq_Ref_Type_Code_Decoding_Firm ( *(iter_etalon->begin()), *iter_target ) == true)
      {
        target.erase(iter_target);
      }
      else
      {
        iter_target++;
      }
    }
  }

  *plog << endl;
  *plog << codec->toUnicode( "#########################################################################" ) << endl;
  *plog << codec->toUnicode( "###         Сравнение элементов c эталонным списком                   ###" ) << endl;
  *plog << codec->toUnicode( "#########################################################################" ) << endl;
  *plog << codec->toUnicode( "### Справочная информация :                                           ###" ) << endl;
  *plog << codec->toUnicode( "###                                                                   ###" ) << endl;
  *plog << codec->toUnicode( "###    Эталонный список содержит помимо непосредственно правильных    ###" ) << endl;
  *plog << codec->toUnicode( "### записей названий элементов, ещё и ранние версии записи этих       ###" ) << endl;
  *plog << codec->toUnicode( "### элементов. Ниже порводится проверка на совпадение со старыми      ###" ) << endl;
  *plog << codec->toUnicode( "### записями. Это сделано для того чтобы отличить название нового     ###" ) << endl;
  *plog << codec->toUnicode( "### введённого элемента от неправильной записи старого элемента.      ###" ) << endl;
  *plog << codec->toUnicode( "#########################################################################" ) << endl;
  *plog << LOG << "START {" << endl;

  for ( iter_etalon = etalon.begin(); iter_etalon < etalon.end(); iter_etalon++ )
  {
    iter_target = target.begin();

    while (iter_target < target.end() )
    {
      iter_etalon_element = iter_etalon->begin();

      while(iter_etalon_element < iter_etalon->end() )
      {
        if( element::eq_Ref_Type_Code_Decoding_Firm( *iter_etalon_element, *iter_target ) == true )
        {
          *plog << "--------------------------------------------" << endl;
          *plog << "WARNING : ";
          *plog << codec->toUnicode("Найдено полное совпадение");
          *plog << " :" << endl;
          *plog << "    ";
          *plog << iter_target->get_record2() << endl;
          *plog << "    ";
          *plog << iter_etalon_element->get_record2() << endl;
          *plog << codec->toUnicode("Таким образом, элемент записанный выше, ПОЛНОСТЬЮ СООТВЕТСТВУЕТ элементу :") << endl;
          *plog << "    ";
          *plog << iter_etalon->begin()->get_record2() << endl;
          *plog << iter_etalon->begin()->get_record9() << endl << endl;
          iter_target->set_removable();
         }
        iter_etalon_element++;
      }
      iter_target++;
    }
  }

  *plog << LOG << "} END " << endl;


  QVector<element>::iterator iter = target.begin();

  while (  iter < target.end() )
  {
    if ( iter->removable() == true )
    {
      target.erase(iter);
    }
    else
    {
      iter++;
    }
  }








  *plog << endl;
  *plog << LOG << "START {" << endl;

  for ( iter_etalon = etalon.begin(); iter_etalon < etalon.end(); iter_etalon++ )
  {
    iter_target = target.begin();

    while (iter_target < target.end() )
    {
      iter_etalon_element = iter_etalon->begin();

      while(iter_etalon_element < iter_etalon->end() )
      {
        class element::sr_ss result = element::is_Similarity( *iter_etalon_element, *iter_target );

        if ( (result.SR != element::SR_SAME) && (result.SR != element::SR_DIFFERENT) )
        {
          *plog << "--------------------------------------------" << endl;
          *plog << "WARNING : ";
          *plog << element::Decoding_Similarity_result ( result );
          *plog << " :" << endl;
          *plog << "    ";
          *plog << iter_target->get_record2() << endl;
          *plog << "    ";
          *plog << iter_etalon_element->get_record2() << endl;
          *plog << codec->toUnicode("Таким образом, элемент записанный выше, скорее всего соответствует элементу :") << endl;
          *plog << iter_etalon->begin()->get_record2() << endl << endl;
        }
        iter_etalon_element++;
      }
      iter_target++;
    }
  }

  *plog << LOG << "} END " << endl;













  *plog << endl;
  *plog << endl;
  *plog << codec->toUnicode( "#########################################################################" ) << endl;
  *plog << codec->toUnicode( "###         Список предположительно новых элементов                   ###" ) << endl;
  *plog << codec->toUnicode( "#########################################################################" ) << endl;
  *plog << codec->toUnicode( "### Справочная информация :                                           ###" ) << endl;
  *plog << codec->toUnicode( "###                                                                   ###" ) << endl;
  *plog << codec->toUnicode( "###    Данные элементы не обнаружились в эталонном списке ни в каком  ###" ) << endl;
  *plog << codec->toUnicode( "### виде. Таким образом, или это новые элементы или эталонный список  ###" ) << endl;
  *plog << codec->toUnicode( "### неполный...                                                       ###" ) << endl;
  *plog << codec->toUnicode( "#########################################################################" ) << endl;
  *plog << LOG << "START {" << endl;

  for ( iter = target.begin(); iter < target.end(); iter++ )
  {
    //*plog << iter->get_record2() << endl;
    *plog << "@" << endl;
    *plog << "+" << iter->get_record6() << endl;
  }

  *plog << LOG << "} END " << endl;
}















