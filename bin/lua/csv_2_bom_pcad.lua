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


local csv_2_bom_pcad = {}


local function version()
  return 'v 1.0a'
end


local function info()
  local info =
[[
  Модуль для преобразования файлов *.CSV (Comma-Separated Values - упрощённый вывод таблиц Microsoft Excel/LibreOffice Calc) в *.bom - файл для VP_auto (стиль bom P-CAD 2006).
  Выполняет:
    1) преобразует файл со строками вида: 
        "<RefDes>";"<Расшифровка названия>";"<Фирменное название>";"<Фирма>"
      в файл со строками вида:
        "<RefDes>"@"1"@"<Расшифровка названия>\<Фирменное название>\<Фирма>"
    2) убирает лишние пробелы в названиях.
]]
  return info
end


local function process_args()
  -- get args set by user in command line
  local t, i = {}, 1
  while i < #arg do
    local a = arg[i]
    if a == "--fin" then
      t.fin_name = arg[i + 1]
      i = i + 2
    elseif a == "--fout" then
      t.fout_name = arg[i + 1]
      i = i + 2
    elseif a == "--flog" then
      t.flog_name = arg[i + 1]
      i = i + 2
    else
      print(usage.."Bad flag: "..a)
      os.exit(-1)
    end
  end
  return t
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


local template = 
[["%s"@"%s"@"%s\%s\%s"
]]


local function exec(fin, fout, flog)
  local line_num = 1

  local line = 1
  for l in fin:lines() do 
    local fields = split(l, ';')

    if (fields[1] ~= nil and
        fields[2] ~= nil and
        fields[3] ~= nil and
        fields[4] ~= nil
       ) then

      for i = 1, 4 do
        fields[i] = replace(fields[i], '"', '')
        fields[i] = string.gsub(fields[i], '[%s]+', ' ')
        fields[i] = string.gsub(fields[i], ' $', '')
        fields[i] = string.gsub(fields[i], '^ ', '')
      end

      local str = string.format(template,
                                fields[1],
                                '1',
                                fields[2],
                                fields[3],
                                fields[4]
                               )
      fout:write(str)
    else
      err = 'WARNING: ignored line '..line..' "'..l..'"\n'
      flog:write(err)
    end

    line = line + 1
  end
end



------------ main ------------

if arg[0]:find('csv_2_bom_pcad.lua') ~= nil then
  local args = process_args()

  local fin = io.open(args.fin_name, 'r')
  if ( fin == nil ) then
    os.exit(-1)
  end

  local fout = io.open(args.fout_name,"w");
  if ( fout == nil ) then
    fin:close()
    os.exit(-1)
  end

  local flog = io.open(args.flog_name,"w");
  if ( flog == nil ) then
    fin:close()
    fout:close()
    os.exit(-1)
  end

  exec(fin, fout, flog)

  fin:close()
  fout:close()
  flog:close()

  os.exit(0)
end


return {
  version = version,
  info = info,
  exec = exec
}


