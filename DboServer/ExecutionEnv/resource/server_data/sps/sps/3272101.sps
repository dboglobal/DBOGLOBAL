
Scene(1,"469")
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3073 )
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 3272101 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 469 )
		Param( "event id", 87 )
	--]
	End()

	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 469 )
			Param( "event id", 91 )
		--]
		End()
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 3272103 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "sps end" )
	--[

	--]
	End()
--]
End()