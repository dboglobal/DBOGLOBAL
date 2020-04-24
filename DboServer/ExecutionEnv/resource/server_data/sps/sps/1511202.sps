Scene(1, "529" )
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3079)
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 2025)
		Param( "run mode", 1 )
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3080)
	--]
	End()

	Action("rotate")
	--[
		Param("dir x", -0.441)
		Param("dir z", 0.898)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 529)
		Param("event id", 257)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 529)
			Param("event id", 259)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3081)
	--]
	End()


	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 529)
			Param("event id", 258)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3082)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 2026)
		Param("run mode", 1)
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 529 )
		Param( "event id",  232)
	--]
	End()

	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 529 )
			Param( "event id", 260)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3081)
	--]
	End()

	

	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 529 )
			Param( "event id", 233)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3082)
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 2027)
		Param( "run mode", 1 )
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3083)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3084)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 529 )
		Param( "event id", 234)
	--]
	End()

	Action( "sps end" )
	--[

	--]
	End()
--]
End()

