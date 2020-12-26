--local R1_12 = require ("R1_12_decode")



function by_Value_Firm_Ref( a_Ref, a_Value_Firm, a_Value_Code,  b_Ref, b_Value_Firm, b_Value_Code )
--  print( a_Ref..' '..a_Value_Code..' '..a_Value_Firm..'  <->  '..b_Ref..' '..b_Value_Code..' '..b_Value_Firm )

  if ( a_Ref < b_Ref ) then
    return true
  elseif ( a_Ref == b_Ref ) then
    if ( a_Value_Firm < b_Value_Firm ) then
      return true
    elseif (a_Value_Firm == b_Value_Firm) then
      if( a_Ref == 'R' ) then
        --if ( R1_12.Detect( a_Value_Code )  and  R1_12.Detect( b_Value_Code ) ) then
        --  local result = R1_12.compare_by_name(a_Value_Code, b_Value_Code)
        --  if( result == true ) then
        --    print( '    --- '..a_Value_Code..' < '..b_Value_Code )
        --  else
        --    print( '    --- '..a_Value_Code..' > '..b_Value_Code )
        --  end
        --  return result
        --else
          if (a_Value_Code < b_Value_Code) then
            return true
          end
        --end
      else
        if (a_Value_Code < b_Value_Code) then
          return true
        end
      end
    end
  end
  return false
end




function by_Type_from_Value_and_Code_from_Value(a_Type_from_Value, a_Code_from_Value, b_Type_from_Value, b_Code_from_Value )
--print( '++++++1++++++'..a_Code_from_Value..' <----> '..b_Code_from_Value )

  if ( a_Type_from_Value < b_Type_from_Value ) then
    return true;
  elseif ( a_Type_from_Value == b_Type_from_Value ) then
    --if ( R1_12.Detect( a_Code_from_Value )  and  R1_12.Detect( b_Code_from_Value ) ) then
    --  local result = R1_12.compare_by_name(a_Code_from_Value, b_Code_from_Value)
      
    --  if( result == true ) then
    --   -- print( '    --- '..a_Code_from_Value..' < '..b_Code_from_Value )
    --  else
    --   -- print( '    --- '..a_Code_from_Value..' > '..b_Code_from_Value )
    --  end
    --  return result
    --else
      if (a_Code_from_Value < b_Code_from_Value) then
        return true
      end
    --end
  end

  return false
end






