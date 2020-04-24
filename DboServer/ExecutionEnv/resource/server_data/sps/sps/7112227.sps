-- UD4 첫번째 방 안쪽 방에서 멋쟁이 관리병(7112225) 뛰쳐나옴


Scene(1, "7112225")
--[
	-- 대사 "침입자다! 침입자가 들어왔다!"
	Action( "say" )
	--[
		Param( "tblidx", 3024 )
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 8 )
		Param( "run mode", 1 )
	--]
	End()

	-- 대사 "침입자다! 침입자가 들어왔다!"
	Action( "say" )
	--[
		Param( "tblidx", 3024 )
	--]
	End()

	-- 절대로 오지 않을 이벤트를 기다림
	Action( "wait" )
	--[
		Condition( "recv event from wps" )
		--[
			Param( "wps index", 28007 )
			Param( "event id", 468 )
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