CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 8072;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSClickObject
			{
				objidx = "4;";
				widx = 713000;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;9;4;6;";
			type = 1;
		}
		CDboTSContGCond
		{
			cid = 1;
			prelnk = "0;";
			nolnk = 2;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckObjState
			{
				mstate = 1;
				oidx = 2;
				osh = 255;
				widx = 713000;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "1;";

			CDboTSActObjWPS
			{
				sid = 29002;
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
				oidx = 4;
				osh_sh = 255;
				widx = 713000;
				osh_uc = 1;
				taid = 1;
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
				id = 861;
				tblidx = 29001;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "1;";
			nolnk = 4;
			rm = 0;
			yeslnk = 5;

			CDboTSCheckObjState
			{
				mstate = 4;
				oidx = 4;
				osh = 255;
				widx = 713000;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 254;
			prelnk = "3;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 622320;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "3;";

			CDboTSActOPObject
			{
				apply = 1;
				taid = 1;
				tblidx = 60060;
				time = 1000;
			}
		}
		CDboTSContGCond
		{
			cid = 6;
			prelnk = "5;";
			nolnk = 254;
			rm = 0;
			yeslnk = 7;

			CDboTSCheckOPObject
			{
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 8;
			prelnk = "6;";

			CDboTSActOPObject
			{
				apply = 0;
				taid = 1;
				tblidx = -1;
				time = 0;
			}
		}
	}
}

