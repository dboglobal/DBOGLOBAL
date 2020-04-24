-------------------------------------------------
--	Data : 2009-06-25
--	Author : Rudin (byerror@ntl-inc.com)
-------------------------------------------------

-- 사용 Path:
-- 90000	쫌구리스 - 새싹마을1
-- 90001	쫌구리스 - 새싹마을2


-- 출발지: 새싹마을
-- 도착지: 쫌구리스 해변


Scene(1, "Path 90000")
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
		Param( "tblidx", 2003)
		---- 쫌구리스 해변행 버스 출발합니다
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
		Param( "tblidx", 90000)
		Param( "run mode", 0)
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
		-- (이동을 원하시면 승차하세요)
	--]
	End()

	Action("wait")
	--[
		Param( "time", 9)
	--]
	End()

	Action("say")
	--[
		Param( "tblidx", 2003)
		---- 쫌구리스 해변행 버스 출발합니다
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
		Param( "tblidx", 90001)
		Param( "run mode", 0)
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