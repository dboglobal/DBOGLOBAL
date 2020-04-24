
Scene(1,"1550")
--[
	Action("char condition")
	--[
		Param("ui disable", 1)
	--]
	End()

	Action("ai bit flag")
	--[
		Param("attack type","offensive")
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 7000)
		Param("run mode",1)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3167)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1550)
		Param("event id", 365)
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
