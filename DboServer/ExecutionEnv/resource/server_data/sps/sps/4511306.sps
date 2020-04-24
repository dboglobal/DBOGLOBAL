Scene(1,"427")
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()
	
	Action( "wait" )
	--[
		Param( "time", 1 )
	--]
	End()

	Action( "direct play" )
	--[
		Param( "tblidx", 60015 )
	--]
	End()

	Action("wait")
	--[
		Param("time",1)
	--]
	End()

	Action( "direct play" )
	--[
		Param( "tblidx", 60016 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 2 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 427 )
		Param( "event id", 18 )
	--]
	End()

	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 427 )
			Param( "event id", 19 )
		--]
		End()
	--]
	End()

	Action("point move")
	--[
		Param("loc x", 5799.726)
		Param("loc z", -3362)
		Param("run mode", 1)
	--]
	End()

	Action( "point move" )
	--[
		Param( "loc x", 5765.807 )
		Param( "loc z", -3328.821 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 427 )
		Param( "event id", 23 )
	--]
	End()

	Action( "sps end" )
	--[
	
	--]
	End()

--]
End()
