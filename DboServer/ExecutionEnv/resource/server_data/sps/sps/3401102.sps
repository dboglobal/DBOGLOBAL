

Scene(1, "370")
--[
	Action("say")
	--[
		Param( "tblidx", 3018)
	--]
	End()

	Action("wait")
	--[
		Param( "time", 2)
	--]
	End()

	Action("say")
	--[
		Param( "tblidx", 3019)
	--]
	End()

	Action("point move")
	--[
		Param( "loc x", 545.805 )
		Param( "loc y", -27.972 )
		Param( "loc z", 512.996 )
		Param( "run mode", 1)
	--]
	End()

	Action("send sps event")
	--[
		Param( "event type", "wps")
		Param( "event id", 11 )
	--]
	End()

	Action("wait")
	--[
		Param( "time", 1)
	--]
	End()

	Action("leave")
	--[
	--]
	End()
--]
End()
