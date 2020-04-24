CNtlTSTrigger
{
	sm = 0;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 11103;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 254;
			prelnk = "4;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1800000;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSClickObject
			{
				objidx = "50;";
				widx = 60000;
			}
		}
		CDboTSContGCond
		{
			cid = 2;
			prelnk = "1;";
			nolnk = 253;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckOPObject
			{
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActOPObject
			{
				apply = 1;
				taid = 1;
				tblidx = 10003;
				time = 10000;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActOPObject
			{
				apply = 0;
				taid = 1;
				tblidx = -1;
				time = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "5;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "2;0;";
			type = 0;
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
				oidx = 50;
				osh_sh = 255;
				widx = 60000;
				osh_uc = 255;
				taid = 1;
			}
			CDboTSActSendSvrEvt
			{
				id = 11210;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 2;
				ttype = 255;
				type = 1;
			}
		}
	}
}

