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
#ifndef TRUE_ELEMENT_H
  #define TRUE_ELEMENT_H

  #include <QtCore/QTextCodec>
  #include <QtCore/QString>
  #include <QtCore/QTextStream>
  #include <QtCore/QFile>
  #include <QtCore/QIODevice>
  #include <QtCore/QVector>
  #include <QtCore/QDir>
  #include <QtCore/QList>
  #include <QtCore/QFileInfo>
  #include <QCoreApplication>
  #include <QApplication>
  #include <QRegExp>

  #include "firm_name.h"
  #include "text_name.h"

  #include "element.h"

extern QTextStream        *plog;
extern QTextCodec         *codec;

extern QVector<text_name> Text_Names;
extern QVector<firm_name> Firm_Names;

class true_element
{
  public:
    static int  reading_true_element_file ( QVector<QVector<element> > &true_tables, QString &PathToResDir);
    static void similarity_elements ( QVector<element> &ls1,  QVector<QVector<element>> &etalon);
  private:
};
#endif // TRUE_ELEMENT_H

