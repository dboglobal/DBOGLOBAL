Scene(1, "1090")
--[
	Action("char condition")
	--[
		Param("ui disable", 1)
	--]
	End()

	Action("point move")
	--[
		Param("loc x", -105.829)
		Param("loc z", 45.229)
		Param("dir x", 0.24)
		Param("dir z", 0.96)
		Param("run mode", 1)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3119)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3120)
	--]
	End()

	Action("point move")
	--[
		Param("loc x",-97.59)
		Param("loc z",76.892)
		Param("run mode", 1)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3121)
	--]
	End()

	Action("point move")
	--[
		Param("loc x",-134.866)
		Param("loc z",115.614)
		Param("run mode",1)
	--]
	End()


	Action("wait")
	--[
		Param("time",3)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index",1090)
		Param("event id",109001)
	--]
	End()

	Action("wait")
	--[
		Param("time",3)
	--]
	End()

	Action("sps end")
	--[
	--]
	End()
--]
End()
