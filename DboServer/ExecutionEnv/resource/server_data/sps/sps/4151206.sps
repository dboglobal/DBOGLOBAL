Scene(1,"1072")
--[
	Action("char condition")
	--[
		Param("ui disable", 1)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3108)
	--]
	End()

	Action("wait")
	--[
		Param("time",3)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index",1072)
		Param("event id", 250)
	--]
	End()

	Action("sps end")
	--[
	--]
	End()
--]
End()

Scene(2,"1072")
--[

	Action("char condition")
	--[
		Param("ui disable", 1)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3109)
	--]
	End()

	Action("wait")
	--[
		Param("time",5)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3110)
	--]
	End()

	Action("wait")
	--[
		Param("time",5)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3111)
	--]
	End()

	Action("point move")
	--[
		Param("loc x", 3088.073)
		Param("loc z", 1878.586)
		Param("run mode",1)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index",1072)
		Param("event id", 251)
	--]
	End()

	Action("sps end")
	--[
	--]
	End()

--]
End()
