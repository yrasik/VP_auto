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
#ifndef SIMILARITY_H
  #define SIMILARITY_H

  #include <QtCore/QTextCodec>
  #include <QtCore/QString>
  #include <QtCore/QTextStream>
  #include <QtCore/QVector>
  #include <QRegExp>


extern QTextStream  *plog;
extern QTextCodec   *codec;






class similarity
{
  public:
    enum ss
    {
      SS_SAME = 0,
      SS_SAME_IN_TRIMMED,
      SS_SAME_IN_LOWER,
      SS_SAME_IN_SPACE,
      SS_SAME_IN_COMMA,
      SS_SAME_IN_TEXTMINUS,
      SS_SAME_IN_TEXTPM,
      SS_SAME_IN_RUS_C,
      SS_SAME_IN_RUS_A,
      SS_SAME_IN_RUS_E,
      SS_SAME_IN_RUS_P,
      SS_SAME_IN_RUS_T,
      SS_SAME_IN_RUS_H,
      SS_SAME_IN_RUS_K,
      SS_SAME_IN_RUS_X,
      SS_SAME_IN_RUS_B,
      SS_SAME_IN_RUS_M,
      SS_SAME_SUBSTRING,
      SS_SAME_PLUS_OR_X__IN_END,
      SS_SAME_PLUSES_OR_XX__IN_END,
      SS_DIFFERENT
    };

    static enum ss is_Similarity ( const QString &str1, const QString &str2 );
    static QString Decoding_Similarity_result ( enum similarity::ss ss);
    
  private:

};




#endif // ELEMENT_H

