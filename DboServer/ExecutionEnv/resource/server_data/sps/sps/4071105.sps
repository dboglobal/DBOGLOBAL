
Scene(1, "1086 지지메트" )
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3113)
	--]
	End()

	Action("wait")
	--[
		Param("time", 2)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 100)
		Param("run mode", 1)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3114)
	--]
	End()

	Action("wait")
	--[
		Param("time", 2)
	--]
	End()	

	Action("path move")
	--[
		Param("tblidx", 101)
		Param("run mode", 1)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3115)
	--]
	End()

	Action("wait")
	--[
		Param("time", 2)
	--]
	End()

	Action("point move")
	--[
		Param("loc x", 353.86)
		Param("loc z", -221.245)
		Param("run mode",1)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1086)
		Param("event id", 252)
	--]
	End()
	
	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1086)
			Param("event id", 254)
		--]
		End()
	--]
	End()


	Action("say")
	--[
		Param("tblidx", 3117)
	--]
	End()

	Action("wait")
	--[
		Param("time",3)
	--]
	End()

	Action("rotate")
	--[
		Param("dir x", -0.595)
		Param("dir z", 0.803)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3118)
	--]
	End()

	Action("wait")
	--[
		Param("time",3)
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 1086 )
		Param( "event id", 255)
	--]
	End()

	Action("point move")
	--[
		Param("loc x", 360.823)
		Param("loc z", -230.015)
		Param("run mode", 1)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1086)
		Param("event id", 277)
	--]
	End()

	Action( "sps end" )
	--[

	--]
	End()
--]
End()