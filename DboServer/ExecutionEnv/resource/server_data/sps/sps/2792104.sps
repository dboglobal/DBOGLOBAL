Scene( 1, "2792104" )
--[
	-- 레이더. 내에. 다수의. 적. 감지. 전략적. 열세. 판정. 전력이. 필요하다.
	Action( "say" )
	--[
		Param( "tblidx", 3273 )
	--]
	End()

	-- 절대로 오지 않을 이벤트를 기다림
	-- 아래의 이벤트 466은 sps 7112225에서 오지 않는 이벤트를 기다리게 하기 위해
	-- 처음 사용했지만 반복해서 사용해도 상관없다고 본다
	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 2680 )
			Param( "event id", 466 )
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
