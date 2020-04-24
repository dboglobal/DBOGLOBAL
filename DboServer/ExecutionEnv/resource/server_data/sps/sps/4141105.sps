
Scene(1, "1209 ¸ð¸ð²¿" )
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	Action("point move")
	--[
		Param("loc x", 1211.261)
		Param("loc y", -108)
		Param("loc z", 2860.938)
		Param("run mode",1)
	--]
	End()

	Action("point move")
	--[
		Param("loc x", 1219.596)
		Param("loc z", 2852.373)
		Param("dir x", 0.126)
		Param("dir z", -0.992)
		Param("run mode", 1)
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 1209 )
		Param( "event id", 278)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index",1209)
			Param("event id",280)
		--]
		End()
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3125)
	--]
	End()

	Action("wait")
	--[
		Param("time", 2)
	--]
	End()

	Action( "direct play" )
	--[
		Param( "tblidx", 60038)
	--]
	End()

	Action("wait")
	--[
		Param("time",3)
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3126 )
	--]
	End()

	Action("point move")
	--[
		Param("loc x", 1211.261)
		Param("loc z", 2860.938)
		Param("run mode", 1)
	--]
	End()
	
	Action("point move")
	--[
		Param("loc x", 1204.94)
		Param("loc y", -107.84)
		Param("loc z", 2865.3)
		Param("dir x", 0.529)
		Param("dir z", -0.849)
		Param("run mode", 1)
	--]
	End()

	Action( "char condition" )
	--[
		Param( "ui disable", 0 )
	--]
	End()

	Action( "sps end" )
	--[

	--]
	End()
--]
End()