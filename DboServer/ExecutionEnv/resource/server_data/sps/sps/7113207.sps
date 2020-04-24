
Scene(1, "TMQ 2-1" )
--[
	Action( "point move" )
	--[
		Param( "loc x", 34.15 )
		Param( "loc y", 148.49 )
		Param( "loc z", 252.48 )
		Param( "run mode", 1 )
	--]
	End()
	
	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 666 )
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

Scene(2, "TMQ 2-1" )
--[
	Action( "point move" )
	--[
		Param( "loc x", 35.78 )
		Param( "loc y", 147.78 )
		Param( "loc z", 246.03 )
		Param( "run mode", 1 )
	--]
	End()
	
	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 666 )
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