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
#include "cmdline.h"
#include "element.h"
#include "do_element.h"
#include "vp.h"

#include "sp.h"
#include "pilot.h"
#include "util_element.h"

#include "pcad_per_for_doc_includes.h"
#include "pcad_per_for_sch.h"
#include "per_to_log.h"

#include "true_elements.h"



#define LOG ""

QTextStream         *plog;
QTextCodec          *codec;

text_width          *Text_Width;

QVector<text_name>  Text_Names;
QVector<firm_name>  Firm_Names;
QVector<QVector<element> >  true_tables;

//Функция удаления папки
int removeFolder ( QDir &dir )
{
  int         res = -1;

  //Получаем список каталогов
  QStringList lstDirs = dir.entryList( QDir::Dirs | QDir::AllDirs | QDir::NoDotAndDotDot );

  //Получаем список файлов
  QStringList lstFiles = dir.entryList( QDir::Files );

  //Удаляем файлы
  foreach( QString entry, lstFiles )
  {
    QString entryAbsPath = dir.absolutePath() + "/" + entry;
    QFile::setPermissions( entryAbsPath, QFile::ReadOwner | QFile::WriteOwner );
    if ( !QFile::remove(entryAbsPath) )
    {
      std::cout << "ERROR:   " << "The file '" << entryAbsPath.toStdString() << "' is not deleted." << std::endl;
      return -2;
    }
  }

  //Для папок делаем рекурсивный вызов
  foreach( QString entry, lstDirs )
  {
    QString entryAbsPath = dir.absolutePath() + "/" + entry;
    QDir    dr ( entryAbsPath );
    if ( removeFolder(dr) != 0 )
    {
      std::cout << "ERROR:   " << "The directory '" << dr.absolutePath().toStdString() << "' is not deleted." << std::endl;
      return -3;
    }
  }

  //Удаляем обрабатываемую папку
  if ( !QDir().rmdir(dir.absolutePath()) )
  {
    res = 0;
  }
  else
  {
    std::cout << "ERROR:   " << "The directory '" << dir.absolutePath().toStdString() << "' is not deleted." << std::endl;
    return -3;
  }

  return res;
}

//Функция удаления содержимого папки
int clearFolder ( QDir &dir )
{
  int         res = -1;

  //Получаем список каталогов
  QStringList lstDirs = dir.entryList( QDir::Dirs | QDir::AllDirs | QDir::NoDotAndDotDot );

  //Получаем список файлов
  QStringList lstFiles = dir.entryList( QDir::Files );

  //Удаляем файлы
  foreach( QString entry, lstFiles )
  {
    QString entryAbsPath = dir.absolutePath() + "/" + entry;
    QFile::setPermissions( entryAbsPath, QFile::ReadOwner | QFile::WriteOwner );
    if ( !QFile::remove(entryAbsPath) )
    {
      std::cout << "ERROR:   " << "The file '" << entryAbsPath.toStdString() << "' is not deleted." << std::endl;
      return -2;
    }
  }

  //Для папок делаем рекурсивный вызов
  foreach( QString entry, lstDirs )
  {
    QString entryAbsPath = dir.absolutePath() + "/" + entry;
    QDir    dr ( entryAbsPath );
    if ( removeFolder(dr) != 0 )
    {
      std::cout << "ERROR:   " << "The directory '" << dr.absolutePath().toStdString() << "' is not deleted." << std::endl;
      return -1;
    }
  }

  res = 0;

  return res;
}

int main ( int argc, char *argv[] )
{
  struct gengetopt_args_info  ai;
  if ( cmdline_parser(argc, argv, &ai) != 0 )
  {
    std::cout << "if (cmdline_parser (argc, argv, &ai) != 0)" << std::endl;
    return -1;
  }

  QString         file_name;
  QVector<element>  ls1;
  QVector<QVector<element> >  tables;
  QVector<do_element>         do_records;
  codec = QTextCodec::codecForName( "Windows-1251" );

  // QString path = QDir::currentPath();
  QString         Number_of_Copy_str;
  int         Number_of_Copy;
  QFile         *log_file;
  QCoreApplication  a ( argc, argv );   //Для обределения пути к самому себе
  QApplication      app ( argc, argv ); //Для QFontMetrics

  //std::cout << ">>>>>>>>>>>>>:" << QCoreApplication::applicationFilePath().toStdString() << std::endl;
  file_name = codec->toUnicode( ai.filename_arg );

  if ( ai.copies_given )
  {
    Number_of_Copy_str = ai.copies_orig;
  }
  else
  {
    Number_of_Copy_str = 1;
  }

  QFileInfo fi ( file_name );
  QString   PathToDoFile = fi.absolutePath();

  // std::cout << "-->>" << PathToDoFile.toStdString() << std::endl;
  QString   PathToOutDir;

  if ( ai.out_folder_given )
  {
    PathToOutDir = codec->toUnicode( ai.out_folder_arg );
  }
  else
  {
    PathToOutDir = PathToDoFile + "/../out";

    //std::cout << "         " << "PathToOutDir '" << PathToOutDir.toStdString() << "'"<< std::endl;
  }

  QDir  dir ( PathToOutDir );
  if ( !dir.exists() )
  {
    std::cout << "WARNING: " << "Cannot find the output directory." << std::endl;
    if ( QDir().mkdir(PathToOutDir) )
    {
      std::cout << "         " << "The directory '" << PathToOutDir.toStdString() << "' is created." << std::endl;
    }
    else
    {
      std::cout << "ERROR:   " << "The directory '" << PathToOutDir.toStdString() << "' is not created." << std::endl;
      return -1;
    }
  }
  else
  {
    if ( clearFolder(dir) == 0 )
    {
      std::cout << "INFO:    " << "The directory '" << PathToOutDir.toStdString() << "' is cleared." << std::endl;
    }
    else
    {
      std::cout << "ERROR:   " << "The directory '" << PathToOutDir.toStdString() << "' is not cleared." << std::endl;
      return -1;
    }
  }

  log_file = new QFile( PathToOutDir + "/VP_auto_utf8.log" );
  if ( !log_file->open(QIODevice::WriteOnly | QIODevice::Text) )
  {
    std::cout << "ERROR:   " << "Error creating log file: '" << PathToDoFile.toStdString() << "'" << std::endl;
    return 1;
  }

  QTextStream log ( log_file );
  log.setCodec( "UTF-8" );

  plog = &log;





  QString   PathToResDir;

  if ( ai.res_folder_given )
  {
    PathToResDir = codec->toUnicode( ai.res_folder_arg );
  }
  else
  {
    PathToResDir = QCoreApplication::applicationDirPath() + "/../res";
  }


  //-------------------------------------------------------------------------------------------------
  if ( text_name::reading_names_file(Text_Names, PathToResDir) != 0 )
  {
    return -1;
  }
  //-------------------------------------------------------------------------------------------------

  if ( firm_name::reading_firms_file(Firm_Names, PathToResDir) != 0 )
  {
    return -1;
  }
  //-------------------------------------------------------------------------------------------------

  if ( true_element::reading_true_element_file(true_tables, PathToResDir) != 0 )
  {
    return -1;
  }
  //-------------------------------------------------------------------------------------------------




  {
    *plog << endl;

    bool  ok;

    Number_of_Copy = Number_of_Copy_str.toInt( &ok );

    if ( ok == false )
    {
      *plog << codec->toUnicode( "ERROR: Не могу конвертировать второй параметр в беззнаковое целое. Бланки заказа будут посчитаны на 1 экземпляр устройства" ) << endl;
      Number_of_Copy = 1;
    }
    else
    {
      if ( Number_of_Copy < 1 )
      {
        *plog << codec->toUnicode( "ERROR: Не могу конвертировать второй параметр в беззнаковое целое. Бланки заказа будут посчитаны на 1 экземпляр устройства" ) << endl;
        Number_of_Copy = 1;
      }
    }

    *plog << codec->toUnicode( "Бланки заказа будут посчитаны для количества экземпляров устройств, равного " ) << Number_of_Copy << endl;
    *plog << endl;
  }

  //-------------------------------------------------------------------------------------------------
  if ( !reading_do_file(file_name, do_records) )
  {
    return -1;
  }

  if ( !reading_bom_files(do_records, tables) )
  {
    return -1;
  }

  element::remove_blocks_and_adding_content( PathToOutDir, tables, ls1 );

  //-------------------------------------------------------------------------------------------------
#if 0
  for ( QVector < QVector<element> >::iterator current_table = tables.begin(); current_table != tables.end(); current_table++ )
  {
    class per_to_log  per ( PathToOutDir, current_table );
    per.generate();
  }
#endif

  //-------------------------------------------------------------------------------------------------
  Text_Width = new text_width( "GOST type B", 12, 0.23 );

  *plog << LOG << PathToOutDir << endl;

#if 1
  if ( codec->toUnicode(ai.per_style_orig) == codec->toUnicode("doc") )
  {
    for ( QVector < QVector<element> >::iterator current_table = tables.begin(); current_table < tables.end(); current_table++ )
    {
      class pcad_per_for_doc_includes per_for_doc ( PathToOutDir, current_table );
      per_for_doc.generate();
    }
  }
  else
  {
    for ( QVector < QVector<element> >::iterator current_table = tables.begin(); current_table < tables.end(); current_table++ )
    {
      class per per ( PathToOutDir, current_table );
      per.generate();
    }
  }
#endif
#if 1
  for ( QVector < QVector<element> >::iterator current_table = tables.begin(); current_table < tables.end(); current_table++ )
  {
    class sp  sp ( PathToOutDir, current_table );
    sp.generate();
  }
#endif
#if 1
  for ( QVector < QVector<element> >::iterator current_table = tables.begin(); current_table < tables.end(); current_table++ )
  {
    class pilot pilot ( PathToOutDir, current_table, Number_of_Copy );
    pilot.generate();
  }
#endif
#if 1
  class vp  rec ( PathToOutDir, &ls1, (*do_records.begin()).get_Detimal_Number(), Number_of_Copy );
  rec.generate();
  rec.create_file_for_excel();
  rec.create_file_for_order();
#endif
#if 1


  for ( QVector<element>::iterator iter = ls1.begin(); iter < ls1.end(); iter++ )
  {
    QVector<element>::iterator iter_second = iter;
    iter_second++;
    while ( iter_second < ls1.end() )
    {
       if (element::eq_Ref_Type_Code_Decoding_Firm ( *iter, *iter_second ) == true)
       {
         ls1.erase(iter_second);
       }
       else
       {
         iter_second++;
       }
    }
  }

  //  *plog << iter->get_record2() << endl;

  element::similarity_elements( ls1);
  element::similarity_firm ( ls1, Firm_Names );

  true_element::similarity_elements( ls1, true_tables);

#endif


  log_file->close();

  delete Text_Width;

  return EXIT_SUCCESS;
}

