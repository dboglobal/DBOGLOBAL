------------------------------------------------------
--
-- 피콜로
-- Author : YeoHyeonJoon (souled@ntl-inc.com)
--
------------------------------------------------------


Scene(1, "TMQ 2 피콜로" )

--피콜로 퇴장명령까지 대기
	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 12080)
	--]
	End()
	
--피콜로 퇴장을 위한 이동
    Action( "point move" )
    --[
		Param( "loc x", -1141.71)
		Param( "loc z", 853.07)
		Param( "run mode", 1 )
    --]
    End()
	
--피콜로 퇴장 연출
	Action( "direct play" )
	--[
		Param( "tblidx", 50042)
	--]     
	End()

	Action( "leave" )
	--[    
	--]
	End()

End()

	
------------------------------------------------------


Scene(2, "TMQ 2 피콜로" )

--이동명령까지 대기
	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 12261)
	--]
	End()

	Action( "point move" )
	--[
		Param( "loc x", 775.87)
		Param( "loc z", -23.43)
		Param( "run mode", 1 )
	--]
	End()
	
	Action( "point move" )
	--[
		Param( "loc x", 831.19)
		Param( "loc z", -127.91)
		Param( "run mode", 1 )
	--]
	End()
	
--피콜로 기습조1 부르기
	Action( "send sps event" )
	--[
		Param( "event type", "tqs" ) 
		Param( "event id", 12270 )
	--]
	End()

	Action( "point move" )
	--[
		Param( "loc x", 853.33)
		Param( "loc z", -147.67)
		Param( "run mode", 1 )
	--]
	End()
	
--이동 확인하기
	Action( "send sps event" )
	--[
		Param( "event type", "tqs" ) 
		Param( "event id", 12271 )
	--]
	End()
	
--피콜로 이동시키기
	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param("event id", 12280)
	--]
	End()

	Action( "point move" )
	--[
		Param( "loc x", 895.92)
		Param( "loc z", -164.60)
		Param( "run mode", 1 )
	--]
	End()
	
	Action( "point move" )
	--[
		Param( "loc x", 924.84)
		Param( "loc z", -199.58)
		Param( "run mode", 1 )
	--]
	End()
	
--피콜로 기습조2 부르기
	Action( "send sps event" )
	--[
		Param( "event type", "tqs" ) 
		Param( "event id", 12290 )
	--]
	End()
	
	Action( "point move" )
	--[
		Param( "loc x", 936.04)
		Param( "loc z", -217.32)
		Param( "run mode", 1 )
	--]
	End()
	
--이동 확인하기
	Action( "send sps event" )
	--[
		Param( "event type", "tqs" ) 
		Param( "event id", 12291 )
	--]
	End()
	
--기습조2 전멸, 피콜로 이동시키기
	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 12300)
	--]
	End()
	
	Action( "point move" )
	--[
		Param( "loc x", 987.45)
		Param( "loc z", -343.88)
		Param( "run mode", 1 )
	--]
	End()

	Action( "point move" )
	--[
		Param( "loc x", 976.68)
		Param( "loc z", -365.78)
		Param( "run mode", 1 )
	--]
	End()

	
--오공 기습조1 부르기
	Action( "send sps event" )
	--[
		Param( "event type", "tqs" ) 
		Param( "event id", 12310 )
	--]
	End()
	
	Action( "point move" )
	--[
		Param( "loc x", 962.58)
		Param( "loc z", -385.83)
		Param( "run mode", 1 )
	--]
	End()
	
--피콜로 퇴장명령까지 대기
	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 12370)
	--]
	End()
	
--피콜로 퇴장을 위한 이동
    Action( "point move" )
    --[
		Param( "loc x", 957.25)
		Param( "loc z", -392.71)
		Param( "run mode", 1 )
    --]
    End()
	
--피콜로 퇴장 연출
	Action( "direct play" )
	--[
		Param( "tblidx", 50042)
	--]     
	End()
	
	Action( "leave" )
	--[    
	--]
	End()  

End()


------------------------------------------------------


Scene(4, "TMQ 2 피콜로" )

	Action( "direct play" )
	--[
		Param( "tblidx", 50040)
	--]     
	End()

	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 24100)
	--]
	End()
	
	Action( "leave" )
	--[    
	--]
	End()

End()


------------------------------------------------------