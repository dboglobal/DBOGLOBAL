-- 불고킨 두 번째 키리 충전


Scene(1, "1452301")
--[
	-- 불고킨 대사 "이런 맹랑한 녀석들!?지옥행 급행열차를 태워줄테니 잠시만 기다려!"
	Action( "say" )
	--[
		Param( "tblidx", 3233 )
	--]
	End()

	Action( "direct play" )
	--[
		Param( "tblidx", 60051 )
	--]
	End()
	
	-- 절대로 오지 않을 이벤트를 기다림
	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 28013 )
			Param( "event id", 470 )
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