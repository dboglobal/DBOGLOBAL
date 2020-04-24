Scene( 1, "3016" )
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
			Param( "wps index", 3016 )
			Param( "event id", 719 )
		--]
		End()
	--]
	End()
	
	Action( "path move" )
	--[
		Param( "tblidx", 127 )
		Param( "run mode", 1)
	--]
	End()	
	

	Action( "say")
	--[
		Param( "tblidx", 3345 )
	--]
	End()	
	
	Action( "path move" )
	--[
		Param( "tblidx", 128)
		Param( "run mode", 1)
	--]
	End()	
	
	Action( "say")
	--[
		Param( "tblidx", 3346 )
	--]
	End()	
	
	Action( "path move" )
	--[
		Param( "tblidx", 129)
		Param( "run mode", 1)
	--]
	End()	

	Action( "say")
	--[
		Param( "tblidx", 3347 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()

	Action( "say")
	--[
		Param( "tblidx", 3348 )
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 130 )
		Param( "run mode", 1)
	--]
	End()	

	Action( "say")
	--[
		Param( "tblidx", 3349 )
	--]
	End()
		
	Action( "send event to wps" )
	--[
		Param( "wps index", 3016 )
		Param( "event id", 720 )
	--]
	End()		
	
	Action( "sps end" )
	--[
	--]
	End()
--]
End()
	