
Scene(1, "711")
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	Action( "ai bit flag" )
	--[
		Param( "attack type", "offensive" )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3066 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 7110 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3067 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 711 )
		Param( "event id", 57 )
	--]
	End()

	Action( "sps end" )
	--[

	--]
	End()
--]
End()