
Scene(1,"440")
--[
	Action( "wait" )
	--[
		Param( "event type", "ts" )
		Param( "event id", 99 )
	--]
	End()

	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	Action( "direct play" )
	--[
		Param( "tblidx", 60028 )
	--]
	End()
	
	Action( "direct play" )
	--[
		Param( "tblidx", 60029 )
	--]
	End()

	Action( "char condition" )
	--[
		Param( "ui disable", 0 )
	--]
	End()
--]
End()
