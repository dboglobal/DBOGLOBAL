------------------------------------------------------
--
-- 크리링
-- Author : YeoHyeonJoon (souled@ntl-inc.com)
--
------------------------------------------------------

	
Scene(1, "TMQ 4 stage1" )

--대기
	Action( "wait" )
	--[
		Param( "time", 5 )
	--]
	End()

--쓰러진 모션
	Action( "sitdown" )
	--[
		Param( "sitdown", "true")
	--]
	End()

--대기
	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 14054)
	--]
	End()
	
--일어서
	Action( "sitdown" )
	--[
		Param( "sitdown", "false")
	--]
	End()
	
--퇴장 명령까지 대기
	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 14090)
	--]
	End()

--퇴장을 위한 이동
    Action( "point move" )
    --[
		Param( "loc x", 834.22)
		Param( "loc z", -878.23)
		Param( "run mode", 1 )
    --]
    End()
	
--퇴장 연출
	Action( "direct play" )
	--[
		Param( "tblidx", 50049)
	--]     
	End()
	
	Action( "leave" )
	--[    
	--]
	End()  

End()
	

------------------------------------------------------

	
Scene(2, "TMQ 4 stage3" )
 
--이동 명령까지 대기
	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 14420)
	--]
	End()

--이동
    Action( "point move" )
    --[
		Param( "loc x", 755.51)
		Param( "loc z", -379.68)
		Param( "run mode", 1 )
    --]
    End()
	
--이동
    Action( "point move" )
    --[
		Param( "loc x", 651.79)
		Param( "loc z", -376.93)
		Param( "run mode", 1 )
    --]
    End()

--이동 명령까지 대기
	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 14460)
	--]
	End()

--이동
    Action( "point move" )
    --[
		Param( "loc x", 664.68)
		Param( "loc z", -479.41)
        Param( "dir x", -0.75)
        Param( "dir z", -0.66)
		Param( "run mode", 1 )
    --]
    End()
	
--이동 명령까지 대기
	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 14463)
	--]
	End()
 
--이동
    Action( "point move" )
    --[
		Param( "loc x", 647.00)
		Param( "loc z", -535.35)
        Param( "dir x", -1.00)
        Param( "dir z", -0.03)
		Param( "run mode", 1 )
    --]
    End()
	
--이동 명령까지 대기
	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 14465)
	--]
	End()
 
--이동
    Action( "point move" )
    --[
		Param( "loc x", 641.51)
		Param( "loc z", -540.25)
        Param( "dir x", 0.959)
        Param( "dir z", 0.283)
		Param( "run mode", 1 )
    --]
    End()
	
--대기
	Action( "wait" )
	--[
		Param( "time", 1 )
	--]
	End()
	
--쓰러진 모션
	Action( "sitdown" )
	--[
		Param( "sitdown", "true")
	--]
	End()
	
--더미대기
	Action( "wait" )
	--[
		Param("event type", "ts")
		Param( "event id", 11020)
	--]
	End()
	
End()