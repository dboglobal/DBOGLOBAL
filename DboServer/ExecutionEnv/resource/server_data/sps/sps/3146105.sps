
Scene(1, "1650" )
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3187)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 7000)
		Param( "run mode", 1 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 1650 )
		Param( "event id", 354)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1650)
			Param("event id", 355)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3188)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1650)
			Param("event id", 356)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3189)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 7001)
		Param("run mode", 1)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1650)
		Param("event id", 357)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1650)
			Param("event id", 358)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3197)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1650)
			Param("event id", 359)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3199)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 7002)
		Param("run mode", 1)
	--]
	End()

	Action("rotate")
	--[
		Param("dir x", 0.191)
		Param("dir y", 0)
		Param("dir z", 0.982)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3190)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1650)
		Param("event id", 363)
	--]
	End()

	Action("sps end")
	--[
	--]
	End()
--]
End()

