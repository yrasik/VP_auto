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


local true_elements_2_lua = {}


local function version()
  return 'v 2.0a'
end


local function info()
  local info = 
[[
  ������ ��� ����������� � ������ 'true_elements.txt' (���� � ����������� �������������� ������� ���������� ���������).
  ���������:
    1) ����������� ���� � ������� lua.
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


local function replace(str, what, with)
  what = what:gsub("[%(%)%.%+%-%*%?%[%]%^%$%%]", "%%%1")
  with = with:gsub("[%%]", "%%%%")
  return str:gsub(what, with)
end


local preamble = 
[[
-- codepage cp1251

if( type(TrueElements) ~= 'table' ) then
  TrueElements = {}
end


]]


local template = 
[==[
TrueElements[#TrueElements + 1] = {
  Ref = "%s",
  Group_Name = "%s",
  Type_from_Value = "%s",
  Code_from_Value = "%s",
  Value_Decode = "%s",
  Value_Firm = "%s",
  Function = "",
  Parameters = "",
  Package_Type = "",
  Package_Size = "",
  Description = "",
  Op_Temp = "",
  Value = "%s",
  Old_Values = {%s
  }
}


]==]


local template_for_old = 
[[
    {
      Ref = "%s",
      Group_Name = "%s",
      Type_from_Value = "%s",
      Code_from_Value = "%s",
      Value_Decode = "%s",
      Value_Firm = "%s",
      Value = "%s",
    },
]]





local STATE =
{
  IDLE                        = 0x00,
  ENTER                       = 0x01,
  MINUS                       = 0x02,
}



local function get_data_from_value(Value, Name, flog, line, state)
  local Type_from_Value
  local Code_from_Value
  local Value_Code
  local Value_Decode
  local Value_Firm
  local Ref
  local Group_Name

  if( Value ~= nil ) then
    Value_Decode_Code_Firm, Ref = Value:match('^([%d%D]+)@([%d%D]+)$')

    if( Value_Decode_Code_Firm == nil or
        Ref == nil ) 
    then
      flog:write('ERROR: in line '..line..' ": '..Value..'"\n')
    end

    Value_Decode_Code, Value_Firm = Value_Decode_Code_Firm:match('^([%d%D]*\\[%d%D]+)\\([%d%D]+)$')

    --print(Value_Decode_Code)
    Value_Decode = Value_Decode_Code:match('^([%d%D]+)\\[%d%D]+$')

    if( Value_Decode == nil ) then
      Value_Decode = ''
      Value_Code = Value_Decode_Code:match('^\\([%d%D]+)$')
    else
      Value_Code = replace(Value_Decode_Code, Value_Decode..'\\', '')
    end

    local i
    for i = 1, #Name  do
      local Name_ = Name[i].Name:gsub(' ', ' *')
      Name_ = Name[i].Name..' ' -- "��� ������ VS-6CWQ10FNTR-M3",
      local res = Value_Code:find(Name_)

      if(res ~= nil) then
        Code_from_Value = Value_Code:gsub(Name_, '')
        Type_from_Value = Name[i].Name
        Group_Name = Name[i].Group_Name
        break
      else
        if( Name[i].Ref == Ref ) then
          Type_from_Value = Name[i].Group_Name_one
          Group_Name = Name[i].Group_Name
        end
      end
    end

    if( Code_from_Value == nil) then
      Code_from_Value = Value_Code
    end

    if( Type_from_Value == nil) then
      Type_from_Value = ''
    end
    
    state = STATE.MINUS
    Value = Value:gsub('\\', '\\\\')
  end

  return Value, Type_from_Value, Code_from_Value, Value_Code, Value_Decode, Value_Firm, Group_Name, Ref, state
end


local function exec(fin, fout, flog, Name)
  fout:write(preamble)

  local state = STATE.IDLE
  local Ref = nil
  local Value_Decode_Code_Firm = nil
  local Value_Decode_Code = nil
  local Group_Name = nil
  local Type_from_Value = nil
  local Code_from_Value = nil
  local Value_Decode = nil
  local Value_Code = nil
  local Value_Firm = nil
  local Value = nil
  local Old_Values = ''
  local line = 1

  for l in fin:lines() do
    if ( state == STATE.IDLE ) then
      if ( l:match('^%@') ~= nil ) then
        state = STATE.ENTER
      end
    elseif( state == STATE.ENTER ) then
      Value = l:match('^%+([%d%D]+)')
      Value,
      Type_from_Value,
      Code_from_Value,
      Value_Code,
      Value_Decode,
      Value_Firm,
      Group_Name,
      Ref,
      state = get_data_from_value(Value, Name, flog, line, state)
      
    elseif( state == STATE.MINUS) then
      local Old_Value = l:match('^%-([%d%D]+)')
      if( Old_Value ~= nil ) then
        --Old_Value = Old_Value:gsub('\\', '\\\\')
        local Value,
        Type_from_Value,
        Code_from_Value,
        Value_Code,
        Value_Decode,
        Value_Firm,
        Group_Name,
        Ref = get_data_from_value(Old_Value, Name, flog, line)

        Old_Value = string.format(template_for_old,
                                   Ref,
                                   Group_Name,
                                   Type_from_Value,
                                   Code_from_Value,
                                   Value_Decode,
                                   Value_Firm,
                                   Value
                                 )
        --Old_Values = Old_Values..'    "'..Old_Value..'",\n'
        Old_Values = Old_Values..Old_Value
      else
        if ( l:find('^%@') ~= nil ) then
          state = STATE.ENTER
          local str = string.format(template,
                               Ref,
                               Group_Name,
                               Type_from_Value,
                               Code_from_Value,
                               Value_Decode,
                               Value_Firm,
                               Value,
                               Old_Values
                            )
          fout:write(str)
          Value_Decode_Code_Firm = nil
          Value_Decode_Code = nil
          Group_Name = nil
          Type_from_Value = nil
          Code_from_Value = nil
          Value_Decode = nil
          Value_Code = nil
          Value_Firm = nil
          Ref = nil
          Value = nil
          Old_Values = "\n"
        end
      end
    else

    end
    line = line + 1
  end
end



------------ main ------------

if arg[0]:find('true_elements_2_lua.lua') ~= nil then
  local args = process_args()

  local fin = io.open(args.fin_name, 'r')
  if ( fin == nil ) then
    os.exit(-1)
  end

  dofile( args.fin_names_lua ) -- Name = {}

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

  exec(fin, fout, flog, Name)

  fin:close()
  fout:close()

  dofile(args.fout_name)
  for i = 1, #TrueElements do
    flog:write('Reading element "'..TrueElements[i].Code_from_Value..'"\n')
  end

  flog:close()

  os.exit(0)
end


return {
  version = version,
  info = info,
  get_data_from_value = get_data_from_value,
  exec = exec
}

