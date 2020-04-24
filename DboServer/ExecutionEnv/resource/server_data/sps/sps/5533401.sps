--ver02

Scene(1, "242")
--[

	Action("char condition")
	--[
		Param("ui disable", 1)
	--]
	End()

	Action( "point move" )
	--[
		Param( "loc x", 4098.08 )
		Param( "loc z", -2914.46 )
		Param( "dir x", 0.98 )
		Param( "dir z", 0.17 )
		Param( "run mode", 1)
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3008 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 2 )
	--]
	End()

	Action( "rotate" )
	--[
		Param( "dir x", 0.98 )
		Param( "dir y", 0 )
		Param( "dir z", 0.17 )
	--]
	End()

	Action("char condition")
	--[
		Param("ui disable", 0)
	--]
	End()

	Action( "sps end" )
	--[

	--]
	End()

--]
End()

