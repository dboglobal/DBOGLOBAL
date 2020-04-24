
Scene(1, "980" )
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	Action("say")
	--[
		Param("tblidx",3099)
	--]
	End()

	Action("wait")
	--[
		Param("time",3)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 2004)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3100)
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 980 )
		Param( "event id", 264)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 980)
			Param("event id", 265)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3101)
	--]
	End()

	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 980 )
			Param( "event id", 266)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3102)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 2005)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3103)
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 980 )
		Param( "event id", 267)
	--]
	End()

	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 980 )
			Param( "event id", 268)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3101)
	--]
	End()

	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 980 )
			Param( "event id", 269)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3102)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 2006)
	--]
	End()

	Action("rotate")
	--[
		Param("dir x", -0.198)
		Param("dir y", 0)
		Param("dir z", -0.98)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3104)
	--]
	End()

	Action("wait")
	--[
		Param("time",6)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3105)
	--]
	End()

	Action("wait")
	--[
		Param("time",3)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3106)
	--]
	End()

	Action("wait")
	--[
		Param("time",3)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 2028)
		Param("run mode", 1)
	--]
	End()

	Action("point move")
	--[
		Param("loc x", 1995.48)
		Param("loc z", 874.98)
		Param("dir x", -0.54)
		Param("dir z", 0.84)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3107)
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 980)
		Param( "event id", 270)
	--]
	End()

	Action("char condition")
	--[
		Param("ui disable", 0)
	--]
	End()

	Action( "sps end" )
	--[

	--]
	End()
--]
End()

