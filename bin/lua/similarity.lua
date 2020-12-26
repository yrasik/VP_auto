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
  SAME                      = {0, "найдено совпадение" },
  SAME_IN_TRIMMED           = {-1, "найдено сходство при удалении пробелов и табуляций по краям"},
  SAME_IN_LOWER             = {-2, "найдено сходство при переводе в нижний регистр"},
  SAME_IN_SPACE             = {-3, "найдено сходство при переводе в нижний регистр, удалении пробелов и табуляций"},
  SAME_IN_COMMA             = {-4, "найдено сходство при переводе в нижний регистр, удалении пробелов  и замене всех точек запятыми"},
  SAME_IN_TEXTMINUS         = {-5, "найдено сходство при переводе в нижний регистр, удалении пробелов, замене всех точек запятыми, замене длинного/среднего тире на '-' (минус)"},
  SAME_IN_TEXTPM            = {-6, "найдено сходство при переводе в нижний регистр, удалении пробелов, замене всех точек запятыми, замене '+-' на '±'"},
  SAME_IN_RUS_C             = {-7, "найдено сходство при замене русских букв 'с' на латинские (похожие по внешнему виду буквы)"},
  SAME_IN_RUS_A             = {-8, "найдено сходство при замене русских букв 'а' на латинские (похожие по внешнему виду буквы)"},
  SAME_IN_RUS_E             = {-9, "найдено сходство при замене русских букв 'е' на латинские (похожие по внешнему виду буквы)"},
  SAME_IN_RUS_P             = {-10, "найдено сходство при замене русских букв 'р' на латинские (похожие по внешнему виду буквы)"},
  SAME_IN_RUS_T             = {-11, "найдено сходство при замене русских букв 'Т' на латинские (похожие по внешнему виду буквы)"},
  SAME_IN_RUS_H             = {-12, "найдено сходство при замене русских букв 'Н' на латинские (похожие по внешнему виду буквы)"},
  SAME_IN_RUS_K             = {-13, "найдено сходство при замене русских букв 'К' на латинские (похожие по внешнему виду буквы)"},
  SAME_IN_RUS_X             = {-14, "найдено сходство при замене русских букв 'Х' на латинские (похожие по внешнему виду буквы)"},
  SAME_IN_RUS_B             = {-15, "найдено сходство при замене русских букв 'В' на латинские (похожие по внешнему виду буквы)"},
  SAME_IN_RUS_M             = {-16, "найдено сходство при замене русских букв 'М' на латинские (похожие по внешнему виду буквы)"},
  SAME_IN_RUS_O             = {-17, "найдено сходство при замене русских букв 'О' на латинские (похожие по внешнему виду буквы)"},
  SAME_SUBSTRING            = {-18, "найдено сходство при переводе в нижний регистр, удалении пробелов, замене всех точек запятыми, замене '+-' на '±' поиска по строке"},
  SAME_PLUS_OR_X__IN_END    = {-19, "найдено сходство при переводе в нижний регистр, удалении пробелов, замене всех точек запятыми, замене '+-' на '±' поиска по строке, удалении '+' и 'x' из конца строки"},
  SAME_PLUSES_OR_XX__IN_END = {-20, "найдено сходство при переводе в нижний регистр, удалении пробелов, замене всех точек запятыми, замене '+-' на '±' поиска по строке, нескольких '+' и 'x' из конца строки"},
  EMPTY                     = {-21, "одно поле пустое"},
  
  DIFFERENT                 = {-30, "сходство не найдено"},
}


local charList = {
  { 'А', 'а' },
  { 'Б', 'б' },
  { 'В', 'в' },
  { 'Г', 'г' },
  { 'Д', 'д' },
  { 'Е', 'е' },
  { 'Ё', 'ё' },
  { 'Ж', 'ж' },
  { 'З', 'з' },
  { 'И', 'и' },
  
  { 'Й', 'й' },
  { 'К', 'к' },
  { 'Л', 'л' },
  { 'М', 'м' },
  { 'Н', 'н' },
  { 'О', 'о' },
  { 'П', 'п' },
  { 'Р', 'р' },
  { 'С', 'с' },
  { 'Т', 'т' },
  
  { 'У', 'у' },
  { 'Ф', 'ф' },
  { 'Х', 'х' },
  { 'Ц', 'ц' },
  { 'Ч', 'ч' },
  { 'Ш', 'ш' },
  { 'Щ', 'щ' },
  { 'Ъ', 'ъ' },
  { 'Ы', 'ы' },
  { 'Ь', 'ь' },
  
  { 'Э', 'э' },
  { 'Ю', 'ю' },
  { 'Я', 'я' },
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
    return SS.EMPTY   -- Пустое поле. Смысла нет сравнивать 
  end

  if( #temp_str1 > 0 and #temp_str2 == 0 ) then
    return SS.EMPTY   -- Пустое поле. Смысла нет сравнивать 
  end

  if( temp_str1 == temp_str2 ) then
    return SS.SAME   -- Одно и тоже
  end

  --Убрали проелы и табуляцию по краям
  temp_str1 = trim(temp_str1);
  temp_str2 = trim(temp_str2);

  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_TRIMMED
  end

  -------- Попробуем поискать сходство при переводе в нижний регистр -----------
  temp_str1 = toLower(temp_str1)
  temp_str2 = toLower(temp_str2)

  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_LOWER
  end

  ------ Попробуем удалить все пробелы и опять поискать сходство ---------
  temp_str1 = temp_str1:gsub('%s', '')
  temp_str2 = temp_str2:gsub('%s', '')
  
  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_SPACE
  end

  --------------- Попробуем заменить все точки запятыми ------------------
  temp_str1 = temp_str1:gsub('%.', ',')
  temp_str2 = temp_str2:gsub('%.', ',')

  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_COMMA
  end

  --------------- Попробуем заменить все знаки "—"(длинное тире) знаком "-" -------------
  --------------- Попробуем заменить все знаки "–"(среднее тире) знаком "-" -------------
  temp_str1 = temp_str1:gsub('[%—%–]', '-')
  temp_str2 = temp_str2:gsub('[%—%–]', '-')
  
  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_TEXTMINUS
  end
  
  
 -- print(' "'..temp_str1..'"')
 -- print(' "'..temp_str2..'"')

  --------------- Попробуем заменить все знаки "+-" знаком "±" -------------
  temp_str1 = temp_str1:gsub('%+%-', '±')
  temp_str2 = temp_str2:gsub('%+%-', '±')

  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_TEXTPM
  end

  ------- Попробуем заменить все русские "с" на английские "c" ---------------
  temp_str1 = temp_str1:gsub('с', 'c')
  temp_str2 = temp_str2:gsub('с', 'c')

  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_RUS_C
  end

  ------- Попробуем заменить все русские "а" на английские "а" ---------------
  temp_str1 = temp_str1:gsub('а', 'a')
  temp_str2 = temp_str2:gsub('а', 'a')

  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_RUS_A
  end

  ------- Попробуем заменить все русские "е" на английские "e" ---------------
  temp_str1 = temp_str1:gsub('е', 'e')
  temp_str2 = temp_str2:gsub('е', 'e')

  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_RUS_E
  end

  ------- Попробуем заменить все русские "р" на английские "p" ---------------
  temp_str1 = temp_str1:gsub('р', 'p')
  temp_str2 = temp_str2:gsub('р', 'p')

  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_RUS_P
  end

  ------- Попробуем заменить все русские "Т" на английские "T" ---------------
  temp_str1 = temp_str1:gsub('т', 't')
  temp_str2 = temp_str2:gsub('т', 't')

  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_RUS_T
  end


  ------- Попробуем заменить все русские "Н" на английские "H" ---------------
  temp_str1 = temp_str1:gsub('н', 'h')
  temp_str2 = temp_str2:gsub('н', 'h')

  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_RUS_H
  end

  ------- Попробуем заменить все русские "К" на английские "K" ---------------
  temp_str1 = temp_str1:gsub('к', 'k')
  temp_str2 = temp_str2:gsub('к', 'k')
  
  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_RUS_K
  end

  ------- Попробуем заменить все русские "Х" на английские "X" ---------------
  temp_str1 = temp_str1:gsub('х', 'x')
  temp_str2 = temp_str2:gsub('х', 'x')

  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_RUS_X
  end

  ------- Попробуем заменить все русские "В" на английские "B" ---------------
  temp_str1 = temp_str1:gsub('в', 'b')
  temp_str2 = temp_str2:gsub('в', 'b')

  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_RUS_B
  end

  ------- Попробуем заменить все русские "М" на английские "M" ---------------
  temp_str1 = temp_str1:gsub('м', 'm')
  temp_str2 = temp_str2:gsub('м', 'm')

  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_RUS_M
  end

  ------- Попробуем заменить все русские "О" на английские "O" ---------------
  temp_str1 = temp_str1:gsub('о', 'o')
  temp_str2 = temp_str2:gsub('о', 'o')

  if( temp_str1 == temp_str2 ) then
    return SS.SAME_IN_RUS_O
  end
  
  ------ Попробуем поискать фрагменты строки в другой строке (лишние буквы в наименовании) ------
  if( (#temp_str1 > 3) and
      (#temp_str2 > 3)
    )
  then
    if ( #temp_str1 < #temp_str2 ) then
      if ( string.find(temp_str2, temp_str1, 1, true) ~=nil ) then -- Нашли подстроку в строке
        return SS.SAME_SUBSTRING
      end
    else
      if ( string.find(temp_str1, temp_str2, 1, true) ~=nil ) then --Нашли подстроку в строке
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
      if ( string.find(temp_str2, temp_str1, 1, true) ~=nil ) then -- Нашли подстроку в строке
        return SS.SAME_PLUS_OR_X__IN_END
      end
    else
      if ( string.find(temp_str1, temp_str2, 1, true) ~=nil ) then --Нашли подстроку в строке
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
      if ( string.find(temp_str2, temp_str1, 1, true) ~=nil ) then -- Нашли подстроку в строке
        return SS.SAME_PLUSES_OR_XX__IN_END
      end
    else
      if ( string.find(temp_str1, temp_str2, 1, true) ~=nil ) then --Нашли подстроку в строке
        return SS.SAME_PLUSES_OR_XX__IN_END;
      end
    end
  else
    return SS.DIFFERENT
  end



  --local log = "'"..temp_str1.."' <> '"..temp_str2.."'"
  --print(log)

  return SS.DIFFERENT  --Очень большие различия
end






return {
  version = version,
  is_Similarity = is_Similarity,
}


