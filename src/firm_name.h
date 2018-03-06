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
#ifndef FIRM_NAME_H
  #define FIRM_NAME_H

  #include <QtCore/QTextCodec>
  #include <QtCore/QString>
  #include <QtCore/QTextStream>
  #include <QtCore/QFile>
  #include <QtCore/QVector>
  #include <QtCore/QDir>
  #include <QtCore/QFileInfo>
  #include <QCoreApplication>
  #include <QApplication>
  #include <QRegExp>


extern QTextStream  *plog;
extern QTextCodec   *codec;


class firm_name
{ public:
    firm_name ()
    {

    }

    firm_name ( QString Firm )
    {
      this->Firm = Firm;
    }

    firm_name ( const firm_name &fn )           //Конструктор копирования
    {
      Firm = fn.Firm;
    }

    firm_name operator= ( const firm_name &fn ) // Оператор присваивания
    {
      Firm = fn.Firm;
      return *this;
    }

    QString get_Firm ( void )
    {
      return Firm;
    }

    static class firm_name create ( QString Firm)
    {
      class firm_name temp ( Firm );
      return temp;
    }

    static int reading_firms_file ( QVector<firm_name> &Firms_Names, QString &PathToResDir )
    {
      QFile   *firms_file;

      *plog << endl;
      *plog << endl;
      QString file_name = PathToResDir + "/firms.txt";

      firms_file = new QFile( file_name );

      if ( !firms_file->open(QIODevice::ReadOnly | QIODevice::Text) )
      {
        *plog << codec->toUnicode( "ERROR: Не могу открыть \"" ) << file_name << codec->toUnicode( "\" файл;" ) << endl;
        return -1;
      }

      *plog << codec->toUnicode( "Открываем \"" ) << file_name << codec->toUnicode( "\" файл;" ) << endl;

      QString firms_file_record;

      while ( !firms_file->atEnd() )
      {
        firms_file_record = firms_file->readLine();
        firms_file_record = firms_file_record.trimmed(); //Убрали проелы и табуляцию по краям

        if ( firms_file_record.indexOf("#") != 0 )  //прокидываем незначащие строчки (начинающиеся с '#')
        {
          if( firms_file_record.size() != 0 )
          {
            Firms_Names.push_back( firm_name::create(firms_file_record) );
          }
        }
      }

      firms_file->close();

      *plog << codec->toUnicode( "#############################################################################" ) << endl;
      *plog << codec->toUnicode( "###  Прочитаны утверждённые названия фирм из файла '") << file_name << "'" << endl;
      *plog << codec->toUnicode( "#############################################################################" ) << endl;
      *plog << endl;
      *plog << "START {" << endl;

      QVector<firm_name>::iterator  iter;
      for ( iter = Firms_Names.begin(); iter < Firms_Names.end(); iter++ )
      {
        *plog << iter->get_Firm() << endl;
      }

      *plog << "} END " << endl;

      delete firms_file;
      return 0;
    }
  private:
    QString Firm;
};


#endif // FIRM_NAME_H

