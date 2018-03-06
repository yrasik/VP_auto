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
#ifndef TEXT_WIDTH_H
  #define TEXT_WIDTH_H

  #include <QtCore/QTextCodec>
  #include <QtCore/QString>
  #include <QtCore/QTextStream>
  #include <QFontMetrics>


extern QTextStream  *plog;
extern QTextCodec   *codec;



class text_width
{ public:
    text_width ( QString Font_Name, int Font_Size, float Pixel_Width_mm )
    {
      this->Pixel_Width_mm = Pixel_Width_mm;

      FontMetric = new QFontMetrics( QFont(Font_Name, Font_Size, QFont::Normal) );

      if ( FontMetric == NULL )
      {
        *plog << codec->toUnicode( "ERROR: Не могу инициализировать QFontMetrics" ) << endl;
      }
    }

    ~text_width ()
    {
      if ( FontMetric != NULL )
      {
        delete FontMetric;
      }
    }

    float mm ( QString &str )
    {
      return ( FontMetric->width(str) * Pixel_Width_mm );
    }

  private:
    QFontMetrics  *FontMetric;
    float         Pixel_Width_mm;
};


#endif // TEXT_WIDTH_H

