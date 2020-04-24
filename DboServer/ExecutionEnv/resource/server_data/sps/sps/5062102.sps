
Scene(1, "1209 puker" )
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	Action( "direct play" )
	--[
		Param( "tblidx", 60032)
	--]
	End()

	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 1209 )
			Param( "event id", 279)
		--]
		End()
	--]
	End()

	Action("direct play")
	--[
		Param("tblidx", 60035)
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3124)
	--]
	End()

	Action( "direct play" )
	--[
		Param( "tblidx", 60033)
	--]
	End()

	Action("direct play")
	--[
		Param("tblidx", 60036)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3127)
	--]
	End()

	Action("direct play")
	--[
		Param("tblidx", 60034)
	--]
	End()

	Action("wait")
	--[
		Param("time", 2)
	--]
	End()
	
	Action("direct play")
	--[
		Param("tblidx", 60037)
	--]
	End()


	Action( "char condition" )
	--[
		Param( "ui disable", 0 )
	--]
	End()

	Action( "sps end" )
	--[

	--]
	End()
--]
End()