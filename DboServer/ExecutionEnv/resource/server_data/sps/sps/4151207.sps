
Scene(1, "507" )

--[
	Action( "escort" )
	--[
		Param( "type", "under escort" )
	--]
	End()

	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 2 )
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 4151202 )
		Param( "run mode", 1 )
	--]
	End()
	
	Action( "send sps event")
	--[
		Param( "event type", "wps" )
		Param( "trigger type", "quest" )
		Param( "event id", 77 )
	--]
	End()

	Action( "sps end" )
	--[
	
	--]
	End()
--]
End()
