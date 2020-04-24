
Scene( 1, "Q292" )
--[	
	Action("char condition")
	--[
		Param("ui disable", 1)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3211)
	--]
	End()
	
	Action( "path move" )
	--[
		Param( "tblidx", 292 )
		Param( "run mode", 0 )
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 292)
		Param("event id", 442)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 292)
			Param("event id", 443)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3212)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 292)
			Param("event id", 444)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3214)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 7014)
		Param("run mode", 0)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 292)
		Param("event id", 445)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 292)
			Param("event id", 446)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3212)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 292)
			Param("event id", 447)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3213)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 7015)
		Param("run mode", 0)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3215)
	--]
	End()

	Action("wait")
	--[
		Param("time", 4)
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 292 )
		Param( "event id", 213 )
	--]
	End()

	Action( "sps end" )
	--[

	--]
	End()
--]
End()