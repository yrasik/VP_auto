--cp1251
--[==[
/*
 * This file is part of the "SystemC_AMS_Lua" distribution (https://github.com/yrasik/SystemC_AMS_Lua).
 * Copyright (c) 2021 Yuri Stepanenko.
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

local wav = require("wav")



local ngspice_csv_2_wave = {}

--dofile("lua/wav.lua")


local function version()
  return 'v 1.0a'
end


local function info()
  local info = 
[[
  Модуль для извлечения сигнала из ngspice csv - файлов и сохранения в формате wave.
  Выполняет:
    1) Загружает файл со строчками вида
-------------------------------
 time           "OUT"         
 0,0000000e+00 -1,9470244e+00 
 6,2500000e-07 -1,9470630e+00 
 6,3168713e-07 -1,9470635e+00 
-------------------------------
   2) Создаёт wave - файл в формате моно, 16 бит 48000 Гц.

  Внимание ! Программа ngspice выдаёт таблицу с переменным шагом дискретизации.
  Возможно нужно добавить методы ЦОС перед децимацией (вычисление минимального шага,
  экстраполяцию, фильтрацию и в заключение - децимацию). Из - за того, что шаг мелкий,
  компьютеру не хватает ресурсов. Нужно применять потоковые адгоритмы для этого.
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
    elseif a == "--size" then
      t.size = arg[i + 1]
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


local function trim(instr)
  return string.gsub(instr, "^%s*(.-)%s*$", "%1")
end






--[===[

const int16_t B[55] = {
  2785,  -1584,  -1470,  -1444,  -1392,  -1227,   -915,   -457,     79,
   599,    999,   1175,   1064,    653,      1,   -776,  -1513,  -2022,
 -2131,  -1713,   -718,    811,   2739,   4852,   6888,   8582,   9703,
 10095,   9703,   8582,   6888,   4852,   2739,    811,   -718,  -1713,
 -2131,  -2022,  -1513,   -776,      1,    653,   1064,   1175,    999,
   599,     79,   -457,   -915,  -1227,  -1392,  -1444,  -1470,  -1584,
  2785
};


int32_t fir_init(fir *f, const int16_t *coeffs, int16_t *signal, int32_t size, uint8_t akk_rshift)
{
  int32_t i;
  f->coeffs = coeffs;
  f->signal = signal;
  f->size = size;
  f->akk_rshift = akk_rshift;
  f->index = 0;

  for (i = 0; i < f->size; i++)
    f->signal[i] = 0;

  return 0;
}


int32_t fir_exec(fir *f, const int16_t *in, int16_t *out, int len)
{
  int32_t in_index;
  int32_t index_signal;
  int32_t i;
  int64_t akk;

  for(in_index = 0; in_index < len; in_index++)
  {
    akk = 0;
    f->signal[f->index] = in[in_index];

    for(i = 0; i < f->size; i++)
    {
      index_signal = f->index + i;
      index_signal = index_signal % (f->size);
      akk += f->coeffs[i] * f->signal[index_signal];
    }

    f->index--;
    if(f->index < 0)
    {
      f->index = (f->size - 1);
    }

    out[in_index] = akk >> f->akk_rshift;
  }

  return 0;
}

]===]


local coeff = {  --55
  2785,  -1584,  -1470,  -1444,  -1392,  -1227,   -915,   -457,     79,
   599,    999,   1175,   1064,    653,      1,   -776,  -1513,  -2022,
 -2131,  -1713,   -718,    811,   2739,   4852,   6888,   8582,   9703,
 10095,   9703,   8582,   6888,   4852,   2739,    811,   -718,  -1713,
 -2131,  -2022,  -1513,   -776,      1,    653,   1064,   1175,    999,
   599,     79,   -457,   -915,  -1227,  -1392,  -1444,  -1470,  -1584,
  2785
}


local function fir_init(coeffs)
  local fir = {}
  fir.coeffs = coeffs
  fir.size = #coeffs
  fir.index = 1
  fir.signal = {}

  for i = 1, fir.size do
    fir.signal[i] = 0
  end

  return fir
end


local function fir_exec(fir, input, output, len)
  local in_index
  local index_signal
  local akk

  for in_index = 1, len do
    akk = 0
    fir.signal[fir.index] = input[in_index]

    for i = 1, fir.size do
      index_signal = fir.index + i
      index_signal = index_signal % (fir.size)
      akk = akk + fir.coeffs[i] * fir.signal[index_signal + 1]
    end

    fir.index = fir.index - 1
    if (fir.index < 1) then
      fir.index = fir.size
    end

    output[in_index] = akk
  end
end


local function timestep_min(records)
  local timestep = 1
  local ts
  
  for i = 2, #records do
    ts = records[i].time - records[i - 1].time
   -- if( ts ~= timestep) then
   --   print('ts = '..ts)
   -- end
    
    if (ts < timestep) then
      timestep = ts
      --print('timestep = '..timestep)
    end
  end
  
  return timestep
end


local function exec(fin, fout_name, flog)
  local records = {}
  local writer = wav.create_context(fout_name, "w")
  writer.init(1, 48000, 16)
    
  for l in fin:lines() do 
    local fields = trim(l)
    fields = split(fields, ' ')

    if( (fields[1] ~= nil) and (fields[2] ~= nil) ) then
      fields[1] = fields[1]:gsub(",", ".")
      fields[1] = tonumber(fields[1])
      fields[2] = fields[2]:gsub(",", ".")
      fields[2] = tonumber(fields[2])
      if(type(fields[2]) == 'number') then
        records[#records + 1] = {}
        records[#records].time = fields[1]
        records[#records].sample = fields[2]
      end
    end
  end

  local timestep_min = timestep_min(records)
  local wave_timestep = 0.0625 * 10^-3

  --print('wave_timestep/timestep_min = '..wave_timestep/timestep_min)


  local time = 0
  local s = {}

  print('#records = '..#records)
  
  local i = 1
  while i < #records do
    if( records[i].time > time ) then
      s[#s + 1] = records[i].sample
      time = time + wave_timestep
      --print(time)
    end
    i = i + 1
  end
  
 
 
 
 
  local akk = 0
  for i = 1, #s do 
    akk = akk + s[i] 
  end
  local mid = akk/#s

  print('mid = '..mid)
  for i = 1, #s do 
    s[i] = (s[i] - mid ) * 32767
  end
 
  writer.write_samples_interlaced(s)
  writer.finish()

end





------------ main ------------

if arg[0]:find('ngspice_csv_2_wave.lua') ~= nil then
  local args = process_args()


  local flog = io.open(args.flog_name,"w");
  if ( flog == nil ) then
    fout:close()
    os.exit(-1)
  end


  local fin = io.open(args.fin_name,"r");
  if ( fin == nil ) then
    flog:close()
    os.exit(-1)
  end

  exec(fin, args.fout_name, flog)

  fin:close()
  flog:close()

  os.exit(0)
end


return {
  version = version,
  info = info,
  exec = exec
}


