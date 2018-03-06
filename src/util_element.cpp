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
    *plog << codec->toUnicode( "ERROR: �� ���� ������� \"" ) << file_name << codec->toUnicode( "\" ����;" ) << endl;
    return false;
  }

  *plog << codec->toUnicode( "��������� \"" ) << file_name << codec->toUnicode( "\" ����;" ) << endl;
  {
    QString do_file_record;

    *plog << endl;
    *plog << endl;
    *plog << codec->toUnicode( "#########################################################################" ) << endl;
    *plog << codec->toUnicode( "###           ����� ��� ��������� (����������� � *.do - �����         ###" ) << endl;
    *plog << codec->toUnicode( "#########################################################################" ) << endl;
    *plog << codec->toUnicode( "### ���������� ���������� :                                           ###" ) << endl;
    *plog << codec->toUnicode( "###                                                                   ###" ) << endl;
    *plog << codec->toUnicode( "##################### !!!! codepage 1251!!!! ############################" ) << endl;
    *plog << codec->toUnicode( "#####   ���� ������������ ������� (*.bom - ������) �                #####" ) << endl;
    *plog << codec->toUnicode( "#####          ������������ ��������,   ����� ���������� *.do       #####" ) << endl;
    *plog << codec->toUnicode( "#########################################################################" ) << endl;
    *plog << codec->toUnicode( "# ������������ ��� ����������� ������������ ����� ������������ �������� #" ) << endl;
    *plog << codec->toUnicode( "# ������ � ��������������� �� ������ ������� (*.bom).                   #" ) << endl;
    *plog << codec->toUnicode( "# ������ �����:                                                         #" ) << endl;
    *plog << codec->toUnicode( "# \"����_��_�����/���_�����.bom\"  \"�����������_�����\"   \"������_�����\"   #" ) << endl;
    *plog << codec->toUnicode( "# -----------------------------  ---------------------  --------------- #" ) << endl;
    *plog << codec->toUnicode( "# |���� ���� ���������        |  |����� ��������� -  |  |����� ����:  | #" ) << endl;
    *plog << codec->toUnicode( "# |� ��� �� ��������, ��� �   |  |��������� �������  |  |pcad         | #" ) << endl;
    *plog << codec->toUnicode( "# |VP_auto.do,                |  |����, ���������    |  |ad           | #" ) << endl;
    *plog << codec->toUnicode( "# |�� ����_��_����� =.        |  |����, ����,        |  |��� �������  | #" ) << endl;
    *plog << codec->toUnicode( "# | ���� � ��� �� ������      |  |������ _ .         |  |� ���������  | #" ) << endl;
    *plog << codec->toUnicode( "# |��������� ������� ���� �   |  |�� ������ ���������|  |��������     | #" ) << endl;
    *plog << codec->toUnicode( "# |��������                   |  |�������:           |  |.bom - ������| #" ) << endl;
    *plog << codec->toUnicode( "# ----------------------------   |\"[, ~!@$%^&*()+|   |  --------------- #" ) << endl;
    *plog << codec->toUnicode( "#          |                     |\\{}:<>?]           |         |        #" ) << endl;
    *plog << codec->toUnicode( "#          |                     ---------------------         |        #" ) << endl;
    *plog << codec->toUnicode( "#          |                              |                    |        #" ) << endl;
    *plog << codec->toUnicode( "# \"./top_level.bom\"              \"����.768367.123\"          \"pcad\"      #" ) << endl;
    *plog << codec->toUnicode( "# \"./same_module1.bom\"           \"����.668374.050\"          \"ad\"        #" ) << endl;
    *plog << codec->toUnicode( "# \"./same_module2.bom\"           \"����.668374.060\"          \"ad\"        #" ) << endl;
    *plog << codec->toUnicode( "#                                                                       #" ) << endl;
    *plog << codec->toUnicode( "# ��������������, ��� ����� ������� ������ ������������� ��������       #" ) << endl;
    *plog << codec->toUnicode( "# ������ �������.                                                       #" ) << endl;
    *plog << codec->toUnicode( "# ��� ����������� �������� ��������� ��������, ����������, ����� ������ #" ) << endl;
    *plog << codec->toUnicode( "# ������ ����������� �������� ������� (�� ���� � ����� top_level.bom    #" ) << endl;
    *plog << codec->toUnicode( "# ���� ������ � ������������ �������� ����.668374.050 (� ��������� ��   #" ) << endl;
    *plog << codec->toUnicode( "# �����, ������ ����� �-������� � ����� �-��������� - ��� ������ �����  #" ) << endl;
    *plog << codec->toUnicode( "# � �.�.) �, �������� ����.668374.060. �������� �� �����������, �� ���� #" ) << endl;
    *plog << codec->toUnicode( "# ������, ����� ������������ ���� same_module1.bom �������� � ��������  #" ) << endl;
    *plog << codec->toUnicode( "# ��������� ����.668374.060, � ���� same_module2.bom �������� � ��������#" ) << endl;
    *plog << codec->toUnicode( "# ��������� ����.668374.050. ��� �� ���� same_module1.bom ��������      #" ) << endl;
    *plog << codec->toUnicode( "# ��������� ����.668374.050.                                            #" ) << endl;
    *plog << codec->toUnicode( "#                                                                       #" ) << endl;
    *plog << codec->toUnicode( "# ���� # ��������� ������������                                         #" ) << endl;
    *plog << codec->toUnicode( "#                                                                       #" ) << endl;
    *plog << codec->toUnicode( "# ����� �������, ����� ������� �������� ������� ����� ���� cp1251       #" ) << endl;
    *plog << codec->toUnicode( "#########################################################################" ) << endl;
    *plog << endl;
    *plog << LOG << "START {" << endl;

    while ( !do_file->atEnd() )
    {
      do_file_record = codec->toUnicode( do_file->readLine() );
      if ( (do_file_record.indexOf(".") != -1) && (do_file_record.indexOf("#") == -1) ) //����������� ���������� ������� (��� ��� ����� ��� ������������  ������� �����������)
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
  *plog << codec->toUnicode( "###      ��������� ���������� *.bom - ������, ������������� ����      ###" ) << endl;
  *plog << codec->toUnicode( "#########################################################################" ) << endl;
  *plog << codec->toUnicode( "### ���������� ���������� :                                           ###" ) << endl;
  *plog << codec->toUnicode( "###                                                                   ###" ) << endl;
  *plog << codec->toUnicode( "###    ��� ���������� ��������, ������������ ��������� ��             ###" ) << endl;
  *plog << codec->toUnicode( "### �������������� ����� ������ �������� �������, ������������        ###" ) << endl;
  *plog << codec->toUnicode( "### ������������ �������������. ���������� �������� �������, �������  ###" ) << endl;
  *plog << codec->toUnicode( "### �� ���������� � �����, �� ���������� � ������ � ������������      ###" ) << endl;
  *plog << codec->toUnicode( "### (������, �������� ��� ������������ � ��..).                       ###" ) << endl;
  *plog << codec->toUnicode( "###    ���� �������������� ������������ ������, �� ��� ����������     ###" ) << endl;
  *plog << codec->toUnicode( "### ��������, ����� ������ �������� � ����� ��� �������� �������.     ###" ) << endl;
  *plog << codec->toUnicode( "### ������ ��������� ������������ ��� ��� ����� ��������� (� ����     ###" ) << endl;
  *plog << codec->toUnicode( "### �����) � ����������� ��, ������, � ����� ������ ���� ������       ###" ) << endl;
  *plog << codec->toUnicode( "### �������� ����� �� �����. � ���� ������, �������� ����������       ###" ) << endl;
  *plog << codec->toUnicode( "### � *.bom - ������.                                                 ###" ) << endl;
  *plog << codec->toUnicode( "###    ���������� � �������� ������, ����� ������� �� ����� ��        ###" ) << endl;
  *plog << codec->toUnicode( "### �������� �������� (������ ��������, ����������� �����). � ����    ###" ) << endl;
  *plog << codec->toUnicode( "### ������, ���� ������� � ���������� � *.bom - �����, �� �� ������   ###" ) << endl;
  *plog << codec->toUnicode( "### ����������� � �������, ������������, ��������� ��������.          ###" ) << endl;
  *plog << codec->toUnicode( "###    ��� ���� ���������� �������, ��������������� ���������        ###" ) << endl;
  *plog << codec->toUnicode( "### �������������� ����������� �����������:                           ###" ) << endl;
  *plog << codec->toUnicode( "### \"KT\" - ����������� ����� (��� ������������ � ���������            ###" ) << endl;
  *plog << codec->toUnicode( "###      ��������������) �� ���������� � ��������� ����������;        ###" ) << endl;
  *plog << codec->toUnicode( "### \"0\" - \"9\" (����������� �����������, ������������ � �����) -       ###" ) << endl;
  *plog << codec->toUnicode( "###      ������ �������� �� ���������� � ��������� ����������;        ###" ) << endl;
  *plog << codec->toUnicode( "### \"REF\", \"REF_P\", \"REF_G\"  - �������� ����� (����������             ###" ) << endl;
  *plog << codec->toUnicode( "###      ��������� �� ������, ����� P-CAD �������� ���                ###" ) << endl;
  *plog << codec->toUnicode( "###      �������� ���������) �� ���������� � ��������� ����������;    ###" ) << endl;
  *plog << codec->toUnicode( "### \"UNUZED\" - �� ���������� � ��������� ����������                   ###" ) << endl;
  *plog << codec->toUnicode( "### \"COMPO\" - ��� ��������, ������� ������ ������������ �             ###" ) << endl;
  *plog << codec->toUnicode( "###      ������������ � ��������� ��������, � �� ������ ������������  ###" ) << endl;
  *plog << codec->toUnicode( "###      � ������� ��������� (������, ��������, ���������, �����..). ###" ) << endl;
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
      *plog << LOG << codec->toUnicode( "ERROR: �� ���� ������� \"" ) << iter->get_File_Name() << codec->toUnicode( "\" ����;" ) << endl;
      return false;
    }

    *plog << endl;
    *plog << "-------------------------------------------------------------------------" << endl;
    *plog << LOG << codec->toUnicode( "��������� \"" ) << iter->get_File_Name() << codec->toUnicode( "\" ����;" ) << endl;
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
        *plog << LOG << codec->toUnicode( "   ������� ������� :  " ) << temp_el.get_record2() << endl;
      }
    }

    *plog << LOG << codec->toUnicode( "��������� \"" ) << iter->get_File_Name() << codec->toUnicode( "\" ����;" ) << endl;
    bom_file->close();
    delete bom_file;
    tables.push_back( temp_ls );
  }

  *plog << LOG << "} END " << endl;

  *plog << endl;
  *plog << endl;
  *plog << codec->toUnicode( "#########################################################################" ) << endl;
  *plog << codec->toUnicode( "###      ������������� ���� �������� ����� ����������� ����������     ###" ) << endl;
  *plog << codec->toUnicode( "#########################################################################" ) << endl;
  *plog << endl;
  *plog << LOG << "START {" << endl;

  for ( QVector < QVector<element> >::iterator current_table = tables.begin(); current_table < tables.end(); current_table++ )
  {
    QVector<element>::iterator  iter = current_table->begin();
    *plog << LOG << codec->toUnicode( "��������, �������� � ������ : " ) << iter->get_Detimal_Number() << endl;
    for ( ; iter < current_table->end(); iter++ )
    {
      *plog << "   " << iter->get_record2() << endl;
    }

    *plog << endl;
  }

  *plog << LOG << "} END " << endl;

  return true;
}

