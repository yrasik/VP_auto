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

#ifndef PCAD_PER_FOR_SCH_H
  #define PCAD_PER_FOR_SCH_H

  #include "pcad_per_for_sch.h"


class pcad_per_for_doc_includes : public per
{
  public:
    pcad_per_for_doc_includes ( QString &PathToOutDir, QVector<element> *el_ ):per(PathToOutDir, el_)
    {

    }
  protected:
    void  page_setup ( void );
    bool  open_files ( void );
};
#endif // PCAD_PER_FOR_SCH_H

