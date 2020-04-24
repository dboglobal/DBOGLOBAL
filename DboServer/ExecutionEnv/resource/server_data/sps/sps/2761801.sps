------------------------------------------------------
--
-- Fake Goku. TLQ 3 follow PC
-- Author : YeoHyeonJoon (souled@ntl-inc.com)
--
------------------------------------------------------

Scene( 1, "TLQ 3 Fake Goku" )

--User Move
	Action( "escort" )
	--[
		Param( "type", "target follow" )     
		Param( "dest x", -123.44 )
		Param( "dest z", -16.34 )
		Param( "radius", 20 )
		Param( "event id", 16770 )      
	--]
	End()
	
    Action( "point move" )
    --[
		Param( "loc x", -123.12)
		Param( "loc z", -9.57)
		Param( "run mode", 1 )
    --]
    End()
	
    Action( "point move" )
    --[
		Param( "loc x", -123.44)
		Param( "loc z", -13.67)
		Param( "run mode", 1 )
    --]
    End()
	
	Action( "sps end" )
	--[
	--]
	End()
	
End()