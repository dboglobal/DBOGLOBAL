
Scene(1,"1251")

--[
	Action( "wait" )
	--[
		Param( "event type", "ts" )
		Param( "event id", 110 )
	--]
	End()

	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", "3026" )
	--]
	End()

	Action( "char condition" )
	--[
		Param( "ui disable", 0 )
	--]
	End()
--]
End()