CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 1090;
	title = 109002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 10;
			elnk = 255;
			nextlnk = 6;
			prelnk = "5;";

			CDboTSActCustomEvt
			{
				ceid = 10901;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 9;
			elnk = 255;
			nextlnk = 254;
			prelnk = "8;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = 45;
				osh_sh = 255;
				widx = 4;
				osh_uc = 0;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 9;
			prelnk = "7;";

			CDboTSActTWaitTS
			{
				taid = 2;
				time = 1000;
			}
			CDboTSActObjState
			{
				mstate = 3;
				oidx = 45;
				osh_sh = 255;
				widx = 4;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 7;
			nextlnk = 7;
			prelnk = "10;";

			CDboTSActObjWPS
			{
				sid = 1090;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActTWaitTS
			{
				taid = 2;
				time = 3000;
			}
			CDboTSActPCConv
			{
				taid = 3;
				tblidx = 109018;
			}
			CDboTSActObjState
			{
				mstate = 1;
				oidx = 45;
				osh_sh = 255;
				widx = 4;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 10;
			prelnk = "4;";

			CDboTSActTWaitTS
			{
				taid = 2;
				time = 3000;
			}
			CDboTSActObjState
			{
				mstate = 2;
				oidx = 45;
				osh_sh = 255;
				widx = 4;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 8;
			prelnk = "6;6;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "9;2;";
			type = 1;
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
		CDboTSContGCond
		{
			cid = 2;
			prelnk = "1;";
			nolnk = 254;
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
				time = 3000;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 109007;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckCustomEvent
			{
				qid = 1090;
			}
			CDboTSCheckAttachObj
			{
				objidx = "45;";
				widx = 4;
			}
		}
	}
}

