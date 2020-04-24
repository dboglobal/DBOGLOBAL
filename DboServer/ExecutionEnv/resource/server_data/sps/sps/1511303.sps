
Scene(1, "1300" )
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	Action( "say")
	--[
		Param("tblidx", 3134)
	--]
	End()

	Action("wait")
	--[
		Param("time",3)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 2009)
		Param("run mode",1)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1300)
		Param("event id", 287)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1300)
			Param("event id", 288)
		--]
		End()
	--]
	End()
	
	Action("path move")
	--[
		Param("tblidx", 2010)
		Param("run mode",1)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1300)
		Param("event id", 289)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index",1300)
			Param("event id", 290)
		--]
		End()
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 2011)
		Param("run mode", 1)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3135)
	--]
	End()

	Action("wait")
	--[
		Param("time",3)
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 1300 )
		Param( "event id", 291)
	--]
	End()

	Action( "sps end" )
	--[

	--]
	End()
--]
End()