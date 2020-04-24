Scene( 1, "7541301" )
--[
	Action( "char condition" )
	--[
		Param( "invincible", 0 )
		Param( "ui disable", 1 )
	--]
	End()

	-- 이제 슬슬 도망쳐 볼까….
	Action( "say" )
	--[
		Param( "tblidx", 3269 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()

	Action( "point move" )
	--[
		Param( "loc x", -1939.09 )
		Param( "loc y", -74.10 )
		Param( "loc z", -3940.41 )
		Param( "dir x", 0.00 )
		Param( "dir z", 1.00 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 2569 )
		Param( "event id", 525 )
	--]
	End()

	Action( "sps end" )
	--[
	--]
	End()
--]
End()
