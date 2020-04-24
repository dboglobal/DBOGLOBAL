Scene(1,"966")
--[

	Action("char condition")
	--[
		Param("ui disable", 1)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from item")
		--[
			Param("item index", 98010)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3203)
	--]
	End()

	Action("point move")
	--[
		Param("loc x", 2021)
		Param("loc z", 1004)
		Param("run mode", 1)
	--]
	End()

	Action("point move")
	--[
		Param("loc x", 2026)
		Param("loc z", 1014)
		Param("run mode", 1)
	--]
	End()

	Action("point move")
	--[
		Param("loc x", 2017.03)
		Param("loc z", 1008.28)
		Param("dir x", -0.79)
		Param("dir z", -0.62)
		Param("run mode", 1)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1000)
		Param("event id", 405)
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

