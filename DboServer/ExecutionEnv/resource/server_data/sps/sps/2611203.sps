
Scene(1, "551" )
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	Action("say")
	--[
		Param("tblidx",3053)
	--]
	End()

	Action("wait")
	--[
		Param("time",10)
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3054 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 2 )
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 2611203 )
		Param( "run mode", 1 )
	--]
	End()

	Action("ai bit flag")
	--[
		Param("attack type", "offensive")
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3055)
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 551 )
		Param( "event id", 203 )
	--]
	End()

	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 551 )
			Param( "event id", 76 )
		--]
		End()
	--]
	End()

	Action( "sps end" )
	--[

	--]
	End()
--]
End()