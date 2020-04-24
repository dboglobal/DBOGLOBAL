Scene(1, "1355")
--[
	Action("char condition")
	--[
		Param("ui disable", 1)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3143)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 7008)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3144)
	--]
	End()

	Action("function")
	--[
		Condition("child")
		--[
			Action("wait")
			--[
				Param("time", 10)
			--]
			End()

			Action("say")
			--[
				Param("tblidx", 3146)
			--]
			End()

			Action("send event to wps")
			--[
				Param("wps index", 1355)
				Param("event id", 300)
			--]
			End()

			Action("sps end")
			--[
			--]
			End()
		--]
		End()

		Action("wait")
		--[
			Condition("recv event from item")
			--[
				Param("item index", 98003)
			--]
			End()
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3145)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 7009)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3144)
	--]
	End()

	Action("function")
	--[
		Condition("child")
		--[
			Action("wait")
			--[
				Param("time", 10)
			--]
			End()

			Action("say")
			--[
				Param("tblidx", 3146)
			--]
			End()

			Action("send event to wps")
			--[
				Param("wps index", 1355)
				Param("event id", 300)
			--]
			End()

			Action("sps end")
			--[
			--]
			End()
		--]
		End()

		Action("wait")
		--[
			Condition("recv event from item")
			--[
				Param("item index", 98003)
			--]
			End()
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3145)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 7010)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3144)
	--]
	End()

	Action("function")
	--[
		Condition("child")
		--[
			Action("wait")
			--[
				Param("time", 10)
			--]
			End()

			Action("say")
			--[
				Param("tblidx", 3146)
			--]
			End()

			Action("send event to wps")
			--[
				Param("wps index", 1355)
				Param("event id", 300)
			--]
			End()

			Action("sps end")
			--[
			--]
			End()
		--]
		End()

		Action("wait")
		--[
			Condition("recv event from item")
			--[
				Param("item index", 98003)
			--]
			End()
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3145)
	--]
	End()
		
	Action("path move")
	--[
		Param("tblidx", 7011)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3144)
	--]
	End()

	Action("function")
	--[
		Condition("child")
		--[
			Action("wait")
			--[
				Param("time", 10)
			--]
			End()

			Action("say")
			--[
				Param("tblidx", 3146)
			--]
			End()

			Action("send event to wps")
			--[
				Param("wps index", 1355)
				Param("event id", 300)
			--]
			End()

			Action("sps end")
			--[
			--]
			End()
		--]
		End()

		Action("wait")
		--[
			Condition("recv event from item")
			--[
				Param("item index", 98003)
			--]
			End()
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3145)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 7012)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3144)
	--]
	End()

	Action("function")
	--[
		Condition("child")
		--[
			Action("wait")
			--[
				Param("time", 10)
			--]
			End()

			Action("say")
			--[
				Param("tblidx", 3146)
			--]
			End()

			Action("send event to wps")
			--[
				Param("wps index", 1355)
				Param("event id", 300)
			--]
			End()

			Action("sps end")
			--[
			--]
			End()
		--]
		End()

		Action("wait")
		--[
			Condition("recv event from item")
			--[
				Param("item index", 98003)
			--]
			End()
		--]
		End()
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3145)
	--]
	End()

	Action("path move")
	--[
		Param("tblidx", 7013)
	--]
	End()

	Action("say")
	--[
		Param("tblidx", 3146)
	--]
	End()

	Action("wait")
	--[
		Param("time" ,3)
	--]
	End()

	Action("send event to wps")
	--[
		Param("wps index", 1355)
		Param("event id", 297)
	--]
	End()

	Action("sps end")
	--[
	--]
	End()
--]
End()
