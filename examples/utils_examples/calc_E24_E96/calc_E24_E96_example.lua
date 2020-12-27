-- codepage cp1251

local calc = require ('calc_E24_E96')


------------ main ------------

print('---------')
local r_value = 33.5
print("r_value = ",r_value, "--> E24 --> ", calc.calc_E24(r_value))
print("r_value = ",r_value, "--> E24_plus_E96 --> ", calc.calc_E24_plus_E96(r_value))


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



os.exit(0)
