CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 8153;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 9;
			elnk = 255;
			nextlnk = 6;
			prelnk = "8;";

			CDboTSActSendSvrEvt
			{
				id = 878;
				tblidx = 34003;
				stradius = 0;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 9;
			prelnk = "5;";

			CDboTSActSendSvrEvt
			{
				id = 877;
				tblidx = 34009;
				stradius = 0;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckAttachObj
			{
				objidx = "83;101;";
				widx = 717000;
			}
			CDboTSCheckObjState
			{
				mstate = 0;
				oidx = 83;
				osh = 255;
				widx = 717000;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "0;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = 83;
				osh_sh = 255;
				widx = 717000;
				osh_uc = 1;
				taid = 1;
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
				oidx = 101;
				osh_sh = 255;
				widx = 717000;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 8;
			prelnk = "4;";

			CDboTSActSendSvrEvt
			{
				id = 872;
				tblidx = 34001;
				stradius = 0;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 7;
			prelnk = "9;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 2000;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 254;
			prelnk = "6;";

			CDboTSActObjState
			{
				mstate = 2;
				oidx = 101;
				osh_sh = 255;
				widx = 717000;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "7;";
			type = 1;
		}
	}
}

