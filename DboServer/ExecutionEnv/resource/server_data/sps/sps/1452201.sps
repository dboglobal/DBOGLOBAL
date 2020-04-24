-- 불고킨 첫 번째 키리 충전


Scene(1, "1452201")
--[
	-- 불고킨 대사 "으어어어~ 힘이 느껴진다~ 으어어어~ "
	Action( "say" )
	--[
		Param( "tblidx", 3232 )
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
			Param( "wps index", 28012 )
			Param( "event id", 469 )
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