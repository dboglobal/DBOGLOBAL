
Scene(1, "Escort" )
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3200 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 5 )
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 4 )
		Param( "run mode", 0 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 2 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3201 )
	--]
	End()

	Action( "direct play" )
	--[
		Param( "tblidx", 60045 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 6243 )
		Param( "event id", 340 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()

	Action( "point move" )
	--[
		Param( "loc x", -52.884 )
		Param( "loc y", 27.916 )
		Param( "loc z", 24.986 )
		Param( "dir x", 0.636 )
		Param( "dir z", -0.772 )
		Param( "run mode", 0 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 2 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 6243 )
		Param( "event id", 343 )
	--]
	End()

	Action( "sps end" )
	--[

	--]
	End()
--]
End()
