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
#ifndef TEXT_NAME_H
  #define TEXT_NAME_H

  #include <QtCore/QTextCodec>
  #include <QtCore/QString>
  #include <QtCore/QTextStream>
  #include <QtCore/QFile>
  #include <QtCore/QIODevice>
  #include <QtCore/QVector>
  #include <QtCore/QDir>
  #include <QtCore/QFileInfo>
  #include <QCoreApplication>
  #include <QApplication>
  #include <QRegExp>


extern QTextStream  *plog;
extern QTextCodec   *codec;


class text_name
{ public:
    text_name ()
    {

    }

    text_name ( QString Ref, QString Name, QString Names, QString Group_Name_one, QString Group_Name )
    {
      this->Ref = Ref;
      this->Name = Name;
      this->Names = Names;
      this->Group_Name_one = Group_Name_one;
      this->Group_Name = Group_Name;
    }

    text_name ( const text_name &tn )           //Конструктор копирования
    {
      Ref = tn.Ref;
      Name = tn.Name;
      Names = tn.Names;
      Group_Name_one = tn.Group_Name_one;
      Group_Name = tn.Group_Name;
    }

    text_name operator= ( const text_name &tn ) // Оператор присваивания
    {
      Ref = tn.Ref;
      Name = tn.Name;
      Names = tn.Names;
      Group_Name_one = tn.Group_Name_one;
      Group_Name = tn.Group_Name;
      return *this;
    }

    QString get_Ref ( void )
    {
      return Ref;
    }

    QString get_Name ( void )
    {
      return Name;
    }

    QString get_Names ( void )
    {
      return Names;
    }

    QString get_Group_Name_one ( void )
    {
      return Group_Name_one;
    }

    QString get_Group_Name ( void )
    {
      return Group_Name;
    }

    QString get_full_record ( void )
    {
      return get_Ref() + "\t\t\t" + get_Name() + "\t\t\t" + get_Names() + "\t\t\t" + get_Group_Name_one() + "\t\t\t" + get_Group_Name();
    }

    static class text_name create ( QString Ref, QString Name, QString Names, QString Group_Name_one, QString Group_Name )
    {
      class text_name temp ( Ref, Name, Names, Group_Name_one, Group_Name );
      return temp;
    }

    static int reading_names_file ( QVector<text_name> &Text_Names, QString &PathToResDir)
    {
      QFile   *names_file;

      *plog << endl;
      *plog << endl;
      QString file_name = PathToResDir + "/names.txt";

      names_file = new QFile( file_name );

      if ( !names_file->open(QIODevice::ReadOnly | QIODevice::Text) )
      {
        *plog << codec->toUnicode( "ERROR: Не могу открыть \"" ) << file_name << codec->toUnicode( "\" файл;" ) << endl;
        return -1;
      }

      *plog << codec->toUnicode( "Открываем \"" ) << file_name << codec->toUnicode( "\" файл;" ) << endl;

      QString names_file_record;

      while ( !names_file->atEnd() )
      {
        names_file_record = names_file->readLine();
        names_file_record = names_file_record.trimmed();
        if ( names_file_record.indexOf("#") != 0 )  //прокидываем незначащие строчки (начинающиеся с '#')
        {
          QStringList lst = names_file_record.split( "|", QString::SkipEmptyParts );
          if ( lst.size() > 4 )
          {
            Text_Names.push_back( text_name::create(lst.at(0), lst.at(1), lst.at(2), lst.at(3), lst.at(4)) );
          }
          else if ( lst.size() > 3 )
          {
            Text_Names.push_back( text_name::create(lst.at(0), lst.at(1), lst.at(2), lst.at(3), "") );
          }
        }
      }

      names_file->close();

      *plog << codec->toUnicode( "#########################################################################" ) << endl;
      *plog << codec->toUnicode( "###  Прочитаны из файла '") << file_name << "'" << endl;
      *plog << codec->toUnicode( "#########################################################################" ) << endl;
      *plog << endl;
      *plog << codec->toUnicode( "Ref\t\t\tЕдинств.число\t\t\tМнож.число\t\t\tНазвание группы един.ч.\t\t\tНазвание группы множ.ч." ) << endl;
      *plog << endl;
      *plog << "START {" << endl;

      QVector<text_name>::iterator  iter;
      for ( iter = Text_Names.begin(); iter < Text_Names.end(); iter++ )
      {
        *plog << iter->get_full_record() << endl;
      }

      *plog << "} END " << endl;

      delete names_file;
      return 0;
    }
  private:
    QString Ref;
    QString Name;
    QString Names;
    QString Group_Name_one;
    QString Group_Name;
};


#endif // TEXT_NAME_H

