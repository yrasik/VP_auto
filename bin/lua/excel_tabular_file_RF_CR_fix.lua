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

local excel_tabular_file_RF_CR_fix = {}

local function version()
  return 'v 2.0a'
end


local function info()
  local info = 
[[
  Модуль для выправления экспорта MS Excel (файл с разделителями-табуляторам).
  Выполняет:
    1) убирает последовательность 0x0D 0x0A в бинарном файле.
      Эта последовательность переводит каретку неуместным образом.
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


local function exec(fin_bin, fout_bin, flog)
  local content_bin = fin_bin:read("*all")
  local prev_bin
  local bin

  for i = 1, #content_bin  do
    prev_bin = bin
    bin = string.byte(content_bin, i)

    if( bin == 0x0A ) then   -- LF
      if( prev_bin == 0x0D ) then --CR
        fout_bin:write( string.char(bin) )
      end
    else
      fout_bin:write( string.char(bin) )
    end
  end
end




------------ main ------------

if arg[0]:find('excel_tabular_file_RF_CR_fix.lua') ~= nil then
  local args = process_args()

  local fin = io.open(args.fin_name, 'rb')
  if ( fin == nil ) then
    os.exit(-1)
  end

  local fout = io.open(args.fout_name,"wb");
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


