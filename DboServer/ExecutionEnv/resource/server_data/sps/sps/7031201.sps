-- DWC1
-- 뿔나로돈 spawn되어 대사 함

Scene( 1, "70001" )
--[
	-- npc ui 안나오도록 함
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()
	
	-- 어디서 맛있는 냄새가 나는구나~
	Action( "say" )
	--[
		Param( "tblidx", 3320 )
	--]
	End()
	
	-- User에게 뛰쳐나옴
	Action( "point move" )
	--[
		Param( "loc x", 1691.8 )
		Param( "loc y", -109.38 )
		Param( "loc z", -1506.27 )
		Param( "dir x", 0.48 )
		Param( "dir z", 0.87 )
		Param( "run mode", 1 )
	--]
	End()
	
	-- 뿔나로돈 뛰쳐나오기 완료
	Action( "send event to wps" )
	--[
		Param( "wps index", 70001 )
		Param( "event id", 622 )
	--]
	End()
	
	Action( "sps end" )
	--[
	--]
	End()
--]
End()



-- DWC1 용서 받고 뿔나로돈 사라짐
Scene( 3, "70002" )
--[
	-- npc ui 안나오도록 함
--	아래 code가 있으면 WPS가 중단되는듯? 확인 필요.
--	Action( "char condition" )
--	--[
--		Param( "ui disable", 1 )
--	--]
--	End()
	
	-- 고마워. 엉엉. 앞으로 착하게 살께.
	Action( "say" )
	--[
		Param( "tblidx", 3321 )
	--]
	End()
	
	Action( "point move" )
	--[
		Param( "loc x", 1777.07 )
		Param( "loc y", -108.22 )
		Param( "loc z", -1491.11 )
		Param( "dir x", -0.96 )
		Param( "dir z", 0.28 )
		Param( "run mode", 1 )
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


-- DWC1 용서 안받고 뿔나로돈 도망감
Scene( 4, "70001" )
--[
	-- npc ui 안나오도록 함
--	아래 code가 있으면 WPS가 중단되는듯? 확인 필요.
--	Action( "char condition" )
--	--[
--		Param( "ui disable", 1 )
--	--]
--	End()

	-- 나쁜 놈들... 두고 보자!! 복수할테다!!
	Action( "say" )
	--[
		Param( "tblidx", 3322 )
	--]
	End()
	
	Action( "point move" )
	--[
		Param( "loc x", 1777.07 )
		Param( "loc y", -108.22 )
		Param( "loc z", -1491.11 )
		Param( "dir x", -0.96 )
		Param( "dir z", 0.28 )
		Param( "run mode", 1 )
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

