Scene(1, "1405")
--[
	Action("char condition")
	--[
		Param("ui disable",1)
	--]
	End()
	
	Action("say")
	--[
		Param("tblidx", 3150)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action("sps end")
	--[
	--]
	End()

--]
End()

Scene(2, "1405")
--[

	Action("say")
	--[
		Param("tblidx", 3151)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3152)
	--]
	End()

	Action("point move")
	--[
		Param("loc x", 3793.2)
		Param("loc z", 3713.4)
		Param("run mode", 1)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1405)
		Param("event id", 303)
	--]
	End()

	Action("sps end")
	--[
	--]
	End()
--]
End()
