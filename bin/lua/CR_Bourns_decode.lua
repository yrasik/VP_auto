--cp1251
--[==[
/*
 * This file is part of the "VP_auto" distribution (https://github.com/yrasik/VP_auto).
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


local CR_Bourns_decode = {}
local calc = require ('calc_E24_E96')


local function version()
  local info = 'v 1.3 \t 2021 �.'
  return info
end


local function info()
  local info = 
[[
������ ��� ������������� �������� ���������� ����� Bourns.
�� ����� ������� 'CR0603-FX-4641Ex'
�� ������ ������� '(CR0603�1 %X-4,64 ���)'

����� �������������� ��� ��������� ���������� ���������� ���
���� ������� �������, ������� ��������� ����������
� ��..
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


local SIZE = {
  {'01005', '0.4x0.2x0.13',  'Power rating at 70 �C 1/32 W',  (1/32), '1/32 ��', '-55...+125', '15 �', '0.5 A'},
  {'0201' , '0.6x0.3x0.23',  'Power rating at 70 �C 1/20 W',  (1/20), '1/20 ��', '-55...+125', '30 �', '0.5 A'},
  {'0402' , '1.0x0.5x0.35',  'Power rating at 70 �C 1/16 W',  (1/16), '1/16 ��', '-55...+155', '50 �', '1 A'},
  {'0603' , '1.6x0.8x0.45',  'Power rating at 70 �C 0.10 W',  0.1,    '0.1 ��', '-55...+155', '75 �', '1 A'},
  {'0805' , '2.0x1.25x0.5',  'Power rating at 70 �C 0.125 W', 0.125,  '0.125 ��', '-55...+155', '150 �', '2 A'},
  {'1206' , '3.2x1.6x0.6',   'Power rating at 70 �C 0.25 W',  0.25,   '0.25 ��', '-55...+155', '200 �', '2 A'},
  {'1210' , '3.1x2.55x0.55', 'Power rating at 70 �C 1/3 W',   (1/3),  '1/3 ��', '-55...+125', '200 �', '2 A'},
  {'2010' , '5.0x2.5x0.6',   'Power rating at 70 �C 1/2 W',   (1/2),  '0.5 ��', '-55...+155', '200 �', '2 A'},
  {'2512' , '6.3x3.1x0.6',   'Power rating at 70 �C 1 W',     1,      '1 ��', '-55...+155', '200 �', '2 A'},
}


local PACKAGING = {
  {'01005', 'G', 'Paper Tape (10,000 pcs.) on 7 � Plastic Reel'},
  {'0201' , 'G', 'Paper Tape (10,000 pcs.) on 7 � Plastic Reel'},
  {'0402' , 'G', 'Paper Tape (10,000 pcs.) on 7 � Plastic Reel'},
  {'0603' , 'E', 'Paper Tape (5,000 pcs.) on 7 � Plastic Reel'},
  {'0805' , 'E', 'Paper Tape (5,000 pcs.) on 7 � Plastic Reel'},
  {'1206' , 'E', 'Paper Tape (5,000 pcs.) on 7 � Plastic Reel'},
  {'1210' , 'E', 'Paper Tape (5,000 pcs.) on 7 � Plastic Reel'},
  {'2010' , 'E', 'Paper Tape (5,000 pcs.) on 7 � Plastic Reel'},
  {'2512' , 'E', 'Paper Tape (5,000 pcs.) on 7 � Plastic Reel'},
}


local TCR_1_PERCENT = {
  {'01005', '10<=;<100;V', '100<=;<1000000;W' },
  {'0201' , '1<=;<10;/'  , '10<=;<3000000;W'  },
  {'0402' , '1<=;<10;/'  , '10<=;<1000000;X', '1000000<=;<10000000;W'},
  {'0603' , '1<=;<10;W'  , '10<=;<1000000;X', '1000000<=;<10000000;W'},
  {'0805' , '1<=;<10;W'  , '10<=;<1000000;X', '1000000<=;<10000000;W'},
  {'1206' , '1<=;<10;W'  , '10<=;<1000000;X', '1000000<=;<10000000;W'},
  {'1210' , '1<=;<10;W'  , '10<=;<1000000;X', '1000000<=;<10000000;W'},
  {'2010' , '1<=;<10;W'  , '10<=;<1000000;X', '1000000<=;<10000000;W'},
  {'2512' , '1<=;<10;W'  , '10<=;<1000000;X', '1000000<=;<10000000;W'},
}


local TCR_5_PERCENT = {
  {'01005', '10<=;<100;�300', '100<=;<1000000;�200'},
  {'0201' , '1<=;<10;/'     , '10<=;<3000000;W'},
  {'0402' , '1<=;<10;/'     , '10<=;<10000000;W', '10000000<=;<20000000;Z'},
  {'0603' , '1<=;<10;Z'     , '10<=;<10000000;W', '10000000<=;<20000000;Z'},
  {'0805' , '1<=;<10;Z'     , '10<=;<10000000;W', '10000000<=;<20000000;Z'},
  {'1206' , '1<=;<10;Z'     , '10<=;<10000000;W', '10000000<=;<20000000;Z'},
  {'1210' , '1<=;<10;Z'     , '10<=;<10000000;W', '10000000<=;<20000000;Z'},
  {'2010' , '1<=;<10;Z'     , '10<=;<10000000;W', '10000000<=;<20000000;Z'},
  {'2512' , '1<=;<10;Z'     , '10<=;<10000000;W', '10000000<=;<20000000;Z'},
}


local TOLERANCE = {
  { 'F', '�1 %', '�������� �1 %', 1},
  { 'J', '�5 %', '�������� �5 %', 5},
}


local TCR = {
  {'X', 'X', '�100'},
  {'W', 'W', '�200'},
  {'V', 'V', '�300'},
  {'Z', 'Z', '�400'},
  {'/', '/', '-200 to +500'},
}


local MUX = {
  {'0', 1, '��', false},
  {'1', 10, '��', true},
  {'2', 100, '��', false},
  {'3', 1000, '���', false},
  {'4', 10000, '���', true},
  {'5', 100000, '���', false},
  {'6', 1000000, '���', false},
  {'7', 10000000, '���', true},
}


local TERMINATION = {
  {'LF', 'Tin-plated (RoHS compliant)'},
--  {'x', '�� ����� ����� ��� �������'},
}


local function split(instr, sep)
  local t = {}
  for str in string.gmatch(instr, "([^"..sep.."]+)") do
    table.insert(t, str)
  end
  return t
end


local function trim(instr)
  return string.gsub(instr, "^%s*(.-)%s*$", "%1")
end


local function replace(str, what, with)
  what = what:gsub("[%(%)%.%+%-%*%?%[%]%^%$%%]", "%%%1")
  with = with:gsub("[%%]", "%%%%")
  return str:gsub(what, with)
end


local function format_x(Value)
  local Value_str
  Value_str = string.format('%.1f',Value)
  Value_str = Value_str:gsub('%.0', '')
  Value_str = Value_str:gsub('%.', ',')
  return Value_str
end


local function format_xx(Value)
  local Value_str
  Value_str = string.format('%.2f',Value)
  local Value_str_H = Value_str:match('^(%d+)%.')
  
  local Value_str_L = Value_str:match('^%d+%.(%d+)$')
  --print(Value_str..'  Value_str_L = "'..Value_str_L..'"')
  Value_str_L = Value_str_L:gsub('0', '')

  if( #Value_str_L > 0) then
    Value_str = Value_str_H..','..Value_str_L
  else
    Value_str = Value_str_H
  end

  return Value_str
end


local function Resistance_Value_Decode(TOLERANCE_Num, Value)
  local Value_str
  local E_message = ''
  local Value_E
  local Value_float
  local num_R = Value:find('[R]')

  if( num_R == nil) then
    local Mux = string.char(Value:byte(#Value))
    Value_E = Value:sub(1, #Value - 1)

    for i = 1, #MUX do
      if( Mux == MUX[i][1] ) then
        Mux = MUX[i][2]
        break
      end
    end

    Value_float = tonumber(Value_E) * Mux
    local Value_f = Value_float / 1000

  if (TOLERANCE_Num == 1 ) then  -- 1%
    if( Value_f < 1) then
      Value_str = format_xx(Value_float)
      Value_str = Value_str..' ��'
    else
      Value_f = Value_f / 1000
      if( Value_f < 1 ) then
        Value_str = format_xx(Value_float/1000)
        Value_str = Value_str..' ���'
      else
        Value_str = format_xx(Value_f)
        Value_str = Value_str..' ���'
      end
    end
  elseif (TOLERANCE_Num == 2 ) then  -- 5%
    if( Value_f < 1) then
      Value_str = format_x(Value_float)
      Value_str = Value_str..' ��'
    else
      Value_f = Value_f / 1000
      if( Value_f < 1 ) then
        Value_str = format_x(Value_float/1000)
        Value_str = Value_str..' ���'
      else
        Value_str = format_x(Value_f)
        Value_str = Value_str..' ���'
      end
    end
  else


  end
    
    
    
    
    if( (TOLERANCE_Num == 1) and (Value_float < 100) ) then-- 1%
      E_message = E_message..'ERROR: For 1 % Tolerance: �R� designates decimal point (example: 24R3 = 24.3 ohms).\n'
    end

    if( (TOLERANCE_Num == 2) and (Value_float < 10) ) then-- 5%
      E_message = E_message..'ERROR: For 5 % Tolerance: �R� designates decimal point (example: 4R7 = 4.7 ohms).\n'
    end

  else
    Value_str = Value:gsub('[R]', ',')
    Value_str = Value_str:gsub(',0', '')..' ��' --FIXME ���� ��� ����� ����� ����???
    
    Value_E = Value:gsub('[R]', '')
    
    Value_float = Value:gsub('[R]', '.')
    Value_float = tonumber(Value_float)

    if( (TOLERANCE_Num == 1) and (Value_float >= 100) ) then-- 1%
      E_message = E_message..'ERROR: For 1 % Tolerance: First three digits are significant, fourth digit represents number of zeros to follow (example: 8252 = 82.5k ohms).\n'
    end

    if( (TOLERANCE_Num == 2) and (Value_float >= 10) ) then-- 5%
      E_message = E_message..'ERROR: For 5 % Tolerance: First two digits are significant, third digit represents number of zeros to follow (example: 474 = 470k ohms; 000 = Jumper).\n'
    end
  end




  local finded_E = false

  if (TOLERANCE_Num == 1 ) then  -- 1%
    if( finded_E == false ) then
      for i = 1, #E24 do
        local E24_0 = E24[i]..'0'
        if( Value_E == E24_0 ) then
          finded_E = true
          --E_message = 'E24'
          break
        end
      end
    end

    if( finded_E == false ) then
      for i = 1, #E96 do
        if( Value_E == E96[i] ) then
          finded_E = true
          --E_message = 'E96'
          break
        end
      end
    end
    
    if( finded_E == false ) then
      E_message = E_message..'ERROR: �������� ������������� "'..Value_E..'" �� �������� � ����������� ���� E24, E96 ��� 1 % ���������� Bourns. \n    ������ ��������� �������� "'..Value..'".\n'
    end
  elseif (TOLERANCE_Num == 2 ) then  -- 5%
    if( Value_E == '00' ) then
      finded_E = true
    end
    if( finded_E == false ) then
      for i = 1, #E24 do
        if( Value_E == E24[i] ) then
          finded_E = true
          --E_message = 'E24'
          break
        end
      end
    end

    if( finded_E == false ) then
      E_message = E_message..'ERROR: �������� ������������� "'..Value_E..'" �� �������� � ����������� ��� E24 ��� 5 % ���������� Bourns. \n    ������ ��������� �������� "'..Value..'".\n'
    end
  else


  end

  return finded_E, E_message, Value_str
end







local function Packagin_Value_Check(SIZE_Num, PACKAGING_Num)
  local result = false
  local E_message = ''

  if( (SIZE_Num == 2) and (PACKAGING_Num == 1))  then-- 0402 <-> G
   result = true
  end

  if( (SIZE_Num > 2) and (PACKAGING_Num == 2))  then-- 0402 <-> G
   result = true
  end

  if( result == false ) then
    E_message = 'WARNING: ��������� ����������� "'..SIZE[SIZE_Num][1]..'" �� ������������ � �������� "'..PACKAGING[PACKAGING_Num][1]..'" ('..PACKAGING[PACKAGING_Num][2]..')'
  end

  return result, E_message
end


local function CR_Bourns_Decode(Name)
  local Decode
  local SIZE_Num
  local TOLERANCE_Num
  local TCR_Num
  local PACKAGING_Num
  local TERMINATION_Num
  local name = Name
  local Value_Decode

  local finded = false
  local E_message = ''
  ------------
  local Model = name:match('^(%a%a)')
  if( Model == 'CR' ) then
    name = name:sub(1 + #Model)
    Decode = '(CR'
    finded = true
    E_message = E_message..'ERROR: ��� �� �������� Bourns\n'
  end

  if( finded == false )  then
    return false, E_message
  end

  ------------------
  finded = false
  local Size = name:match('^(%d%d%d%d)')
  if( Size ~= nil ) then
    name = name:sub(1 + #Size)
    SIZE_Num = 1
    while SIZE_Num <= #SIZE do
      if( Size == SIZE[SIZE_Num][1] ) then
        Decode = Decode..SIZE[SIZE_Num][1]
        finded = true
        break
      end
      SIZE_Num = SIZE_Num + 1
    end
    if( finded == false) then 
      E_message = E_message..'EROOR: ���������� ���������� ������� � "'..Name..'"\n'
    end
  else
    E_message = E_message..'EROOR: ���������� ���������� ������� � "'..Name..'"\n'
  end

  if( finded == false )  then
    return false, E_message
  end

  ----------------
  finded = false
  local Tolerance = name:match('^%-([%a/])')
  if( Tolerance ~= nil ) then
    name = name:sub(2 + #Tolerance)
    TOLERANCE_Num = 1
    while TOLERANCE_Num <= #TOLERANCE do
      if( Tolerance == TOLERANCE[TOLERANCE_Num][1] ) then
        Decode = Decode..TOLERANCE[TOLERANCE_Num][2]
        finded = true
        break
      end
      TOLERANCE_Num = TOLERANCE_Num + 1
    end
    
    if( finded == false) then 
      --print('ERROR: Tolerance in "'..Name..'"')
      E_message = E_message..'EROOR: ���������� ������ �������� � "'..Name..'"\n'
    end
  else
    E_message = E_message..'EROOR: ���������� ������ �������� � "'..Name..'"\n'
  end

  if( finded == false )  then
    return false, E_message
  end


  --------------
  finded = false
  local Tcr = name:match('^([%a/])')
  if( Tcr ~= nil ) then
    name = name:sub( 1 + #Tcr)
    TCR_Num = 1
    while TCR_Num <= #TCR do
      if( Tcr == TCR[TCR_Num][1] ) then
        Decode = Decode..TCR[TCR_Num][2]
        finded = true
        break
      end
      TCR_Num = TCR_Num + 1
    end
    if( finded == false) then 
      E_message = E_message..'EROOR: ���������� ������������� ������� ���������� � "'..Name..'"\n'
    end
  else
    E_message = E_message..'EROOR: ���������� ������������� ������� ���������� � "'..Name..'"\n'
  end

  if( finded == false )  then
    return false, E_message
  end

  --------------
  finded = false
  local Value = name:match('^%-([R%d][R%d][R%d][R%d]*[R%d]*)')
  if( Value ~= nil ) then
    name = name:sub( 2 + #Value)
    finded, E_message, Value_Decode = Resistance_Value_Decode(TOLERANCE_Num, Value)
    if( finded == true ) then
      Decode = Decode..'-'..Value_Decode..')'
    end
  else
    E_message = E_message..'EROOR: ���������� ������� ������������� � "'..Name..'"\n'
  end

  if( finded == false )  then
    return false, E_message, Decode
  end

  --------------
  finded = false
  local Packaging = name:match('^(%a)')
  if( Packaging ~= nil ) then
    name = name:sub( 1 + #Packaging)
    PACKAGING_Num = 1
    while PACKAGING_Num <= #PACKAGING do
      if( Packaging == PACKAGING[PACKAGING_Num][1] ) then
        finded = true
        break
      end
      PACKAGING_Num = PACKAGING_Num + 1
    end

    if( finded == false ) then 
      E_message = E_message..'ERROR: ���������� ��� �������� � "'..Name..'"\n'
    end
  else
    E_message = E_message..'ERROR: ���������� ��� �������� � "'..Name..'"\n'
  end

  if( finded == false )  then
    return false, E_message, Decode
  end

  --------------
  finded = false
  local Termination = name:match('^(%a[%a]*)$')
  if( Termination ~= nil ) then
    name = name:sub( 1 + #Termination)
    TERMINATION_Num = 1
    while TERMINATION_Num <= #TERMINATION do
      if( Termination == TERMINATION[TERMINATION_Num][1] ) then
        finded = true
        break
      end
      TERMINATION_Num = TERMINATION_Num + 1
    end

    if( finded == false ) then 
      E_message = E_message..'ERROR: ���������� ��� ������� ��������� �"'..Name..'"\n'
    end
  else
    E_message = E_message..'ERROR: ���������� ��� ������� ��������� � "'..Name..'"\n'
  end

  if( finded == false )  then
    return false, E_message, Decode
  end

  local Parameters = '������������� '..Value_Decode..'; �������� '..TOLERANCE[TOLERANCE_Num][2]..'; �������� '..SIZE[SIZE_Num][5]..'.'
  local Package_Type = SIZE[SIZE_Num][1]
  local Package_Size = SIZE[SIZE_Num][2]
  local Op_Temp = SIZE[SIZE_Num][6]

  return true, E_message, Decode, Parameters, Package_Type, Package_Size, Op_Temp
end






















local function Resistance_Value_Decode(TOLERANCE_Num, Value)
  local Value_str
  local E_message = ''
  local Value_E
  local Value_float
  local Mux
  local finded = false
  
  print('Value = ', Value)
  
  Mux = Value:match('([����komMKO]+)')
  Mux = Mux:gsub('[M]', '�')
  Mux = Mux:gsub('[kK]', '�')
  Mux = Mux:gsub('[oO]', '�')
  Mux = Mux:gsub('[m]', '�')
  
  if( type(Mux) == nil ) then
    Mux = '��'
  end
  
  if( #Mux == 0 ) then
    Mux = '��'
  end  
  
  if( #Mux == 1 ) then
    if ( Mux == '�') then
      Mux = '���'
    end
    if ( Mux == '�') then
      Mux = '���'
    end 
  end
 
  
  if( Mux ~= nil ) then
    Value_E = Value:gsub('[����komMKO ]', '')
    Value_E = Value_E:gsub(',', '.')
    print('Value_E = ', Value_E)

    for i = 1, #MUX do
      if( Mux == MUX[i][3] ) then
        Mux = MUX[i][2]
        finded = true
        break
      end
    end

    print('Mux =', Mux)


    if( finded == false) then 
      E_message = E_message..'ERROR: ���������� ������� � ��������: "'..Value..'"\n'
    end
  else
    if( finded == false) then 
      E_message = E_message..'ERROR: ���������� ������� � ��������: "'..Value..'"\n'
    end
  end

  if( finded == false )  then
    return false, E_message
  end


  finded = false
  Value_float = tonumber(Value_E) * Mux

  print(Value_float)

  if( Value_float == 0 ) then  -- ������� ��������
    if ( TOLERANCE_Num == 1 ) then -- 1%
      E_message = E_message..'ERROR: �� ��������� ����� ������ �� ������� �������� ���������\n'
      return false, E_message    
    end
    
    if ( TOLERANCE_Num == 2 ) then -- 5%
      E_message = E_message..'INFO: ������� ��������\n'
      return true, E_message, 0, '00', 0
    end
  end


  local Value_float_true
  if ( TOLERANCE_Num == 1 ) then -- 1%
    Value_float_true, E, R_Mantiss, R_Mux = calc.calc_E24_plus_E96(Value_float)
  elseif ( TOLERANCE_Num == 2 ) then -- 5%
    Value_float_true, E, R_Mantiss, R_Mux = calc.calc_E24(Value_float)
  else
    E_message = E_message..'ERROR: �� ��������� ����� ������ �� �������\n'
    return false, E_message
  end

  if ( Value_float ~= Value_float_true) then
    E_message = E_message..'WARNING: ��������� �������� ������������� ��������� �� ���� '..E..'\n'
  else
    E_message = E_message..'INFO: �������� ������������� ��������� ������������� ���� '..E..'\n'
  end

  return true, E_message, Value_float_true, R_Mantiss, R_Mux
end









local function CR_Bourns_Code(Size, Value, Tolerance)
  local Code = 'CR'
  local Decode = '(CR'
  
  local finded
  local E_message = ''
  local SIZE_Num
  local TCR_Num
  local TCR_1_PERCENT_Num
  local TCR_5_PERCENT_Num
  local TOLERANCE_Num
  local MUX_Num
  local PACKAGING_Num
  local TERMINATION_Num

  finded = false
  Size = Size:gsub('%D', '')

  if( Size ~= nil ) then
    for i = 1, #SIZE do
      if( Size == SIZE[i][1] ) then
        Code = Code..SIZE[i][1]..'-'
        Decode = Decode..SIZE[i][1]
        SIZE_Num = i
        finded = true
        break
      end
    end
    if( finded == false) then 
      E_message = E_message..'ERROR: ���������� ���������� ������� (��������) � "'..Name..'"\n'
    end
  else
    E_message = E_message..'ERROR: ���������� ���������� ������� (��������) � "'..Name..'"\n'
  end
  
----------------------------------------------
  finded = false
  local Tolerance_

  if( type(Tolerance) == 'number' ) then
    Tolerance_ = Tolerance
  elseif( type(Tolerance) == 'string' ) then
    Tolerance_ = Tolerance:gsub('%%', '')
    Tolerance_ = Tolerance_:gsub('%,', '.')
    Tolerance_ = Tolerance_:gsub(' ', '')
    Tolerance_ = Tolerance_:gsub('%a', '')
    Tolerance_ = tonumber(Tolerance_)
  else
    E_message = E_message..'ERROR: ���������� ��� �������� �������\n'
    return false, E_message
  end
  
  TOLERANCE_Num = 1
  while TOLERANCE_Num <= #TOLERANCE do
    if( Tolerance_ == TOLERANCE[TOLERANCE_Num][4] ) then
      finded = true
      break
    end
    TOLERANCE_Num = TOLERANCE_Num + 1
  end
  
  if( finded == false) then 
    E_message = E_message..'ERROR: ���������� ������ �������� � "'..Tolerance..'"\n'
    return false, E_message
  end

  Code = Code..TOLERANCE[TOLERANCE_Num][1]
  Decode = Decode..TOLERANCE[TOLERANCE_Num][2]
  
----------------------------------------------  
  local Result, E_R_message, Value_float, R_Mantiss, R_Mux = Resistance_Value_Decode(TOLERANCE_Num, Value)
  
  print(E_R_message)
  
  print('Value_float = ', Value_float)
  print('R_Mantiss = ', R_Mantiss)
 -- print('R_Mux = ', 10^R_Mux)
  
  
  R_Mux = 10^R_Mux
    for i = 1, #MUX do
      if( R_Mux == MUX[i][2] ) then
        finded = true
        MUX_Num = i
        break
      end
    end
  
  
 -- print ('MUX_Num = ', MUX_Num)
  
  
----------------------------------------------
----  print( type(TCR_1_PERCENT[SIZE_Num]) )
--  print( #(TCR_1_PERCENT[SIZE_Num]) )
--  print(TCR_1_PERCENT[SIZE_Num][2])

  local tcr
  if( TOLERANCE_Num == 1 ) then  -- 1%
    tcr = TCR_1_PERCENT[SIZE_Num]
  elseif( TOLERANCE_Num == 2 ) then  -- 5%
    tcr = TCR_5_PERCENT[SIZE_Num]
  else
  
  end
  
  
  if( Value_float == 0 ) then
    TCR_Num = 5 -- '/'
  else
    for i = 2, #tcr do
      local fields = split(tcr[i], ';') 
      min_val = fields[1]
      max_val = fields[2]
      TCR_val = fields[3]
      min_val = min_val:gsub('%D', '')
      max_val = max_val:gsub('%D', '')
      min_val = tonumber(min_val)
      max_val = tonumber(max_val)
     -- print(min_val)
    --  print(Value_float)
      if( (min_val <= Value_float) and (Value_float < max_val) ) then
        for j = 1, #TCR do
          if ( TCR[j][1] == TCR_val ) then
            TCR_Num = j
            break
          end
        end
        break
      end
    end
  end
  
  
  
 -- print ('TCR_Num = ', TCR_Num)
  
  Code = Code..TCR[TCR_Num][1]..'-'
  Decode = Decode..TCR[TCR_Num][1]..'-'
 -- print(Code)
  
  
-------------------------













  local R_Code
  local R_Decode  = ''
  if( TOLERANCE_Num == 1 ) then  -- 1%
    if( Value_float < 10 ) then
      print('ERROR:  1%  < 10 Om')  
    elseif( Value_float < 100 ) then  -- ok
      if( #R_Mantiss == 2 ) then  -- E24
        R_Code = R_Mantiss
        R_Code = string.format('%dR0', R_Code)
        
        R_Decode = R_Mantiss
        R_Decode = string.format('%d', R_Decode)
        R_Decode = R_Decode..' '..'��'
      else  -- E96
        R_Code = R_Mantiss/10
        R_Code = string.format('%.01f', R_Code)
        R_Code = R_Code:gsub('%.', 'R')
        
        R_Decode = R_Mantiss/10
        R_Decode = string.format('%.01f', R_Decode)
        R_Decode = R_Decode:gsub('%.', ',')
        R_Decode = R_Decode:gsub(',0$', '')
        R_Decode = R_Decode..' '..'��'
      end      
    elseif( Value_float < 1000 ) then
      if( #R_Mantiss == 2 ) then  -- E24
        R_Code = R_Mantiss..'1'
        
        R_Decode = R_Mantiss
        R_Decode = string.format('%d', R_Decode)
        R_Decode = R_Decode..'0'..' '..'��'        
      else  -- E96
        R_Code = R_Mantiss..'0'    

        R_Decode = R_Mantiss
        R_Decode = string.format('%d', R_Decode)
        R_Decode = R_Decode..' '..'��'
      end
    elseif( Value_float < 10000 ) then
      if( #R_Mantiss == 2 ) then  -- E24 
        R_Code = R_Mantiss..'01'

        R_Decode = R_Mantiss/10
        R_Decode = string.format('%.01f', R_Decode)
        R_Decode = R_Decode:gsub('%.', ',')
        R_Decode = R_Decode:gsub(',0$', '')
        R_Decode = R_Decode..' '..'���'        
      else  -- E96  
        R_Code = R_Mantiss..'1' 

        R_Decode = R_Mantiss/100
        R_Decode = string.format('%.02f', R_Decode)
        R_Decode = R_Decode:gsub('%.', ',')
        R_Decode = R_Decode:gsub(',00$', '')
        R_Decode = R_Decode:gsub('0$', '')
        R_Decode = R_Decode..' '..'���'
      end 
    elseif( Value_float < 100000 ) then
      if( #R_Mantiss == 2 ) then  -- E24     
        R_Code = R_Mantiss..'02'
      
        R_Decode = R_Mantiss
        R_Decode = R_Decode..' '..'���'
      else  -- E96 
        R_Code = R_Mantiss..'2'
      
        R_Decode = R_Mantiss/10
        R_Decode = string.format('%.01f', R_Decode)
        R_Decode = R_Decode:gsub('%.', ',')
        R_Decode = R_Decode:gsub(',0$', '')
        R_Decode = R_Decode..' '..'���'
      end 
    elseif( Value_float < 1000000 ) then
      if( #R_Mantiss == 2 ) then  -- E24      
        R_Code = R_Mantiss..'03'
      
        R_Decode = R_Mantiss..'0'
        R_Decode = R_Decode..' '..'���'
      else  -- E96
        R_Code = R_Mantiss..'3'
        
        R_Decode = R_Mantiss
        R_Decode = R_Decode..' '..'���'
      end 
    elseif( Value_float < 10000000 ) then
      if( #R_Mantiss == 2 ) then  -- E24      
        R_Code = R_Mantiss..'04'
      
        R_Decode = R_Mantiss/10
        R_Decode = string.format('%.01f', R_Decode)
        R_Decode = R_Decode:gsub('%.', ',')
        R_Decode = R_Decode:gsub(',0$', '')
        R_Decode = R_Decode..' '..'���'
      else  -- E96
        R_Code = R_Mantiss..'4'
        
        R_Decode = R_Mantiss/100
        R_Decode = string.format('%.02f', R_Decode)
        R_Decode = R_Decode:gsub('%.', ',')
        R_Decode = R_Decode:gsub(',00$', '')
        R_Decode = R_Decode:gsub('0$', '')
        R_Decode = R_Decode..' '..'���'
      end 
    else
      print('ERROR: Out of Range')
    end
  
  
--[==[  

   
      if( #R_Mantiss == 2 ) then  --ok
        R_Code = R_Mantiss/10
        R_Code = string.format('%.02f', R_Code) 
        R_Code = R_Code:gsub('%.', 'R')
        R_Decode = R_Mantiss/10
        R_Decode = string.format('%.01f', R_Decode)
        R_Decode = R_Decode:gsub('%.', ',')
        R_Decode = R_Decode:gsub(',0$', '')
        R_Decode = R_Decode..' '..'��'    
      else  --ok
        R_Code = R_Mantiss/100
        R_Code = string.format('%.02f', R_Code)
        R_Code = R_Code:gsub('%.', 'R')
        R_Decode = R_Mantiss/100
        R_Decode = string.format('%.02f', R_Decode)
        R_Decode = R_Decode:gsub('%.', ',')
        R_Decode = R_Decode:gsub(',00$', '')
        R_Decode = R_Decode:gsub('0$', '')
        R_Decode = R_Decode..' '..'��'
      end   
  


R_Decode
MUX[MUX_Num][3]


    if ( Value_float == 0 ) then
      -- ERROR
    elseif ( Value_float < 100 ) then
      R_Code = R_Mantiss:sub(1, (#R_Mantiss - 1))..'R'..R_Mantiss:sub(#R_Mantiss)
    else
      R_Code = R_Mantiss..MUX[MUX_Num][1]
      
      if( MUX[MUX_Num][3] == false )  then
        R_Decode = R_Mantiss:sub(1, (#R_Mantiss - 1))..','..R_Mantiss:sub(#R_Mantiss)
      else
        R_Decode = R_Mantiss
      end
    end
]==]    
    
  elseif( TOLERANCE_Num == 2 ) then  -- 5%
    if ( Value_float == 0 ) then
      R_Code = R_Mantiss..'0'
    elseif ( Value_float < 10 ) then
      R_Code = R_Mantiss:sub(1, (#R_Mantiss - 1))..'R'..R_Mantiss:sub(#R_Mantiss)
    else
      R_Code = R_Mantiss..MUX[MUX_Num][1]
      
      if( MUX[MUX_Num][3] == false ) then
        R_Decode = R_Mantiss:sub(1, (#R_Mantiss - 1))..','..R_Mantiss:sub(#R_Mantiss)
      else
        R_Decode = R_Mantiss
      end
    end  
  else
  
  end
  
  
  
  
  
  Code = Code..R_Code..PACKAGING[SIZE_Num][2]..TERMINATION[1][1]
  --Decode = Decode..R_Decode..' '..MUX[MUX_Num][3]..')'
  Decode = Decode..R_Decode..')'
  print(Code)
  print(Decode)
---------------------------  
  
  
  
  
  
  
  
  
  
  
  
  
end
















local function exec(fin, fout, flog)

end



------------ main ------------

if arg[0]:find('CR_Bourns_decode.lua') ~= nil then
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
  Decode = CR_Bourns_Decode,
  Code = CR_Bourns_Code,
}


