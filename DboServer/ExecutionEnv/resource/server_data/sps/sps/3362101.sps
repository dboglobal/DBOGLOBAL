
Scene(1,"1232" )
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
		Param("tblidx", 3128)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 2007)
		Param("run mode", 1)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1232)
		Param("event id", 281)
	--]
	End()

	Action("wait")
	---[
		Condition("recv event from wps")
		--[
			Param("wps index", 1232)
			Param("event id", 318)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3129 )
	--]
	End()


	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1232)
			Param("event id", 282)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3130)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 2008)
		Param("run mode", 1)
	--]
	End()

	Action("rotate")
	--[
		Param("dir x", -0.999)
		Param("dir y", 0)
		Param("dir z", 0.036)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3131)
	--]
	End()

	Action( "wait" )
	--[
		Param("time",3)
	--]
	End()


	Action( "send event to wps" )
	--[
		Param( "wps index", 1232 )
		Param( "event id", 283)
	--]
	End()

	Action( "sps end" )
	--[

	--]
	End()
--]
End()