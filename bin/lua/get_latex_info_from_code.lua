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


local get_latex_info_from_code = {}


local function version()
  return 'v 2.0a'
end


local function info()
  local info = 
[[
  Модуль для извлечения LaTeX - описаия из многострочных комментариев в коде.
  Альтернатива Doxygen. Код содержит более удобоваримое описание.
  Выполняет:
    1) извлечение в файл всего между тегами '_latex_start_'  и '_latex_end_';
    2) '_' -> '\_',     '\_' -> '\_'.
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


local function exec(fin, fout, flog)
  local in_part_for_latex = false

  for l in fin:lines() do
    local is_latex_start = l:match('^%s*_latex_start_')
    local is_latex_end = l:match('^%s*_latex_end_')

    if( in_part_for_latex == true ) then
      if( is_latex_end == nil ) then
        l = l:gsub('_', '\\_')
        l = l:gsub('\\\\_', '\\_')
        fout:write(l..'\n')
      end
    end

    if( is_latex_start ~= nil ) then
      in_part_for_latex = true
    end

    if( is_latex_end ~= nil ) then
      in_part_for_latex = false
    end
  end
end



------------ main ------------

if arg[0]:find('get_latex_info_from_code.lua') ~= nil then
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

