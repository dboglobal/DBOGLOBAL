
Scene(1, "1059")
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1  )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3075 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 10 )
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 10591 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 1059 )
		Param( "event id", 236 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3076 )
	--]
	End()

	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 1059 )
			Param( "event id", 237 )
		--]
		End()
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3077 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 2 )
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 10592 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3078 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 1059 )
		Param( "event id", 238 )
	--]
	End()

	Action( "sps end" )
	--[

	--]
	End()
--]
End()