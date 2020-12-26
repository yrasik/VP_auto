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

local AD_LIA_fix_bug = {}


local function version()
  return 'v 2.0a'
end


local function info()
  local info = 
[[
  Модуль для устранеия ошибки в *.lia - файле при переносе Altium *.SchLib -> *.lia -> P-CAD 2006 SP2 PCB ASCII - файлов.
  Выполняет:
    1) Устраняет косяк Altium *.lia файла.
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


local function convert(val)
  val = tonumber(val)
  if( type(val) ~= 'number') then
    print('ERR')
  end
  
  return string.format("%.2f", val)
end


local function line_width(val)
  val = tonumber(val)
  if( type(val) ~= 'number') then
    print('ERR')
  end
  
  if (val < 0.001) then
    val = 0.1
    return string.format("%.3f", val)
  end

  if (val < 0.075) then
    val = 0.075
  end

  return string.format("%.3f", val)
end


local request_pinDes = '^ *%(pinDes    %(text  %(pt [%-%.0-9]+ [%-%.0-9]+%) "([a-ZA-Z0-9]+)" %(textStyleRef'


local function exec(fin, fout, flog)
  local line_num = 1
  local defaultPinDes_line  = nil
  local add_line = -1
  for line in fin:lines() do
    local finded = false
    if( finded == false ) then
      local cap = {}
      cap[1], cap[2], cap[3], cap[4], cap[5], cap[6], cap[7], cap[8], cap[9], cap[10] = line:match(request_pinDes)
      if ( cap[1] ~= nil ) then
        flog:write(string.format('line_num = %06d\n', line_num))
        flog:write('---'..line..'\n')       
        add_line = 4
        defaultPinDes_line = "(defaultPinDes \""..cap[1].."\")";
        flog:write('+++'..defaultPinDes_line..'\n')
        finded = true
      end
    end

    fout:write(line..'\n')
    add_line = add_line - 1
    
    if ( add_line == 0 ) then
      fout:write(defaultPinDes_line..'\n')
    end

    line_num = line_num + 1
  end
end





------------ main ------------

if arg[0]:find('AD_LIA_fix_bug.lua') ~= nil then
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


