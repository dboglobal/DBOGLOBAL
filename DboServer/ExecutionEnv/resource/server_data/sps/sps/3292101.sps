------------------------------------------------------
--
-- 인조인간
-- Author : YeoHyeonJoon (souled@ntl-inc.com)
--
------------------------------------------------------


Scene( 1, "TMQ1" )

-- 인조인간 이동 명령까지 대기
	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 11090)
	--]
	End()

--작전 지시 이후 퇴장시키기
    Action( "point move" )
	--[
		Param( "loc x", 787.53)
		Param( "loc z", -741.38)
		Param( "run mode", 1 )
	--]
	End()
	
	Action( "point move" )
	--[
		Param( "loc x", 764.61)
		Param( "loc z", -719.56)
		Param( "run mode", 1 )
	--]
	End()
	
	Action( "leave" )
	--[    
	--]
	End()
	
End()

	
------------------------------------------------------


Scene( 2, "TMQ1" )

-- 인조인간 이동 명령까지 대기
	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 11325)
	--]
	End()

--작전 지시 이후 퇴장시키기
    Action( "point move" )
	--[
		Param( "loc x", 634.16)
		Param( "loc z", -159.56)
		Param( "run mode", 1 )
	--]
	End()
	
    Action( "point move" )
	--[
		Param( "loc x", 587.38)
		Param( "loc z", -147.45)
		Param( "run mode", 1 )
	--]
	End()
	
    Action( "point move" )
	--[
		Param( "loc x", 562.22)
		Param( "loc z", -110.00)
		Param( "run mode", 1 )
	--]
	End()
	
	Action( "leave" )
	--[    
	--]
	End()
	
End()

	
------------------------------------------------------