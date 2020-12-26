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


local vp_auto_2_lua = {}


local function version()
  return 'v 2.0a'
end


local function info()
  local info = 
[[
  Модуль для манипуляций с выходным файлом программы 'VP_auto' '*_для_заказа.txt' (файл со списком ПКИ для изделия).
  Выполняет:
    1) преобразует файл в таблицу lua.
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
    elseif a == "--fin_names_lua" then
      t.fin_names_lua = arg[i + 1]
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


local function replace(str, what, with)
  what = what:gsub("[%(%)%.%+%-%*%?%[%]%^%$%%]", "%%%1")
  with = with:gsub("[%%]", "%%%%")
  return str:gsub(what, with)
end  


local preamble = 
[[
-- codepage cp1251

if( type(DeviceBase) ~= 'table' ) then
  DeviceBase = {}
end


]]


local template = 
[[
DeviceBase[#DeviceBase + 1] = {
  Ref = "",
  Group_Name = "",
  Type_from_Value = "%s",
  Code_from_Value = "%s",
  Value_Decode = "%s",
  Value_Firm = "%s",
  Function = "",
  Parameters = "",
  Package_Type = "",
  Package_Size = "",
  Description = "",
  Op_Temp = "",
  Value = "",
  Old_Values = {
  },

  Items = "%s"
}


]]


local function exec(fin, fout, flog)

end



------------ main ------------

if arg[0]:find('vp_auto_2_lua.lua') ~= nil then
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

