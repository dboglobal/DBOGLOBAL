
Scene(1, "242")
--[
	Action( "say" )
	--[
		Param( "tblidx", 3003 )
	--]
	End()

	Action( "wait")
	--[
		Param( "time", 8 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 242 )
		Param( "event id", 214 )
	--]
	End()

	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 242 )
			Param( "event id", 216 )
		--]
		End()
	--]
	End()


	Action( "rotate" )
	--[
		Param( "dir x", -0.991 )
		Param( "dir y", 0 )
		Param( "dir z", -0.328 )
	--]
	End()

	Action( "wait")
	--[
		Param( "time", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3005 )
	--]
	End()


	Action( "send event to wps" )
	--[
		Param( "wps index", 242 )
		Param( "event id", 218 )
	--]
	End()

	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 242 )
			Param( "event id", 220 )
		--]
		End()
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3006 )
	--]
	End()

	Action( "point move" )
	--[
		Param( "loc x", 4060.823 )
		Param( "loc z", -2878.556 )
		Param( "run mode", 1)
	--]
	End()


	Action( "send event to wps" )
	--[
		Param( "wps index", 242 )
		Param( "event id", 222 )
	--]
	End()


	Action( "sps end" )
	--[
	
	--]
	End()
--]
End()
