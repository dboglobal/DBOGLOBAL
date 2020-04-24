
Scene(1, "1264")
--[
	Action("point move")
	--[
		Param("loc x", 2225.917)
		Param("loc z", 3213.926)
		Param("run mode",1)
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 1264 )
		Param( "event id", 216 )
	--]
	End()

	Action("sps end")
	--[

	--]
	End()
--]
End()
