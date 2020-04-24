-- 신품 포크포크 spawn 후 이동

Scene( 1, "1" )
--[
	Action( "path move" )
	--[
		Param( "tblidx", 46 )
		Param( "run mode", 0 )
	--]
	End()

	-- 절대로 오지 않는 이벤트를 기다리게 함
	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 28003 )
			Param( "event id", 666 )
		--]
		End()
	--]
	End()
--]
End()


Scene( 2, "2" )
--[
	Action( "wait" )
	--[
		Param( "time", 1 )
	--]
	End()
	
	Action( "path move" )
	--[
		Param( "tblidx", 47 )
		Param( "run mode", 0 )
	--]
	End()

	-- 절대로 오지 않는 이벤트를 기다리게 함
	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 28003 )
			Param( "event id", 666 )
		--]
		End()
	--]
	End()
--]
End()


Scene( 3, "3" )
--[
	Action( "path move" )
	--[
		Param( "tblidx", 48 )
		Param( "run mode", 0 )
	--]
	End()

	-- 절대로 오지 않는 이벤트를 기다리게 함
	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 28003 )
			Param( "event id", 666 )
		--]
		End()
	--]
	End()
--]
End()


Scene( 4, "4" )
--[
	Action( "wait" )
	--[
		Param( "time", 2 )
	--]
	End()
	
	Action( "path move" )
	--[
		Param( "tblidx", 49 )
		Param( "run mode", 0 )
	--]
	End()

	-- 절대로 오지 않는 이벤트를 기다리게 함
	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 28003 )
			Param( "event id", 666 )
		--]
		End()
	--]
	End()
--]
End()


Scene( 5, "5" )
--[
	Action( "wait" )
	--[
		Param( "time", 2 )
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 50 )
		Param( "run mode", 0 )
	--]
	End()

	-- 절대로 오지 않는 이벤트를 기다리게 함
	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 28003 )
			Param( "event id", 666 )
		--]
		End()
	--]
	End()
--]
End()


Scene( 6, "6" )
--[
	Action( "path move" )
	--[
		Param( "tblidx", 51 )
		Param( "run mode", 0 )
	--]
	End()

	-- 절대로 오지 않는 이벤트를 기다리게 함
	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 28003 )
			Param( "event id", 666 )
		--]
		End()
	--]
	End()
--]
End()


Scene( 7, "7" )
--[
	Action( "wait" )
	--[
		Param( "time", 1 )
	--]
	End()
	
	Action( "path move" )
	--[
		Param( "tblidx", 52 )
		Param( "run mode", 0 )
	--]
	End()

	-- 절대로 오지 않는 이벤트를 기다리게 함
	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 28003 )
			Param( "event id", 666 )
		--]
		End()
	--]
	End()
--]
End()


Scene( 8, "8" )
--[
	Action( "wait" )
	--[
		Param( "time", 2 )
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 53 )
		Param( "run mode", 0 )
	--]
	End()

	-- 절대로 오지 않는 이벤트를 기다리게 함
	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 28003 )
			Param( "event id", 666 )
		--]
		End()
	--]
	End()
--]
End()
