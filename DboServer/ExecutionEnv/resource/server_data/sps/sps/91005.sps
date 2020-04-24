-- 서쪽 도시 버스
-- 버스 내선 서행
-- 중간에 꺾어지지 않는 노선
-- 사용 Path: 6


Scene(1, "6")
--[
	Action("sitdown")
	--[
		Param( "sitdown", "true" )
	--]
	End()

	Action("wait")
	--[
		Param( "time", 3 )
	--]
	End()

	Action("say")
	--[
		Param( "tblidx", 2000 )
		-- 이동을 원하시면 승차하세요
	--]
	End()

	Action("wait")
	--[
		Param( "time", 9 )
	--]
	End()

	Action("say")
	--[
		Param( "tblidx", 2036 )
		---- 서쪽 도시 내선 버스 출발하겠습니다
	--]
	End()

	Action("sitdown")
	--[
		Param( "sitdown", "false" )
	--]
	End()

	Action("wait")
	--[
		Param( "time", 3)
	--]
	End()

	Action("path move")
	--[
		Param( "tblidx", 6 )
		Param( "run mode", 0 )
		---- 사용 Path가 단 하나뿐임
	--]
	End()

	Action("say")
	--[
		Param( "tblidx", 2001 )
		-- 정류장에 도착했습니다
	--]
	End()

	Action("wait")
	--[
		Param( "time", 3 )
	--]
	End()

	Action("sitdown")
	--[
		Param( "sitdown", "true" )
	--]
	End()

	Action("wait")
	--[
		Param( "time", 5 )
	--]
	End()

	Action( "leave" )
	--[
	--]
	End()

	Action( "sps end" )
	--[
	--]
	End()
--]
End()