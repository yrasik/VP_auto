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


local names_2_lua = {}


local function version()
  return 'v 2.0a'
end


local function info()
  local info =
[[
  Модуль для манипуляций с файлом 'names.txt' (файл с названиями элементов в единственном/множественном числе).
  Выполняет:
    1) преобразует файл в таблицу lua
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


local preamble = 
[[
-- codepage cp1251

if( type(Name) ~= 'table' ) then
  Name = {}
end


]]


local template = 
[[
Name[#Name + 1] = {
  Ref = "%s",
  Name = "%s",
  Names = "%s",
  Group_Name_one = "%s",
  Group_Name = "%s"
}


]]


local function exec(fin, fout, flog)
  local line_num = 1

  fout:write(preamble)

  local line = 1
  for l in fin:lines() do 
    local it_comment = l:find('^%s*#')

    if ( it_comment == nil ) then
      fields = split(l, '|')

      if (fields[1] ~= nil and
          fields[2] ~= nil and
          fields[3] ~= nil and
          fields[4] ~= nil and
          fields[5] ~= nil 
         ) then

        local str = string.format(template,
                                 fields[1],
                                 fields[2],
                                 fields[3],
                                 fields[4],
                                 fields[5]
                                )

        fout:write(str)
      else
        err = 'WARNING: ignored line '..line..' "'..l..'"\n'
        flog:write(err)
      end
    else
      err = 'WARNING: ignored line '..line..' "'..l..'"\n'
      flog:write(err)
    end
    line = line + 1
  end

end



------------ main ------------

if arg[0]:find('names_2_lua.lua') ~= nil then
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


