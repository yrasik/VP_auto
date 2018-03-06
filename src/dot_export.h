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

#ifndef DOT_EXPORT_H
#define DOT_EXPORT_H

#include <QtCore/QString>
#include <QFileInfo>
#include <QtCore/QTextStream>
#include <QtCore/QTextCodec>

#include <QRegExp>
extern QTextStream *plog;
extern QTextCodec *codec;






class dot_export
{
public:
  dot_export();

  int create_dot(QString File_Name);
  bool is_created(void);
  void set_record(QString bot, QString top);
  void close_dot(void);  
  void set_name_record(QString box, QString name);
private:
  QFile *dot_file;
  QTextStream *dot;
  bool created;
};

#endif // DOT_EXPORT_H
