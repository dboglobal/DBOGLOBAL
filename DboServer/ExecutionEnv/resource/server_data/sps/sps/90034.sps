-------------------------------------------------
--	Data : 2009-06-25
--	Author : Rudin (byerror@ntl-inc.com)
-------------------------------------------------
-- 중간 기착지가 없는 노선
-- 사용 Path:
-- 90034	그레이트 발자국 순환 - 1 (Path를 한 개만 사용한다)
-- 


-- 출발지: 타카라프토 화산 지구
-- 도착지: 용의 코골이


Scene(1, "Path 90034")
--[
	Action("sitdown")
	--[
		Param( "sitdown", "true")
	--]
	End()

	Action("wait")
	--[
		Param( "time", 3)
	--]
	End()

	Action("say")
	--[
		Param( "tblidx", 2000)
		-- 이동을 원하시면 승차하세요
	--]
	End()

	Action("wait")
	--[
		Param( "time", 9)
	--]
	End()

	Action("say")
	--[
		Param( "tblidx", 2019)
		---- 용의 코골이 캠프행 버스 출발합니다
	--]
	End()

	Action("sitdown")
	--[
		Param( "sitdown", "false")
	--]
	End()

	Action("wait")
	--[
		Param( "time", 3)
	--]
	End()

	Action("path move")
	--[
		Param( "tblidx", 90034)
		Param( "run mode", 0)
		---- 첫번째 사용 Path
	--]
	End()

	Action("say")
	--[
		Param( "tblidx", 2001)
		-- 정류장에 도착했습니다
	--]
	End()

	Action("wait")
	--[
		Param( "time", 3)
	--]
	End()

	Action("sitdown")
	--[
		Param( "sitdown", "true")
	--]
	End()

	Action("wait")
	--[
		Param( "time", 5)
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