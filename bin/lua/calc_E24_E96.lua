-- codepage cp1251
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

local calc_E24_E96 = {}

local function version()
  return 'v 1.0a'
end


local function info()
  local info = 
[[
  Модуль для округления значений резисторов до стандартных рядов E24 и E96.
  Выполняет:
    1) Выше обозначенное округление.
]]
  return info
end

--[[
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
]]



local E12 = {
  '10', '12', '15', '18',
  '22', '27',
  '33', '39',
  '47',
  '56',
  '68',
  '82',
}


local E24 = {
  '10', '11', '12', '13', '15', '16', '18',
  '20', '22', '24', '27',
  '30', '33', '36', '39',
  '43', '47',
  '51', '56',
  '62', '68',
  '75',
  '82',
  '91',
}


local E96 = {
  '100', '102', '105', '107', '110', '113', '115', '118', '121', '124', '127', '130', '133', '137', '140', '143', '147', '150', '154', '158', '162', '165', '169', '174', '178', '182', '187', '191', '196',
  '200', '205', '210', '215', '221', '226', '232', '237', '243', '249', '255', '261', '267', '274', '280', '287', '294',
  '301', '309', '316', '324', '332', '340', '348', '357', '365', '374', '383', '392',
  '402', '412', '422', '432', '442', '453', '464', '475', '487', '499',
  '511', '523', '536', '549', '562', '576', '590',
  '604', '619', '634', '649', '665', '681', '698',
  '715', '732', '750', '768', '787',
  '806', '825', '845', '866', '887',
  '909', '931', '953', '976',
}


local function calc_Exx(Exx, r_value)
  local Mux = -12
  local R_Exx = 0
  local R_Exx_Mux = 0
  --print('----')
  --print(r_value)
  while ( true ) do
    local max = tonumber(Exx[#Exx]) * 10^Mux
    local min_mux_10 = tonumber(Exx[1]) * 10^(Mux + 1)
    local min = tonumber(Exx[1]) * 10^Mux
    
    if ( ( r_value > max ) and ( r_value <= min_mux_10 ) ) then
      local to_Max = math.abs(r_value - max)
      local to_Min = math.abs(r_value - min_mux_10)
      
      if ( to_Max > to_Min ) then
        R_Exx = tonumber(Exx[1])
        R_Exx_Mux = Mux + 1
      else
        R_Exx = tonumber(Exx[#Exx])
        R_Exx_Mux = Mux
      end
      --print('  '..R_Exx..' * 10^ '..R_Exx_Mux)
      return R_Exx * 10^R_Exx_Mux
    elseif ( ( r_value > min ) and ( r_value <= max ) ) then
      for i = 1, (#Exx - 1) do
        local min = tonumber(Exx[i]) * 10^Mux
        local max = tonumber(Exx[i + 1]) * 10^Mux
        local to_Min = math.abs(r_value - min)
        local to_Max = math.abs(r_value - max)
        local delta = max - min
        --io.write('.')
        if ( to_Min < delta ) then
          R_Exx = tonumber(Exx[i])
          R_Exx_Mux = Mux 
          --print('  '..R_Exx..' * 10^ '..R_Exx_Mux)
          return R_Exx * 10^R_Exx_Mux
        end
      end
    end
    Mux = Mux + 1
  end
end


local function calc_E24(r_value)
  return calc_Exx(E24, r_value), 'E24'
end


local function calc_E24_plus_E96(r_value)
  local R_E24 = calc_Exx(E24, r_value)
  local R_E96 = calc_Exx(E96, r_value) 
  local d_E24 = math.abs(r_value - R_E24)
  local d_E96 = math.abs(r_value - R_E96)
  
  if(d_E24 <= d_E96) then
    return R_E24, 'E24'
  else
    return R_E96, 'E96'
  end
end


------------ main ------------

if arg[0]:find('calc_E24_E96.lua') ~= nil then
  local args = process_args()
--[[
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
]]
  os.exit(0)
end


return {
  version = version,
  info = info,
  calc_E24 = calc_E24,
  calc_E24_plus_E96 = calc_E24_plus_E96,
  exec = exec
}


