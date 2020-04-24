
Scene(1,"1405" )
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3149)
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 2021)
		Param( "run mode", 1 )
	--]
	End()

	Action("wait")
	--[
		Param("time",2)
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 1405 )
		Param( "event id", 302)
	--]
	End()

	Action( "sps end" )
	--[

	--]
	End()
--]
End()

