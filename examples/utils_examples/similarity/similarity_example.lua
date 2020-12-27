-- codepage cp1251

local s = require ('similarity')


------------ main ------------
print('-----------')
local a = "(GRM188-X7R-16б-0,1 ЛЙт╠20%)\\GRM188R71C104Mxx\\Murata"
local b = "(GRM188-X7R-16б-0,1 ЛЙт╠20%)\\GRM188R71C104Mxx\\Murata"

local result = s.is_Similarity(a, b)
if (result[1] == 0 ) then
  print('онкмне янбоюдемхе')
else
  print('пюгкхвхе: '..result[2])
  print(a..'<>'..b)
end


print('-----------')
local a = "(GRM188-X7R-16б-0,1 ЛЙт╠20%)"
local b = "(GRM188-X7R-16B-0,1 ЛЙт╠20%)"

local result = s.is_Similarity(a, b)
if (result[1] == 0 ) then
  print('онкмне янбоюдемхе')
else
  print('пюгкхвхе: '..result[2])
  print(a..'<>'..b)
end


os.exit(0)
