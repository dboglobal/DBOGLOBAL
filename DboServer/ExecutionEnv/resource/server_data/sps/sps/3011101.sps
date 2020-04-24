
Scene(1, "6226")
--[
	Action( "char condition")
	--[
		Param( "ui disable", 1 )
		Param( "invincible", 1 )
	--]
	End()
	
	Action( "wait" )
	--[
		Param( "time", 2 )    
	--]
	End()
	
	Action( "path move" )
	--[
		Param( "tblidx", 4 )
		Param( "run mode", 0 )
	--]
	End()

	Action( "char condition")
	--[
		Param( "ui disable", 0 )
	--]
	End()

	Action( "sps end" )
	--[

	--]
	End()
--]
End()