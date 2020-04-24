
Scene(1, "492" )
--[
	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 492 )
			Param( "event id", 82 )
		--]
		End()
	--]
	End()


	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3095)
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 1511301 )
		Param( "run mode", 1 )
	--]
	End()

---
	Action("send event to wps")
	--[
		Param("wps index", 492)
		Param("event id", 455)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 492)
			Param("event id", 456)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3212)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 492)
			Param("event id", 457)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3097)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 7020)
		Param("run mode", 1)
	--]
	End()

---
	Action( "send event to wps" )
	--[
		Param( "wps index", 492 )
		Param( "event id", 83 )
	--]
	End()

	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 492 )
			Param( "event id",  272)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3221)
	--]
	End()

	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 492 )
			Param( "event id", 85 )
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3222)
	--]
	End()

	Action("wait")
	--[
		Param("time", 4)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3223)
	--]
	End()

	Action("wait")
	--[
		Param("time", 2)
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 1511302 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "send event to wps")
	--[
		Param( "wps index", 492 )
		Param( "event id", 224 )
	--]
	End()

	Action( "sps end" )
	--[

	--]
	End()
--]
End()
