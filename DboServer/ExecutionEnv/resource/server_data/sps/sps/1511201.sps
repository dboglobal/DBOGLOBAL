
Scene(1,"469")
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
		Param( "attack disallow", 1 )
		Param( "invincible", 1 )
	--]
	End()

	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 469 )
			Param( "event id", 89 )
		--]
		End()
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 1511201 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 469 )
		Param( "event id", 90 )
	--]
	End()

	Action( "sps end" )
	--[

	--]
	End()
--]
End()
