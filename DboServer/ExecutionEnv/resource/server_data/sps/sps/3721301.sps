Scene(1, "3720301" )
--[
	Action( "char condition" )
	--[
		Param( "invincible", 0 )
		Param( "ui disable", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3237 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 5 )
	--]
	End()
	
	Action( "path move" )
	--[
		Param( "tblidx", 7022 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3238 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 2174 )
		Param( "event id", 490 )
	--]
	End()

	Action( "sps end" )
	--[
	--]
	End()
--]
End()
