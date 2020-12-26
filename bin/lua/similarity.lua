--cp1251
--[==[
/*
 * This file is part of the "VP_auto" distribution (https://github.com/yrasik/VP_auto).
 * Copyright (c) 2020 Yuri Stepanenko.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
]==]


local similarity = {}

local function version()
  return 'v 1.0'
end


local function split(instr, sep)
  local t = {}
  for str in string.gmatch(instr, "([^"..sep.."]+)") do
    table.insert(t, str)
  end
  return t
end


local function trim(instr)
  return string.gsub(instr, "^%s*(.-)%s*$", "%1")
end


local function replace(str, what, with)
  what = what:gsub("[%(%)%.%+%-%*%?%[%]%^%$%%]", "%%%1")
  with = with:gsub("[%%]", "%%%%")
  return str:gsub(what, with)
end


local SS = {
  SAME                      = {0, "������� ����������" },
  SAME_IN_TRIMMED           = {-1, "������� �������� ��� �������� �������� � ��������� �� �����"},
  SAME_IN_LOWER             = {-2, "������� �������� ��� �������� � ������ �������"},
  SAME_IN_SPACE             = {-3, "������� �������� ��� �������� � ������ �������, �������� �������� � ���������"},
  SAME_IN_COMMA             = {-4, "������� �������� ��� �������� � ������ �������, �������� ��������  � ������ ���� ����� ��������"},
  SAME_IN_TEXTMINUS         = {-5, "������� �������� ��� �������� � ������ �������, �������� ��������, ������ ���� ����� ��������, ������ ��������/�������� ���� �� '-' (�����)"},
  SAME_IN_TEXTPM            = {-6, "������� �������� ��� �������� � ������ �������, �������� ��������, ������ ���� ����� ��������, ������ '+-' �� '�'"},
  SAME_IN_RUS_C             = {-7, "������� �������� ��� ������ ������� ���� '�' �� ��������� (������� �� �������� ���� �����)"},
  SAME_IN_RUS_A             = {-8, "������� �������� ��� ������ ������� ���� '�' �� ��������� (������� �� �������� ���� �����)"},
  SAME_IN_RUS_E             = {-9, "������� �������� ��� ������ ������� ���� '�' �� ��������� (������� �� �������� ���� �����)"},
  SAME_IN_RUS_P             = {-10, "������� �������� ��� ������ ������� ���� '�' �� ��������� (������� �� �������� ���� �����)"},
  SAME_IN_RUS_T             = {-11, "������� �������� ��� ������ ������� ���� '�' �� ��������� (������� �� �������� ���� �����)"},
  SAME_IN_RUS_H             = {-12, "������� �������� ��� ������ ������� ���� '�' �� ��������� (������� �� �������� ���� �����)"},
  SAME_IN_RUS_K             = {-13, "������� �������� ��� ������ ������� ���� '�' �� ��������� (������� �� �������� ���� �����)"},
  SAME_IN_RUS_X             = {-14, "������� �������� ��� ������ ������� ���� '�' �� ��������� (������� �� �������� ���� �����)"},
  SAME_IN_RUS_B             = {-15, "������� �������� ��� ������ ������� ���� '�' �� ��������� (������� �� �������� ���� �����)"},
  SAME_IN_RUS_M             = {-16, "������� �������� ��� ������ ������� ���� '�' �� ��������� (������� �� �������� ���� �����)"},
  SAME_IN_RUS_O             = {-17, "������� �������� ��� ������ ������� ���� '�' �� ��������� (������� �� �������� ���� �����)"},
  SAME_SUBSTRING            = {-18, "������� �������� ��� �������� � ������ �������, �������� ��������, ������ ���� ����� ��������, ������ '+-' �� '�' ������ �� ������"},
  SAME_PLUS_OR_X__IN_END    = {-19, "������� �������� ��� �������� � ������ �������, �������� ��������, ������ ���� ����� ��������, ������ '+-' �� '�' ������ �� ������, �������� '+' � 'x' �� ����� ������"},
  SAME_PLUSES_OR_XX__IN_END = {-20, "������� �������� ��� �������� � ������ �������, �������� ��������, ������ ���� ����� ��������, ������ '+-' �� '�' ������ �� ������, ���������� '+' � 'x' �� ����� ������"},
  EMPTY                     = {-21, "���� ���� ������"},
  
  DIFFERENT                 = {-30, "�������� �� �������"},
}


local charList = {
  { '�', '�' },
  { '�', '�' },
  { '�', '�' },
  { '�', '�' },
  { '�', '�' },
  { '�', '�' },
  { '�', '�' },
  { '�', '�' },
  { '�', '�' },
  { '�', '�' },
  
  { '�', '�' },
  { '�', '�' },
  { '�', '�' },
  { '�', '�' },
  { '�', '�' },
  { '�', '�' },
  { '�', '�' },
  { '�', '�' },
  { '�', '�' },
  { '�', '�' },
  
  { '�', '�' },
  { '�', '�' },
  { '�', '�' },
  { '�', '�' },
  { '�', '�' },
  { '�', '�' },
  { '�', '�' },
  { '�', '�' },
  { '�', '�' },
  { '�', '�' },
  
  { '�', '�' },
  { '�', '�' },
  { '�', '�' },
}


local function toLower(text)
  for i = 1, #charList do
  --print(i..'  '..charList[i][1])
    text = string.gsub(text, "%"..charList[i][1], charList[i][2])
  end
  text = string.lower(text)
  
  --local log = "toLower >> '"..text.."' '"
  --print(log)
  return text
end


local function is_Similarity(str1, str2)
  local temp_str1 = str1
  local temp_str2 = str2

  if( #temp_str1 == 0 and #temp_str2 > 0 ) then
    return SS.EMPTY   -- ������ ����. ������ ��� ���������� 
  end

  if( #temp_str1 > 0 and #temp_str2 == 0 ) then
    return SS.EMPTY   -- ������ ����. ������ ��� ���������� 
  end

  if( temp_str1 == temp_str2 ) then
    return SS.SAME   -- ���� � ����
  end

  --������ ������ � ��������� �� �����
  temp_str1 = trim(temp_str1);
  temp_str2 = trim(temp_str2);

  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_TRIMMED
  end

  -------- ��������� �������� �������� ��� �������� � ������ ������� -----------
  temp_str1 = toLower(temp_str1)
  temp_str2 = toLower(temp_str2)

  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_LOWER
  end

  ------ ��������� ������� ��� ������� � ����� �������� �������� ---------
  temp_str1 = temp_str1:gsub('%s', '')
  temp_str2 = temp_str2:gsub('%s', '')
  
  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_SPACE
  end

  --------------- ��������� �������� ��� ����� �������� ------------------
  temp_str1 = temp_str1:gsub('%.', ',')
  temp_str2 = temp_str2:gsub('%.', ',')

  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_COMMA
  end

  --------------- ��������� �������� ��� ����� "�"(������� ����) ������ "-" -------------
  --------------- ��������� �������� ��� ����� "�"(������� ����) ������ "-" -------------
  temp_str1 = temp_str1:gsub('[%�%�]', '-')
  temp_str2 = temp_str2:gsub('[%�%�]', '-')
  
  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_TEXTMINUS
  end
  
  
 -- print(' "'..temp_str1..'"')
 -- print(' "'..temp_str2..'"')

  --------------- ��������� �������� ��� ����� "+-" ������ "�" -------------
  temp_str1 = temp_str1:gsub('%+%-', '�')
  temp_str2 = temp_str2:gsub('%+%-', '�')

  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_TEXTPM
  end

  ------- ��������� �������� ��� ������� "�" �� ���������� "c" ---------------
  temp_str1 = temp_str1:gsub('�', 'c')
  temp_str2 = temp_str2:gsub('�', 'c')

  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_RUS_C
  end

  ------- ��������� �������� ��� ������� "�" �� ���������� "�" ---------------
  temp_str1 = temp_str1:gsub('�', 'a')
  temp_str2 = temp_str2:gsub('�', 'a')

  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_RUS_A
  end

  ------- ��������� �������� ��� ������� "�" �� ���������� "e" ---------------
  temp_str1 = temp_str1:gsub('�', 'e')
  temp_str2 = temp_str2:gsub('�', 'e')

  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_RUS_E
  end

  ------- ��������� �������� ��� ������� "�" �� ���������� "p" ---------------
  temp_str1 = temp_str1:gsub('�', 'p')
  temp_str2 = temp_str2:gsub('�', 'p')

  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_RUS_P
  end

  ------- ��������� �������� ��� ������� "�" �� ���������� "T" ---------------
  temp_str1 = temp_str1:gsub('�', 't')
  temp_str2 = temp_str2:gsub('�', 't')

  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_RUS_T
  end


  ------- ��������� �������� ��� ������� "�" �� ���������� "H" ---------------
  temp_str1 = temp_str1:gsub('�', 'h')
  temp_str2 = temp_str2:gsub('�', 'h')

  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_RUS_H
  end

  ------- ��������� �������� ��� ������� "�" �� ���������� "K" ---------------
  temp_str1 = temp_str1:gsub('�', 'k')
  temp_str2 = temp_str2:gsub('�', 'k')
  
  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_RUS_K
  end

  ------- ��������� �������� ��� ������� "�" �� ���������� "X" ---------------
  temp_str1 = temp_str1:gsub('�', 'x')
  temp_str2 = temp_str2:gsub('�', 'x')

  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_RUS_X
  end

  ------- ��������� �������� ��� ������� "�" �� ���������� "B" ---------------
  temp_str1 = temp_str1:gsub('�', 'b')
  temp_str2 = temp_str2:gsub('�', 'b')

  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_RUS_B
  end

  ------- ��������� �������� ��� ������� "�" �� ���������� "M" ---------------
  temp_str1 = temp_str1:gsub('�', 'm')
  temp_str2 = temp_str2:gsub('�', 'm')

  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_RUS_M
  end

  ------- ��������� �������� ��� ������� "�" �� ���������� "O" ---------------
  temp_str1 = temp_str1:gsub('�', 'o')
  temp_str2 = temp_str2:gsub('�', 'o')

  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_RUS_O
  end
  
  ------ ��������� �������� ��������� ������ � ������ ������ (������ ����� � ������������) ------
  if( (#temp_str1 > 3) and
      (#temp_str2 > 3)
    )
  then
    if ( #temp_str1 < #temp_str2 ) then
      if ( string.find(temp_str2, temp_str1, 1, true) ~=nil ) then -- ����� ��������� � ������
        return SS.SAME_SUBSTRING
      end
    else
      if ( string.find(temp_str1, temp_str2, 1, true) ~=nil ) then --����� ��������� � ������
        return SS.SAME_SUBSTRING
      end
    end
  else
    return SS.DIFFERENT
  end


  -------------------------------------------------------------------------------
  temp_str1 = temp_str1:gsub('[x%+]*$', '')
  temp_str2 = temp_str2:gsub('[x%+]*$', '')

  if( (#temp_str1 > 3) and
      (#temp_str2 > 3)
    )
  then
    if ( #temp_str1 < #temp_str2 ) then
      if ( string.find(temp_str2, temp_str1, 1, true) ~=nil ) then -- ����� ��������� � ������
        return SS.SAME_PLUS_OR_X__IN_END
      end
    else
      if ( string.find(temp_str1, temp_str2, 1, true) ~=nil ) then --����� ��������� � ������
        return SS.SAME_PLUS_OR_X__IN_END;
      end
    end
  else
    return SS.DIFFERENT
  end


  temp_str1 = temp_str1:gsub('[x%+]+$', '')
  temp_str2 = temp_str2:gsub('[x%+]+$', '')

  if( (#temp_str1 > 3) and
      (#temp_str2 > 3)
    )
  then
    if ( #temp_str1 < #temp_str2 ) then
      if ( string.find(temp_str2, temp_str1, 1, true) ~=nil ) then -- ����� ��������� � ������
        return SS.SAME_PLUSES_OR_XX__IN_END
      end
    else
      if ( string.find(temp_str1, temp_str2, 1, true) ~=nil ) then --����� ��������� � ������
        return SS.SAME_PLUSES_OR_XX__IN_END;
      end
    end
  else
    return SS.DIFFERENT
  end



  --local log = "'"..temp_str1.."' <> '"..temp_str2.."'"
  --print(log)

  return SS.DIFFERENT  --����� ������� ��������
end






return {
  version = version,
  is_Similarity = is_Similarity,
}


