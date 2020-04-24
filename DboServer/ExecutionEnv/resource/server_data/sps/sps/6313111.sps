
Scene(1, "2400" )
--[
        Action( "wait" )
        --[
                Condition( "recv event from item" )
                --[
                        Param( "item index", 98018 )
                --]
                End()
        --]
        End()

        Action( "say" )
        --[
                Param( "tblidx", 3286 )
        --]
        End()

        Action( "send event to wps" )
        --[
                Param( "wps index", 2400 )
                Param( "event id", 541 )
        --]
        End()

        Action( "sps end" )
        --[
        
        --]
        End()
--]
End()