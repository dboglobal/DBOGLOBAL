-------------------------------------------------
--	Data : 2009-06-25
--	Author : Rudin (byerror@ntl-inc.com)
-------------------------------------------------

-- 사용 Path:
-- 90020	아지랑이샘 - 기란 부락1
-- 90021	아지랑이샘 - 기란 부락2


-- 출발지: 아지랑이샘
-- 도착지: 기란 부락


Scene(1, "Path 90020")
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
		Param( "tblidx", 2012)
		---- 기란 부락행 버스 출발합니다
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
		Param( "tblidx", 90020)
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
		Param( "tblidx", 2012)
		---- 기란 부락행 버스 출발합니다
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
		Param( "tblidx", 90021)
		Param( "run mode", 0)
		---- 두번째 사용 Path
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