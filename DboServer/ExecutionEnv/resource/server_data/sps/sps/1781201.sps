
Scene(1, "Always" )
--[
	Action( "sitdown" )
	--[
		Param( "sitdown", "true" )
	--]
	End()

	Action( "sps end" )
	--[
	
	--]
	End()
--]
End()

Scene(2, "559")
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	Action( "sitdown" )
	--[
		Param( "sitdown", "false" )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 2 )
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 1000 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 559 )
		Param( "event id", 70 )
	--]
	End()

	Action( "sps end" )
	--[

	--]
	End()
--]
End()