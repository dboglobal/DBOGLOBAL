
Scene(1,"1251")

--[
	Action( "wait" )
	--[
		Param( "event type", "ts" )
		Param( "event id", 111 )
	--]
	End()	

	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	Action( "rotate" )
	--[
		Param( "dir x", 0 )
		Param( "dir y", 0 )
		Param( "dir z", 0 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", "3025" )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()

	Action( "rotate" )
	--[
		Param( "dir x", -0.97 )
		Param( "dir y", 0 )
		Param( "dir z", 0.24 )
	--]
	End()

	Action( "char condition" )
	--[
		Param( "ui disable", 0 )
	--]
	End()
--]
End()
