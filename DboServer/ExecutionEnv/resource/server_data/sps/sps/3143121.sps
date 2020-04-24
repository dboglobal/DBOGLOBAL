
Scene(1, "305")
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
		Param("tblidx", 3216)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 7016)
		Param("run mode", 1)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 7017)
		Param("run mode", 1)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 305)
		Param("event id", 448)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 305)
			Param("event id", 449)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3218)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 305)
			Param("event id", 450)
		--]
		End()
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 7018)
		Param("run mode", 1)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 305)
		Param("event id", 451)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 305)
			Param("event id", 452)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3218)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 305)
			Param("event id", 453)
		--]
		End()
	--]
	End()
	
	Action("path move")
	--[
		Param("tblidx", 7019)
		Param("run mode", 1)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3217)
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 4)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 305)
		Param("event id", 454)
	--]
	End()

	Action("sps end")
	--[

	--]
	End()

--]
End()
