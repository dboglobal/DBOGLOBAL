
Scene(1,"449")
--[
	Action( "wait" )
	--[
		Param( "event type", "ts" )
		Param( "event id", 98 )
	--]
	End()

	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	Action( "direct play" )
	--[
		Param( "tblidx", 60030 )
	--]
	End()

	Action( "direct play" )
	--[
		Param( "tblidx", 60031 )
	--]
	End()
	
	Action( "char condition" )
	--[
		Param( "ui disable", 0 )
	--]
	End()
--]
End()
