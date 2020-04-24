
Scene(1,"1556")

	Action("char condition")
	--[
		Param("ui disable", 1)
		Param("invincible", 0)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 7004)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1556)
		Param("event id", 367)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1556)
			Param("event id", 368)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3168)
	--]
	End()

	Action("sitdown")
	--[
		Param("sitdown", "true")
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1556)
			Param("event id", 369)
		--]
		End()
	--]
	End()


	Action("say")
	--[
		Param("tblidx", 3169)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 7005)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1556)
		Param("event id", 370)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1556)
			Param("event id", 371)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3194)
	--]
	End()

	Action("sitdown")
	--[
		Param("sitdown","true")
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1556)
			Param("event id", 372)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3195)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 7006)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1556)
		Param("event id", 373)
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index",1556)
			Param("event id", 374)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3168)
	--]
	End()

	Action("sitdown")
	--[
		Param("sitdown", "true")
	--]
	End()

	Action("wait")
	--[
		Condition("recv event from wps")
		--[
			Param("wps index", 1556)
			Param("event id", 375)
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3196)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 7007)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3170)
	--]
	End()

	Action("wait")
	--[
		Param("time", 3)
	--]
	End()
	
	Action("send event to wps")
	--[
		Param("wps index", 1556)
		Param("event id", 376)
	--]
	End()

	Action("sps end")
	--[
	--]
	End()
--]
End()
