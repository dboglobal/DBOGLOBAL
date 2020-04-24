-- 구리스 가스 포동
-- spawn직후 중앙으로 이동

Scene( 1, "1881201" )
--[
	-- 최초 spawn은 약간 오른쪽에 치우쳐 있다.
	-- 중앙으로 이동
	Action( "point move" )
	--[
		Param( "loc x", 237.20 )
		Param( "loc y", 17.57 )
		Param( "loc z", 301.46 )
		Param( "dir x", -0.64 )
		Param( "dir z", 0.77 )
		Param( "run mode", 1 )
	--]
	End()

	-- 최초 전투 개시시까지 대기
	-- 전투 발생하면 이벤트 날아오며 이후로는 원래 spawn지점에서 대기하게 됨
	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 32004 )
			Param( "event id", 593 )
		--]
		End()
	--]
	End()

	Action( "sps end" )
	--[
	--]
	End()

--]
End()