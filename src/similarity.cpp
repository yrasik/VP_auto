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



//���������� ������� �������� ���������
//�������� ���� ������ ������ � ������������, �������� ����� �������� �
//������/������� ��������, �������������� ��������� �������� � ����� ������������ ��������

enum similarity::ss similarity::is_Similarity ( const QString &str1, const QString &str2 )
{
  QString temp_str1 = str1;
  QString temp_str2 = str2;

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME;            //���� � ����
  }

  //������ ������ � ��������� �� �����
  temp_str1 = temp_str1.trimmed();
  temp_str2 = temp_str2.trimmed();

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME_IN_TRIMMED;
  }

  //-------- ��������� �������� �������� ��� �������� � ������ ������� -----------
  temp_str1 = temp_str1.toLower();
  temp_str2 = temp_str2.toLower();

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME_IN_LOWER;
  }

  //------ ��������� ������� ��� ������� � ����� �������� �������� ---------
  temp_str1.remove( QChar(' '), Qt::CaseInsensitive );
  temp_str2.remove( QChar(' '), Qt::CaseInsensitive );

  temp_str1.remove( QChar('\t'), Qt::CaseInsensitive );
  temp_str2.remove( QChar('\t'), Qt::CaseInsensitive );

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME_IN_SPACE;
  }

  //--------------- ��������� �������� ��� ����� �������� ------------------
  temp_str1.replace( codec->toUnicode("."), codec->toUnicode(",") );
  temp_str2.replace( codec->toUnicode("."), codec->toUnicode(",") );

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME_IN_COMMA;
  }


  //--------------- ��������� �������� ��� ����� "�"(������� ����) ������ "-" -------------
  temp_str1.replace( codec->toUnicode("�"), codec->toUnicode("-") );
  temp_str2.replace( codec->toUnicode("�"), codec->toUnicode("-") );

  //--------------- ��������� �������� ��� ����� "�"(������� ����) ������ "-" -------------
  temp_str1.replace( codec->toUnicode("�"), codec->toUnicode("-") );
  temp_str2.replace( codec->toUnicode("�"), codec->toUnicode("-") );

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME_IN_TEXTMINUS;
  }

  //--------------- ��������� �������� ��� ����� "+-" ������ "�" -------------
  temp_str1.replace( codec->toUnicode("+-"), codec->toUnicode("�") );
  temp_str2.replace( codec->toUnicode("+-"), codec->toUnicode("�") );

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME_IN_TEXTPM;
  }

  //------- ��������� �������� ��� ������� "�" �� ���������� "c" ---------------
  temp_str1.replace( codec->toUnicode("�"), codec->toUnicode("c") );
  temp_str2.replace( codec->toUnicode("�"), codec->toUnicode("c") );

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME_IN_RUS_C;
  }

  //------- ��������� �������� ��� ������� "�" �� ���������� "�" ---------------
  temp_str1.replace( codec->toUnicode("�"), codec->toUnicode("a") );
  temp_str2.replace( codec->toUnicode("�"), codec->toUnicode("a") );

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME_IN_RUS_A;
  }

  //------- ��������� �������� ��� ������� "�" �� ���������� "e" ---------------
  temp_str1.replace( codec->toUnicode("�"), codec->toUnicode("e") );
  temp_str2.replace( codec->toUnicode("�"), codec->toUnicode("e") );

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME_IN_RUS_E;
  }

  //------- ��������� �������� ��� ������� "�" �� ���������� "p" ---------------
  temp_str1.replace( codec->toUnicode("�"), codec->toUnicode("p") );
  temp_str2.replace( codec->toUnicode("�"), codec->toUnicode("p") );

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME_IN_RUS_P;
  }

  //------- ��������� �������� ��� ������� "�" �� ���������� "T" ---------------
  temp_str1.replace( codec->toUnicode("�"), codec->toUnicode("t") );
  temp_str2.replace( codec->toUnicode("�"), codec->toUnicode("t") );

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME_IN_RUS_T;
  }

  //------- ��������� �������� ��� ������� "�" �� ���������� "H" ---------------
  temp_str1.replace( codec->toUnicode("�"), codec->toUnicode("h") );
  temp_str2.replace( codec->toUnicode("�"), codec->toUnicode("h") );

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME_IN_RUS_H;
  }

  //------- ��������� �������� ��� ������� "�" �� ���������� "K" ---------------
  temp_str1.replace( codec->toUnicode("�"), codec->toUnicode("k") );
  temp_str2.replace( codec->toUnicode("�"), codec->toUnicode("k") );

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME_IN_RUS_K;
  }

  //------- ��������� �������� ��� ������� "�" �� ���������� "X" ---------------
  temp_str1.replace( codec->toUnicode("�"), codec->toUnicode("x") );
  temp_str2.replace( codec->toUnicode("�"), codec->toUnicode("x") );

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME_IN_RUS_X;
  }

  //------- ��������� �������� ��� ������� "�" �� ���������� "B" ---------------
  temp_str1.replace( codec->toUnicode("�"), codec->toUnicode("b") );
  temp_str2.replace( codec->toUnicode("�"), codec->toUnicode("b") );

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME_IN_RUS_B;
  }

  //------- ��������� �������� ��� ������� "�" �� ���������� "M" ---------------
  temp_str1.replace( codec->toUnicode("�"), codec->toUnicode("m") );
  temp_str2.replace( codec->toUnicode("�"), codec->toUnicode("m") );

  if( temp_str1.compare(temp_str2, Qt::CaseSensitive) == 0 )
  {
    return SS_SAME_IN_RUS_M;
  }

  //------ ��������� �������� ��������� ������ � ������ ������ (������ ����� � ������������) ------
  if ( (temp_str1.length() > 2) &&
       (temp_str2.length() > 2)
     )
  {
    if ( temp_str1.length() < temp_str2.length() )
    {
      if ( temp_str2.indexOf(temp_str1) != -1 )
      {               //����� ��������� � ������
        return SS_SAME_SUBSTRING;
      }
    }
    else
    {
      if ( temp_str1.indexOf(temp_str2) != -1 )
      {               //����� ��������� � ������
        return SS_SAME_SUBSTRING;
      }
    }
  }
  else
  {
    return SS_DIFFERENT;
  }


  //------ ��������� �������� ��������� ������ � ������ ������ (������ ����� � ������������) ------
if ( (temp_str1.length() > 2) &&
     (temp_str2.length() > 2)
   )
{
  if ( temp_str1.length() < temp_str2.length() )
  {
    if ( temp_str2.indexOf(temp_str1) != -1 )
    {               //����� ��������� � ������
      return SS_SAME_SUBSTRING;
    }
  }
  else
  {
    if ( temp_str1.indexOf(temp_str2) != -1 )
    {               //����� ��������� � ������
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
    {               //����� ��������� � ������
      return SS_SAME_PLUS_OR_X__IN_END;
    }
  }
  else
  {
    if ( temp_str1.indexOf(temp_str2) != -1 )
    {               //����� ��������� � ������
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
    {               //����� ��������� � ������
      return SS_SAME_PLUSES_OR_XX__IN_END;
    }
  }
  else
  {
    if ( temp_str1.indexOf(temp_str2) != -1 )
    {               //����� ��������� � ������
      return SS_SAME_PLUSES_OR_XX__IN_END;
    }
  }
}

  //*plog << LOG << "'"<<  temp_str1 <<  "' <> '"  << temp_str2 << "'" << endl;
  return SS_DIFFERENT; //����� ������� ��������
}



QString similarity::Decoding_Similarity_result ( enum similarity::ss ss)
{
  QString result;

  switch ( ss )
  {
  case SS_SAME:
    result = codec->toUnicode( " ������� ����������" );
    break;
  case SS_SAME_IN_TRIMMED:
    result = codec->toUnicode( " ������� �������� ��� �������� �������� � ��������� �� �����" );
    break;
  case SS_SAME_IN_LOWER:
    result = codec->toUnicode( " ������� �������� ��� �������� � ������ �������" );
    break;
  case SS_SAME_IN_SPACE:
    result = codec->toUnicode( " ������� �������� ��� �������� � ������ �������, �������� �������� � ���������" );
    break;
  case SS_SAME_IN_COMMA:
    result = codec->toUnicode( " ������� �������� ��� �������� � ������ �������, �������� ��������  � ������ ���� ����� ��������" );
    break;
  case SS_SAME_IN_TEXTMINUS:
    result = codec->toUnicode( " ������� �������� ��� �������� � ������ �������, �������� ��������, ������ ���� ����� ��������, ������ ��������/�������� ���� �� '-' (�����)" );
    break;
  case SS_SAME_IN_TEXTPM:
    result = codec->toUnicode( " ������� �������� ��� �������� � ������ �������, �������� ��������, ������ ���� ����� ��������, ������ '+-' �� '�'" );
    break;
  case SS_SAME_IN_RUS_C:
    result = codec->toUnicode( " ������� �������� ��� ������ ������� ���� '�' �� ��������� (������� �� �������� ���� �����)" );
    break;
  case SS_SAME_IN_RUS_A:
    result = codec->toUnicode( " ������� �������� ��� ������ ������� ���� '�' �� ��������� (������� �� �������� ���� �����)" );
    break;
  case SS_SAME_IN_RUS_E:
    result = codec->toUnicode( " ������� �������� ��� ������ ������� ���� '�' �� ��������� (������� �� �������� ���� �����)" );
    break;
  case SS_SAME_IN_RUS_P:
    result = codec->toUnicode( " ������� �������� ��� ������ ������� ���� '�' �� ��������� (������� �� �������� ���� �����)" );
    break;
  case SS_SAME_IN_RUS_T:
    result = codec->toUnicode( " ������� �������� ��� ������ ������� ���� '�' �� ��������� (������� �� �������� ���� �����)" );
    break;
  case SS_SAME_IN_RUS_H:
    result = codec->toUnicode( " ������� �������� ��� ������ ������� ���� '�' �� ��������� (������� �� �������� ���� �����)" );
    break;
  case SS_SAME_IN_RUS_K:
    result = codec->toUnicode( " ������� �������� ��� ������ ������� ���� '�' �� ��������� (������� �� �������� ���� �����)" );
    break;
  case SS_SAME_IN_RUS_X:
    result = codec->toUnicode( " ������� �������� ��� ������ ������� ���� '�' �� ��������� (������� �� �������� ���� �����)" );
    break;
  case SS_SAME_IN_RUS_B:
    result = codec->toUnicode( " ������� �������� ��� ������ ������� ���� '�' �� ��������� (������� �� �������� ���� �����)" );
    break;
  case SS_SAME_IN_RUS_M:
    result = codec->toUnicode( " ������� �������� ��� ������ ������� ���� '�' �� ��������� (������� �� �������� ���� �����)" );
    break;
  case SS_SAME_SUBSTRING:
    result = codec->toUnicode( " ������� �������� ��� �������� � ������ �������, �������� ��������, ������ ���� ����� ��������, ������ '+-' �� '�' ������ �� ������" );
    break;
  case SS_SAME_PLUS_OR_X__IN_END:
    result = codec->toUnicode( " ������� �������� ��� �������� � ������ �������, �������� ��������, ������ ���� ����� ��������, ������ '+-' �� '�' ������ �� ������, �������� '+' � 'x' �� ����� ������" );
    break;
  case SS_SAME_PLUSES_OR_XX__IN_END:
    result = codec->toUnicode( " ������� �������� ��� �������� � ������ �������, �������� ��������, ������ ���� ����� ��������, ������ '+-' �� '�' ������ �� ������, ���������� '+' � 'x' �� ����� ������" );
    break;
  case SS_DIFFERENT:
    result = codec->toUnicode( " �������� �� �������" );
    break;
  default:
    result = codec->toUnicode( "UNSUPPORTED");
    break;
  }

  return result;
}

