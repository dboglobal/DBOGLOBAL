Scene( 1, "2988" )
--[
	Action( "char condition" )
	--[
		Param( "invincible", 0 )
		Param( "ui disable", 1 )
	--]
	End()	
	
	Action( "say")
	--[
		Param( "tblidx", 3338 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()

	Action( "say")
	--[
		Param( "tblidx", 3339 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()

	Action( "say")
	--[
		Param( "tblidx", 3340 )
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 131 )
		Param( "run mode", 1)
	--]
	End()		

	Action( "say")
	--[
		Param( "tblidx", 3341 )
	--]
	End()
	
	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()	

	Action( "path move" )
	--[
		Param( "tblidx", 132 )
		Param( "run mode", 1)
	--]
	End()		

	Action( "say")
	--[
		Param( "tblidx", 3342 )
	--]
	End()
	
	Action( "path move" )
	--[
		Param( "tblidx", 133 )
		Param( "run mode", 1)
	--]
	End()		

	Action( "say")
	--[
		Param( "tblidx", 3343 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 134 )
		Param( "run mode", 1)
	--]
	End()		

	Action( "say")
	--[
		Param( "tblidx", 3344 )
	--]
	End()
	
	Action( "send event to wps" )
	--[
		Param( "wps index", 2988)
		Param( "event id", 707 )
	--]
	End()		
	
	Action( "sps end" )
	--[
	--]
	End()
--]
End()
	