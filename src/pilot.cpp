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
#include "pilot.h"

#define LOG ""



void pilot::by_count(void)
{
 // *plog << endl;
 // *plog << "########################################" << endl;
 // *plog << "###        Sorting by Count:         ###" << endl;
 // *plog << "########################################" << endl;
 // *plog << "START {" << endl;

  QVector<element>::iterator iter;
  QVector<element>::iterator iter_start;
  iter_start = el->begin();
  iter = el->begin();

  while (iter_start < el->end())
  {
    if ((*iter_start == *iter) && (iter_start != iter))
    {
      iter_start->set_Count((*iter_start).get_Count() + (*iter).get_Count());
      el->erase(iter);
      iter = iter_start;
    }
    iter++;
    if (iter == el->end())
    {
      iter_start++;
      iter = iter_start;
    }
  }

/*  for (iter = el->begin(); iter != el->end(); iter++)
  {
    element temp_el = *iter;
    *plog << temp_el.get_record() << endl;
  }
*/
 // *plog << "} END " << endl << endl;
}


void pilot::by_value_code(void)
{
 // *plog << endl;
 // *plog << "########################################" << endl;
 // *plog << "###       Sorting by Value Code:     ###" << endl;
 // *plog << "########################################" << endl;
 // *plog << "START {" << endl;

  { // Сортирнём по значению компанентов
    QVector<element>::iterator iter;
    QVector<element>::iterator iter_start;

    iter_start = el->begin();
    iter = el->begin();

    while (iter_start != el->end())
    {
      if ((*iter_start).get_RefDes() != (*iter).get_RefDes())
      {
        std::sort(iter_start, iter, element::by_Value_Code);
        iter_start = iter;
      }
      iter++;
      if (iter == el->end())
      {
        if ((*iter_start).get_RefDes() == (*(--iter)).get_RefDes())
          std::sort(iter_start, el->end(), element::by_Value_Code);
        break;
      }
    }
/*    for (iter = el->begin(); iter != el->end(); iter++)
    {
      *plog << (*iter).get_record() << endl;
    }
*/
  //  *plog << "} END " << endl;
  }

}


void pilot::by_refdes(void)
{
 // *plog << endl;
 // *plog << "########################################" << endl;
 // *plog << "###        Sorting by RefDes:        ###" << endl;
 // *plog << "########################################" << endl;
  std::sort(el->begin(), el->end());//Отсортировали по буквам в рефдесе
 // *plog << "START {" << endl;

  QVector<element>::iterator iter;
 /* for (iter = el->begin(); iter != el->end(); iter++)
    *plog << (*iter).get_record() << endl;
  *plog << "} END " << endl;
  */
}


void pilot::pcad_page_setup(void)
{

}





void pilot::pcad_create_next_page(void)
{
  QString     str;
  QTextStream temp ( &str );

  unsigned int number_of_pages = qCeil((float)(records.size())/72);

  for (int cols = 0; cols < 18; cols++)
  {
    for(int str = 0; str < 4; str++)
    {
        temp << "(text (pt " <<  (1 + (str * 52.5)) << " " << (281 - ( cols * 15.45 )) << ") \"" <<  (*current_record) << "\" (textStyleRef \"GostB-10\"))" << endl;

        current_record++;
        if(current_record >= records.end())
        {
            break;
        }
    }

    if(current_record >= records.end())
    {
        break;
    }

  }

  temp << "(text (pt " <<  129 << " " << 13 << ") \"" <<  (*(*el).begin()).get_Detimal_Number()   << "\" (textStyleRef \"GostB-12\"))" << endl;
  temp << "(text (pt " <<  138 << " " << 7 << ") \"" <<  records.size() << "\" (textStyleRef \"GostB-12\"))" << endl;

  temp << "(text (pt " <<  205 << " " << 14 << ") \"" <<  PageNumber << "\" (textStyleRef \"GostB-12\"))" << endl;
  temp << "(text (pt " <<  205 << " " << 6 << ") \"" <<  number_of_pages << "\" (textStyleRef \"GostB-12\"))" << endl;
document->push_back(str);
PageNumber++;
}

void pilot::pcad_create_records(void)
{
    QVector<element>::iterator current_element;
    QVector<element>::iterator temp_iter;
    QString str;
    QTextStream temp ( &str );

    unsigned int Count_summ;
    unsigned int Number = 1;

    current_element = (*el).begin();

    while( current_element < (*el).end() )
    {
      str.clear();
      temp_iter = current_element;
      Count_summ = 0;

      temp <<  Number << ". " << (*current_element).get_RefDes() << " " << (*current_element).get_Type_from_Value().left(29) << "\\r\\n";
      temp << "  " << (*current_element).get_Code_from_Value().left(29) << "\\r\\n";
      temp << "  " << (*current_element).get_Value_Decoding().left(29) << "\\r\\n";


      temp_iter++;
      if (temp_iter < (*el).end())
      {
        Count_summ += (*current_element).get_Count();
        while (*current_element != *temp_iter)
        {
          if (++current_element > (*el).end())
            break;
          if (++temp_iter > (*el).end())
            break;
          Count_summ += (*current_element).get_Count();
        }
      }
      else
      {
        if(temp_iter == (*el).end())
        {
          Count_summ += (*current_element).get_Count();
        }
      }

     temp << "  " << Count_summ << " x " << Number_of_Copy << " = " << (Count_summ * Number_of_Copy);

//     *plog << endl;
//     *plog << str << endl;

     records.push_back(str);
     current_element++;
     Number++;
      }


  //  *plog << codec->toUnicode("Создаём \"") << FullFileName_order_txt << codec->toUnicode("\" файл") << endl;
  //  vp_order_file.close();
  //  *plog << codec->toUnicode("Закрываем \"") << FullFileName_order_txt << codec->toUnicode("\" файл") << endl;
    return;
}

void pilot::pcad_put_record(QString &str)
{

}

void pilot::set_normal_record(QString &str)
{

}

bool pilot::pcad_open_files(void)
{
    canvas_file.open(FullCanvasFileName.toLatin1(), std::ios::in);
    if (!canvas_file)
    {
      *plog << codec->toUnicode("ERROR: Не могу открыть \"pilot_canvas.sch\" файл") << endl;
      return false;
    }
    else
    {
      *plog << codec->toUnicode("Открываем \"") << FullCanvasFileName <<  codec->toUnicode("\" файл;") << endl;
      target_file.open(codec->fromUnicode(FullFileName).constData(), std::ios::out);
      if (!target_file)
      {
        *plog << codec->toUnicode("ERROR: Не могу создать \"") << FullFileName << codec->toUnicode("\" файл") << endl;
        return false;
      }
      *plog << codec->toUnicode("Создаём \"") << FullFileName << codec->toUnicode("\" файл") << endl;
    }
    return true;
}

void pilot::pcad_close_files(void)
{
    target_file.close();
    *plog << codec->toUnicode("Закрываем \"") << FullFileName << codec->toUnicode("\" файл") << endl;
    canvas_file.close();
    *plog <<  codec->toUnicode("Закрываем \"") << FullCanvasFileName << codec->toUnicode("\" файл") << endl;
}

pilot::pilot(QString &PathToOutDir, QVector<element> *el_, signed int Number_of_Copy_)
{
    this->PathToOutDir = PathToOutDir;
    Number_of_Copy = Number_of_Copy_;

    el = new QVector<element>;
    document = new QVector<QString> ;
    for (QVector<element>::iterator i = el_->begin(); i != el_->end(); i++)
    {
      el->push_back(*i);
    }
    FullFileName = this->PathToOutDir + "/" + "pilot_" + (*((*el).begin())).get_Detimal_Number() + codec->toUnicode(".sch");
    FullCanvasFileName = QCoreApplication::applicationDirPath() + "/../canvas/pilot_canvas.sch";
}

pilot::~pilot()
{
    delete el;
    delete document;
}

void pilot::generate(void)
{
    std::string canvas_string;

    *plog << endl;
    *plog                  << "#########################################################################" << endl;
    *plog << codec->toUnicode("###             Генерация бирок для опытного производства             ###") << endl;
    *plog                  << "#########################################################################" << endl;
    *plog << endl;
    *plog << "START {" << endl;

    by_refdes();
    by_value_code();
    by_count();
    pcad_create_records();

   current_record = records.begin();
   PageNumber = 1;

   while (current_record < records.end())
   {
       pcad_create_next_page();
   }

    QVector<QString>::iterator  current_page = document->begin();

    if ( pcad_open_files() )
    {
      while ( !getline(canvas_file, canvas_string, '\n').eof() )
      {
        target_file << canvas_string << std::endl;
        if ( canvas_string.find("fieldSetRef") != std::string::npos )
        {
          if ( current_page < document->end() )
          {
            target_file << codec->fromUnicode( *current_page ).constData();
            current_page++;
          }
        }
      }

      pcad_close_files();
    }

    *plog << "} END " << endl;
}







