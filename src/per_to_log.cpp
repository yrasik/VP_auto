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
#include "per_to_log.h"

#define LOG ""

per_to_log::per_to_log(QString &PathToOutDir, QVector<element> *el)
{
    this->PathToOutDir = PathToOutDir;
    processing = true;
    this->el = new QVector<element>;
    document = new QVector<QString>;

    state = PROCESSING;

    for ( QVector<element>::iterator i = el->begin(); i != el->end(); i++ )
    {
      this->el->push_back( *i );
    }

    FullFileName = this->PathToOutDir + "/" + ( *((*this->el).begin()) ).get_Detimal_Number() + codec->toUnicode( ".txt" );
}


per_to_log::~per_to_log()
{
    delete el;
    delete document;
}


void per_to_log::generate(void)
{
    *plog << endl;
    *plog                  << "#########################################################################" << endl;
    *plog << codec->toUnicode("###           ��������� ������� ��������� � �������� ����             ###") << endl;
    *plog                  << "#########################################################################" << endl;
    *plog << endl;
    *plog << LOG  << "START {" << endl;

    by_refdes_and_value_code(); // ��������� � 4-� ������ �������
    page_setup();

    PageNumber = 1;





    *plog << LOG  << "***************************************************************************************************" << endl;
   iter_same_Ref_before = NULL;
   for ( iter_same_Ref = perechen.begin(); iter_same_Ref != perechen.end(); iter_same_Ref++ )
    {
      if( (iter_same_Ref_before == NULL ) || ( (*(*( *iter_same_Ref_before ).begin()).begin()).get_RefDes() != (*(*( *iter_same_Ref ).begin()).begin()).get_RefDes() ) ) //�������� Ref ������
        {
          QString Ref = (*(*( *iter_same_Ref ).begin()).begin()).get_RefDes();
          if( (Ref == "A") || (Ref == "B") )
            {
              *plog << LOG  << endl;
            }
           else
            {
              put_group_title();
            }
        }


     // *plog << LOG <<"++++++" << endl;
      //*plog << LOG  << "   $$$$$$$$$$$ " << codec->toUnicode("��������, ������������� � ����� �����") << " $$$$$$$$$$$" << endl;
      if( (*iter_same_Ref).size() > 1  )
      {
    //    *plog << LOG << "    ======= "<< (*(*( *iter_same_Ref ).begin()).begin()).get_Group_Name() << "\t" << (*(*( *iter1 ).begin()).begin()).get_Types_from_Value() << " " << (*(*( *iter1 ).begin()).begin()).get_Value_Firm() << endl;
      }
      else
      {
    //      *plog << LOG  << "    ======= "<<  (*(*( *iter_same_Ref ).begin()).begin()).get_Group_Name() << "\t" << (*(*( *iter1 ).begin()).begin()).get_Type_from_Value() << " " << (*(*( *iter1 ).begin()).begin()).get_Value_Firm() << endl;
      }

      for ( iter_same_Ref_Firm = (*iter_same_Ref).begin(); iter_same_Ref_Firm != (*iter_same_Ref).end(); iter_same_Ref_Firm++ )
      {
        //*plog << LOG  << codec->toUnicode("          %%%%%%%%%%%%%%%% ���������� ������ ������ �������� %%%%%%%%%%%%%%%%%")  << endl;
        put_record();
      }

      iter_same_Ref_before = iter_same_Ref;
    }
    *plog << "***************************************************************************************" << endl << endl;














/*

    if (create_first_page() == 0)
    {
      while ( create_next_page() == 0 );
    }

*/







    QVector<QString>::iterator  current_page = document->begin();

    if ( open_files() )
    {
          while ( current_page != document->end() )
          {
            target_file << codec->fromUnicode( *current_page ).constData();
            current_page++;
          }

      close_files();
    }
    *plog << LOG  << "} END " << endl;

}
















void per_to_log::page_setup(void)
{

}


bool per_to_log::open_files(void)
{
      *plog << LOG  << codec->toUnicode("��������� \"") << FullCanvasFileName << codec->toUnicode("\" ����;") << endl;
      target_file.open( codec->fromUnicode(FullFileName).constData(), std::ios::out );
      if ( !target_file )
      {
        *plog << LOG  << codec->toUnicode("ERROR: �� ���� ������� \"") << FullFileName << codec->toUnicode("\" ����;") << endl;
        return false;
      }
      *plog << LOG  << codec->toUnicode("������ \"") << FullFileName << codec->toUnicode("\" ����") << endl;

    return true;
}


void per_to_log::close_files(void)
{
    target_file.close();
    *plog << LOG  << codec->toUnicode("��������� \"") << FullFileName << codec->toUnicode("\" ����") << endl;
}


void per_to_log::put_group_title(void)
{
  put_group_title_and_firm = false;

  QString Ref_current = (*(*( *iter_same_Ref ).begin()).begin()).get_RefDes();

  QVector < QVector < QVector<element> > >::iterator iter1 = (iter_same_Ref + 1);


  if( iter1 < perechen.end() )
  {
    QString Ref = (*(*( *iter1 ).begin()).begin()).get_RefDes();

    if(Ref_current == Ref)
      {
        //� ������ "�������������" ��������� ����� ������ ������ ����
         *plog << LOG  << endl;
         *plog << LOG << "### " << (*(*( *iter_same_Ref ).begin()).begin()).get_Group_Name()  <<  " ###" << endl;
      }
    else
      {
         QVector < QVector<element> >::iterator iter2 = (*iter_same_Ref).begin();
         QVector < QVector<element> >::iterator iter2_second = iter2 + 1;

         if( iter2_second < (*iter_same_Ref).end() )
         {
           //� ������ "�������������" ��������� ����� ������ ����� �����
           *plog << LOG  << endl;
           *plog << LOG << "### " << (*(*( *iter_same_Ref ).begin()).begin()).get_Group_Name() << "   " << (*(*( *iter_same_Ref ).begin()).begin()).get_Value_Firm() <<  " ###" << endl;
           put_group_title_and_firm = true;
         }
         else
         {
           //� ������ "�������������" ������ ���� ��� ������
           *plog << LOG  << endl;

         }
      }
  }
  else
  {
     //iter_same_Ref - ��������� ������� � ������
    *plog << LOG << "?????????" << endl;
  }






}



int per_to_log::create_next_page(void)
{



  PageNumber++;

  return 1;
}


int per_to_log::create_first_page(void)
{

  return 0;
}


void per_to_log::put_record(void)
{
  iter_same_Ref_Firm_Name = (*iter_same_Ref_Firm).end() - 1;

  QString RefDes_last = ( *iter_same_Ref_Firm_Name ).get_RefDes() +   QString::number(( *iter_same_Ref_Firm_Name ).get_RefDes_Count());

  iter_same_Ref_Firm_Name = (*iter_same_Ref_Firm).begin();


  if( put_group_title_and_firm == false)
  {
    *plog << LOG  << "               " << ( *iter_same_Ref_Firm_Name ).get_full_record() << "\t" << RefDes_last << endl;
  }
  else
  {
    *plog << LOG  << "               " << ( *iter_same_Ref_Firm_Name ).get_full_record_empty_firm() << "\t" << RefDes_last << endl;

  }


}


void per_to_log::create_perechen_image(void)
{

}


void per_to_log::create_first_stamp(void)
{

}


















void per_to_log::by_refdes_and_value_code(void)
{
    QVector<QVector<element> >  tables;   //��� �������� ������ � ����������� ����������
    QVector<QVector<element> >  groups;   //��� �������� ����� ����������� �������� c ���������� ������
    QVector<element>          temp_table;
    QVector<element>::iterator  ele;

    tables.clear();
    std::sort( el->begin(), el->end(), element::by_RefDes_RefDes_count ); //����������� �� Ref  � �� Des

/*
    *plog << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
        *plog << "           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
        for ( QVector<element>::iterator iter3 =  el->begin(); iter3 != el->end(); iter3++ )
        {
          *plog << "               " << ( *iter3 ).get_full_record() << endl;
        }
    *plog << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl << endl;
*/

    while ( el->empty() == false )
    {
      ele = el->begin();
      temp_table.push_back( *ele );       //�������� ������������ ������� � �������
      ele = el->erase( ele );             //������� ������ �� ������� ���� �� ��������
      QVector<element>::iterator  temp = temp_table.begin();

      for ( ; ele != el->end(); ele++ )   //������������ � ������� temp_table ��� ���������� �������� �� �������, �� ������� �������������
      {
        if ( *temp == *ele )
        {
          temp_table.push_back( *ele );
          ele = el->erase( ele );
          ele--;
        }
        else
        {
          break;
        }
      }

      tables.push_back( temp_table );
      temp_table.clear();
    }

/*
    *plog << "**************************************************************************************" << endl;
      *plog << "     ================================================================================" << endl;
      for ( QVector < QVector<element> >::iterator iter2 = tables.begin(); iter2 != tables.end(); iter2++ )
      {
        *plog << "           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
        for ( QVector<element>::iterator iter3 = (*iter2).begin(); iter3 != (*iter2).end(); iter3++ )
        {
          *plog << "               " << ( *iter3 ).get_full_record() << endl;
        }
      }
    *plog << "***************************************************************************************" << endl << endl;
*/

   perechen.clear();

   groups.push_back( *(tables.begin()) );
   tables.erase( tables.begin() );

   while ( tables.empty() == false )
   {

     if( ( (*(*tables.begin()).begin()).get_Value_Firm() == (*(*groups.begin()).begin()).get_Value_Firm() )
         && ( (*(*tables.begin()).begin()).get_RefDes() == (*(*groups.begin()).begin()).get_RefDes() ) )
     {
        groups.push_back( *(tables.begin()) );
        tables.erase( tables.begin() );
     }
     else
     {
       perechen.push_back( groups );
       groups.clear();
       groups.push_back( *(tables.begin()) );
       tables.erase( tables.begin() );
     }

   }

   if( groups.empty() == false  )
   {
       perechen.push_back( groups );
   }


/*
       *plog << "**************************************************************************************" << endl;
       for ( QVector < QVector < QVector<element> > >::iterator iter1 = perechen.begin(); iter1 != perechen.end(); iter1++ )
       {
         *plog << "     ================================================================================" << endl;
         for ( QVector < QVector<element> >::iterator iter2 = (*iter1).begin(); iter2 != (*iter1).end(); iter2++ )
         {
           *plog << "           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
           for ( QVector<element>::iterator iter3 = (*iter2).begin(); iter3 != (*iter2).end(); iter3++ )
           {
             *plog << "               " << ( *iter3 ).get_full_record() << endl;
           }
         }
       }
       *plog << "***************************************************************************************" << endl << endl;
*/


    for ( QVector < QVector < QVector<element> > >::iterator iter1 = perechen.begin(); iter1 != perechen.end(); iter1++ ) //��������� ��� ���������� ��������, ������� ���������.
    {
      for ( QVector < QVector<element> >::iterator iter2 = (*iter1).begin(); iter2 != (*iter1).end(); iter2++ )
      {
        unsigned int count = 0;
        for ( QVector<element>::iterator iter3 = (*iter2).begin(); iter3 != (*iter2).end(); iter3++ )
        {
            count += ( *iter3 ).get_Count();
            ( *iter3 ).set_Count(0);
        }
        (*( *iter2 ).begin()).set_Count(count);
      }
    }

/*
       *plog << LOG  << "***************************************************************************************************" << endl;
      QVector < QVector < QVector<element> > >::iterator iter1_before = NULL;
      for ( QVector < QVector < QVector<element> > >::iterator iter1 = perechen.begin(); iter1 != perechen.end(); iter1++ )
       {
         if( (iter1_before == NULL ) || ( (*(*( *iter1_before ).begin()).begin()).get_RefDes() != (*(*( *iter1 ).begin()).begin()).get_RefDes() ) ) //�������� Ref ������
           {
             QString Ref = (*(*( *iter1 ).begin()).begin()).get_RefDes();
             if( (Ref == "A") || (Ref == "B") )
               {

               }
              else
               *plog << LOG << "### " << (*(*( *iter1 ).begin()).begin()).get_Group_Name() << " ###" << endl;
           }
         *plog << LOG  << "   $$$$$$$$$$$ " << codec->toUnicode("��������, ������������� � ����� �����") << " $$$$$$$$$$$" << endl;
         if( (*iter1).size() > 1  )
         {
       //    *plog << LOG << "    ======= "<< (*(*( *iter1 ).begin()).begin()).get_Group_Name() << "\t" << (*(*( *iter1 ).begin()).begin()).get_Types_from_Value() << " " << (*(*( *iter1 ).begin()).begin()).get_Value_Firm() << endl;
         }
         else
         {
       //      *plog << LOG  << "    ======= "<<  (*(*( *iter1 ).begin()).begin()).get_Group_Name() << "\t" << (*(*( *iter1 ).begin()).begin()).get_Type_from_Value() << " " << (*(*( *iter1 ).begin()).begin()).get_Value_Firm() << endl;
         }

         for ( QVector < QVector<element> >::iterator iter2 = (*iter1).begin(); iter2 != (*iter1).end(); iter2++ )
         {
           *plog << LOG  << codec->toUnicode("          %%%%%%%%%%%%%%%% ���������� ������ ������ �������� %%%%%%%%%%%%%%%%%")  << endl;
           for ( QVector<element>::iterator iter3 = (*iter2).begin(); iter3 != (*iter2).end(); iter3++ )
           {
             *plog << LOG  << "               " << ( *iter3 ).get_full_record() << endl;
           }
         }

         iter1_before = iter1;
       }
       *plog << "***************************************************************************************" << endl << endl;
*/
}


