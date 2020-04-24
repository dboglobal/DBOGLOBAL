
Scene(1, "1314" )
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

	Action( "say")
	--[
		Param("tblidx", 3139)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 2016)
		Param( "run mode", 1 )
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 2017)
		Param("run mode",1)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3140)
	--]
	End()

	Action("wait")
	--[
		Param("time",3)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 2018)
		Param("run mode", 1)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 2019)
		Param("run mode", 1)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3141)
	--]
	End()

	Action("wait")
	--[
		Param("time",3)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3142)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 1314 )
		Param( "event id", 295)
	--]
	End()

	Action( "sps end" )
	--[

	--]
	End()
--]
End()

