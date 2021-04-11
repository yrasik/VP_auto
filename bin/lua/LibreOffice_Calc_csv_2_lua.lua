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

local LibreOffice_Calc_csv_2_lua = {}


local function version()
  return 'v 1.0a'
end


local function info()
  local info = 
[[
  Модуль для .
  @Номер-ключ@Наименование@Обозначение@Фирма/ТУ@Функциональное назначение@Автор заявки@Дата
  
  Выполняет:
    1) 
    2)
    3)
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


local function split(instr, sep)
  local t = {}
  for str in string.gmatch(instr, "([^"..sep.."]+)") do
    table.insert(t, str)
  end
  return t
end


local function qotes(instr)
  return string.gsub(instr, '"', '')
end


local function trim(instr)
  return string.gsub(instr, "^%s*(.-)%s*$", "%1")
end



local function filter(instr)
  instr = qotes(instr)
  instr = string.gsub(instr, "%s+", " ")
  instr = string.gsub(instr, "\\", "/")
  instr = trim(instr)
  return instr
end


local function filter_Op_Temp(instr)
  return string.gsub(instr, "^[%D]*(%d+)[%D]+(%d+)[%D]*", "-%1...+%2")
end


local function find_Op_Temp(instr)
  local find = instr:find('%-%s*%d%d[%D]+%d%d%d*[%s%?]+')
  if( find ~= nil) then
    return find
  end
  
  find = instr:find('%-%s*%d%d[%D]+%d%d%d*$')
  if( find ~= nil) then
    return find
  end
  
  return nil
end


local function filter_Package(instr)
  local Package_Type = instr
  local Package_Size = ''

  instr = string.gsub(instr, '[хx%*]','_')
  instr = string.gsub(instr, '[%абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ%-]','n')
  instr = string.gsub(instr, '[%,]','.')
  instr = string.gsub(instr, '[n]%d+','')

  instr = string.gsub(instr, '%(',' ')
  instr = string.gsub(instr, '%)',' ')

  instr = string.match(instr, '(%d[%d%_%. ]+)')
  
  if( instr ~= nil) then
  
  local Size = {}
  for str in string.gmatch(instr, '(%d+[%.]*%d*)') do
    table.insert(Size, str)
  end


  if( Size[1] ~= nil and
      Size[2] ~= nil
  )
  then
    Size[1] = string.gsub(Size[1], '%,','.')
    Size[2] = string.gsub(Size[2], '%,','.')
    Package_Size = Size[1]..'x'..Size[2]
  
    if( Size[3] ~= nil ) then
      Size[3] = string.gsub(Size[3], '%,','.')
      Package_Size = Package_Size.. 'x'..Size[3]
    end

  end

  else
   instr = ''
  end



  local pack = string.match(Package_Type, '%(([%a%d%-]+)%) *')
  
  if ( pack == nil ) then
    pack = string.match(Package_Type, '^([%a%d%-]+) *')
  end
  
  
  if ( pack == nil ) then
    pack = ''
  end

  if (#pack < 3)  then
    pack = ''
  end


  return pack, Package_Size

end


















local preamble = 
[[
-- codepage cp1251
Base = {}


]]


local template = 
[[
Base[#Base + 1] = {
  Ref = "",
  Group_Name = "%s",
  Type_from_Value = "",
  Code_from_Value = "%s",
  Value_Decode = "",
  Value_Firm = "%s",
  Function = "%s",
  Parameters = "%s",
  Package_Type = "%s",
  Package_Size = "%s",
  Description = "",
  Op_Temp = "%s",
  Value = "",
  Rus_Analog = "%s",
  Old_Values = {
  },
  
  Trash = {%s
  }
  
}


]]


local function LibreOffice_Calc_csv_fix(file_name)
  local fin = io.open(file_name, 'rb')
  if (fin == nil ) then
    return -1
  end

  local content_bin = fin:read("*all")
  fin:close()

  local fout = io.open(file_name,"wb");
  if (fout == nil ) then
    return -2
  end

  local prev_bin
  local bin
  
  for i = 1, #content_bin  do
    prev_bin = bin
    bin = string.byte(content_bin, i)

    if (prev_bin == 0x0D) and (bin == 0x0A) then   -- CR LF
      fout:write( string.char(bin) )
    elseif (prev_bin ~= 0x0D) and (bin == 0x0A) then   -- ? LF

    else
      fout:write( string.char(bin) )
    end
  end

  fout:close()
  return 0
end







local function exec(fin, fout, flog)
  fout:write(preamble)


  local line = 1
  
  for l in fin:lines() do 
    l = string.gsub(l, '@@','@ @') --Пустые графы пробелами заполним
    local fields = split(l, '@')

    if( 
       ( type(tonumber(fields[1])) == 'number' ) and  -- Номер в базе
         fields[2] ~= nil and  -- Наименование
         fields[3] ~= nil and  -- Обозначение
         fields[4] ~= nil and  -- Фирма/ТУ
         fields[5] ~= nil and  -- Функциональное назначение
         fields[6] ~= nil and  -- Автор заявки
         fields[7] ~= nil and  -- Дата заяв.
         fields[8] ~= nil and  -- Параметры элемента
         fields[9] ~= nil and  -- Раб. темпер.
         fields[10] ~= nil     -- Тип корпуса
      ) then
   
      Group_Name = fields[2] -- Наименование
      Code_from_Value = fields[3]  -- Обозначение
      Code_from_Value = qotes(Code_from_Value)
      
      
      Value_Firm = fields[4]  -- Фирма/ТУ
      
      
      Function = fields[5] -- Функциональное назначение
      Function = qotes(Function)
      Function = Function:gsub("\\", "/")
      Function = trim(Function)
      
      
      Parameters = fields[8] -- Параметры элемента
      Parameters = qotes(Parameters)
      Parameters = Parameters:gsub('_x000D_', '; ') 
      Parameters = Parameters:gsub(';;', ';') 
      Parameters = trim(Parameters)
    
    
      Package = fields[10] -- Тип корпуса
      Package = qotes(Package)
      Package = Package:gsub("\\", "/")
      
      local Package_Type, Package_Size = filter_Package(Package)
      local Trash = '\n    "'..Package..'",'
      
      
      
      Op_Temp = fields[9]  -- Раб. темпер.
      Op_Temp = qotes(Op_Temp)
      Op_Temp = Op_Temp:gsub("\\", "/")
      Op_Temp = trim(Op_Temp)
      Op_Temp = filter_Op_Temp(Op_Temp)
      
      
      Rus_Analog = ""
   
--[==[
   local Op_Temp
   if ( index_of_Temp ~= nil ) then
     Op_Temp = filter_Op_Temp(Package:sub(index_of_Temp))
     Package = Package:sub(1, (index_of_Temp - 1))
   end
]==]   
   
   
   
      local str = string.format(template,
                               Group_Name,
                               Code_from_Value,
                               Value_Firm,
                               Function,
                               Parameters,
                               Package_Type,
                               Package_Size,
                               Op_Temp,
                               Rus_Analog,
                               Trash
                            )

     fout:write(str)   
   
   
   
   
    else
      flog:write('--------ERROR--------\n')
      flog:write('     '..line..' >>>>"'..l..'"\n') 
    end

    line = line + 1
  end
end









------------ main ------------
if arg[0]:find('LibreOffice_Calc_csv_2_lua.lua') ~= nil then
  local args = process_args()

  local result = LibreOffice_Calc_csv_fix(args.fin_name)

  if(result < 0) then
    os.exit(-1)
  end  
  
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
  
  
  dofile(args.fout_name)

  for i = 1, #Base do
    flog:write('Reading element "'..Base[i].Code_from_Value..'"\n')
  end  
  
  
  flog:close()
  os.exit(0)
end


return {
  version = version,
  info = info,
  LibreOffice_Calc_csv_fix = LibreOffice_Calc_csv_fix,
  exec = exec
}

