
Scene(1, "2445" )
--[
	Action( "point move" )
	--[
		Param( "loc x", 167.862 )
		Param( "loc y", -33.456 )
		Param( "loc z", 383.496 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index",  2445 )
			Param( "event id", 666 )
		--]
		End()
	--]
	End()
	        
        Action( "sps end" )
        --[
        
        --]
        End()
--]
End()

Scene(2, "2445" )
--[
	Action( "point move" )
	--[
		Param( "loc x", 164.094 )
		Param( "loc y", -33.727 )
		Param( "loc z", 383.212 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index",  2445 )
			Param( "event id", 666 )
		--]
		End()
	--]
	End()
        
        Action( "sps end" )
        --[
        
        --]
        End()
--]
End()

Scene(3, "2445" )
--[
	Action( "point move" )
	--[
		Param( "loc x", 160.534 )
		Param( "loc y", -33.201 )
		Param( "loc z", 384.052 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index",  2445 )
			Param( "event id", 666 )
		--]
		End()
	--]
	End()
        
        Action( "sps end" )
        --[
        
        --]
        End()
--]
End()