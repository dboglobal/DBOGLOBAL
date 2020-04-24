
Scene(1, "615")
--[
	Action("wait")
	--[
		Param("time", 1 )
	--]
	End()

	Action("point move")
	--[
		Param( "loc x", 5430 )
		Param( "loc y", -110.627 )
		Param( "loc z", -904 )
		Param( "run mode", 1 )
	--]
	End()

	Action("say")
	--[
		-- "¼Ò°ï ¼Ò°ï¡¦
		Param( "tblidx", 3317 )
	--]
	End()

	Action("wait")
	--[
		Param("time", 5)
	--]
	End()

	Action("point move")
	--[
		Param( "loc x", 5457 )
		Param( "loc y", -114 )
		Param( "loc z", -893 )
		Param( "run mode", 1 )
	--]
	End()

	Action("send sps event")
	--[
		Param("event type", "wps")
		Param("trigger type", "quest")
		Param("event id", 47)
	--]
	End()

	Action("sps end")
	--[

	--]
	End()
--]
End()

