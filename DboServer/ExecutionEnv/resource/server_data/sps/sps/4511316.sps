Scene( 1, "2948" )
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
			Param( "wps index", 2948 )
			Param( "event id", 699 )
		--]
		End()
	--]
	End()
	
	Action( "path move" )
	--[
		Param( "tblidx", 120 )
		Param( "run mode", 1)
	--]
	End()	
	
	Action( "say" )
	--[
		Param( "tblidx", 3333 )
	--]
	End()	
	
	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()	
	
	Action( "path move" )
	--[
		Param( "tblidx", 121 )
		Param( "run mode", 1 )
	--]
	End()			
	
	Action( "say" )
	--[
		Param( "tblidx", 3334 )
	--]
	End()							
		
	Action( "say" )
	--[
		Param( "tblidx", 3335 )
	--]
	End()	
	 
	Action( "path move" )
	--[
		Param( "tblidx", 122 )
		Param( "run mode", 1 )
	--]
	End()		
	
	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()	
	
	Action( "path move" )
	--[
		Param( "tblidx", 123 )
		Param( "run mode", 1 )
	--]
	End()		
		
	Action( "say" )
	--[
		Param( "tblidx", 3337 )
	--]
	End()
		
	
	Action( "send event to wps" )
	--[
		Param( "wps index", 2948 )
		Param( "event id", 700 )
	--]
	End()		
	
	Action( "sps end" )
	--[
	--]
	End()
--]
End()
	