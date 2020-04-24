
Scene(1,"1384" )
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3147)
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 2020)
		Param( "run mode", 1 )
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3148)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 1384 )
		Param( "event id", 299)
	--]
	End()

	Action( "sps end" )
	--[

	--]
	End()
--]
End()