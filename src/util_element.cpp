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
#include "util_element.h"

#define LOG ""

bool reading_do_file ( const QString &file_name, QVector<do_element> &do_records )
{
  QFile     *do_file;
  QFileInfo fi ( file_name );

  do_file = new QFile( file_name );

  QTextCodec  *codec = QTextCodec::codecForName( "Windows-1251" );
  QString     PathToDoFile = fi.absolutePath();

  if ( !do_file->open(QIODevice::ReadOnly | QIODevice::Text) )
  {
    *plog << codec->toUnicode( "ERROR: Не могу открыть \"" ) << file_name << codec->toUnicode( "\" файл;" ) << endl;
    return false;
  }

  *plog << codec->toUnicode( "Открываем \"" ) << file_name << codec->toUnicode( "\" файл;" ) << endl;
  {
    QString do_file_record;

    *plog << endl;
    *plog << endl;
    *plog << codec->toUnicode( "#########################################################################" ) << endl;
    *plog << codec->toUnicode( "###           Файлы для обработки (перечислены в *.do - файле         ###" ) << endl;
    *plog << codec->toUnicode( "#########################################################################" ) << endl;
    *plog << codec->toUnicode( "### Справочная информация :                                           ###" ) << endl;
    *plog << codec->toUnicode( "###                                                                   ###" ) << endl;
    *plog << codec->toUnicode( "##################### !!!! codepage 1251!!!! ############################" ) << endl;
    *plog << codec->toUnicode( "#####   Файл соответствий отчетов (*.bom - файлов) с                #####" ) << endl;
    *plog << codec->toUnicode( "#####          децимальными номерами,   имеет расширение *.do       #####" ) << endl;
    *plog << codec->toUnicode( "#########################################################################" ) << endl;
    *plog << codec->toUnicode( "# Предназначен для определения соответствий между децимальными номерами #" ) << endl;
    *plog << codec->toUnicode( "# блоков и соответствующих им файлов отчётов (*.bom).                   #" ) << endl;
    *plog << codec->toUnicode( "# Формат файла:                                                         #" ) << endl;
    *plog << codec->toUnicode( "# \"путь_до_файла/имя_файла.bom\"  \"децимальный_номер\"   \"формат_файла\"   #" ) << endl;
    *plog << codec->toUnicode( "# -----------------------------  ---------------------  --------------- #" ) << endl;
    *plog << codec->toUnicode( "# |Если файл находится        |  |любая последова -  |  |может быть:  | #" ) << endl;
    *plog << codec->toUnicode( "# |в том же каталоге, что и   |  |тельность русских  |  |pcad         | #" ) << endl;
    *plog << codec->toUnicode( "# |VP_auto.do,                |  |букв, латинских    |  |ad           | #" ) << endl;
    *plog << codec->toUnicode( "# |то путь_до_файла =.        |  |букв, цифр,        |  |Это связано  | #" ) << endl;
    *plog << codec->toUnicode( "# | Путь и имя не должны      |  |знаков _ .         |  |с различным  | #" ) << endl;
    *plog << codec->toUnicode( "# |содержать русских букв и   |  |НЕ должно содержать|  |форматом     | #" ) << endl;
    *plog << codec->toUnicode( "# |пробелов                   |  |символы:           |  |.bom - файлов| #" ) << endl;
    *plog << codec->toUnicode( "# ----------------------------   |\"[, ~!@$%^&*()+|   |  --------------- #" ) << endl;
    *plog << codec->toUnicode( "#          |                     |\\{}:<>?]           |         |        #" ) << endl;
    *plog << codec->toUnicode( "#          |                     ---------------------         |        #" ) << endl;
    *plog << codec->toUnicode( "#          |                              |                    |        #" ) << endl;
    *plog << codec->toUnicode( "# \"./top_level.bom\"              \"АБВГ.768367.123\"          \"pcad\"      #" ) << endl;
    *plog << codec->toUnicode( "# \"./same_module1.bom\"           \"АБВГ.668374.050\"          \"ad\"        #" ) << endl;
    *plog << codec->toUnicode( "# \"./same_module2.bom\"           \"АБВГ.668374.060\"          \"ad\"        #" ) << endl;
    *plog << codec->toUnicode( "#                                                                       #" ) << endl;
    *plog << codec->toUnicode( "# Предполагается, что самая верхняя звпись соответствует верхнему       #" ) << endl;
    *plog << codec->toUnicode( "# уровню проекта.                                                       #" ) << endl;
    *plog << codec->toUnicode( "# Для корректного создания ведомости покупных, необходимо, чтобы внутри #" ) << endl;
    *plog << codec->toUnicode( "# файлов соблюдалась иерархия проекта (то есть в файле top_level.bom    #" ) << endl;
    *plog << codec->toUnicode( "# были строки с децимальными номерами АБВГ.668374.050 (с точностью до   #" ) << endl;
    *plog << codec->toUnicode( "# буквы, причём буква о-русская и буква о-латинская - это разные буквы  #" ) << endl;
    *plog << codec->toUnicode( "# и т.п.) и, допустим АБВГ.668374.060. РЕКУРСИИ НЕ ДОПУСКАЕТСЯ, то есть #" ) << endl;
    *plog << codec->toUnicode( "# нельзя, чтобы одновременно файл same_module1.bom содержал в качестве  #" ) << endl;
    *plog << codec->toUnicode( "# включения АБВГ.668374.060, а файл same_module2.bom содержал в качестве#" ) << endl;
    *plog << codec->toUnicode( "# включения АБВГ.668374.050. Или же файл same_module1.bom содержал      #" ) << endl;
    *plog << codec->toUnicode( "# включения АБВГ.668374.050.                                            #" ) << endl;
    *plog << codec->toUnicode( "#                                                                       #" ) << endl;
    *plog << codec->toUnicode( "# Знак # считается комментарием                                         #" ) << endl;
    *plog << codec->toUnicode( "#                                                                       #" ) << endl;
    *plog << codec->toUnicode( "# Нужно следить, чтобы кодовая страница данного файла была cp1251       #" ) << endl;
    *plog << codec->toUnicode( "#########################################################################" ) << endl;
    *plog << endl;
    *plog << LOG << "START {" << endl;

    while ( !do_file->atEnd() )
    {
      do_file_record = codec->toUnicode( do_file->readLine() );
      if ( (do_file_record.indexOf(".") != -1) && (do_file_record.indexOf("#") == -1) ) //прокидываем незначащие строчки (где нет точек или присутствуют  символы комментария)
      {
        do_element  temp_do;
        if ( temp_do.do_element_soft_check(PathToDoFile, do_file_record) )
        {
          do_records.push_back( temp_do );
        }
      }
    }

    do_file->close();

    QVector<do_element>::iterator iter;
    for ( iter = do_records.begin(); iter < do_records.end(); iter++ )
    {
      *plog << iter->get_record() << endl;
    }

    *plog << LOG << "} END " << endl;
  }

  delete do_file;
  return true;
}

bool reading_bom_files ( QVector<do_element> &do_records, QVector<QVector<element> > &tables )
{
  QFile         *bom_file;
  QString       bom_file_record;
  QString       detimal;
  enum bom_type bom_type_;
  QVector<do_element>::iterator iter;
  QTextCodec  *codec = QTextCodec::codecForName( "Windows-1251" );

  *plog << endl;
  *plog << endl;
  *plog << codec->toUnicode( "#########################################################################" ) << endl;
  *plog << codec->toUnicode( "###      Считываем содержимое *.bom - файлов, перечисленных выше      ###" ) << endl;
  *plog << codec->toUnicode( "#########################################################################" ) << endl;
  *plog << codec->toUnicode( "### Справочная информация :                                           ###" ) << endl;
  *plog << codec->toUnicode( "###                                                                   ###" ) << endl;
  *plog << codec->toUnicode( "###    Как показывает практика, недостаточно указывать на             ###" ) << endl;
  *plog << codec->toUnicode( "### принципиальной схеме только покупные изделия, обозначенные        ###" ) << endl;
  *plog << codec->toUnicode( "### позиционными обозначениями. Существуют покупные изделия, которые  ###" ) << endl;
  *plog << codec->toUnicode( "### не фигурируют в схеме, но фигурируют в сборке и спецификации      ###" ) << endl;
  *plog << codec->toUnicode( "### (экраны, колпачки для соединителей и др..).                       ###" ) << endl;
  *plog << codec->toUnicode( "###    Если придерживаться классических правил, то как показывает     ###" ) << endl;
  *plog << codec->toUnicode( "### практика, легко забыть включить в заказ эти покупные изделия.     ###" ) << endl;
  *plog << codec->toUnicode( "### Удобно создавать малозаметное УГО для таких элементов (в виде     ###" ) << endl;
  *plog << codec->toUnicode( "### точки) и располагать их, скажем, в левом нижнем углу первой       ###" ) << endl;
  *plog << codec->toUnicode( "### страницы схемы на рамке. В этом случае, элементы фигурируют       ###" ) << endl;
  *plog << codec->toUnicode( "### в *.bom - файлах.                                                 ###" ) << endl;
  *plog << codec->toUnicode( "###    Существует и обратный случай, когда элемент на схеме не        ###" ) << endl;
  *plog << codec->toUnicode( "### является покупным (паяные контакты, контрольные точки). В этом    ###" ) << endl;
  *plog << codec->toUnicode( "### случае, хоть элемент и фигурирует в *.bom - файле, но не должен   ###" ) << endl;
  *plog << codec->toUnicode( "### проявляться в перечне, спецификации, ведомости покупных.          ###" ) << endl;
  *plog << codec->toUnicode( "###    Для выше приведённых случаев, зарезервированы следующие        ###" ) << endl;
  *plog << codec->toUnicode( "### буквосочетания позиционных обозначений:                           ###" ) << endl;
  *plog << codec->toUnicode( "### \"KT\" - Контрольные точки (так обозначаются в некоторых            ###" ) << endl;
  *plog << codec->toUnicode( "###      нормоконтролях) не фигурируют в текстовых документах;        ###" ) << endl;
  *plog << codec->toUnicode( "### \"0\" - \"9\" (позиционные обозначения, начинающиеся с цифры) -       ###" ) << endl;
  *plog << codec->toUnicode( "###      Паяные контакты не фигурируют в текстовых документах;        ###" ) << endl;
  *plog << codec->toUnicode( "### \"REF\", \"REF_P\", \"REF_G\"  - Реперные знаки (приходится             ###" ) << endl;
  *plog << codec->toUnicode( "###      указывать на схемах, иначе P-CAD ругается при                ###" ) << endl;
  *plog << codec->toUnicode( "###      проверке нетлистов) не фигурируют в текстовых документах;    ###" ) << endl;
  *plog << codec->toUnicode( "### \"UNUZED\" - Не фигурируют в текстовых документах                   ###" ) << endl;
  *plog << codec->toUnicode( "### \"COMPO\" - Под элементы, которые должны фигурировать в             ###" ) << endl;
  *plog << codec->toUnicode( "###      спецификации и ведомости покупных, и не должны фигурировать  ###" ) << endl;
  *plog << codec->toUnicode( "###      в перечне элементов (экраны, колпачки, радиаторы, крепёж..). ###" ) << endl;
  *plog << "#########################################################################" << endl;
  *plog << endl;
  *plog << LOG << "START {" << endl;

  for ( iter = do_records.begin(); iter < do_records.end(); iter++ )
  {
    QVector<element>  temp_ls;
    bom_file = new QFile( iter->get_File_Name() );
    if ( !bom_file->open(QIODevice::ReadOnly | QIODevice::Text) )
    {
      *plog << endl;
      *plog << "-------------------------------------------------------------------------" << endl;
      *plog << LOG << codec->toUnicode( "ERROR: Не могу открыть \"" ) << iter->get_File_Name() << codec->toUnicode( "\" файл;" ) << endl;
      return false;
    }

    *plog << endl;
    *plog << "-------------------------------------------------------------------------" << endl;
    *plog << LOG << codec->toUnicode( "Открываем \"" ) << iter->get_File_Name() << codec->toUnicode( "\" файл;" ) << endl;
    while ( !bom_file->atEnd() )
    {
      bom_file_record = codec->toUnicode( bom_file->readLine() );
      detimal = iter->get_Detimal_Number();
      bom_type_ = iter->get_bom_type();

      element temp_el ( &bom_file_record, &detimal, &bom_type_ );
      if ( temp_el.removable() == false )
      {
        temp_ls.push_back( temp_el );
      }

      if ( temp_el.removable() == true )
      {
        *plog << LOG << codec->toUnicode( "   Удаляем элемент :  " ) << temp_el.get_record2() << endl;
      }
    }

    *plog << LOG << codec->toUnicode( "Закрываем \"" ) << iter->get_File_Name() << codec->toUnicode( "\" файл;" ) << endl;
    bom_file->close();
    delete bom_file;
    tables.push_back( temp_ls );
  }

  *plog << LOG << "} END " << endl;

  *plog << endl;
  *plog << endl;
  *plog << codec->toUnicode( "#########################################################################" ) << endl;
  *plog << codec->toUnicode( "###      Перечисленные ниже элементы будут подвергнуты сортировке     ###" ) << endl;
  *plog << codec->toUnicode( "#########################################################################" ) << endl;
  *plog << endl;
  *plog << LOG << "START {" << endl;

  for ( QVector < QVector<element> >::iterator current_table = tables.begin(); current_table < tables.end(); current_table++ )
  {
    QVector<element>::iterator  iter = current_table->begin();
    *plog << LOG << codec->toUnicode( "Элементы, входящие в сборку : " ) << iter->get_Detimal_Number() << endl;
    for ( ; iter < current_table->end(); iter++ )
    {
      *plog << "   " << iter->get_record2() << endl;
    }

    *plog << endl;
  }

  *plog << LOG << "} END " << endl;

  return true;
}

