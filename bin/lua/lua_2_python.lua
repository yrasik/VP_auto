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


local lua_2_python = {}


local function version()
  return 'v 1.0a'
end


local function info()
  local info = 
[[
  Модуль для манипуляций с таблицей элементов lua.
  Выполняет:
    1) преобразует файл в таблицу python.
  
    
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
      print(info().."Bad flag: "..a)
      os.exit(-1)
    end
  end
  return t
end


function dofile1(ilename)
  local f = assert(loadfile(filename))
  return asert(f(), 'ERROR in file "'..filename..'"')
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
#-- codepage cp1251

try:
    if type(Base).__name__ != 'list': raise
except:
    Base = []


]]


local template = 
[==[
Base.append(
{
  'Ref' : '%s',
  'Group_Name' : '%s',
  'Type_from_Value' : '%s',
  'Code_from_Value' : '%s',
  'Value_Decode' : '%s',
  'Value_Firm' : '%s',
  'Function' : '%s',
  'Parameters' : '%s',
  'Package_Type' : '%s',
  'Package_Size' : '%s',
  'Description' : '%s',
  'Op_Temp' : '%s',
  'Value' : '%s',
  'Old_Values' : {%s
  },
  'TO_Values' : {%s
  }
}
)


]==]


local template_TO_Values = 
[==[

    'text_prev' : '%s',
    'table' : '%s',
    'text_post_rus' : '%s',
    'text_post_en' : '%s'
]==]






local function exec(base, fout, flog, Name)
  fout:write(preamble)
  for i = 1, #base do
    local element = base[i]
      
    element.Function = element.Function:gsub('\n','\\\n')
    element.Parameters = element.Parameters:gsub('\n','\\\n')      
      
    local TO_Value = {} 
    if ( type(element.TO_Values) == 'nil' ) then
      TO_Value.text_prev = ''
      TO_Value.table = ''
      TO_Value.text_post_rus = ''
      TO_Value.text_post_en = ''
    else
      TO_Value.text_prev = element.TO_Values.text_prev:gsub('\n','\\\n')
      TO_Value.table = element.TO_Values.table:gsub('\n','\\\\\n')
      TO_Value.text_post_rus = element.TO_Values.text_post_rus:gsub('\n','\\\n')
      TO_Value.text_post_en = element.TO_Values.text_post_en:gsub('\n','\\\n')
    end
    
    local TO_Values = string.format(
                        template_TO_Values,
                        TO_Value.text_prev,
                        TO_Value.table,
                        TO_Value.text_post_rus,
                        TO_Value.text_post_en
                      )
    
    
    local str = string.format(
                  template,
                  element.Ref,
                  element.Group_Name,
                  element.Type_from_Value,
                  element.Code_from_Value,
                  element.Value_Decode,
                  element.Value_Firm,
                  element.Function,
                  element.Parameters,
                  element.Package_Type,
                  element.Package_Size,
                  element.Description,
                  element.Op_Temp,
                  element.Value,
                  '',
                  TO_Values
                )

    fout:write(str)
  end
end


------------ main ------------

if arg[0]:find('lua_2_python.lua') ~= nil then
  local args = process_args()

  local flog = io.open(args.flog_name,"w");
  if ( flog == nil ) then
    os.exit(-1)
  end



  dofile( args.fin_name ) -- DeviceBase = {}



  local fout = io.open(args.fout_name,"w");
  if ( fout == nil ) then
    flog:close()
    os.exit(-1)
  end

  exec(Base, fout, flog, Name)

  fout:close()
  flog:close()

  os.exit(0)
end


return {
  version = version,
  info = info,
  get_data_from_value = get_data_from_value,
  exec = exec
}

