
Scene (1, "242")
--[
	Action( "point move" )
	--[
		Param( "loc x", 4096.167 )
		Param( "loc z", -2915.384 )
		Param( "run mode", 1)
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 242 )
		Param( "event id", 215 )
	--]
	End()

	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 242 )
			Param( "event id", 217 )
		--]
		End()
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3004 )
	--]
	End()

	Action( "direct play" )
	--[
		Param( "tblidx", 60008 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 4 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 242 )
		Param( "event id", 219 )
	--]
	End()

	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 242 )
			Param( "event id", 221 )
		--]
		End()
	--]
	End()

	Action( "direct play" )
	--[
		Param( "tblidx", 60009 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3007 )
	--]
	End()

	Action("wait")
	--[
		Param("time", 1)
	--]
	End()

	Action( "point move" )
	--[
		Param( "loc x", 4061.244 )
		Param( "loc z", -2879.062 )
		Param( "run mode", 1)
	--]
	End()


	Action( "send event to wps" )
	--[
		Param( "wps index", 242 )
		Param( "event id", 223 )
	--]
	End()

	Action( "sps end" )
	--[
	
	--]
	End()

--]
End()