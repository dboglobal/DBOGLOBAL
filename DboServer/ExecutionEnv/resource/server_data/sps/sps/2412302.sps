
Scene(1,"1302" )
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	Action("ai bit flag")
	--[
		Param("attack type", "defensive")
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3136)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 2012)
		Param("run mode",1)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 2013)
		Param("run mode",1)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3137)
	--]
	End()

	Action("wait")
	--[
		Param("time",3)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 2014)
		Param("run mode",1)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3138 )
	--]
	End()

	Action("wait")
	--[
		Param("time",3)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 2015)
		Param("run mode",1)
	--]
	End()

	Action("wait")
	--[
		Param("time",3)
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 1302 )
		Param( "event id", 293)
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
