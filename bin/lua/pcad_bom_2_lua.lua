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

local pcad_bom_2_lua = {}


local function version()
  return 'v 1.0a'
end


local function info()
  local info = 
[[
  Модуль для манипуляций с выходным файлом программы P-CAD 2006 SP2 SCH ASCII '*.bom' (файл со списком ПКИ для перечня элементов).
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

if( type(DeviceBaseTree) ~= 'table' ) then
  DeviceBaseTree = {}
end

DeviceBaseTree[#DeviceBaseTree + 1] = {}


DeviceBaseTree[#DeviceBaseTree].info = {
  bom_file_name = "%s",
  device_id = "",
}


DeviceBaseTree[#DeviceBaseTree].PKI = {}
local PKI = DeviceBaseTree[#DeviceBaseTree].PKI

]]


local template = 
[[
PKI[#PKI + 1] = {
  Ref = "%s",
  Des = "%s",
  Group_Name = "",
  Type_from_Value = "",
  Code_from_Value = "",
  Value_Decode = "",
  Value_Firm = "",
  Function = "",
  Parameters = "",
  Package_Type = "",
  Package_Size = "",
  Description = "",
  Op_Temp = "",
  Value = "%s",
  Old_Values = {
  },

}


]]


local function exec(fin, fout, flog, bom_file_name)
  local str = string.format( preamble, 
                             bom_file_name
                            )
  fout:write(str)
  local line_num = 1

  for l in fin:lines() do 
    --print (l)
    local line_is_good = true
    local field = split(l, '@')
    local Ref, Des
    local Value
    
    if(field[1] == nil or field[2] == nil or field[3] == nil) then
      line_is_good = false
      flog:write('WARNING: Игнорируется строка '..line_num..'": '..l..'"\n')
    end
    
    if ( line_is_good == true ) then
      field[1] = field[1]:gsub('"', '')
      Ref, Des = field[1]:match('^([%D]+)([%d]+)')
      Value = field[3]
    end

    if ( line_is_good == true ) then
      if ( Value == nil ) then
        flog:write('WARNING: Игнорируется строка '..line_num..'": '..l..'"\n')
        flog:write('  if ( Value == nil )\n')
        line_is_good = false
      end
    end
    
    if ( line_is_good == true ) then
      if ( Ref == nil ) then
        flog:write('WARNING: Игнорируется строка '..line_num..'": '..l..'"\n')
        flog:write('  if ( Ref == nil )\n')
        line_is_good = false
      end
    end
    
    if ( line_is_good == true ) then
      if ( Des == nil ) then
        flog:write('WARNING: Игнорируется строка '..line_num..'": '..l..'"\n')
        flog:write("  if ( Des == nil )\n")
        line_is_good = false
      end
    end
    
    if ( line_is_good == true ) then   
      Value = Value:gsub('"', '')
      Value = Value:gsub('\\', '\\\\')
    end
    
    if ( line_is_good == true ) then
      if ( #Value == 0 ) then
        flog:write('WARNING: Игнорируется строка '..line_num..'": '..l..'"\n')
        flog:write('  if ( #Value == 0 )\n')
        line_is_good = false
      end
    end    

    if ( line_is_good == true ) then
      local str = string.format( template, 
                                 Ref, 
                                 Des,
                                 Value
                               )
      fout:write(str)

    end
    line_num = line_num + 1
  end
end



------------ main ------------

if arg[0]:find('pcad_bom_2_lua.lua') ~= nil then
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

  exec(fin, fout, flog, args.fin_name)

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

