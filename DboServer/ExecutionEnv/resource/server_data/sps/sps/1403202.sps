Scene( 1, "3013" )
--[
	Action( "char condition" )
	--[
		Param( "invincible", 0 )
		Param( "ui disable", 1 )
	--]
	End()
	
	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 3013 )
			Param( "event id", 716 )
		--]
		End()
	--]
	End()
	
	Action( "path move" )
	--[
		Param( "tblidx", 3013 )
		Param( "run mode", 1)
	--]
	End()	
	
	Action( "send event to wps" )
	--[
		Param( "wps index", 3013 )
		Param( "event id", 717 )
	--]
	End()		
	
	Action( "sps end" )
	--[
	--]
	End()
--]
End()
	