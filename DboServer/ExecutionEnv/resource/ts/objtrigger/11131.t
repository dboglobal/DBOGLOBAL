CNtlTSTrigger
{
	sm = 0;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 11131;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 244;
			elnk = 255;
			nextlnk = 254;
			prelnk = "243;";

			CDboTSActSendSvrEvt
			{
				id = -1;
				tblidx = 3;
				stradius = 0;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 3;
			}
		}
		CDboTSContGAct
		{
			cid = 243;
			elnk = 255;
			nextlnk = 244;
			prelnk = "242;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = 46;
				osh_sh = 255;
				widx = 191000;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 242;
			elnk = 255;
			nextlnk = 243;
			prelnk = "241;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = 46;
				osh_sh = 255;
				widx = 191000;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 241;
			elnk = 255;
			nextlnk = 242;
			prelnk = "240;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 2000;
			}
		}
		CDboTSContGCond
		{
			cid = 240;
			prelnk = "1;";
			nolnk = 6;
			rm = 0;
			yeslnk = 241;

			CDboTSCheckInWorld
			{
				widx = 191000;
			}
		}
		CDboTSContGAct
		{
			cid = 9;
			elnk = 255;
			nextlnk = 254;
			prelnk = "8;";

			CDboTSActSendSvrEvt
			{
				id = -1;
				tblidx = 3;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 3;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 8;
			prelnk = "6;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = 46;
				osh_sh = 255;
				widx = 60000;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 9;
			prelnk = "7;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = 46;
				osh_sh = 255;
				widx = 60000;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 1;
			prelnk = "0;";
			nolnk = 240;
			rm = 0;
			yeslnk = 2;

			CDboTSCheckInWorld
			{
				widx = 11000;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 7;
			prelnk = "240;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 2000;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "244;9;0;5;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 2000;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = 46;
				osh_sh = 255;
				widx = 11000;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = 46;
				osh_sh = 255;
				widx = 11000;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 254;
			prelnk = "4;";

			CDboTSActSendSvrEvt
			{
				id = -1;
				tblidx = 3;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 3;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 254;
			rm = 0;
			yeslnk = 1;
		}
	}
}

