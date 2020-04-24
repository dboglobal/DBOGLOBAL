
Scene( 1, "Q161" )
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
		Param("tblidx", 3206)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 201)
		Param("run mode", 1)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3207)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 202)
		Param("run mode", 1)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3208)
	--]
	End()

	Action("wait")
	--[
		Param("time", 2)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 7000)
		Param("run mode", 1)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3209)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3210)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 161)
		Param("event id", 441)
	--]
	End()

	Action("char condition")
	--[
		Param("ui disable", 0)
	--]
	End()

	Action("sps end")
	--[
	--]
	End()

--]
End()