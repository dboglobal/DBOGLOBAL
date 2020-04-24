
Scene(1, "2584" )
--[
        Action( "wait" )
        --[
                Condition( "recv event from item" )
                --[
                        Param( "item index", 98013 )
                --]
                End()
        --]
        End()
        
        Action( "char condition" )
        --[
                Param( "ui disable", 1 )
        --]
        End()

        Action( "say" )
        --[
                Param( "tblidx", 3275 )
        --]
        End()
        
        Action( "wait" )
        --[
                Param( "time", 3 )
        --]
        End()
        
        Action( "say" )
        --[
                Param( "tblidx", 3276 )
        --]
        End()
        
        Action( "wait" )
        --[
                Param( "time", 3 )
        --]
        End()
        
        Action( "say" )
        --[
                Param( "tblidx", 3277 )
        --]
        End()
        
        Action( "wait" )
        --[
                Param( "time", 1 )
        --]
        End()
        
        Action( "point move" )
        --[
                Param( "loc x", -965.142 )
                Param( "loc y", -130.297 )
                Param( "loc z", -3721.039 )
                Param( "run mode", 1 )
        --]
        End()
        
        Action( "send event to wps" )
        --[
                Param( "wps index", 2584 )
                Param( "event id", 574 )
        --]
        End()

        Action( "sps end" )
        --[
        
        --]
        End()
--]
End()