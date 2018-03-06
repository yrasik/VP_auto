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
#include "similarity.h"

#define LOG ""



//Определяет степень схожести элементов
//Допустим есть лишний пробел в наименовании, название фирмы записано в
//нижнем/верхнем регистре, дополнительные несколько символов в конце наименования элемента

enum similarity::ss similarity::is_Similarity ( const QString &str1, const QString &str2 )
{
  QString temp_str1 = str1;
  QString temp_str2 = str2;

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME;            //Одно и тоже
  }

  //Убрали проелы и табуляцию по краям
  temp_str1 = temp_str1.trimmed();
  temp_str2 = temp_str2.trimmed();

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME_IN_TRIMMED;
  }

  //-------- Попробуем поискать сходство при переводе в нижний регистр -----------
  temp_str1 = temp_str1.toLower();
  temp_str2 = temp_str2.toLower();

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME_IN_LOWER;
  }

  //------ Попробуем удалить все пробелы и опять поискать сходство ---------
  temp_str1.remove( QChar(' '), Qt::CaseInsensitive );
  temp_str2.remove( QChar(' '), Qt::CaseInsensitive );

  temp_str1.remove( QChar('\t'), Qt::CaseInsensitive );
  temp_str2.remove( QChar('\t'), Qt::CaseInsensitive );

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME_IN_SPACE;
  }

  //--------------- Попробуем заменить все точки запятыми ------------------
  temp_str1.replace( codec->toUnicode("."), codec->toUnicode(",") );
  temp_str2.replace( codec->toUnicode("."), codec->toUnicode(",") );

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME_IN_COMMA;
  }


  //--------------- Попробуем заменить все знаки "—"(длинное тире) знаком "-" -------------
  temp_str1.replace( codec->toUnicode("—"), codec->toUnicode("-") );
  temp_str2.replace( codec->toUnicode("—"), codec->toUnicode("-") );

  //--------------- Попробуем заменить все знаки "–"(среднее тире) знаком "-" -------------
  temp_str1.replace( codec->toUnicode("–"), codec->toUnicode("-") );
  temp_str2.replace( codec->toUnicode("–"), codec->toUnicode("-") );

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME_IN_TEXTMINUS;
  }

  //--------------- Попробуем заменить все знаки "+-" знаком "±" -------------
  temp_str1.replace( codec->toUnicode("+-"), codec->toUnicode("±") );
  temp_str2.replace( codec->toUnicode("+-"), codec->toUnicode("±") );

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME_IN_TEXTPM;
  }

  //------- Попробуем заменить все русские "с" на английские "c" ---------------
  temp_str1.replace( codec->toUnicode("с"), codec->toUnicode("c") );
  temp_str2.replace( codec->toUnicode("с"), codec->toUnicode("c") );

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME_IN_RUS_C;
  }

  //------- Попробуем заменить все русские "а" на английские "а" ---------------
  temp_str1.replace( codec->toUnicode("а"), codec->toUnicode("a") );
  temp_str2.replace( codec->toUnicode("а"), codec->toUnicode("a") );

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME_IN_RUS_A;
  }

  //------- Попробуем заменить все русские "е" на английские "e" ---------------
  temp_str1.replace( codec->toUnicode("е"), codec->toUnicode("e") );
  temp_str2.replace( codec->toUnicode("е"), codec->toUnicode("e") );

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME_IN_RUS_E;
  }

  //------- Попробуем заменить все русские "р" на английские "p" ---------------
  temp_str1.replace( codec->toUnicode("р"), codec->toUnicode("p") );
  temp_str2.replace( codec->toUnicode("р"), codec->toUnicode("p") );

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME_IN_RUS_P;
  }

  //------- Попробуем заменить все русские "Т" на английские "T" ---------------
  temp_str1.replace( codec->toUnicode("т"), codec->toUnicode("t") );
  temp_str2.replace( codec->toUnicode("т"), codec->toUnicode("t") );

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME_IN_RUS_T;
  }

  //------- Попробуем заменить все русские "Н" на английские "H" ---------------
  temp_str1.replace( codec->toUnicode("н"), codec->toUnicode("h") );
  temp_str2.replace( codec->toUnicode("н"), codec->toUnicode("h") );

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME_IN_RUS_H;
  }

  //------- Попробуем заменить все русские "К" на английские "K" ---------------
  temp_str1.replace( codec->toUnicode("к"), codec->toUnicode("k") );
  temp_str2.replace( codec->toUnicode("к"), codec->toUnicode("k") );

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME_IN_RUS_K;
  }

  //------- Попробуем заменить все русские "Х" на английские "X" ---------------
  temp_str1.replace( codec->toUnicode("х"), codec->toUnicode("x") );
  temp_str2.replace( codec->toUnicode("х"), codec->toUnicode("x") );

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME_IN_RUS_X;
  }

  //------- Попробуем заменить все русские "В" на английские "B" ---------------
  temp_str1.replace( codec->toUnicode("в"), codec->toUnicode("b") );
  temp_str2.replace( codec->toUnicode("в"), codec->toUnicode("b") );

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME_IN_RUS_B;
  }

  //------- Попробуем заменить все русские "М" на английские "M" ---------------
  temp_str1.replace( codec->toUnicode("м"), codec->toUnicode("m") );
  temp_str2.replace( codec->toUnicode("м"), codec->toUnicode("m") );

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME_IN_RUS_M;
  }

  //------ Попробуем поискать фрагменты строки в другой строке (лишние буквы в наименовании) ------
  if ( (temp_str1.length() > 2) &&
       (temp_str2.length() > 2)
     )
  {
    if ( temp_str1.length() < temp_str2.length() )
    {
      if ( temp_str2.indexOf(temp_str1) != -1 )
      {               //Нашли подстроку в строке
        return SS_SAME_SUBSTRING;
      }
    }
    else
    {
      if ( temp_str1.indexOf(temp_str2) != -1 )
      {               //Нашли подстроку в строке
        return SS_SAME_SUBSTRING;
      }
    }
  }
  else
  {
    return SS_DIFFERENT;
  }


  //------ Попробуем поискать фрагменты строки в другой строке (лишние буквы в наименовании) ------
if ( (temp_str1.length() > 2) &&
     (temp_str2.length() > 2)
   )
{
  if ( temp_str1.length() < temp_str2.length() )
  {
    if ( temp_str2.indexOf(temp_str1) != -1 )
    {               //Нашли подстроку в строке
      return SS_SAME_SUBSTRING;
    }
  }
  else
  {
    if ( temp_str1.indexOf(temp_str2) != -1 )
    {               //Нашли подстроку в строке
      return SS_SAME_SUBSTRING;
    }
  }
}
else
{
  return SS_DIFFERENT;
}


  temp_str1.remove(QRegExp("[x|+]?$"));
  temp_str2.remove(QRegExp("[x|+]?$"));

  if ( (temp_str1.length() > 2) &&
       (temp_str2.length() > 2)
     )
  {
  if ( temp_str1.length() < temp_str2.length() )
  {
    if ( temp_str2.indexOf(temp_str1) != -1 )
    {               //Нашли подстроку в строке
      return SS_SAME_PLUS_OR_X__IN_END;
    }
  }
  else
  {
    if ( temp_str1.indexOf(temp_str2) != -1 )
    {               //Нашли подстроку в строке
      return SS_SAME_PLUS_OR_X__IN_END;
    }
  }
}
else
{
  return SS_DIFFERENT;
}



  temp_str1.remove(QRegExp("[x|+]{1,2}$"));
  temp_str2.remove(QRegExp("[x|+]{1,2}$"));


  if ( (temp_str1.length() > 2) &&
       (temp_str2.length() > 2)
     )
  {
  if ( temp_str1.length() < temp_str2.length() )
  {
    if ( temp_str2.indexOf(temp_str1) != -1 )
    {               //Нашли подстроку в строке
      return SS_SAME_PLUSES_OR_XX__IN_END;
    }
  }
  else
  {
    if ( temp_str1.indexOf(temp_str2) != -1 )
    {               //Нашли подстроку в строке
      return SS_SAME_PLUSES_OR_XX__IN_END;
    }
  }
}

  //*plog << LOG << "'"<<  temp_str1 <<  "' <> '"  << temp_str2 << "'" << endl;
  return SS_DIFFERENT; //Очень большие различия
}



QString similarity::Decoding_Similarity_result ( enum similarity::ss ss)
{
  QString result;

  switch ( ss )
  {
  case SS_SAME:
    result = codec->toUnicode( " Найдено совпадение" );
    break;
  case SS_SAME_IN_TRIMMED:
    result = codec->toUnicode( " Найдено сходство при удалении пробелов и табуляций по краям" );
    break;
  case SS_SAME_IN_LOWER:
    result = codec->toUnicode( " Найдено сходство при переводе в нижний регистр" );
    break;
  case SS_SAME_IN_SPACE:
    result = codec->toUnicode( " Найдено сходство при переводе в нижний регистр, удалении пробелов и табуляций" );
    break;
  case SS_SAME_IN_COMMA:
    result = codec->toUnicode( " Найдено сходство при переводе в нижний регистр, удалении пробелов  и замене всех точек запятыми" );
    break;
  case SS_SAME_IN_TEXTMINUS:
    result = codec->toUnicode( " Найдено сходство при переводе в нижний регистр, удалении пробелов, замене всех точек запятыми, замене длинного/среднего тире на '-' (минус)" );
    break;
  case SS_SAME_IN_TEXTPM:
    result = codec->toUnicode( " Найдено сходство при переводе в нижний регистр, удалении пробелов, замене всех точек запятыми, замене '+-' на '±'" );
    break;
  case SS_SAME_IN_RUS_C:
    result = codec->toUnicode( " Найдено сходство при замене русских букв 'с' на латинские (похожие по внешнему виду буквы)" );
    break;
  case SS_SAME_IN_RUS_A:
    result = codec->toUnicode( " Найдено сходство при замене русских букв 'а' на латинские (похожие по внешнему виду буквы)" );
    break;
  case SS_SAME_IN_RUS_E:
    result = codec->toUnicode( " Найдено сходство при замене русских букв 'е' на латинские (похожие по внешнему виду буквы)" );
    break;
  case SS_SAME_IN_RUS_P:
    result = codec->toUnicode( " Найдено сходство при замене русских букв 'р' на латинские (похожие по внешнему виду буквы)" );
    break;
  case SS_SAME_IN_RUS_T:
    result = codec->toUnicode( " Найдено сходство при замене русских букв 'Т' на латинские (похожие по внешнему виду буквы)" );
    break;
  case SS_SAME_IN_RUS_H:
    result = codec->toUnicode( " Найдено сходство при замене русских букв 'Н' на латинские (похожие по внешнему виду буквы)" );
    break;
  case SS_SAME_IN_RUS_K:
    result = codec->toUnicode( " Найдено сходство при замене русских букв 'К' на латинские (похожие по внешнему виду буквы)" );
    break;
  case SS_SAME_IN_RUS_X:
    result = codec->toUnicode( " Найдено сходство при замене русских букв 'Х' на латинские (похожие по внешнему виду буквы)" );
    break;
  case SS_SAME_IN_RUS_B:
    result = codec->toUnicode( " Найдено сходство при замене русских букв 'В' на латинские (похожие по внешнему виду буквы)" );
    break;
  case SS_SAME_IN_RUS_M:
    result = codec->toUnicode( " Найдено сходство при замене русских букв 'М' на латинские (похожие по внешнему виду буквы)" );
    break;
  case SS_SAME_SUBSTRING:
    result = codec->toUnicode( " Найдено сходство при переводе в нижний регистр, удалении пробелов, замене всех точек запятыми, замене '+-' на '±' поиска по строке" );
    break;
  case SS_SAME_PLUS_OR_X__IN_END:
    result = codec->toUnicode( " Найдено сходство при переводе в нижний регистр, удалении пробелов, замене всех точек запятыми, замене '+-' на '±' поиска по строке, удалении '+' и 'x' из конца строки" );
    break;
  case SS_SAME_PLUSES_OR_XX__IN_END:
    result = codec->toUnicode( " Найдено сходство при переводе в нижний регистр, удалении пробелов, замене всех точек запятыми, замене '+-' на '±' поиска по строке, нескольких '+' и 'x' из конца строки" );
    break;
  case SS_DIFFERENT:
    result = codec->toUnicode( " Сходство не найдено" );
    break;
  default:
    result = codec->toUnicode( "UNSUPPORTED");
    break;
  }

  return result;
}

