-- 와타 대장 뛰어 내리는 연출

Scene(1, "8391201")
--[
	-- 와타 대장 '이 녀석들 여기까지 올 줄이야...'
	Action( "say" )
	--[
		Param( "tblidx", 3234 )
	--]
	End()

	-- 2초간 기다리며 대사를 보여줌
	Action( "wait" )
	--[
		Param( "time", 2 )
	--]
	End()

	-- 다리 아래로 뛰어 내리는 연출
	Action( "direct play" )
	--[
		Param( "tblidx", 60056 )
	--]
	End()

	-- 연출 종료 사실을 wps로 보냄
	Action( "send event to wps" )
	--[
		Param( "wps index", 32006 )
		Param( "event id", 487 )
	--]
	End()

	Action( "sps end" )
	--[
	--]
	End()
--]
End()