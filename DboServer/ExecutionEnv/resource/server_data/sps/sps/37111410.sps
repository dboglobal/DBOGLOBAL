-- TMQ 2-1 Hard Mode
-- 레진 스토커 이동

Scene(1, "TMQ 2-1" )
--[
	Action( "point move" )
	--[
		Param( "loc x", 37.35 )
		Param( "loc y", 148.83 )
		Param( "loc z", 251.93 )
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
		Param( "loc x", 33.58 )
		Param( "loc y", 147.97 )
		Param( "loc z", 249.54 )
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

Scene(3, "TMQ 2-1" )
--[
	Action( "point move" )
	--[
		Param( "loc x", 36.28 )
		Param( "loc y", 148.14 )
		Param( "loc z", 249.24 )
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

