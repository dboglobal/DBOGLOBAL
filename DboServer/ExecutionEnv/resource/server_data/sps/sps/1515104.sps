Scene( 1, "1515104" )
--[
	-- 머쉬룸 갱 혈투사 spawn되자마자 대사
	-- 머쉬룸 갱 혈투대장님의 명령이다! 수행자를 잡아서 꼼짝 못하게 붙들어라!
	Action( "say" )
	--[
		Param( "tblidx", 3030 )
	--]
	End()
	
	-- 절대로 오지 않을 이벤트를 기다림
	-- 아래의 이벤트 466은 sps 7112225에서 오지 않는 이벤트를 기다리게 하기 위해
	-- 처음 사용했지만 반복해서 사용해도 상관없다고 본다
	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 2576 )
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
