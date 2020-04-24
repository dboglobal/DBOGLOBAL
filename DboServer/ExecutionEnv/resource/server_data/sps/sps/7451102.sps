--- The SPS battery used in the past          ---
--- 05 May 13, 2011 Currently unused ---
Scene(1, "6013")
--[


		 -- 3. 6013으로 부터 WPS Event ID 받음
		Action( "wait" )
		--[
			Param("event type", "wps")
			Param( "event id", 601301)
		--]
		End()

		-- 4. 시험관 연출 모드
		 Action( "char condition" )
		 --[
				Param( "attack disallow", 1)   
		 --]
		 End()
		 
		 
		-- 5. 무도가 시험관 이야기 하기
--		Action( "say" )
		--[
--			  무도 연구소의 새로운 도전자인가 보군 
			--Param( "tblidx", 1 )           
		--]     
--		End()

		Action( "wait" )
		--[
			   Param( "time", 2)
		--]
		End()

--		Action( "say" )
		--[
--			 응? 아직 어린아이아잖아
			--Param( "tblidx", 1 )           
		--]     
--		End()

		Action( "wait" )
		--[
			   Param( "time", 2)
		--]
		End()

--	       Action( "say" )
		--[
--			 "어디 너의 실력을 한번 볼까?"
			--Param( "tblidx", 1 )           
		--]     
--		End()

		Action( "wait" )
		--[
			   Param( "time", 2)
		--]
		End()

		-- 6. 대화 종료 WPS로 EventId 보냄
		Action( "send sps event" )
		--[
				Param( "event type", "wps" ) 
				Param( "event id", 601302 )
		--]
		End()

		-- 4. 시험관 연출 모드
		 Action( "char condition" )
		 --[
				Param( "attack disallow", 0)   
		 --]
		 End()

--]
End()



