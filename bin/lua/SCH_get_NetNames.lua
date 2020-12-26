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


local SCH_get_NetNames = {}


local function version()
  return 'v 2.0a'
end


local function info()
  local info = 
[[
  Модуль для частичного разбора P-CAD 2006 SP2 SCH ASCII - файлов.
  Выполняет:
    1) Чтение в файл названий цепей.
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

function compare( a, b )
  return a < b
end


--  (net "M_DDR_Z0_"
local net_req = '^ *%(net "([a-zA-Z0-9%-%+_%~%.]+)"'

local nets = {}
local function exec(fin, fout, flog)
  local line_num = 1
  for line in fin:lines() do
    local finded = false
    
    if( finded == false ) then
      local cap = {}
      cap[1], cap[2], cap[3], cap[4], cap[5], cap[6], cap[7], cap[8], cap[9], cap[10] = line:match(net_req)
      if ( cap[1] ~= nil ) then
        flog:write(string.format('line_num = %06d\n', line_num))
        flog:write('---'..line..'\n') 
        nets[#nets + 1] = cap[1]
        finded = true
      end
    end

    line_num = line_num + 1
  end

  table.sort(nets, compare)

  for i = 1, #nets do
    fout:write(nets[i]..'\n')
  end
end





------------ main ------------

if arg[0]:find('SCH_get_NetNames.lua') ~= nil then
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


