Scene(1, "1086 파견대원")
--[

	Action("char condition")
	--[
		Param("ui disable", 1)
	--]
	End()

	Action("point move")
	--[
		Param("loc x", 354.842)
		Param("loc z", -222.575)
		Param("run mode", 1)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1086)
		Param("event id", 253)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3116)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1086)
			Param("event id", 275)
		--]
		End()
	--]
	End()

	Action("point move")
	--[
		Param("loc x", 360.823)
		Param("loc z", -230.015)
		Param("run mode", 1)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1086)
		Param("event id", 276)
	--]
	End()

	Action("sps end")
	--[
	--]
	End()

--]
End()
