-------------------------------------------------
--	Data : 2009-06-25
--	Author : Rudin (byerror@ntl-inc.com)
-------------------------------------------------

-- 사용 Path:
-- 90026	나메크 스파 - 달팽이촌1
-- 90027	나메크 스파 - 달팽이촌2


-- 출발지: 나메크 스파
-- 도착지: 달팽이촌


Scene(1, "Path 90026")
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
		Param( "tblidx", 2015)
		---- 달팽이촌행 버스 출발합니다
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
		Param( "tblidx", 90026)
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
		Param( "tblidx", 2015)
		---- 달팽이촌행 버스 출발합니다
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
		Param( "tblidx", 90027)
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