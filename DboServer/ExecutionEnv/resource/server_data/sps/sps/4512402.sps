Scene( 1, "3023" )
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
			Param( "wps index", 3023 )
			Param( "event id", 722 )
		--]
		End()
	--]
	End()
	
	Action( "path move" )
	--[
		Param( "tblidx", 302301)
		Param( "run mode", 1)
	--]
	End()		
	
	Action( "send event to wps" )
	--[
		Param( "wps index", 3023 )
		Param( "event id", 723 )
	--]
	End()		
	
	Action( "sps end" )
	--[
	--]
	End()
--]
End()
	