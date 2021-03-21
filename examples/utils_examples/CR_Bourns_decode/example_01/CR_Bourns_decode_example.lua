-- codepage cp1251

local CR = require ('CR_Bourns_decode')


------------ main ------------

--print('---------------------------------')
--local Res, Mess = CR.Code('0805', '14,1 kOm','1%')

--print('---------------------------------')
--local Res, Mess = CR.Code('0805', '60,1 Om','1%')

--print('---------------------------------')
--local Res, Mess = CR.Code('0805', '2,0 Om','1%')

--print('---------------------------------')
--local Res, Mess = CR.Code('0805', '2,42 Om','1%')


--print('---------------------------------')
--local Res, Mess = CR.Code('0805', '20,0 Om','1%')

--print('---------------------------------')
--local Res, Mess = CR.Code('0805', '24,2 Om','1%')


--print('---------------------------------')
--local Res, Mess = CR.Code('0805', '200 Om','1%')

--print('---------------------------------')
--local Res, Mess = CR.Code('0805', '242 Om','1%')


--print('---------------------------------')
--local Res, Mess = CR.Code('0805', '2 kOm','1%')

--print('---------------------------------')
--local Res, Mess = CR.Code('0805', '2,42 kOm','1%')


--print('---------------------------------')
--local Res, Mess = CR.Code('0805', '20 kOm','1%')

--print('---------------------------------')
--local Res, Mess = CR.Code('0805', '24,2 kOm','1%')


--print('---------------------------------')
--local Res, Mess = CR.Code('0805', '200 kOm','1%')

--print('---------------------------------')
--local Res, Mess = CR.Code('0805', '242 kOm','1%')


print('---------------------------------')
local Res, Mess = CR.Code('0805', '2 MOm','1%')

print('---------------------------------')
local Res, Mess = CR.Code('0805', '2,42 MOm','1%')







--[==[
print(Mess)

print('---------')
local Res, Mess = CR.Code('0201', '14,1 kOm','1%')

print('---------')
local Res, Mess = CR.Code('0201', '14,1 kOm','5 %')

print('---------')
local Res, Mess = CR.Code('0201', '9,1 Om','5 %')


print('---------')
local Res, Mess = CR.Code('0805', '0 Om','5 %')


--print("r_value = ",r_value, "--> E24 --> ", calc.calc_E24(r_value))
--print("r_value = ",r_value, "--> E24_plus_E96 --> ", calc.calc_E24_plus_E96(r_value))
]==]


--[==[
print('---------')
local r_value = 3.39
print("r_value = ",r_value, "--> E24 --> ", calc.calc_E24(r_value))
print("r_value = ",r_value, "--> E24_plus_E96 --> ", calc.calc_E24_plus_E96(r_value))



print('---------')
local r_value = 978
print("r_value = ",r_value, "--> E24 --> ", calc.calc_E24(r_value))
print("r_value = ",r_value, "--> E24_plus_E96 --> ", calc.calc_E24_plus_E96(r_value))


print('---------')
local r_value = 988
print("r_value = ",r_value, "--> E24 --> ", calc.calc_E24(r_value))
print("r_value = ",r_value, "--> E24_plus_E96 --> ", calc.calc_E24_plus_E96(r_value))
]==]


os.exit(0)
